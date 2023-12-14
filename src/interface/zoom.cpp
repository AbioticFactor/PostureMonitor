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
    std::cout << "got to showing big image" << std::endl;
    QPixmap pixmap(imagePath);

    QLabel *imageLabel = ui->scrollArea->findChild<QLabel *>("imageLabel");
    if (!imageLabel) {
        imageLabel = new QLabel(ui->scrollArea->widget());
        ui->scrollArea->widget()->setLayout(new QVBoxLayout());
        ui->scrollArea->widget()->layout()->addWidget(imageLabel);
    }

    double scaleFactor = 0.88;
    int newWidth = ui->scrollArea->width() * scaleFactor;
    int newHeight = pixmap.height() * newWidth / pixmap.width();

    imageLabel->setPixmap(pixmap.scaled(newWidth, newHeight, Qt::KeepAspectRatioByExpanding));
    imageLabel->setAlignment(Qt::AlignCenter); 
}
