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
    tesseract::TessBaseAPI *api;
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
        api = new tesseract::TessBaseAPI(); 
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
        delete api;
        delete [] outText;
        pixDestroy(&image);
    }

    // TODO: finish logic
    std::string getCardInfo(std::string imPath)
    {
        cv::Mat im = cv::imread(imPath, cv::IMREAD_COLOR);
        // now transform the image

        // TALK WITH RAINA HERE: either need a foolproof way to check or need to check all and compare output (database)
        // both, 1, 2
        std::string res = detectBottomLeft(im);
        if(size(res)){
            
        }

        //at the end get the image back and set it here
        api->SetImage(im.data, im.cols, im.rows, 3, im.step);

        outText = api->GetUTF8Text();

        return std::string(outText);
    }



};


// IF NEED TO IMPROVE OUTPUT QUALITY FOLLOW THIS: https://github.com/tesseract-ocr/tessdoc/blob/main/ImproveQuality.md
// This could help: https://github.com/tesseract-ocr/tessdoc/blob/main/APIExample-user_patterns.md
// If totally off try LSTM

// DEFINITELY ADD USER PATTERNS FOR BOTTOM LEFT TEXT
