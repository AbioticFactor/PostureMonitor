#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <string>
#include <chrono>
#include <cv/CardOCR.hpp>
#include <regex>
#include <aws/core/Aws.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h>
#include <aws/rekognition/RekognitionClient.h>
#include <aws/rekognition/model/DetectTextRequest.h>
#include <aws/rekognition/model/DetectTextResult.h>

#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h>
#include <aws/core/config/AWSProfileConfigLoader.h>
#include <aws/rekognition/RekognitionClient.h>
#include <aws/rekognition/model/ListCollectionsRequest.h>
#include <aws/rekognition/model/ListCollectionsResult.h>
#include <aws/rekognition/model/CreateCollectionRequest.h>
#include <aws/rekognition/model/IndexFacesRequest.h>
#include <aws/rekognition/model/Image.h>
#include <aws/rekognition/model/SearchFacesByImageRequest.h>
#include <aws/rekognition/model/ListFacesRequest.h>
#include <aws/rekognition/model/ListFacesResult.h>
#include <aws/core/platform/Environment.h>

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>




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
}

bool CardOCR::initializeOCR() {
    // char config1[] = "path/to/my.patterns.config";
    // char* configs[] = {config1};
    // int configs_size = 1;

    if (api->Init(NULL, "eng", tesseract::OEM_DEFAULT)) {
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

std::string CardOCR::execOCR(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::tuple<std::string, cv::Mat> CardOCR::getCardName(const cv::Mat& im) {
    // Initialize the AWS SDK
    Aws::Auth::AWSCredentials awsCredentials("AKIAZ3IGBYQVTIU6XXMA", "WC5pjz0RgfUCuFWD/HbYhB/Q4keXHUSxuwnxAESB");

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    std::string textResult;

    try {
        // Convert OpenCV Mat to a byte array
        std::vector<uchar> buf;
        cv::imencode(".png", im, buf);
        auto byteBuffer = Aws::Utils::ByteBuffer(buf.data(), buf.size());
        try {
            Aws::Rekognition::RekognitionClient rekognitionClient(awsCredentials);

            // Create a request to Rekognition
            Aws::Rekognition::Model::DetectTextRequest request;
            request.WithImage(Aws::Rekognition::Model::Image().WithBytes(byteBuffer));

            // Call DetectText API
            auto outcome = rekognitionClient.DetectText(request);

            if (outcome.IsSuccess()) {
                // Process the result
                const auto& textDetections = outcome.GetResult().GetTextDetections();
                if (!textDetections.empty()) {
                    textResult = textDetections.front().GetDetectedText();
                    std::cout << textResult << std::endl;
                } else {
                    textResult = "No text detected";
                    onProcessTimerTimeout();
                }
            } else {
                // Handle error
                textResult = "Error in DetectText: ";
                textResult += outcome.GetError().GetMessage();
            }

        } catch (const std::exception& e) {
            textResult = "Exception: ";
            textResult += e.what();
        }
        // Create a Rekognition client

    } catch (const std::exception& e) {
        textResult = "Exception: ";
        textResult += e.what();
    }

    // Shutdown the AWS SDK
    Aws::ShutdownAPI(options);

    // Return the extracted text and the image
    return std::make_tuple(textResult, im);
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



    emit feedCardRequested();
    // emit requestProcessingDelay(3000);
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
            cv::Mat croppedImage = preprocessImage(frame, 0.28, 0.45, 1.0/4.0, 3.5/6.0);

            
            // Process the cropped image with Tesseract
            auto output = getCardName(croppedImage);

            
            std::string cardText = std::get<0>(output);
            emit cardTextGotten(cardText);

            std::vector<std::string> cardNames;




            // Save the image and add card to database
            // std::string imagePath = "/home/pi/mtg-collection-manager/src/database/images" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + ".png";
            // cv::imwrite(imagePath, croppedImage);

            emit findAndAddMostSimilarCard(cardText);

        }
    }

}

void CardOCR::onProcessTimerTimeout() {
    // processCard(); // Process the card

    if (!stopRequested) {
        emit feedCardRequested();
        // requestProcessingDelay(5000);
    } else {
        capFace.release();
        emit finishedScanning();
    }
}

void CardOCR::onFeedTimerTimeout() {

}

void CardOCR::handleStopScanning() {
    stopRequested = true;
}


// Function to preprocess the image and return a binarized version
cv::Mat CardOCR::preprocessImage(const cv::Mat &image, double leftMarginPercent, 
                                 double rightMarginPercent, double topMarginPercent,
                                 double bottomMarginPercent) {
    // cv::Mat gray;
    // if (image.channels() > 1) {
    //     cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    // } else {
    //     gray = image.clone();
    // }

    cv::Mat thresh = image;
    // cv::threshold(gray, thresh, 150, 255, cv::THRESH_BINARY_INV);

    int leftMargin = static_cast<int>(thresh.cols * leftMarginPercent);
    int rightMargin = static_cast<int>(thresh.cols * rightMarginPercent);
    int topMargin = static_cast<int>(thresh.rows * topMarginPercent);
    int bottomMargin = static_cast<int>(thresh.rows * bottomMarginPercent);

    leftMargin = std::max(0, std::min(leftMargin, thresh.cols - 1));
    rightMargin = std::max(0, std::min(rightMargin, thresh.cols - 1 - leftMargin));
    topMargin = std::max(0, std::min(topMargin, thresh.rows - 1));
    bottomMargin = std::max(0, std::min(bottomMargin, thresh.rows - 1 - topMargin));

    int width = thresh.cols - leftMargin - rightMargin;
    int height = thresh.rows - topMargin - bottomMargin;

    cv::Rect roi(leftMargin, topMargin, width, height);

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





// IF NEED TO IMPROVE OUTPUT QUALITY FOLLOW THIS: https://github.com/tesseract-ocr/tessdoc/blob/main/ImproveQuality.md
// This could help: https://github.com/tesseract-ocr/tessdoc/blob/main/APIExample-user_patterns.md
// If totally off try LSTM

// DEFINITELY ADD USER PATTERNS FOR BOTTOM LEFT TEXT
