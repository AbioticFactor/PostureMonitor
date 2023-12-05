#ifndef RARITIES_H
#define RARITIES_H

#include <QMainWindow>
#include <QCheckBox>

namespace Ui {
    class Rarities;
}

class Rarities : public QMainWindow {
    Q_OBJECT

public:
    explicit Rarities(QMainWindow *parent = nullptr);
    ~Rarities();

signals:
    void raritiesSelected(const QStringList &rarities);

private slots:
    void on_okButton_clicked();

private:
    Ui::Rarities *ui;
    QList<QCheckBox*> rarityCheckBoxes;
};

#endif // RARITIES_H
