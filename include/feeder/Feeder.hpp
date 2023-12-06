#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <utility>
#include <opencv2/opencv.hpp>

class Feeder {
public:
    void feedCard();
    void stop();

private:
    int motorPin;
};

