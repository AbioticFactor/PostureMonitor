#ifndef COLLECTION_H
#define COLLECTION_H

#include <QMainWindow>

//#include "MainWindow.h"

namespace Ui {
class Collection;
}

class Collection : public QMainWindow
{
    Q_OBJECT

public:
    explicit Collection(QMainWindow *parent = nullptr);
    ~Collection();

signals:
    void backRequested();

private slots:
    void on_doneButton_clicked();

private:
    Ui::Collection *ui;
    //MainWindow *mainWindow;

    void loadImages();
    std::vector<std::string> convertQStringList(const QStringList &list);
    
};

#endif // COLLECTION_H
