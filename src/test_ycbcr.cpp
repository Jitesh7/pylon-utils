#include "basler_utils.h"
#include "ycbcr_to_jpeg.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace Pylon;

void make_test_image_bgr8packed(size_t size, size_t stride,
                                uint8_t* data) {

    uint8_t* rowptr = data;

    for (size_t row=0; row<size; ++row) {

        uint8_t* dst = rowptr;
        
        for (size_t col=0; col<size; ++col) {

            if (row + col < size) {
                
                float r = float(col) / (size-1);
                float b = float(row) / (size-1);
                float g = 1.0 - r - b;

                dst[0] = 255*b;
                dst[1] = 255*g;
                dst[2] = 255*r;

            } else {

                float l = float(row + col - size) / (size);

                dst[0] = dst[1] = dst[2] = 255*l;
                
            }
            

            dst += 3;
            
        }

        rowptr += stride;
        
    }
    
}

void ycbcr444_to_ycbcr_422(const uint8_t* src,
                           size_t width, size_t height, size_t src_stride,
                           uint8_t* dst, size_t dst_stride) {

    const uint8_t* src_row = src;
    uint8_t* dst_row = dst;

    for (size_t row=0; row<height; ++row) {

        const uint8_t* src_px = src_row;
        uint8_t* dst_px = dst_row;
        
        for (size_t col=0; col<width; ++col) {

            uint8_t y = src_px[0];
            uint8_t cb = src_px[1];
            uint8_t cr = src_px[2];

            dst_px[0] = (col % 2 == 0) ? cb : cr;
            dst_px[1] = y;

            src_px += 3;
            dst_px += 2;
            
        }

        src_row += src_stride;
        dst_row += dst_stride;
            
    }

}
                           


int main(int argc, char** argv) {

    PylonInitialize();

    try {

        CPylonImage pylonInput;
        bool ok = load_image_raw("../images/debug_YCbCr422_8.pylon_raw", pylonInput);
        if (!ok) {
            std::cout << "error loading image!\n";
        }

        ycbcr_422_to_jpeg("foo.jpg", 70,
                          pylonInput.GetWidth(),
                          pylonInput.GetHeight(),
                          2*pylonInput.GetWidth() + pylonInput.GetPaddingX(),
                          (const uint8_t*)pylonInput.GetBuffer());
                      


        /*
        cv::Mat result = cv::imread("foo.jpg");
        cv::namedWindow("win");
        cv::imshow("win", result);
        cv::waitKey();
        */

    } catch (GenericException& e) {

        std::cerr << "error: " << e.GetDescription() << "\n";
        return 1;

    }

    return 0;
    
}
