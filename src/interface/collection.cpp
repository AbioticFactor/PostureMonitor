#include "collection.h"
#include "ui_collection.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>
#include <iostream>
#include <QStandardItemModel>
#include <QPixmap>
#include <QFileInfo>
#include <QPushButton>

Collection::Collection(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Collection),
      zoom(nullptr)
      
{
    ui->setupUi(this);

    // connect(ui->doneButton, &QPushButton::clicked, this, &Collection::on_doneButton_clicked);
}

Collection::~Collection()
{
    delete ui;
    delete zoom;
}

void Collection::on_doneButton_clicked()
{
    emit backRequested();
}

void Collection::loadImages()
{
    
    // // Convert QString and QStringList to std::string and std::vector<std::string>
    // std::string keywords = currentCriteria.keywords.toStdString();
    // std::vector<std::string> rarities = convertQStringList(currentCriteria.rarities);
    // std::vector<std::string> types = convertQStringList(currentCriteria.types);
    // std::vector<std::string> colors = convertQStringList(currentCriteria.colors);
    // std::vector<int> manaCosts(currentCriteria.manaCosts.begin(), currentCriteria.manaCosts.end()); // Convert QList<int> to std::vector<int>

    // db.addCard(1, "Black Lotus", 0, "Colorless", "Artifact", "Alpha", "Rare", 0, 0, "Christopher Rush", "/home/pi/mtg-collection-manager/src/cv/img1.jpg");

    // // std::vector<DatabaseManager::CardInfo> cards = db.searchCards(keywords, rarities, types, manaCosts, colors);
    // std::vector<DatabaseManager::CardInfo> cards; // Get this data from your database manager

    // QVBoxLayout *layout = new QVBoxLayout();
    // for (const auto &card : cards)
    // {
    //     QPixmap pixmap(QString::fromStdString(card.imagePath));
    //     QLabel *imageLabel = new QLabel();
    //     imageLabel->setPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio));
    //     layout->addWidget(imageLabel);
    // }

    // // Create a container widget and set the layout
    // QWidget *container = new QWidget();
    // container->setLayout(layout);

    // // Create a scroll area to hold the container
    // QScrollArea *scrollArea = new QScrollArea(this);
    // scrollArea->setWidget(container);
    // scrollArea->setWidgetResizable(true); // Make the scroll area resizable

    // // Instead of setCentralWidget, you add the scrollArea to the main layout of the Collection widget
    // QVBoxLayout *mainLayout = new QVBoxLayout(this); // 'this' ensures the layout is set on the current instance
    // mainLayout->addWidget(scrollArea);

    // // Set the main layout as the layout of the Collection widget
    // this->setLayout(mainLayout);
}

void Collection::setSearchCriteria(const Collection::SearchCriteria &criteria)
{
    // currentCriteria = criteria;
    // loadImages(); // Assuming loadImages uses currentCriteria to load images
}
void Collection::on_Card_clicked()
{
    // Zoom* zoom(new Zoom(this));
    // connect(this, &Collection::showEnlargedCardImage, zoom, &Zoom::showEnlargedCardImage);
    // connect(zoom, &Zoom::backRequested, this, &Collection::backFromZoom);
    // QPushButton *button = qobject_cast<QPushButton *>(sender());
    // if (button) {
    //     QString imagePath = button->property("imagePath").toString();
    //     emit showEnlargedCardImage(imagePath);

    
    //     // Implement logic to display card info and image
    //     // For example, open a new dialog with the card info and image
    
    if (!zoom) {  // Create Zoom screen only if it doesn't exist
        zoom = new Zoom(this);
        connect(this, &Collection::showEnlargedCardImage, zoom, &Zoom::showEnlargedCardImage);
        connect(zoom, &Zoom::backRequested, this, &Collection::backFromZoom);
    }
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        QString imagePath = button->property("imagePath").toString();
        emit showEnlargedCardImage(imagePath);
        zoom->show();  // Show the Zoom screen
    }
}

void Collection::backFromZoom() {
    if (zoom) {
        zoom->deleteLater();  // Safely delete the Zoom screen
        zoom = nullptr;       // Reset the pointer
    }
}

void Collection::showCardImages(std::vector<DatabaseManager::CardInfo> cards) {
    //IMPLEMENT LOGIC TO SHOW CARDS HERE
    std::cout << "got to showing images" << std::endl;
    QWidget *container = new QWidget;
    QGridLayout *layout = new QGridLayout(container);

    // Scaled dimensions (half of the original size)
    int scaledWidth = 119;  // Half of 238
    int scaledHeight = 166; // Half of 333
    QStringList imagePaths; 
    for (DatabaseManager::CardInfo &card : cards) {
        std::cout << card.imagePath << std::endl;
        imagePaths.append(QString::fromStdString(card.imagePath));
    }
    int row = 0, col = 0;
    for (int i = 0; i < imagePaths.size(); ++i) {
        const QString &path = imagePaths[i];
        if (QFileInfo::exists(path)) {
            QPixmap pixmap(path);
            QPushButton *button = new QPushButton;
            button->setIcon(QIcon(pixmap.scaled(scaledWidth, scaledHeight, Qt::KeepAspectRatio)));
            button->setIconSize(QSize(scaledWidth, scaledHeight));
            button->setFixedSize(scaledWidth, scaledHeight);

            // Set a custom property to identify the button
            button->setProperty("imagePath", path);

            connect(button, &QPushButton::clicked, this, &Collection::on_Card_clicked);

            layout->addWidget(button, row, col, Qt::AlignCenter);

            // Increment column; if at end, reset column and increment row
            col++;
            if (col >= 2) { // Assuming 2 images per row
                col = 0;
                row++;
            }
        }
    }

    ui->scrollArea->setWidget(container);
    // auto model = new QStandardItemModel(this);

    // int scaledWidth = 119;
    // int scaledHeight = 166;

    // // get image paths from card info
    // QStringList imagePaths; 
    // for (DatabaseManager::CardInfo &card : cards) {
    //     std::cout << card.imagePath << std::endl;
    //     imagePaths.append(QString::fromStdString(card.imagePath));
    // }

    // for (const QString &path : imagePaths) {
    //     if (QFileInfo::exists(path) && QFileInfo(path).isFile()) {
    //         QPixmap pixmap(path);
    //         auto item = new QStandardItem(QIcon(pixmap.scaled(scaledWidth, scaledHeight, Qt::KeepAspectRatio)), "");
    //         model->appendRow(item);
    //     }
    // }

    // ui->listView->setModel(model);
    // ui->listView->setIconSize(QSize(scaledWidth, scaledHeight));
    // ui->listView->setViewMode(QListView::IconMode);

    // // Adjust grid size to accommodate two images side by side
    // ui->listView->setGridSize(QSize(2 * scaledWidth, scaledHeight));
    // ui->listView->setResizeMode(QListView::Adjust);



//     QWidget *container = new QWidget;
//     QGridLayout *layout = new QGridLayout(container);

// // Scaled dimensions (half of the original size)
//     int scaledWidth = 119;  // Half of 238
//     int scaledHeight = 166; // Half of 333

//     int row = 0, col = 0;
//     QStringList imagePaths; 
//     for (DatabaseManager::CardInfo &card : cards) {
//         std::cout << card.imagePath << std::endl;
//         imagePaths.append(QString::fromStdString(card.imagePath));
//     }
//     for (const QString &path : imagePaths) {
//         if (QFileInfo::exists(path) && QFileInfo(path).isFile()) {
//             QPixmap pixmap(path);
//             QLabel *label = new QLabel;
//             label->setPixmap(pixmap.scaled(scaledWidth, scaledHeight, Qt::KeepAspectRatio));
//             label->setAlignment(Qt::AlignCenter);
//             layout->addWidget(label, row, col);

//             // Increment column; if at end, reset column and increment row
//             col++;
//             if (col >= 2) { // Assuming 2 images per row
//                 col = 0;
//                 row++;
//             }
//         }
//     }

//     ui->scrollArea->setWidget(container);
}

// std::vector<std::string> Collection::convertQStringList(const QStringList &list)
// {
//     std::vector<std::string> result;
//     for (const auto &item : list)
//     {
//         result.push_back(item.toStdString());
//     }
//     return result;
// }
