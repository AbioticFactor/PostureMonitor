#include "/home/pi/mtg-collection-manager/src/interface/MainWindow.h"
#include <QApplication>
#include <iostream>
#include <string>
#include <chrono>
#include <QTimer>





int main(int argc, char *argv[])
{
    // Initialize Qt Application
    QApplication a(argc, argv);

    QTimer timer;
    timer.setSingleShot(true); // Ensure the timer only fires once
    QObject::connect(&timer, &QTimer::timeout, &a, &QApplication::quit);
    timer.start(10000);

    // Custom initialization code (if any)
    std::cout << "Initiating boot sequence" << std::endl;

    // Create and display the main window
    MainWindow w;
    std::cout << "Initiating boot sequence2" << std::endl;
    w.show();

    

    // Start the Qt event loop
    return a.exec();

}
