#include "Feeder.hpp"
#include <QTimer>
#include <pigpio.h>

Feeder::Feeder()
    : SERVO(5), pwmDispense(1710), pwmNeutral(1580)
{
    if (gpioInitialise() < 0)
    {
        std::cerr << "pigpio initialisation failed." << std::endl;
    }
    gpioServo(SERVO, pwmNeutral);
    std::cout << "Motors online" << std::endl;
}

Feeder::~Feeder()
{
    gpioTerminate();
}

void Feeder::feedCard()
{
    gpioServo(SERVO, pwmDispense);
    QTimer::singleShot(1740, this, [this]()
                       { stop(); });
    
}

void Feeder::stop()
{
    gpioServo(SERVO, pwmNeutral);
}
