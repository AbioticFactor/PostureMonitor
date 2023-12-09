#include "rarity.h"
#include "ui_rarity.h"
#include <QTimer>

Rarities::Rarities(QWidget *parent) : QWidget(parent),
                                          ui(new Ui::Rarities)
{
    ui->setupUi(this);

    // Initialize the list of checkboxes
    rarityCheckBoxes = {
        ui->checkBox,   // Basic Lands
        ui->checkBox_2, // Common
        ui->checkBox_3, // Uncommon
        ui->checkBox_4, // Rare
        ui->checkBox_5  // Mythic
    };

    // Connect the OK button signal to the respective slot
    // connect(ui->okButton, &QPushButton::clicked, this, &Rarities::on_okButton_clicked);
}

Rarities::~Rarities()
{
    delete ui;
}

void Rarities::on_okButton_clicked()
{
    ui->okButton->setEnabled(false);

    // Re-enable the button after 500 ms
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
