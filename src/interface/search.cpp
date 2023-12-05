#include "search.h"
#include "ui_search.h"

Search::Search(QMainWindow *parent) 
    : QMainWindow(parent), 
      ui(new Ui::Search) {
    ui->setupUi(this);

    // Connect button signals to respective slots
    connect(ui->pushButton_2, &QPushButton::clicked, this, &Search::on_searchButton_clicked);
    connect(ui->pushButton, &QPushButton::clicked, this, &Search::on_editFiltersButton_clicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &Search::on_backButton_clicked);
}

Search::~Search() {
    delete ui;
}

void Search::on_searchButton_clicked() {
    QString keywords = ui->lineEdit->text();
    emit searchRequested(keywords); // Emit signal with keywords for searching
}

void Search::on_editFiltersButton_clicked() {
    emit editFilters(); // Emit signal to indicate filter editing is requested
}

void Search::on_backButton_clicked() {
    emit backRequested(); // Emit signal to navigate back to the main menu
}
