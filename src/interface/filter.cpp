#include "filter.h"
#include "ui_filter.h"
// #include "MainWindow.h"
#include <QTimer>
#include <QPushButton>

Filter::Filter(QWidget *parent) : QWidget(parent),
                                  ui(new Ui::Filter)
{
    ui->setupUi(this); // Set up the UI elements based on the XML

    // connect(ui->rarityButton, &QPushButton::clicked, this, &Filter::on_rarityButton_clicked, Qt::UniqueConnection);
    // connect(ui->typesButton, &QPushButton::clicked, this, &Filter::on_typesButton_clicked, Qt::UniqueConnection);
    // connect(ui->okButton, &QPushButton::clicked, this, &Filter::on_okButton_clicked, Qt::UniqueConnection);
    // DatabaseManager db("test_collection");
}

Filter::~Filter()
{
    delete ui; // Clean up the ui instance
}

void Filter::on_okButton_clicked()
{
    ui->okButton->setEnabled(false);

    // Re-enable the button after 500 ms
    QTimer::singleShot(500, [this]()
                       { ui->okButton->setEnabled(true); });
    QList<int> manaCosts;
    QList<QString> colors;
    gatherManaCosts(manaCosts);
    gatherColors(colors);
    emit filtersUpdated(manaCosts, colors);
}

void Filter::on_rarityButton_clicked()
{
    ui->rarityButton->setEnabled(false);

    // Re-enable the button after 500 ms
    QTimer::singleShot(500, [this]()
                       { ui->rarityButton->setEnabled(true); });
    emit navigateToRarityScreen();
}

void Filter::on_typesButton_clicked()
{
    ui->typesButton->setEnabled(false);

    QTimer::singleShot(500, [this]()
                       { ui->typesButton->setEnabled(true); });
    emit navigateToTypeScreen();
}

void Filter::gatherManaCosts(QList<int> &manaCosts)
{
    // Iterate over manaCostCheckBoxes and gather selected costs
    for (QPushButton *pushButton : {ui->b0, ui->b1, ui->b2, ui->b3, ui->b4, ui->b5, ui->b6})
    {
        if (pushButton->isChecked())
        {
            manaCosts.append(pushButton->objectName().mid(1).toInt());
        }
    }
}

void Filter::gatherColors(QList<QString> &colors)
{
    // Iterate over colorCheckBoxes and gather selected colors
    for (QPushButton *pushbutton : {ui->bWhite, ui->bBlue, ui->bGreen, ui->bBlack, ui->bRed, ui->bMulticolored, ui->bColorless})
    {
        if (pushbutton->isChecked())
        {
            colors.append(pushbutton->objectName().mid(1));
        }
    }
}

// void Filter::gatherManaCosts(QList<int> &manaCosts)
// {
//     // Iterate over manaCostCheckBoxes and gather selected costs
//     for (QCheckBox *checkBox : {ui->checkBox, ui->checkBox_2, ui->checkBox_3, ui->checkBox_4, ui->checkBox_5, ui->checkBox_6, ui->checkBox_7})
//     {
//         if (checkBox->isChecked())
//         {
//             manaCosts.append(checkBox->text().toInt());
//         }
//     }
// }

// void Filter::gatherColors(QList<QString> &colors)
// {
//     // Iterate over colorCheckBoxes and gather selected colors
//     for (QCheckBox *checkBox : {ui->checkBox_8, ui->checkBox_9, ui->checkBox_10, ui->checkBox_11, ui->checkBox_12, ui->checkBox_13, ui->checkBox_14})
//     {
//         if (checkBox->isChecked())
//         {
//             colors.append(checkBox->text());
//         }
//     }
// }

// In the future need multi color card support better (by text, flipped cards, etc)
