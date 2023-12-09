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
#include <database/DatabaseManager.hpp>
#include <cv/CardOCR.hpp>
#include <feeder/Feeder.hpp>
#include <QTimer>


namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QMainWindow *parent = nullptr);

    Collection::SearchCriteria currentCriteria;

    ~MainWindow();
    std::vector<std::string> convertQStringList(const QStringList &list);
    void handleGpioEvent(int gpio, int level, uint32_t tick);


signals:
    void searchPerformed(const QString &keywords);
    void searchCriteriaChanged(Collection::SearchCriteria criteria);
    void gpio22Triggered();
    void gpio17Triggered();
    
public slots:
    void showMainMenuScreen();
    void showRarityScreen();
    void showTypeScreen();
    void showEditFiltersScreen();
    void applyRaritiesFilter(const QStringList &rarities);
    void applyTypeFilter(const QStringList &types);
    void applyCostColorFilter(const QList<int> &manaCosts, const QList<QString> &colors);
    void performSearch();
    void showCollectionScreen();
    void handleGpio17Trigger();
    void handleGpio22Trigger();
    void on_searchCollectionButton_clicked();
    void on_EmailButton_clicked();
    void on_scanButton_clicked();
    void feedCard();
    void startProcessTimer(int msec);
    void onProcessTimerTimeout();
    


    



private:
    Ui::MainWindow *ui;
    QStackedWidget stackedWidget;
    Feeder *feeder;
    CardOCR *cardOCR;
    DatabaseManager *dbManager;
    QTimer *processTimer;
    QTimer *feedTimer;


    enum ScreenIndices
    {
        MainMenuIndex,
        ScanScreenIndex,
        SearchScreenIndex,
        RarityScreenIndex,
        TypeScreenIndex,
        FilterScreenIndex,
        CollectionScreenIndex
    };
};

#endif // MainWindow_H
