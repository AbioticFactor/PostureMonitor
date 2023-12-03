// https://www.instructables.com/Card-Feeder-for-a-Trading-Card-Machine/

// https://www.stlfinder.com/model/lego-40-tooth-gear-IQaJXudq/755501/ 

// feeder code to feed cards and fire off camera

// Finish setting up Conan and Cmake
#include <iostream>
#include <pigpio.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <utility>
#include <condition_variable>
#include <opencv2/opencv.hpp>

// use pigpio to control the motors

// use openCV to take picture
 
class Feeder
{
public:
    
    Feeder()
    {


        if (gpioInitialise() < 0) {
            std::cerr << "pigpio initialisation failed." << std::endl;
        }
        gpioServo(SERVO, pwmNeutral);
        std::cout << "Motors online";
        std::cout << "All systems online";
        

    }

    ~Feeder()
    {
        gpioTerminate();
    }

    void stop()
    {
        // gpioWrite(SERVO, 0);
    }

private:
    
    const int SERVO = 0;
    const int pwmDispense = 1600;
    const int pwmNeutral = 1500;

    void feedCard()
    {
        gpioServo(SERVO, pwmDispense);
        std::this_thread::sleep_for(std::chrono::seconds(3));
        gpioServo(SERVO, pwmNeutral);

    }


};
