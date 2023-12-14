#include "MainWindow.h"
#include <QApplication>
#include <iostream>
#include <string>
#include <chrono>
#include <QTimer>
#include <pigpio.h>
#include <vector>

#include <QCoreApplication>
#include <pigpio.h>
#include <iostream>
#include <QTimer>

class GpioMonitor : public QObject
{
    Q_OBJECT

public:
    GpioMonitor(QObject *parent = nullptr) : QObject(parent)
    {
        if (gpioInitialise() < 0)
        {
            std::cerr << "pigpio initialization failed" << std::endl;
            return;
        }

        gpioSetMode(17, PI_INPUT);
        gpioSetMode(22, PI_INPUT);
        gpioSetPullUpDown(17, PI_PUD_UP);
        gpioSetPullUpDown(22, PI_PUD_UP);

        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &GpioMonitor::checkGpio);
        timer->start(100);
    }

    ~GpioMonitor()
    {
        gpioTerminate();
    }

private slots:
    void checkGpio()
    {
        if (gpioRead(17) == 0 )
        {
            QCoreApplication::quit();
        }
        else if (gpioRead(22) == 0) {
            QCoreApplication::quit();
            system("sudo shutdown -h now");
        }
    }
};

int main(int argc, char *argv[])
{
    bool addToReference = false;
    bool addToUser = true;

    // DatabaseManager dbManager("collection.db3");
    // dbManager.addCard("Fallaji Archaeologist", 2, "blue", "Creature", "Common", "/home/pi/mtg-collection-manager/src/interface/images/fallaji_archaeologist.png", addToUser);
    // dbManager.addCard("Swiftgear Drake", 5, "colorless", "Artifact", "Common", "/home/pi/mtg-collection-manager/src/interface/images/swiftgear_drake.png", addToUser);
    // dbManager.addCard("Gnawing Vermin", 1, "black", "Creature", "Uncommon", "/home/pi/mtg-collection-manager/src/interface/images/gnawing_vermin.png", addToUser);
    // dbManager.addCard("Giant Cindermaw", 3, "red", "Creature", "Uncommon", "/home/pi/mtg-collection-manager/src/interface/images/giant_cindermaw.png", addToReference);
    // dbManager.addCard("Gaea's Courser", 5, "green", "Creature", "Uncommon", "/home/pi/mtg-collection-manager/src/interface/images/gaeas_courser.png", addToReference);
    // dbManager.addCard("Sardian Cliffstomper", 2, "red", "Creature", "Uncommon", "/home/pi/mtg-collection-manager/src/interface/images/sardian_cliffstomper.png", addToUser);
    // dbManager.addCard("Phallanx Vanguard", 2, "white", "Creature", "Common", "/home/pi/mtg-collection-manager/src/interface/images/phallanx_vanguard.png", addToUser);
    // dbManager.addCard("Blitz Automoton", 7, "colorless", "Artifact", "Common", "/home/pi/mtg-collection-manager/src/interface/images/blitz_automoton.png", addToUser);
    // dbManager.addCard("Meticulous Excavation", 1, "white", "Enchantment", "Uncommon", "/home/pi/mtg-collection-manager/src/interface/images/meticulous_excavation.png", addToUser);
    // dbManager.addCard("Lat-Nam Adept", 4, "blue", "Creature", "Common", "/home/pi/mtg-collection-manager/src/interface/images/lat-nam_adept.png", addToUser);

    // Initialize Qt Application
    QApplication a(argc, argv);
    GpioMonitor monitor;

    // Custom initialization code (if any)
    std::cout << "Initiating boot sequence" << std::endl;

    // Create and display the main window
    MainWindow w;
    w.show();

    // Start the Qt event loop
    return a.exec();
}

#include "main.moc"
