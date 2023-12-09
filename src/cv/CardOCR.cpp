#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <string>
#include <chrono>
#include <cv/CardOCR.hpp>
#include <regex>




// use openCV to load image and find/ crop bottom left location
 
// tesserect to read the card and return bottom left info text

// if no bgottom left text for tesserect then match by name and image maybe


CardOCR::CardOCR() : api(new tesseract::TessBaseAPI(), [](tesseract::TessBaseAPI* ptr) {
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

CardOCR::~CardOCR() {
    // Destructor implementation (if needed)
}

bool CardOCR::initializeOCR() {
    // char config1[] = "path/to/my.patterns.config";
    // char* configs[] = {config1};
    // int configs_size = 1;

    if (api->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY)) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        return false;
    }
    // api->SetPageSegMode(tesseract::PSM_AUTO);
    api->SetPageSegMode(tesseract::PSM_SINGLE_LINE);

    std::cout << "OCR engaged" << std::endl;;
    return true;
}

bool CardOCR::initializeCamera() {

}

std::tuple<std::string, cv::Mat> CardOCR::getCardName(const cv::Mat& im)
{
    // Read in card and preprocess img
    cv::Mat returnFrame = im;
    cv::Mat grayImg = preprocessImage(im);


    // Extract the top third of the image
    int heightTopThird = im.rows / 2;
    cv::Rect roi(0, 0, im.cols, heightTopThird); // ROI covering the top third
    cv::imwrite("/home/pi/mtg-collection-manager/src/cv/uncropped.jpg", im);

    cv::Mat croppedImg = im(roi); // Crop the image to the top third
    cv::imwrite("/home/pi/mtg-collection-manager/src/cv/cropped.jpg", croppedImg);

    // Process the cropped image with Tesseract
    api->SetVariable("user_defined_dpi", "300");
    api->SetImage(croppedImg.data, croppedImg.cols, croppedImg.rows, 3, croppedImg.step);
    outText.reset(api->GetUTF8Text());

    // Process OCR output with regex
    std::string ocrOutput = outText.get();
    std::string filteredOutput;
    std::regex wordRegex(R"(\b[a-zA-Z]{5,}\b)"); // Matches words with 5 or more letters


    std::sregex_iterator wordsBegin(ocrOutput.begin(), ocrOutput.end(), wordRegex);
    std::sregex_iterator wordsEnd;

    std::string imagePath = "/home/pi/mtg-collection-manager/src/cv/test.jpg";

    

    
    // emit frameProcessed(returnFrame);
    return std::tuple<std::string, cv::Mat> {std::string(outText.get()), returnFrame};
}


void CardOCR::run()
{
    stopRequested = false;
    cv::VideoCapture capFace(0);

    if (!capFace.isOpened()) {
        std::cerr << "Camera failed" << std::endl;
    }
    else {
        std::cout << "Camera online" << std::endl;
    }

    capFace.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    capFace.set(cv::CAP_PROP_FRAME_HEIGHT, 720);


    // bool cardsLeft = true; // handle making this false with timer


    emit feedCardRequested();
    emit requestProcessingDelay(3000);
}

void CardOCR::processCard() {
    
    if(capFace.isOpened()){
        cv::Mat frame;
        capFace.read(frame);

        if (frame.empty()) {
            std::cerr << "Error: Empty frame captured." << std::endl;
            emit finishedScanning();
            stopRequested = true;

        } else {
            // Emit signal for processed frame
            // emit frameProcessed(frame);
            cv::Mat croppedImage = preprocessImage(frame);
            
            // Process the cropped image with Tesseract
            auto output = getCardName(croppedImage);

            
            std::string cardText = std::get<0>(output);
            std::cout << cardText << std::endl;

            std::vector<std::string> cardNames;




            // Save the image and add card to database
            std::string imagePath = "/home/pi/mtg-collection-manager/src/database/images" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + ".png";
            cv::imwrite(imagePath, croppedImage);

            emit findAndAddMostSimilarCard(cardText, imagePath);

        }
    }

    // std::this_thread::sleep_for(std::chrono::seconds(1));

}

void CardOCR::onProcessTimerTimeout() {
    processCard(); // Process the card

    if (!stopRequested) {
        emit feedCardRequested();
    } else {
        capFace.release(); // Release the camera
        emit finishedScanning(); // Notify that the process is finished
    }
}

void CardOCR::onFeedTimerTimeout() {
    processCard(); // Process the card

    if (!stopRequested) {
        emit feedCardRequested();
    } else {
        capFace.release(); // Release the camera
        emit finishedScanning(); // Notify that the process is finished
    }
}

void CardOCR::handleStopScanning() {
    stopRequested = true;
}


// Function to preprocess the image and return a binarized version
cv::Mat CardOCR::preprocessImage(const cv::Mat &image) {
    cv::Mat gray, thresh;
    cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    threshold(gray, thresh, 150, 255, cv::THRESH_BINARY_INV);

    // Calculate the dimensions of the bottom third
    int sideMargin = thresh.cols * 0.3; // 10% of the width of the image for each side

    // Calculate the dimensions of the cropped area
    int x = sideMargin; // New X coordinate, 10% in from the left edge
    int y = thresh.rows * 2 / 3; // Y coordinate, two-thirds down from the top
    int width = thresh.cols - 2 * sideMargin; // Width of the ROI, minus the side margins
    int height = thresh.rows / 3; // Height of the ROI, one-third of the image height

    // Create the rectangle for the region of interest
    cv::Rect roi(x, y, width, height);

    // Crop the image
    cv::Mat croppedImage = thresh(roi);

    cv::Mat flippedImage;
    cv::flip(croppedImage, flippedImage, 1);

    return flippedImage;
}



void CardOCR::destroyApi(tesseract::TessBaseAPI* api) {
    if (api) {
        api->End();
        delete api;
    }
}

void CardOCR::destroyPix(Pix* p) {
    if (p) {
        pixDestroy(&p);
    }
}

void CardOCR::destroyChar(char* c) {
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





// IF NEED TO IMPROVE OUTPUT QUALITY FOLLOW THIS: https://github.com/tesseract-ocr/tessdoc/blob/main/ImproveQuality.md
// This could help: https://github.com/tesseract-ocr/tessdoc/blob/main/APIExample-user_patterns.md
// If totally off try LSTM

// DEFINITELY ADD USER PATTERNS FOR BOTTOM LEFT TEXT
