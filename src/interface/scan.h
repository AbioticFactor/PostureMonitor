#ifndef SCAN_H
#define SCAN_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

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

private slots:
    void on_stopButton_clicked();

private:
    Ui::Scan *ui;
};

#endif // SCAN_H
