#ifndef SCAN_H
#define SCAN_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <opencv2/opencv.hpp>


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

private:
    Ui::Scan *ui;
};

#endif // SCAN_H
