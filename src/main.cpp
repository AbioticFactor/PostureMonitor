#include "MainWindow.h"
#include <QApplication>
#include <iostream>
#include <string>
#include <chrono>
#include <QTimer>
#include <pigpio.h>

#include <QCoreApplication>
#include <pigpio.h>
#include <iostream>
#include <QTimer>


class GpioMonitor : public QObject {
    Q_OBJECT

public:
    GpioMonitor(QObject *parent = nullptr) : QObject(parent) {
        if (gpioInitialise() < 0) {
            std::cerr << "pigpio initialization failed" << std::endl;
            return;
        }

        gpioSetMode(17, PI_INPUT);
        gpioSetMode(22, PI_INPUT);
        gpioSetPullUpDown(17, PI_PUD_UP);
        gpioSetPullUpDown(22, PI_PUD_UP);

        // Setup a timer to periodically check GPIO state
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &GpioMonitor::checkGpio);
        timer->start(100); // Check every 100 ms
    }

    ~GpioMonitor() {
        gpioTerminate();
    }

private slots:
    void checkGpio() {
        if (gpioRead(17) == 0 || gpioRead(22) == 0) {
            QCoreApplication::quit();
        }
    }
};


int main(int argc, char *argv[])
{
    bool addToReference = false;
    bool addToUser = true;

    DatabaseManager dbManager("collection.db3");
    dbManager.addCard("Fallaji Archaeologist", 2, "blue", "Creature", "Common", "/home/pi/mtg-collection-manager/src/images/black_dark.png", addToReference);
    dbManager.addCard("Swiftgear Drake", 5, "colorless", "Artifact", "Common", "/home/pi/mtg-collection-manager/src/images/black_light.png", addToReference);
    dbManager.addCard("Gnawing Vermin", 1, "black", "Creature", "Uncommon", "/home/pi/mtg-collection-manager/src/images/blue_dark.png", addToReference);
    dbManager.addCard("Giant Cindermaw", 3, "red", "Creature", "Uncommon", "/home/pi/mtg-collection-manager/src/images/blue_light.png", addToReference);
    dbManager.addCard("Gaea's Courser", 5, "green", "Creature", "Uncommon", "/home/pi/mtg-collection-manager/src/images/green_dark.png", addToReference);
    dbManager.addCard("Sardian Cliffstomper", 2, "red", "Creature", "Uncommon", "/home/pi/mtg-collection-manager/src/images/green_light.png", addToUser);
    dbManager.addCard("Phallanx Vanguard", 2, "white", "Creature", "Common", "/home/pi/mtg-collection-manager/src/images/red_dark.png", addToUser);
    dbManager.addCard("Blitz Automoton", 7, "colorless", "Artifact", "Common", "/home/pi/mtg-collection-manager/src/images/red_light.png", addToUser);
    dbManager.addCard("Meticulous Excavation", 1, "white", "Enchantment", "Uncommon", "/home/pi/mtg-collection-manager/src/images/white_dark.png", addToUser);
    dbManager.addCard("Lat-Nam Adept", 4, "blue", "Creature", "Common", "/home/pi/mtg-collection-manager/src/images/white_light.png", addToUser);



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
