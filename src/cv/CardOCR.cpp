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

    if (api->Init(NULL, "mtg", tesseract::OEM_DEFAULT)) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        return false;
    }
    // api->SetPageSegMode(tesseract::PSM_AUTO);
    api->SetPageSegMode(tesseract::PSM_SINGLE_LINE);

    api->SetVariable("user_defined_dpi", "300");


    std::cout << "OCR engaged" << std::endl;;
    return true;
}

bool CardOCR::initializeCamera() {
    return true;
}

std::tuple<std::string, cv::Mat> CardOCR::getCardName(const cv::Mat& im)
{


    // Process the cropped image with Tesseract
    api->SetImage(im.data, im.cols, im.rows, 3, im.step);
    outText.reset(api->GetUTF8Text());

    // Process OCR output with regex
    std::string ocrOutput = outText.get();
    std::string filteredOutput;
    std::regex wordRegex(R"(\b[a-zA-Z]{5,}\b)"); // Matches words with 5 or more letters


    std::sregex_iterator wordsBegin(ocrOutput.begin(), ocrOutput.end(), wordRegex);
    std::sregex_iterator wordsEnd;

    std::string imagePath = "/home/pi/mtg-collection-manager/src/cv/test.jpg";

    

    
    // emit frameProcessed(returnFrame);
    return std::tuple<std::string, cv::Mat> {std::string(outText.get()), im};
}


void CardOCR::run()
{
    stopRequested = false;
    capFace.open(0);

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
            cv::imwrite("/home/pi/mtg-collection-manager/src/cv/BEFORECROPPING.jpg", frame);
            cv::Mat croppedImage = preprocessImage(frame, 0.28, 0.45, 3.0/4.0, 1.1/6.0);

            
            // Process the cropped image with Tesseract
            auto output = getCardName(croppedImage);

            
            std::string cardText = std::get<0>(output);
            std::cout << cardText << std::endl;

            std::vector<std::string> cardNames;




            // Save the image and add card to database
            std::string imagePath = "/home/pi/mtg-collection-manager/src/database/images" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + ".png";
            // cv::imwrite(imagePath, croppedImage);

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
cv::Mat CardOCR::preprocessImage(const cv::Mat &image, double leftMarginPercent, 
                                 double rightMarginPercent, double topMarginPercent,
                                 double bottomMarginPercent) {
    cv::Mat gray;
    if (image.channels() > 1) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }

    // Apply thresholding
    cv::Mat thresh;
    cv::threshold(gray, thresh, 150, 255, cv::THRESH_BINARY_INV);

    // Calculate the margins as pixel values
    int leftMargin = static_cast<int>(thresh.cols * leftMarginPercent);
    int rightMargin = static_cast<int>(thresh.cols * rightMarginPercent);
    int topMargin = static_cast<int>(thresh.rows * topMarginPercent);
    int bottomMargin = static_cast<int>(thresh.rows * bottomMarginPercent);

    // Ensure margins do not overlap and exceed image boundaries
    leftMargin = std::max(0, std::min(leftMargin, thresh.cols - 1));
    rightMargin = std::max(0, std::min(rightMargin, thresh.cols - 1 - leftMargin));
    topMargin = std::max(0, std::min(topMargin, thresh.rows - 1));
    bottomMargin = std::max(0, std::min(bottomMargin, thresh.rows - 1 - topMargin));

    // Calculate the width and height of the ROI
    int width = thresh.cols - leftMargin - rightMargin;
    int height = thresh.rows - topMargin - bottomMargin;

    // Create the rectangle for the region of interest
    cv::Rect roi(leftMargin, topMargin, width, height);

    // Crop the image to the ROI
    cv::Mat croppedImg = thresh(roi);


    cv::imwrite("/home/pi/mtg-collection-manager/src/cv/cropped.jpg", croppedImg);


    return croppedImg;
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
