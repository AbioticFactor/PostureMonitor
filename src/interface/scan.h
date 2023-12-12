#ifndef SCAN_H
#define SCAN_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <opencv2/opencv.hpp>
#include <QWidget>


namespace Ui {
    class Scan;
}

class Scan : public QWidget {
    Q_OBJECT

public:
    explicit Scan(QWidget *parent = nullptr);
    ~Scan();

signals:
    void stopClicked();


public slots:
    void on_stopButton_clicked();
    void displayFrame(const cv::Mat &frame);
    void showCardText(const std::string& cardText);

private:
    Ui::Scan *ui;
};

#endif // SCAN_H
