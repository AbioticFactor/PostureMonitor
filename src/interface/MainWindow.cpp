#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::MainWindow),
      stackedWidget(new QStackedWidget())
{

    std::cout << "about to ui arrow setup" << std::endl;

    ui->setupUi(this);
    std::cout << "setup ui" << std::endl;

    // stackedWidget->addWidget(this);
    MainMenu *mainMenuScreen = new MainMenu(this);
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
    stackedWidget->addWidget(mainMenuScreen);
    stackedWidget->addWidget(filterScreen);
    std::cout << "widg filter" << std::endl;
    stackedWidget->addWidget(scan);
    std::cout << "widg scan" << std::endl;
    stackedWidget->addWidget(searchScreen);
    std::cout << "widg search" << std::endl;
    stackedWidget->addWidget(raritiesScreen);
    std::cout << "widg rare" << std::endl;
    stackedWidget->addWidget(typeScreen);
    std::cout << "widg teetee" << std::endl;
    stackedWidget->addWidget(collectionScreen);
    std::cout << "widg col" << std::endl;

    std::cout << "added widgets" << std::endl;

    stackedWidget->setCurrentIndex(MainMenuIndex);
    std::cout << "MAIN WINDOW INDEX" << MainMenuIndex << std::endl;

    // Connect signals from buttons to slots within this class
    QLayout *existingLayout = this->layout();
    if (existingLayout)
    {
        existingLayout->addWidget(stackedWidget);
    }
    else
    {
        // Fallback: If no layout exists, create a new one
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(stackedWidget);
        setLayout(layout);
    }

    // MainWindow
    connect(this, &MainWindow::searchCriteriaChanged, collectionScreen, &Collection::setSearchCriteria);
    connect(this, &MainWindow::gpio17Triggered, this, &MainWindow::handleGpio17Trigger);
    connect(this, &MainWindow::gpio22Triggered, this, &MainWindow::handleGpio22Trigger);

    // MainMenu buttons
    connect(mainMenuScreen, &MainMenu::scanClicked, this, &MainWindow::on_scanButton_clicked);
    connect(mainMenuScreen, &MainMenu::searchCollectionClicked, this, &MainWindow::on_searchCollectionButton_clicked);
    connect(mainMenuScreen, &MainMenu::updatePricesClicked, this, &MainWindow::on_updatePricesButton_clicked);

    // Search screen buttons
    connect(searchScreen, &Search::searchRequested, this, &MainWindow::performSearch);
    connect(searchScreen, &Search::editFilters, this, &MainWindow::showEditFiltersScreen);
    connect(searchScreen, &Search::backRequested, this, &MainWindow::showMainMenuScreen);

    // Filter screen
    connect(filterScreen, &Filter::navigateToRarityScreen, this, &MainWindow::showRarityScreen);
    connect(filterScreen, &Filter::navigateToTypeScreen, this, &MainWindow::showTypeScreen);
    connect(filterScreen, &Filter::filtersUpdated, this, &MainWindow::applyCostColorFilter);

    // Scan screen
    connect(scan, &Scan::stopClicked, this, &MainWindow::showMainMenuScreen);
    connect(scan, &Scan::switchToCollectionView, this, &MainWindow::showCollectionScreen);

    // Rarities screen
    connect(raritiesScreen, &Rarities::raritiesSelected, this, &MainWindow::applyRaritiesFilter);

    // Type screen
    connect(typeScreen, &Type::typesSelected, this, &MainWindow::applyTypeFilter);

    // Collection screen
    connect(collectionScreen, &Collection::backRequested, this, &MainWindow::showMainMenuScreen);

    std::cout << "connected it all" << std::endl;

    QTimer processTimer;
    QTimer feedTimer;
    
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_scanButton_clicked()
{
    // Switch to the scan screen and start scanning
    stackedWidget->setCurrentIndex(ScanScreenIndex);

    // Create the objects
    CardOCR *cardOCR = new CardOCR(); // this should probably be a member variable
    Scan *scan = new Scan();          // this should probably be a member variable or already exist in your stackedWidget
    connect(cardOCR, &CardOCR::frameProcessed, scan, &Scan::displayFrame);
    connect(cardOCR, &CardOCR::finishedScanning, scan, &Scan::onFinishedScanning);
    cardOCR->start(); // Start the thread

    // Move cardOCR to a new thread
    // QThread* thread = new QThread(this);
    // cardOCR->moveToThread(thread);

    // // Connect the finished signal from the thread to deleteLater slots to clean up
    // connect(thread, &QThread::finished, cardOCR, &QObject::deleteLater);
    // connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    // // Connect the frameProcessed signal to the displayFrame slot
    // connect(cardOCR, &CardOCR::frameProcessed, scan, &Scan::displayFrame);

    // // Connect a signal to start the scan to the thread's started signal
    // connect(thread, &QThread::started, cardOCR, &CardOCR::scan);

    // // Start the thread
    // thread->start();

    // Start scanning...
}

void MainWindow::on_searchCollectionButton_clicked()
{
    // Switch to the search collection screen
    stackedWidget->setCurrentIndex(SearchScreenIndex);
}

void MainWindow::showMainMenuScreen()
{
    stackedWidget->setCurrentIndex(MainMenuIndex);
}

void MainWindow::on_updatePricesButton_clicked()
{
    // Handle updating prices
    // This might involve API calls, database updates, etc.
}

void MainWindow::applyRaritiesFilter(const QStringList &rarities)
{
    // Use the selected rarities to filter the search results
    // This might involve updating a query or filter settings
    currentCriteria.rarities = rarities;
    stackedWidget->setCurrentIndex(FilterScreenIndex);
}

void MainWindow::applyTypeFilter(const QStringList &types)
{
    // Use the selected types to filter the search results
    // This might involve updating a query or filter settings
    currentCriteria.types = types;
    stackedWidget->setCurrentIndex(FilterScreenIndex);
}

void MainWindow::showRarityScreen()
{
    stackedWidget->setCurrentIndex(RarityScreenIndex);
}

void MainWindow::showTypeScreen()
{
    // Logic to display the type screen
    stackedWidget->setCurrentIndex(TypeScreenIndex);
}

void MainWindow::showEditFiltersScreen()
{
    // Logic to display the edit filters screen
    stackedWidget->setCurrentIndex(FilterScreenIndex);
}

void MainWindow::showCollectionScreen()
{
    stackedWidget->setCurrentIndex(CollectionScreenIndex);
}

void MainWindow::applyCostColorFilter(const QList<int> &manaCosts, const QList<QString> &colors)
{
    // Logic to apply cost and color filters
    // Update currentCriteria or other relevant data structures with these filters
    currentCriteria.manaCosts = manaCosts;
    currentCriteria.colors = colors;
    stackedWidget->setCurrentIndex(SearchScreenIndex);

    // You may need to trigger a refresh or update of your data display based on these filters
}

void MainWindow::handleGpio17Trigger()
{
    // Perform any cleanup or tasks before exiting
    QApplication::quit();
}

void MainWindow::handleGpio22Trigger()
{
    // Perform any cleanup or tasks before exiting
    QApplication::quit();
    system("sudo shutdown -h now");
}

void MainWindow::performSearch()
{
    // Conversion from QStringList to std::vector<std::string>
    std::vector<std::string> raritiesVec;
    for (const auto &rarity : currentCriteria.rarities)
    {
        raritiesVec.push_back(rarity.toStdString());
    }
    std::vector<std::string> typesVec;
    for (const auto &type : currentCriteria.types)
    {
        typesVec.push_back(type.toStdString());
    }
    std::vector<std::string> colorsVec;
    for (const auto &color : currentCriteria.colors)
    {
        colorsVec.push_back(color.toStdString());
    }
    std::vector<int> manaVec;
    for (const auto &manaCost : currentCriteria.manaCosts)
    {
        manaVec.push_back(manaCost);
    }

    emit searchCriteriaChanged(currentCriteria);
    showCollectionScreen();

    // Call the search method with the collected criteria
    // DatabaseManager db("test-collection");
    // db.searchCards(currentCriteria.keywords.toStdString(),
    //                 raritiesVec,
    //                 typesVec,
    //                 manaVec,
    //                 colorsVec);
}
