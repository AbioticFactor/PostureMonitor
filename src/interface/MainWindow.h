#ifndef MainWindow_H
#define MainWindow_H

#include <QMainWindow>
#include <QStackedWidget>
#include "filter.h"
#include "collection.h"
#include "rarity.h"
#include "scan.h"
#include "search.h"
#include "type.h"
#include "MainMenu.h"

// Include headers for other screens

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QMainWindow *parent = nullptr);

    struct SearchCriteria
    {
        QString keywords;     // User-inputted search keywords
        QStringList rarities; // Selected rarities for filtering
        QStringList types;    // Selected card types for filtering
        QList<int> manaCosts; // Selected mana costs for filtering, changed from QStringList to QList<int>
        QStringList colors;   // Selected colors for filtering
    };

    SearchCriteria currentCriteria;

    ~MainWindow();

signals:
    void searchPerformed(const QString &keywords); // If you have a signal for search

public slots:
    void showMainWindowScreen();
    void showRarityScreen();
    void showTypeScreen();
    void showEditFiltersScreen();
    void applyRaritiesFilter(const QStringList &rarities);
    void applyTypeFilter(const QStringList &types);
    void applyCostColorFilter(const QList<int> &manaCosts, const QList<QString> &colors); // If you have such a slot
    void performSearch();
    void showCollectionScreen();

private:
    Ui::MainWindow *ui;
    QStackedWidget stackedWidget;

    // Enum for stacked widget indices
    enum ScreenIndices
    {
        MainWindowIndex,
        ScanScreenIndex,
        SearchScreenIndex,
        RarityScreenIndex,
        TypeScreenIndex,
        FilterScreenIndex,
        CollectionScreenIndex
        // Add other screen indices as needed
    };
};

#endif // MainWindow_H
