#include "rarity.h"
#include "ui_rarity.h"
#include <QTimer>

Rarities::Rarities(QWidget *parent) : QWidget(parent),
                                          ui(new Ui::Rarities)
{
    ui->setupUi(this);

    rarityCheckBoxes = {
        ui->checkBox,   // Basic Lands
        ui->checkBox_2, // Common
        ui->checkBox_3, // Uncommon
        ui->checkBox_4, // Rare
        ui->checkBox_5  // Mythic
    };

}

Rarities::~Rarities()
{
    delete ui;
}

void Rarities::on_okButton_clicked()
{
    ui->okButton->setEnabled(false);

    QTimer::singleShot(500, [this]()
                       { ui->okButton->setEnabled(true); });
    QStringList selectedRarities;
    for (QCheckBox *checkBox : rarityCheckBoxes)
    {
        if (checkBox->isChecked())
        {
            selectedRarities.append(checkBox->text());
        }
    }
    emit raritiesSelected(selectedRarities);
}
