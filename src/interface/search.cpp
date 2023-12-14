#include "search.h"
#include "ui_search.h"

Search::Search(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Search)
{
    ui->setupUi(this);

}

Search::~Search()
{
    delete ui;
}

void Search::on_searchButton_clicked()
{
    ui->searchButton->setEnabled(false);

    QTimer::singleShot(500, [this]()
                       { ui->searchButton->setEnabled(true); });
    QStringList selectedRarities;
    emit searchRequested();
}

void Search::on_editFiltersButton_clicked()
{
    ui->editFiltersButton->setEnabled(false);

    QTimer::singleShot(500, [this]()
                       { ui->editFiltersButton->setEnabled(true); });
    QStringList selectedRarities;
    emit editFilters();
}

void Search::on_backButton_clicked()
{
    ui->backButton->setEnabled(false);

    QTimer::singleShot(500, [this]()
                       { ui->backButton->setEnabled(true); });
    QStringList selectedRarities;
    emit backRequested();
}
