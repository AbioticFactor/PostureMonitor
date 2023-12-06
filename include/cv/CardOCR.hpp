#pragma once

#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <string>
#include <memory>
#include <functional>

class CardOCR {
public:

    std::string getCardInfo(std::string imPath);

private:
    std::unique_ptr<tesseract::TessBaseAPI, std::function<void(tesseract::TessBaseAPI*)>> api;
    std::unique_ptr<Pix, std::function<void(Pix*)>> image;
    std::unique_ptr<char, std::function<void(char*)>> outText;

    cv::Mat preprocessImage(const cv::Mat &image);
};
