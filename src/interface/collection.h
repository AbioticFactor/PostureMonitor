#ifndef COLLECTION_H
#define COLLECTION_H

#include <QWidget>
#include <database/DatabaseManager.hpp>
// #include "MainWindow.h"
#include <zoom.h>

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
        QString keywords;
        QStringList rarities;
        QStringList types;
        QList<int> manaCosts;
        QStringList colors;
    };
public slots:
    void setSearchCriteria(const SearchCriteria &criteria);
    void backFromZoom();
    void showCardImages(std::vector<DatabaseManager::CardInfo> cards);
    // std::vector<std::string> convertQStringList(const QStringList &list);

signals:
    void backRequested();
    void showEnlargedCardImage(QString imagePath);

private slots:
    void on_doneButton_clicked();
    void on_Card_clicked();

private:
    Ui::Collection *ui;
    Zoom *zoom;
    // MainWindow *mainWindow;

    void loadImages();


};



#endif // COLLECTION_H
