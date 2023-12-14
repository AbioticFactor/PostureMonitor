#include "scan.h"
#include "ui_scan.h"
#include <QTimer>

// #include <opencv2/opencv.hpp>

Scan::Scan(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Scan)
{
    ui->setupUi(this);


}

Scan::~Scan()
{
    delete ui;
}

void Scan::on_stopButton_clicked()
{
    ui->stopButton->setEnabled(false);

    QTimer::singleShot(500, [this]()
                       { ui->stopButton->setEnabled(true); });
    QStringList selectedRarities;
    emit stopClicked();
}

void Scan::displayFrame(const cv::Mat &frame)
{
    cv::Mat rgbFrame;
    cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB);

    QImage qimg(rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(qimg);

    ui->imageLabel->setPixmap(pixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
    ui->imageLabel->setScaledContents(true);
}


void Scan::showCardText(const std::string& cardText)
{
    std::cout << "showing card text" << std::endl;
    ui->imageLabel->setText(QString::fromStdString(cardText));
}
