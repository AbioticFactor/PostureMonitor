#include "MainWindow.h"
#include "ui_MainWindow.h"
// #include <database/DatabaseManager.hpp>
#include "scan.h"
#include "filter.h"
#include "rarity.h"
#include "type.h"
#include "search.h"
#include "collection.h"
#include <iostream>
// Include other screen implementation files as needed


MainWindow::MainWindow(QMainWindow *parent) 
    : QMainWindow(parent), 
      ui(new Ui::MainWindow), 
      stackedWidget(new QStackedWidget(this)) {

    std::cout << "about to ui arrow setup" << std::endl;

    ui->setupUi(this);
    std::cout << "setup ui" << std::endl;

    Filter *filterScreen = new Filter(this);
    std::cout << "about to fs" << std::endl;
    Scan *scan = new Scan(this);
    std::cout << "scan" << std::endl;
    Search *searchScreen = new Search(this);
    std::cout << "ss" << std::endl;
    Rarities *raritiesScreen = new Rarities(this);
    std::cout << "rara" << std::endl;
    Type *typeScreen = new Type(this);
    std::cout << "teetee" << std::endl;
    Collection *collectionScreen = new Collection(this);
    std::cout << "col" << std::endl;
    


    // Initialize the stackedWidget with various screens
    // Add MainWindow as the first screen
    stackedWidget.addWidget(this);
    std::cout << "widg main" << std::endl;
    stackedWidget.addWidget(filterScreen);
    std::cout << "widg filter" << std::endl;
    stackedWidget.addWidget(scan);
    std::cout << "widg scan" << std::endl;
    stackedWidget.addWidget(searchScreen);
    std::cout << "widg search" << std::endl;
    stackedWidget.addWidget(raritiesScreen);
    std::cout << "widg rare" << std::endl;
    stackedWidget.addWidget(typeScreen);
    std::cout << "widg teetee" << std::endl;
    stackedWidget.addWidget(collectionScreen);
    std::cout << "widg col" << std::endl;
    // Add other screens here...

    setCentralWidget(&stackedWidget);
    std::cout << "added widgets" << std::endl;


    // Connect signals from buttons to slots within this class

    // MainWindow buttons
    connect(ui->scanButton, &QPushButton::clicked, this, &MainWindow::on_scanButton_clicked);
    connect(ui->searchCollectionButton, &QPushButton::clicked, this, &MainWindow::on_searchCollectionButton_clicked);
    connect(ui->updatePricesButton, &QPushButton::clicked, this, &MainWindow::on_updatePricesButton_clicked);

    // Search screen buttons
    connect(searchScreen, &Search::searchRequested, this, &MainWindow::performSearch);
    connect(searchScreen, &Search::editFilters, this, &MainWindow::showEditFiltersScreen);
    connect(searchScreen, &Search::backRequested, this, &MainWindow::showMainWindowScreen);

    // Filter screen
    connect(filterScreen, &Filter::navigateToRarityScreen, this, &MainWindow::showRarityScreen);
    connect(filterScreen, &Filter::navigateToTypeScreen, this, &MainWindow::showTypeScreen);
    connect(filterScreen, &Filter::filtersUpdated, this, &MainWindow::applyCostColorFilter);

    // Scan screen
    connect(scan, &Scan::stopClicked, this, &MainWindow::showMainWindowScreen);

    // Rarities screen
    connect(raritiesScreen, &Rarities::raritiesSelected, this, &MainWindow::applyRaritiesFilter);

    // Type screen
    connect(typeScreen, &Type::typesSelected, this, &MainWindow::applyTypeFilter);

    // Collection screen
    connect(collectionScreen, &Collection::backRequested, this, &MainWindow::showMainWindowScreen);
    std::cout << "connected it all" << std::endl;

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_scanButton_clicked() {
    // Switch to the scan screen and start scanning
    stackedWidget.setCurrentIndex(ScanScreenIndex);
    // Start scanning...
}

void MainWindow::on_searchCollectionButton_clicked() {
    // Switch to the search collection screen
    stackedWidget.setCurrentIndex(SearchScreenIndex);
    // Handle search...
}

void MainWindow::showMainWindowScreen() {
    stackedWidget.setCurrentIndex(MainWindowIndex);
}

void MainWindow::on_updatePricesButton_clicked() {
    // Handle updating prices
    // This might involve API calls, database updates, etc.
}

void MainWindow::applyRaritiesFilter(const QStringList &rarities) {
    // Use the selected rarities to filter the search results
    // This might involve updating a query or filter settings
    currentCriteria.rarities = rarities;
}

void MainWindow::applyTypeFilter(const QStringList &types) {
    // Use the selected types to filter the search results
    // This might involve updating a query or filter settings
    currentCriteria.types = types;
}

void MainWindow::showRarityScreen() {
    stackedWidget.setCurrentIndex(RarityScreenIndex);
}

void MainWindow::showTypeScreen() {
    // Logic to display the type screen
    stackedWidget.setCurrentIndex(TypeScreenIndex);
}

void MainWindow::showEditFiltersScreen() {
    // Logic to display the edit filters screen
    stackedWidget.setCurrentIndex(FilterScreenIndex);
}

void MainWindow::showCollectionScreen() {
    stackedWidget.setCurrentIndex(CollectionScreenIndex);
}

void MainWindow::applyCostColorFilter(const QList<int>& manaCosts, const QList<QString>& colors) {
    // Logic to apply cost and color filters
    // Update currentCriteria or other relevant data structures with these filters
    currentCriteria.manaCosts = manaCosts;
    currentCriteria.colors = colors;

    // You may need to trigger a refresh or update of your data display based on these filters
}


void MainWindow::performSearch() {
    // Conversion from QStringList to std::vector<std::string>
    std::vector<std::string> raritiesVec;
    for (const auto &rarity : currentCriteria.rarities) {
        raritiesVec.push_back(rarity.toStdString());
    }
    std::vector<std::string> typesVec;
    for (const auto &type : currentCriteria.types) {
        typesVec.push_back(type.toStdString());
    }
    std::vector<std::string> colorsVec;
    for (const auto &color : currentCriteria.colors) {
        colorsVec.push_back(color.toStdString());
    }
    std::vector<int> manaVec;
    for (const auto &manaCost : currentCriteria.manaCosts) {
        manaVec.push_back(manaCost);
    }
    // Call the search method with the collected criteria
    // DatabaseManager db("test-collection");
    // db.searchCards(currentCriteria.keywords.toStdString(),
    //                 raritiesVec,
    //                 typesVec, 
    //                 manaVec, 
    //                 colorsVec);

}
