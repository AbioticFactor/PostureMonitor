#include "MainMenu.h"
#include "ui_MainMenu.h"
#include <database/DatabaseManager.hpp>
#include "scan.h"
#include "filter.h"
#include "rarity.h"
#include "type.h"
#include "search.h"
// Include other screen implementation files as needed


MainMenu::MainMenu(QWidget *parent) 
    : QMainWindow(parent), 
      ui(new Ui::MainWindow), 
      stackedWidget(new QStackedWidget(this)) {

    ui->setupUi(this);

    Filter *filterScreen = new Filter(this);
    Scan *scan = new Scan(this);
    Search *searchScreen = new Search(this);
    Rarities *raritiesScreen = new Rarities(this);
    Type *typeScreen = new Type(this);
    


    // Initialize the stackedWidget with various screens
    // Add MainMenu as the first screen
    stackedWidget->addWidget(this);
    stackedWidget->addWidget(filterScreen);
    stackedWidget->addWidget(scan);
    stackedWidget->addWidget(searchScreen);
    stackedWidget->addWidget(raritiesScreen);
    stackedWidget->addWidget(typeScreen);
    // Add other screens here...

    setCentralWidget(stackedWidget);


    // Connect signals from buttons to slots within this class

    // MainMenu buttons
    connect(ui->pushButton, &QPushButton::clicked, this, &MainMenu::on_scanButton_clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainMenu::on_searchCollectionButton_clicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainMenu::on_updatePricesButton_clicked);

    // Search screen buttons
    connect(searchScreen, &Search::searchRequested, this, &MainMenu::performSearch);
    connect(searchScreen, &Search::editFilters, this, &MainMenu::showEditFiltersScreen);
    connect(searchScreen, &Search::backRequested, this, &MainMenu::showMainMenuScreen);

    // Filter screen
    connect(filterScreen, &Filter::navigateToRarityScreen, this, &MainMenu::showRarityScreen);
    connect(filterScreen, &Filter::navigateToTypeScreen, this, &MainMenu::showTypeScreen);
    connect(filterScreen, &Filter::filtersUpdated, this, &MainMenu::applyCostColorFilter);

    // Scan screen
    connect(scan, &Scan::stopClicked, this, &MainMenu::showMainMenuScreen);

    // Rarities screen
    connect(raritiesScreen, &Rarities::raritiesSelected, this, &MainMenu::applyRaritiesFilter);

    // Type screen
    connect(typeScreen, &Type::typesSelected, this, &MainMenu::applyTypeFilter);
}

MainMenu::~MainMenu() {
    delete ui;
}

void MainMenu::on_scanButton_clicked() {
    // Switch to the scan screen and start scanning
    stackedWidget->setCurrentIndex(ScanScreenIndex);
    // Start scanning...
}

void MainMenu::on_searchCollectionButton_clicked() {
    // Switch to the search collection screen
    stackedWidget->setCurrentIndex(SearchScreenIndex);
    // Handle search...
}

void MainMenu::showMainMenuScreen() {
    stackedWidget->setCurrentIndex(MainMenuIndex);
}

void MainMenu::on_updatePricesButton_clicked() {
    // Handle updating prices
    // This might involve API calls, database updates, etc.
}

void MainMenu::applyRaritiesFilter(const QStringList &rarities) {
    // Use the selected rarities to filter the search results
    // This might involve updating a query or filter settings
    currentCriteria.rarities = rarities;
}

void MainMenu::applyTypeFilter(const QStringList &types) {
    // Use the selected types to filter the search results
    // This might involve updating a query or filter settings
    currentCriteria.types = types;
}

void MainMenu::performSearch() {
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
    DatabaseManager db("test-collection");
    db.searchCards(currentCriteria.keywords.toStdString(),
                    raritiesVec,
                    typesVec, 
                    manaVec, 
                    colorsVec);

}
