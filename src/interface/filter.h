#ifndef FILTER_H
#define FILTER_H

#include <QMainWindow>
#include <QCheckBox>
#include <QPushButton>
#include <QList>

namespace Ui {
class Filter;
}

class Filter : public QMainWindow {
    Q_OBJECT

public:
    explicit Filter(QMainWindow *parent = nullptr);
    ~Filter();

signals:
    void navigateToRarityScreen();
    void navigateToTypeScreen();
    void filtersUpdated(const QList<int>& manaCosts, const QList<QString>& colors);

private slots:
    void on_okButton_clicked();
    void on_rarityButton_clicked();
    void on_typesButton_clicked();

private:
    Ui::Filter *ui;

    void gatherManaCosts(QList<int>& manaCosts);
    void gatherColors(QList<QString>& colors);
};

#endif // FILTER_H
