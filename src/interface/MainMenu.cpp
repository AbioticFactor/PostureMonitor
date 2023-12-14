#include "MainMenu.h"
#include "ui_MainMenu.h"
#include <QTimer>

MainMenu::MainMenu(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::MainMenu)
{
  ui->setupUi(this);

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

void MainMenu::on_EmailButton_clicked()
{
  ui->EmailButton->setEnabled(false);

  // Re-enable the button after 500 ms
  QTimer::singleShot(500, [this]()
                     { ui->EmailButton->setEnabled(true); });
  emit EmailClicked(); // Emit signal to navigate back to the main menu
}
