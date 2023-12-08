#include "search.h"
#include "ui_search.h"

Search::Search(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Search)
{
    ui->setupUi(this);

    // Connect button signals to respective slots
    // connect(ui->searchButton, &QPushButton::clicked, this, &Search::on_searchButton_clicked);
    // connect(ui->editFiltersButton, &QPushButton::clicked, this, &Search::on_editFiltersButton_clicked);
    // connect(ui->backButton, &QPushButton::clicked, this, &Search::on_backButton_clicked);
}

Search::~Search()
{
    delete ui;
}

void Search::on_searchButton_clicked()
{
    ui->searchButton->setEnabled(false);

    // Re-enable the button after 500 ms
    QTimer::singleShot(500, [this]()
                       { ui->searchButton->setEnabled(true); });
    QStringList selectedRarities;
    emit searchRequested(); // Emit signal with keywords for searching
}

void Search::on_editFiltersButton_clicked()
{
    ui->editFiltersButton->setEnabled(false);

    // Re-enable the button after 500 ms
    QTimer::singleShot(500, [this]()
                       { ui->editFiltersButton->setEnabled(true); });
    QStringList selectedRarities;
    emit editFilters(); // Emit signal to indicate filter editing is requested
}

void Search::on_backButton_clicked()
{
    ui->backButton->setEnabled(false);

    // Re-enable the button after 500 ms
    QTimer::singleShot(500, [this]()
                       { ui->backButton->setEnabled(true); });
    QStringList selectedRarities;
    emit backRequested(); // Emit signal to navigate back to the main menu
}
