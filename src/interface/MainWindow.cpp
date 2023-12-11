#include <QWidget> 
#include <QStackedWidget>
#include <QTimer>
#include <QVBoxLayout> 
#include <QApplication>
#include <iostream>
#include <vector> 
#include <QStringList>

#include "ui_MainWindow.h" 
#include <MainWindow.h>
#include "MainMenu.h"
#include "filter.h"
#include "scan.h"
#include "search.h"
#include "rarity.h"
#include "type.h"
#include "collection.h"
#include "CardOCR.hpp"
#include <pigpio.h>

#include <fstream>


MainWindow::MainWindow(QMainWindow *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      stackedWidget(new QStackedWidget(this)),
      dbManager(new DatabaseManager("collection.db3")),
      cardOCR(new CardOCR()),
      feeder(new Feeder()),
      processTimer(new QTimer()),
      feedTimer(new QTimer())

{
    currentCriteria.keywords = "";
    currentCriteria.rarities = QStringList{"Common"}; // Example values
    //currentCriteria.types = QStringList{"Creature", "Planeswalker", "Instant", "Sorcery", "Land", "Enchantment", "Artifact", "Battle", "Commanders"}; // Example values
    //currentCriteria.manaCosts = QList<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}; // Example values
    //currentCriteria.colors = QStringList{"White", "Blue", "Black", "Red", "Green", "Colorless", "Multi-colored"}; // Exam

    std::cout << "about to ui arrow setup" << std::endl;

    ui->setupUi(this);
    std::cout << "setup ui" << std::endl;


    // stackedWidget.addWidget(this);
    MainMenu *mainMenuScreen = new MainMenu(this);
    Filter *filterScreen = new Filter(this);
    Scan *scanScreen = new Scan(this);
    Search *searchScreen = new Search(this);
    Rarities *raritiesScreen = new Rarities(this);
    Type *typeScreen = new Type(this);
    Collection *collectionScreen = new Collection(this);


    // Initialize the stackedWidget with screens
    stackedWidget.addWidget(mainMenuScreen);
    stackedWidget.addWidget(scanScreen);
    stackedWidget.addWidget(searchScreen);
    stackedWidget.addWidget(raritiesScreen);
    stackedWidget.addWidget(typeScreen);
    stackedWidget.addWidget(filterScreen);
    stackedWidget.addWidget(collectionScreen);

    std::cout << "added widgets" << std::endl;
    
    stackedWidget.setCurrentIndex(MainMenuIndex);
    setCentralWidget(&stackedWidget);

    // MainWindow
    connect(this, &MainWindow::searchCriteriaChanged, collectionScreen, &Collection::setSearchCriteria);
    connect(this, &MainWindow::gpio17Triggered, this, &MainWindow::handleGpio17Trigger);
    connect(this, &MainWindow::gpio22Triggered, this, &MainWindow::handleGpio22Trigger);
    connect(this, &MainWindow::showCards, collectionScreen, &Collection::showCardImages);

    // MainMenu buttons
    connect(mainMenuScreen, &MainMenu::scanClicked, this, &MainWindow::on_scanButton_clicked);
    connect(mainMenuScreen, &MainMenu::searchCollectionClicked, this, &MainWindow::on_searchCollectionButton_clicked);
    connect(mainMenuScreen, &MainMenu::EmailClicked, this, &MainWindow::on_EmailButton_clicked);

    // Search screen buttons
    connect(searchScreen, &Search::searchRequested, this, &MainWindow::performSearch);
    connect(searchScreen, &Search::editFilters, this, &MainWindow::showEditFiltersScreen);
    connect(searchScreen, &Search::backRequested, this, &MainWindow::showMainMenuScreen);

    // Filter screen
    connect(filterScreen, &Filter::navigateToRarityScreen, this, &MainWindow::showRarityScreen);
    connect(filterScreen, &Filter::navigateToTypeScreen, this, &MainWindow::showTypeScreen);
    connect(filterScreen, &Filter::filtersUpdated, this, &MainWindow::applyCostColorFilter);

    // Scan screen
    connect(scanScreen, &Scan::stopClicked, cardOCR, &CardOCR::handleStopScanning);
    connect(scanScreen, &Scan::stopClicked, this, &MainWindow::performSearch);

    

    // Rarities screen
    connect(raritiesScreen, &Rarities::raritiesSelected, this, &MainWindow::applyRaritiesFilter);

    // Type screen
    connect(typeScreen, &Type::typesSelected, this, &MainWindow::applyTypeFilter);

    // Collection screen
    connect(collectionScreen, &Collection::backRequested, this, &MainWindow::showMainMenuScreen);

    // Card OCR
    // connect(cardOCR, &CardOCR::frameProcessed, scanScreen, &Scan::displayFrame);
    // connect(cardOCR, &CardOCR::finishedScanning, scanScreen, &Scan::onFinishedScanning);

    //Card OCR feeder
    connect(cardOCR, &CardOCR::feedCardRequested, this, &MainWindow::feedCard);
    connect(cardOCR, &CardOCR::requestProcessingDelay, this, &MainWindow::feedCard);
    connect(cardOCR, &CardOCR::requestProcessingDelay, this, &MainWindow::startProcessTimer);

    connect(processTimer, &QTimer::timeout, this, &MainWindow::onProcessTimerTimeout);

    std::cout << "connected it all" << std::endl;

}

MainWindow::~MainWindow()
{
    gpioTerminate();
    delete ui;
}

void MainWindow::on_scanButton_clicked()
{
    stackedWidget.setCurrentIndex(ScanScreenIndex);


    cardOCR->start(); 
}

void MainWindow::on_searchCollectionButton_clicked()
{
    // Switch to the search collection screen
    stackedWidget.setCurrentIndex(SearchScreenIndex);
}

void MainWindow::showMainMenuScreen()
{
    stackedWidget.setCurrentIndex(MainMenuIndex);
}

void MainWindow::on_EmailButton_clicked() {
    std::vector<std::string> rarities = convertQStringList(currentCriteria.rarities);
    std::vector<std::string> types = convertQStringList(currentCriteria.types);
    std::vector<std::string> colors = convertQStringList(currentCriteria.colors);
    std::vector<int> manaCosts(currentCriteria.manaCosts.begin(), currentCriteria.manaCosts.end());

    // std::vector<DatabaseManager::CardInfo> cards = db.searchCards(keywords, rarities, types, manaCosts, colors);
    auto cards = dbManager->searchCards(rarities, types, manaCosts, colors, true);
    cards = dbManager->getAllCards(false);

    std::string data;
    for (const auto& card : cards) {
        std::cout << card.name << std::endl;
        data += card.name + ", " + std::to_string(card.mana_cost) + ", " + card.color + "\n"; // etc.
    }
    std::ofstream file("/home/pi/mtg-collection-manager/data.csv");
    file << data;
    file.close();

    std::string command = "echo 'Your card collection data' | mpack  -s 'Database Data' /home/pi/mtg-collection-manager/data.csv aoa34@cornell.edu";
    system(command.c_str());

    // remove("/home/pi/mtg-collection-manager/data.csv");
}

void MainWindow::applyRaritiesFilter(const QStringList &rarities)
{
    currentCriteria.rarities = rarities;
    stackedWidget.setCurrentIndex(FilterScreenIndex);
}

void MainWindow::applyTypeFilter(const QStringList &types)
{

    currentCriteria.types = types;
    stackedWidget.setCurrentIndex(FilterScreenIndex);
}

void MainWindow::showRarityScreen()
{
    stackedWidget.setCurrentIndex(RarityScreenIndex);
}

void MainWindow::showTypeScreen()
{
    stackedWidget.setCurrentIndex(TypeScreenIndex);
}

void MainWindow::showEditFiltersScreen()
{
    stackedWidget.setCurrentIndex(FilterScreenIndex);
}

void MainWindow::showCollectionScreen(std::vector<DatabaseManager::CardInfo> cards)
{
    
    stackedWidget.setCurrentIndex(CollectionScreenIndex);
    emit showCards(cards);
}

void MainWindow::applyCostColorFilter(const QList<int> &manaCosts, const QList<QString> &colors)
{

    currentCriteria.manaCosts = manaCosts;
    currentCriteria.colors = colors;
    stackedWidget.setCurrentIndex(SearchScreenIndex);

}

void MainWindow::handleGpio17Trigger()
{
    QApplication::quit();
}

void MainWindow::handleGpio22Trigger()
{
    
    QApplication::quit();
    system("sudo shutdown -h now");
}

void MainWindow::performSearch()
{
    // Conversion from QStringList to std::vector<std::string>
    std::vector<std::string> raritiesVec = convertQStringList(currentCriteria.rarities);
    std::vector<std::string> typesVec = convertQStringList(currentCriteria.types);
    std::vector<std::string> colorsVec = convertQStringList(currentCriteria.colors);
    std::vector<int> manaVec;
    for (const int manaCost : currentCriteria.manaCosts)
    {
        manaVec.push_back(manaCost);
    }
    std::vector<int> imagePathVec;
    emit searchCriteriaChanged(currentCriteria);

    std::vector<DatabaseManager::CardInfo> cards = dbManager->searchCards(raritiesVec, typesVec, manaVec, colorsVec, true);
    // get the cards with the current criteria
    
    showCollectionScreen(cards);

}

void MainWindow::feedCard(){
    feeder->feedCard();
}

std::vector<std::string> MainWindow::convertQStringList(const QStringList &list)
{
    std::vector<std::string> result;
    for (const auto &item : list)
    {
        result.push_back(item.toStdString());
    }
    return result;
}

void MainWindow::onProcessTimerTimeout() {
    cardOCR->onProcessTimerTimeout();  // Start the timer with a 3-second interval
}

void MainWindow::startProcessTimer(int msec){
    processTimer->start(msec);
}


