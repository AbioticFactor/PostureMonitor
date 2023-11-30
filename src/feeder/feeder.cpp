// https://www.instructables.com/Card-Feeder-for-a-Trading-Card-Machine/

// https://www.stlfinder.com/model/lego-40-tooth-gear-IQaJXudq/755501/ 

// feeder code to feed cards and fire off camera

// Finish setting up Conan and Cmake
#include <iostream>
#include <pigpio.h>
#include <chrono>
#include <thread>
#include <utility>
#include <opencv2/opencv.hpp>

// use pigpio to control the motors

// use openCV to take picture
 
class feeder
{
public:
    
    feeder() : capFace(0), capBack(1)
    {
        if(!capFace.isOpened()){
            std::cerr << "Main camera failed" << std::endl;
            throw std::runtime_error("Main camera failed to open.");
        }
        if(!capBack.isOpened()){
            std::cerr << "Auxilary camera failed" << std::endl;
            throw std::runtime_error("Auxiliary camera failed to open.");
        }
        std::cout << "Cameras online";

        if (gpioInitialise() < 0) {
            std::cerr << "pigpio initialisation failed." << std::endl;
        }
        gpioServo(SERVO, pwmNeutral);
        std::cout << "Motors online";
        std::cout << "All systems online";
        

    }

    ~feeder()
    {
        gpioTerminate();
    }

    void feedCard()
    {
        gpioServo(SERVO, pwmDispense);
        std::this_thread::sleep_for(std::chrono::seconds(5));
        gpioServo(SERVO, pwmNeutral);

    }

    std::pair<cv::Mat, cv::Mat> captureCard()
    {
        cv::Mat faceFrame, backFrame;
        capFace >> faceFrame;
        capBack >> backFrame;
        return std::make_pair(faceFrame, backFrame);

    }

    void stop()
    {
        // gpioWrite(SERVO, 0);
    }

private:
    cv::VideoCapture capFace, capBack;
    const int SERVO = 0;
    const int pwmDispense = 1600;
    const int pwmNeutral = 1500;
};
