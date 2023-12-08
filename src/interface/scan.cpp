#include "scan.h"
#include "ui_scan.h"
#include <QTimer>
// Include OpenCV headers if needed
// #include <opencv2/opencv.hpp>

Scan::Scan(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Scan)
{
    ui->setupUi(this);

    // Assuming 'label' is the placeholder for the video feed and 'pushButton' is the stop button
    // connect(ui->stopButton, &QPushButton::clicked, this, &Scan::on_stopButton_clicked);

    // Initialize OpenCV capture and display loop here...
}

Scan::~Scan()
{
    // Properly release OpenCV resources if needed
    delete ui;
}

void Scan::on_stopButton_clicked()
{
    ui->stopButton->setEnabled(false);

    // Re-enable the button after 500 ms
    QTimer::singleShot(500, [this]()
                       { ui->stopButton->setEnabled(true); });
    QStringList selectedRarities;
    // Stop the OpenCV video capture and any related operations here...
    emit stopClicked(); // Emit signal to indicate stopping the scanning process
}

void Scan::displayFrame(const cv::Mat &frame)
{
    cv::Mat rgbFrame;
    cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB); // Convert BGR to RGB

    QImage qimg(rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(qimg);

    ui->imageLabel->setPixmap(pixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
    ui->imageLabel->setScaledContents(true);
}

void Scan::onFinishedScanning()
{
    // Handle the completion of the scanning process
    emit switchToCollectionView();
}
