#include "zoom.h"
#include "ui_zoom.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>
#include <iostream>
#include <QStandardItemModel>
#include <QPixmap>

Zoom::Zoom(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Zoom)
{
    ui->setupUi(this);

    // connect(ui->doneButton, &QPushButton::clicked, this, &Collection::on_doneButton_clicked);
}

Zoom::~Zoom()
{
    delete ui;
}

void Zoom::on_doneButton_clicked()
{
    emit backRequested();
}

void Zoom::showEnlargedCardImage(QString imagePath) {
    // //IMPLEMENT LOGIC TO SHOW CARDS HERE
    // std::cout << "got to showing big image" << std::endl;
    // QPixmap pixmap(imagePath);
    // // Assuming there is a QLabel inside the scroll area to display the image
    // QLabel *imageLabel = ui->scrollArea->findChild<QLabel *>("imageLabel");
    // if (imageLabel) {
    //     imageLabel->setPixmap(pixmap.scaled(ui->scrollArea->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    // }
    std::cout << "got to showing big image" << std::endl;
    QPixmap pixmap(imagePath);

    QLabel *imageLabel = ui->scrollArea->findChild<QLabel *>("imageLabel");
    if (!imageLabel) {
        // Create a new QLabel if it doesn't exist
        imageLabel = new QLabel(ui->scrollArea->widget());
        ui->scrollArea->widget()->setLayout(new QVBoxLayout());
        ui->scrollArea->widget()->layout()->addWidget(imageLabel);
    }

    // Scaling factor, for example, 90% of the scroll area's width
    double scaleFactor = 0.88;
    int newWidth = ui->scrollArea->width() * scaleFactor;
    int newHeight = pixmap.height() * newWidth / pixmap.width();

    // Set the pixmap to the label with the new dimensions
    imageLabel->setPixmap(pixmap.scaled(newWidth, newHeight, Qt::KeepAspectRatioByExpanding));
    imageLabel->setAlignment(Qt::AlignCenter);  // Center the image within the QLabel

}
