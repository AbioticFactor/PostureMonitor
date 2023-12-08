#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>

namespace Ui
{
    class Search;
}

class Search : public QWidget
{
    Q_OBJECT

public:
    explicit Search(QWidget *parent = nullptr);
    ~Search();

signals:
    void searchRequested(const QString &keywords);
    void editFilters();
    void backRequested();

private slots:
    void on_searchButton_clicked();
    void on_editFiltersButton_clicked();
    void on_backButton_clicked();

private:
    Ui::Search *ui;
};

#endif // SEARCH_H
