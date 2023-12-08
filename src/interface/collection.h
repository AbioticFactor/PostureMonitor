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

signals:
    void backRequested();

private slots:
    void on_doneButton_clicked();

private:
    Ui::Collection *ui;
    // MainWindow *mainWindow;

    void loadImages();
    std::vector<std::string> convertQStringList(const QStringList &list);
};

#endif // COLLECTION_H
