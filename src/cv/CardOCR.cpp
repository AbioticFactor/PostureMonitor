// Finish setting up Conan and Cmake
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <string>
#include <chrono>
#include <feeder/Feeder.hpp>
#include <database/DatabaseManager.hpp>

// use openCV to load image and find/ crop bottom left location
 
// tesserect to read the card and return bottom left info text

// if no bgottom left text for tesserect then match by name and image maybe

class CardOCR {
public:
    CardOCR() : api(new tesseract::TessBaseAPI(), [](tesseract::TessBaseAPI* ptr) {
        if (ptr) {
            ptr->End();
            delete ptr;
        }
    }),
    image(nullptr, [](Pix* ptr) {
        if (ptr) {
            pixDestroy(&ptr);
        }
    }),
    outText(nullptr, [](char* ptr) {
        if (ptr) {
            delete[] ptr;
        }
    }) {
        if (!initializeOCR()) {
            throw std::runtime_error("OCR initialization failed.");
        }
        initializeCamera();
    }

    ~CardOCR() {
        // Destructor implementation (if needed)
    }

    bool initializeOCR() {
        char config1[] = "path/to/my.patterns.config";
        char* configs[] = {config1};
        int configs_size = 1;

        if (api->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY, configs, configs_size, NULL, NULL, false)) {
            fprintf(stderr, "Could not initialize tesseract.\n");
            return false;
        }
        api->SetPageSegMode(tesseract::PSM_AUTO);

        std::cout << "OCR engaged\n";
        return true;
    }

    bool initializeCamera() {
        cv::VideoCapture capFace(0);

        if (!capFace.isOpened()) {
            std::cerr << "Camera failed\n";
            return false;
        }
        std::cout << "Camera online\n";
        return true;
    }


    // TODO: finish logic
    std::tuple<std::string, cv::Mat> getCardInfo(const cv::Mat& im)
    {
        // Read in card and preprocess img
        cv::Mat returnFrame = im;
        cv::Mat grayImg = preprocessImage(im);

        // Assume largest contour is the card and get its bounding rectangle
        // def make sure its a rectangle
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(grayImg, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        double maxArea = 0;
        cv::RotatedRect cardRect;
        for (const auto &contour : contours) {
            double area = cv::contourArea(contour);
            if (area > maxArea) {
                maxArea = area;
                cardRect = cv::minAreaRect(contour);
            }
        }

        // Extract the bottom left corner of the card
        cv::Point2f rectPoints[4];
        cardRect.points(rectPoints);

        // this assumes bottom left corner is the third point in rectPoints, check for sure
        cv::Point2f bottomLeft = rectPoints[2];
        // Define the size of the region you want to extract
        int sizeScan = 100; // adjust to control tesserect input region
        cv::Rect roi(bottomLeft.x - sizeScan, bottomLeft.y - sizeScan, sizeScan, sizeScan);

        // Extract and process with Tesseract
        cv::Mat croppedImg = im(roi);
        api->SetImage(croppedImg.data, croppedImg.cols, croppedImg.rows, 3, croppedImg.step);

        outText.reset(api->GetUTF8Text());

        // Get card info from database HERE

        return std::tuple<std::string, cv::Mat> {std::string(outText.get()), returnFrame};
    }

    // scan cards until no card seen for 10 seconds
    void scan()
    {
        Feeder feeder;
        DatabaseManager db("test_collection");
        bool cardsLeft = true; // handle making this false with timer
        bool scanning = false;
        int waitTime = 10;
        int newCardWaitTime = 1;
        bool waitingForCard = false;

        
        while(cardsLeft){
            feeder.feedCard();
            scanning = true;
            while(scanning){
                auto start = std::chrono::system_clock::now();
                cv::Mat frame;
                capFace >> frame;

                if (frame.empty()) {
                    std::cerr << "Error: Empty frame captured." << std::endl;
                    break;
                }

                if(!waitingForCard) {
                    auto cardInfo = getCardInfo(frame);

                    if(!std::get<0>(cardInfo).empty()){
                        waitingForCard = true;
                        auto cardText = std::get<0>(cardInfo);
                        auto cardImg = std::get<1>(cardInfo);
                        // proccess card text
                        auto processedText = cardText;
                        // PLACEHOLDER
                        db.addCard(1, "Black Lotus", 0, "Colorless", "Artifact", "Alpha", "Rare", 0, 0, "Christopher Rush", "PATH");
                        std::this_thread::sleep_for(std::chrono::seconds(newCardWaitTime));
                        feeder.feedCard();
                        continue;

                    }
                } 

                auto now = std::chrono::system_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start);
                if(elapsed.count() >= waitTime) scanning = false;
                start = std::chrono::system_clock::now();
            }
        }
    }

private:
    std::unique_ptr<tesseract::TessBaseAPI, std::function<void(tesseract::TessBaseAPI*)>> api;
    std::unique_ptr<Pix, std::function<void(Pix*)>> image;
    std::unique_ptr<char, std::function<void(char*)>> outText;
    cv::VideoCapture capFace;

    // Function to preprocess the image and return a binarized version
    cv::Mat preprocessImage(const cv::Mat &image) {
        cv::Mat gray, thresh;
        cvtColor(image, gray, cv::COLOR_BGR2GRAY);
        threshold(gray, thresh, 150, 255, cv::THRESH_BINARY_INV);
        return thresh;
    }

    void destroyApi(tesseract::TessBaseAPI* api) {
        if (api) {
            api->End();
            delete api;
        }
    }

    void destroyPix(Pix* p) {
        if (p) {
            pixDestroy(&p);
        }
    }

    void destroyChar(char* c) {
        if (c) {
            delete[] c;
        }
    }

    // std::pair<cv::Mat, cv::Mat> captureCard()
    // {
    //     cv::Mat faceFrame, backFrame;
    //     capFace >> faceFrame;
    //     capBack >> backFrame;
    //     return std::make_pair(faceFrame, backFrame);

    // }


};


// IF NEED TO IMPROVE OUTPUT QUALITY FOLLOW THIS: https://github.com/tesseract-ocr/tessdoc/blob/main/ImproveQuality.md
// This could help: https://github.com/tesseract-ocr/tessdoc/blob/main/APIExample-user_patterns.md
// If totally off try LSTM

// DEFINITELY ADD USER PATTERNS FOR BOTTOM LEFT TEXT
