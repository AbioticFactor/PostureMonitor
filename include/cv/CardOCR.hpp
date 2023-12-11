#pragma once

#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <string>
#include <memory>
#include <functional>
#include <tuple>
#include <QObject>
#include <QThread>
#include <QTimer>

class CardOCR : public QThread {
    Q_OBJECT
public:
    CardOCR();
    ~CardOCR();
    
    std::tuple<std::string, cv::Mat> getCardName(const cv::Mat& im);
    void run();
    void processCard();

    // Additional methods and signals/slots here...
public slots:
    void handleStopScanning();
    void onProcessTimerTimeout();
    void onFeedTimerTimeout();
    

signals:
    void findAndAddMostSimilarCard(const std::string& inputCardName, const std::string& imagePath);
    void frameProcessed(cv::Mat frame);
    void feedCardRequested();
    void finishedScanning();
    void requestProcessingDelay(int delay);

private:
    std::unique_ptr<tesseract::TessBaseAPI, std::function<void(tesseract::TessBaseAPI*)>> api;
    std::unique_ptr<Pix, std::function<void(Pix*)>> image;
    std::unique_ptr<char, std::function<void(char*)>> outText;
    cv::VideoCapture capFace;

    bool initializeOCR();
    bool initializeCamera();
    cv::Mat preprocessImage(const cv::Mat &image, double leftMarginPercent, 
                                    double rightMarginPercent, double topMarginPercent,
                                    double bottomMarginPercent);
    void destroyApi(tesseract::TessBaseAPI* api);
    void destroyPix(Pix* p);
    void destroyChar(char* c);

    volatile bool stopRequested = false;
};
