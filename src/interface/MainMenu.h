#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include <QStackedWidget>
// Include headers for other screens

namespace Ui {
    class MainMenu;
}

class MainMenu : public QMainWindow {
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

signals:
    void searchPerformed(const QString &keywords); // If you have a signal for search

public slots:
    void showMainMenuScreen();
    void showRarityScreen();
    void showTypeScreen();
    void showEditFiltersScreen();
    void applyRaritiesFilter(const QStringList &rarities);
    void applyTypeFilter(const QStringList &types);
    void applyCostColorFilter(const QList<int>& manaCosts, const QList<QString>& colors); // If you have such a slot
    void performSearch();

private slots:
    void on_scanButton_clicked();
    void on_searchCollectionButton_clicked();
    void on_updatePricesButton_clicked();

private:
    Ui::MainWindow *ui;
    QStackedWidget *stackedWidget;

    struct SearchCriteria {
        QString keywords; // User-inputted search keywords
        QStringList rarities; // Selected rarities for filtering
        QStringList types; // Selected card types for filtering
        QList<int> manaCosts; // Selected mana costs for filtering, changed from QStringList to QList<int>
        QStringList colors; // Selected colors for filtering
    };

    SearchCriteria currentCriteria;

    // Enum for stacked widget indices
    enum ScreenIndices {
        MainMenuIndex,
        ScanScreenIndex,
        SearchScreenIndex,
        RarityScreenIndex,
        TypeScreenIndex
        // Add other screen indices as needed
    };

};

#endif // MAINMENU_H
