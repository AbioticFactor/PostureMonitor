#include "MainMenu.h"
#include "ui_MainMenu.h"

MainMenu::MainMenu(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::MainMenu)
{
  ui->setupUi(this);

  // Connect button signals to respective slots
  // connect(ui->searchButton, &QPushButton::clicked, this, &Search::on_searchButton_clicked);
  // connect(ui->editFiltersButton, &QPushButton::clicked, this, &Search::on_editFiltersButton_clicked);
  // connect(ui->backButton, &QPushButton::clicked, this, &Search::on_backButton_clicked);
}

MainMenu::~MainMenu()
{
  delete ui;
}

void MainMenu::on_scanButton_clicked()
{
  ui->scanButton->setEnabled(false);

  // Re-enable the button after 500 ms
  QTimer::singleShot(500, [this]()
                     { ui->scanButton->setEnabled(true); });
  emit scanClicked(); // Emit signal with keywords for searching
}

void MainMenu::on_searchCollectionButton_clicked()
{
  ui->searchCollectionButton->setEnabled(false);

  // Re-enable the button after 500 ms
  QTimer::singleShot(500, [this]()
                     { ui->searchCollectionButton->setEnabled(true); });
  emit searchCollectionClicked(); // Emit signal to indicate filter editing is requested
}

void MainMenu::on_updatePricesButton_clicked()
{
  ui->updatePricesButton->setEnabled(false);

  // Re-enable the button after 500 ms
  QTimer::singleShot(500, [this]()
                     { ui->updatePricesButton->setEnabled(true); });
  emit updatePricesClicked(); // Emit signal to navigate back to the main menu
}
