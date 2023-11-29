// Finish setting up Conan and Cmake
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <string>




// use openCV to load image and find/ crop bottom left location
 
// tesserect to read the card and return bottom left info text

// if no bgottom left text for tesserect then match by name and image maybe

// if neither (land? then match by set then image (find set symbol))

class cardOCR
{
private:
    std::unique_ptr<tesseract::TessBaseAPI> api;
    Pix *image;
    char *outText;


    // can directly get card data if bottom left exists
    std::string detectBottomLeft(cv::Mat im)
    {
        return "pass";
    }

    // match up card name and picture to get output
    std::string detectNamePicture(cv::Mat im)
    {
        return "pass";
    }


public:
    cardOCR()
    {
        char config1[] = "path/to/my.patterns.config";
        char *configs[] = {config1};
        int configs_size = 1;

        if (api->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY, configs, configs_size, NULL, NULL, false)) 
        {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
        // Page Seg stuff: https://pyimagesearch.com/2021/11/15/tesseract-page-segmentation-modes-psms-explained-how-to-improve-your-ocr-accuracy/
        api->SetPageSegMode(tesseract::PSM_AUTO);

        }
    }

    ~cardOCR()
    {
        api->End();
        delete [] outText;
        pixDestroy(&image);
    }

    // Function to preprocess the image and return a binarized version
    cv::Mat preprocessImage(const cv::Mat &image) {
        cv::Mat gray, thresh;
        cvtColor(image, gray, cv::COLOR_BGR2GRAY);
        threshold(gray, thresh, 150, 255, cv::THRESH_BINARY_INV);
        return thresh;
    }

    // TODO: finish logic
    std::string getCardInfo(std::string imPath)
    {
        // Read in card and preprocess img
        cv::Mat im = cv::imread(imPath, cv::IMREAD_COLOR);
        cv::Mat grayImg = preprocessImage(im);

        // Assume largest contour is the card and get its bounding rectangle
        // def make sure its a rectangle
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(grayImg, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        double maxArea = 0;
        cv::Rect cardRect;
        for (const auto &contour : contours) {
            double area = cv::contourArea(contour);
            if (area > maxArea) {
                maxArea = area;
                cardRect = cv::boundingRect(contour);
            }
        }

        // ADJUST ROI
        cv::Mat roi = im(cardRect).clone();
        api->SetImage(roi.data, roi.cols, roi.rows, 3, roi.step);

        outText = api->GetUTF8Text();

        // Get card info from database HERE

        return std::string(outText);
    }



};


// IF NEED TO IMPROVE OUTPUT QUALITY FOLLOW THIS: https://github.com/tesseract-ocr/tessdoc/blob/main/ImproveQuality.md
// This could help: https://github.com/tesseract-ocr/tessdoc/blob/main/APIExample-user_patterns.md
// If totally off try LSTM

// DEFINITELY ADD USER PATTERNS FOR BOTTOM LEFT TEXT
