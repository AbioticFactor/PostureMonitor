#ifndef SCAN_H
#define SCAN_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>

namespace Ui {
    class Scan;
}

class Scan : public QMainWindow {
    Q_OBJECT

public:
    explicit Scan(QMainWindow *parent = nullptr);
    ~Scan();

signals:
    void stopClicked();

private slots:
    void on_stopButton_clicked();

private:
    Ui::Scan *ui;
};

#endif // SCAN_H
