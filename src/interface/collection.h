#ifndef COLLECTION_H
#define COLLECTION_H

#include <QWidget>

// #include "MainWindow.h"

namespace Ui
{
    class Collection;
}

class Collection : public QWidget
{
    Q_OBJECT

public:
    explicit Collection(QWidget *parent = nullptr);
    ~Collection();
    struct SearchCriteria
    {
        QString keywords;     // User-inputted search keywords
        QStringList rarities; // Selected rarities for filtering
        QStringList types;    // Selected card types for filtering
        QList<int> manaCosts; // Selected mana costs for filtering, changed from QStringList to QList<int>
        QStringList colors;   // Selected colors for filtering
    };
    void setSearchCriteria(const Collection::SearchCriteria &criteria);
    // std::vector<std::string> convertQStringList(const QStringList &list);



signals:
    void backRequested();

private slots:
    void on_doneButton_clicked();

private:
    Ui::Collection *ui;
    // MainWindow *mainWindow;

    void loadImages();


};



#endif // COLLECTION_H
