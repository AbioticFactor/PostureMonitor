#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <utility>
#include <opencv2/opencv.hpp>

class feeder {
public:
    feeder();
    ~feeder();

    void feedCard();
    std::pair<cv::Mat, cv::Mat> captureCard();
    void stop();

private:
    cv::VideoCapture capFace, capBack;
    int motorPin;
};

