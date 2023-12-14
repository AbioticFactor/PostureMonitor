#ifndef ZOOM_H
#define ZOOM_H

#include <QWidget>
#include <QCheckBox>
#include <database/DatabaseManager.hpp>
#include <QString>

namespace Ui
{
    class Zoom;
}

class Zoom : public QWidget
{
    Q_OBJECT

public:
    explicit Zoom(QWidget *parent = nullptr);\
    ~Zoom();

public slots:
    void showEnlargedCardImage(QString imagePath);

signals:
    void backRequested();

private slots:
    void on_doneButton_clicked();

private:
    Ui::Zoom *ui;
    // MainWindow *mainWindow;

};

#endif // ZOOM_H
