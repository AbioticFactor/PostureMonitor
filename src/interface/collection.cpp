#include "collection.h"
#include "ui_collection.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>

Collection::Collection(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Collection)
{
    ui->setupUi(this);

    connect(ui->doneButton, &QPushButton::clicked, this, &Collection::on_doneButton_clicked);
}

Collection::~Collection()
{
    delete ui;
}

void Collection::on_doneButton_clicked()
{
    emit backRequested();
}

void Collection::loadImages()
{

    Feeder feeder;
    DatabaseManager db("test_collection.db3");

    // Convert QString and QStringList to std::string and std::vector<std::string>
    std::string keywords = currentCriteria.keywords.toStdString();
    std::vector<std::string> rarities = convertQStringList(currentCriteria.rarities);
    std::vector<std::string> types = convertQStringList(currentCriteria.types);
    std::vector<std::string> colors = convertQStringList(currentCriteria.colors);
    std::vector<int> manaCosts(currentCriteria.manaCosts.begin(), currentCriteria.manaCosts.end()); // Convert QList<int> to std::vector<int>

    db.addCard(1, "Black Lotus", 0, "Colorless", "Artifact", "Alpha", "Rare", 0, 0, "Christopher Rush", "/home/pi/mtg-collection-manager/src/cv/img1.jpg");

    // std::vector<DatabaseManager::CardInfo> cards = db.searchCards(keywords, rarities, types, manaCosts, colors);
    std::vector<DatabaseManager::CardInfo> cards; // Get this data from your database manager

    QVBoxLayout *layout = new QVBoxLayout();
    for (const auto &card : cards)
    {
        QPixmap pixmap(QString::fromStdString(card.imagePath));
        QLabel *imageLabel = new QLabel();
        imageLabel->setPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio));
        layout->addWidget(imageLabel);
    }

    // Create a container widget and set the layout
    QWidget *container = new QWidget();
    container->setLayout(layout);

    // Create a scroll area to hold the container
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(container);
    scrollArea->setWidgetResizable(true); // Make the scroll area resizable

    // Instead of setCentralWidget, you add the scrollArea to the main layout of the Collection widget
    QVBoxLayout *mainLayout = new QVBoxLayout(this); // 'this' ensures the layout is set on the current instance
    mainLayout->addWidget(scrollArea);

    // Set the main layout as the layout of the Collection widget
    this->setLayout(mainLayout);
}

void Collection::setSearchCriteria(const Collection::SearchCriteria &criteria)
{
    currentCriteria = criteria;
    loadImages(); // Assuming loadImages uses currentCriteria to load images
}

std::vector<std::string> Collection::convertQStringList(const QStringList &list)
{
    std::vector<std::string> result;
    for (const auto &item : list)
    {
        result.push_back(item.toStdString());
    }
    return result;
}
