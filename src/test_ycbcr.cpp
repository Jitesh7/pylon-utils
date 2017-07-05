#include "basler_utils.h"
#include "basler_opencv_utils.h"
#include "image_utils.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace Pylon;

                           
int main(int argc, char** argv) {

    PylonInitialize();

    try {

        CPylonImage p_yuv;
        CPylonImage p_bayer;
        
        if (!(load_image_raw("../images/debug_YCbCr422_8.pylon_raw", p_yuv) &&
              load_image_raw("../images/debug_BayerBG8.pylon_raw", p_bayer))) {

            std::cout << "error loading image!\n";
            exit(1);

        }

        uint32_t width = p_yuv.GetWidth();
        uint32_t height = p_yuv.GetHeight();

        if (p_bayer.GetWidth() != width ||
            p_bayer.GetHeight() != height) {
            
            std::cout << "size mismatch!\n";
            exit(1);

        }

        std::cout << "loaded YUV & Bayer input images with size " << width << "x" << height << "\n";

        ycbcr422_to_jpeg("ycbr422.jpg", 100,
                         width, height, 
                         2*width + p_yuv.GetPaddingX(),
                         (const uint8_t*)p_yuv.GetBuffer());

        std::cout << "wrote ycbr422.jpg\n";

        CImageFormatConverter fc;
        fc.OutputPixelFormat = PixelType_BGR8packed;

        CPylonImage p_yuv2rgb;
        fc.Convert(p_yuv2rgb, p_yuv);
        std::cout << "converted YUV -> RGB\n";

        cv::imwrite("ycbr422.png", pylon_to_cv(p_yuv2rgb));
        std::cout << "wrote ycbr422.png\n";

        cv::Mat cv_bayer = pylon_to_cv(p_bayer);
        cv::Mat cv_bayer2(cv_bayer.size(), CV_8U);
        cv::Mat cv_bayer3(cv_bayer.size(), CV_8U);

        cv::imwrite("bayerBG8_raw.png", cv_bayer);
        std::cout << "wrote bayerBG8_raw.png\n";

        bayer_shuffle(width, height,
                      cv_bayer.data, width,
                      cv_bayer2.data, width);
        
        std::cout << "shuffled Bayer data\n";
        
        cv::imwrite("bayerBG8_raw2.png", cv_bayer2);
        std::cout << "wrote bayerBG8_raw2.png\n";

        bayer_unshuffle(width, height,
                        cv_bayer2.data, width,
                        cv_bayer3.data, width);
        
        std::cout << "unshuffled Bayer data\n";
        
        cv::imwrite("bayerBG8_raw3.png", cv_bayer3);
        std::cout << "wrote bayerBG8_raw3.png\n";
        
        CPylonImage p_bayer2rgb;
        fc.Convert(p_bayer2rgb, p_bayer);
        std::cout << "used Pylon to convert Bayer -> RGB\n";

        cv::imwrite("bayerBG8_p.png", pylon_to_cv(p_bayer2rgb));
        std::cout << "wrote bayerBG8_p.png\n";

        cv::Mat cv_bayer2rgb_cv;

        cv::cvtColor(cv_bayer, cv_bayer2rgb_cv, CV_BayerBG2RGB);
        std::cout << "used OpenCV to convert Bayer -> RGB\n";

        cv::imwrite("bayerBG8_cv.png", cv_bayer2rgb_cv);
        std::cout << "wrote bayerBG8_cv.png\n";
        
    } catch (GenericException& e) {

        std::cerr << "error: " << e.GetDescription() << "\n";
        return 1;

    }

    return 0;
    
}
