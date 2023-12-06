#include "collection.h"
#include "ui_collection.h"
#include <QLabel>
#include <QVBoxLayout>
#include <database/DatabaseManager.hpp>
#include "/home/pi/mtg-collection-manager/include/feeder/Feeder.hpp"
//#include "MainWindow.h"



Collection::Collection(QMainWindow *parent)
    : QMainWindow(parent),
      ui(new Ui::Collection) {
    ui->setupUi(this);

    connect(ui->doneButton, &QPushButton::clicked, this, &Collection::on_doneButton_clicked);
}


Collection::~Collection() {
    delete ui;
}

void Collection::on_doneButton_clicked() {
    emit backRequested();
}

void Collection::loadImages() {
    // if (!mainWindow) return; // Check if mainWindow is valid

    // Feeder feeder;
    // DatabaseManager db("test_collection.db3");


    // auto &criteria = mainWindow->currentCriteria; // Access currentCriteria from mainWindow

    // // Convert QString and QStringList to std::string and std::vector<std::string>
    // std::string keywords = criteria.keywords.toStdString();
    // std::vector<std::string> rarities = convertQStringList(criteria.rarities);
    // std::vector<std::string> types = convertQStringList(criteria.types);
    // std::vector<std::string> colors = convertQStringList(criteria.colors);
    // std::vector<int> manaCosts(criteria.manaCosts.begin(), criteria.manaCosts.end()); // Convert QList<int> to std::vector<int>

    // db.addCard(1, "Black Lotus", 0, "Colorless", "Artifact", "Alpha", "Rare", 0, 0, "Christopher Rush", "gg");

    // // std::vector<DatabaseManager::CardInfo> cards = db.searchCards(keywords, rarities, types, manaCosts, colors);
    // std::vector<DatabaseManager::CardInfo> cards;


    // QVBoxLayout *layout = new QVBoxLayout();
    // for (const auto& card : cards) {
    //     QPixmap pixmap(QString::fromStdString(card.imagePath));
    //     QLabel *imageLabel = new QLabel(this);
    //     imageLabel->setPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio));
    //     layout->addWidget(imageLabel);
    // }
    // QWidget *container = new QWidget();
    // container->setLayout(layout);
    // setCentralWidget(container); 
}

std::vector<std::string> Collection::convertQStringList(const QStringList &list) {
    std::vector<std::string> result;
    for (const auto &item : list) {
        result.push_back(item.toStdString());
    }
    return result;
}
