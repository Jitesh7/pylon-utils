#include "basler_utils.h"
#include "basler_opencv_utils.h"
#include "image_utils.h"
#include "Profile.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <unistd.h>

using namespace Pylon;



class OutputFileWrapper {
public:
    
    FILE* fp;

    OutputFileWrapper(const char* filename) {
        fp = fopen(filename, "wb");
    }

    
    ~OutputFileWrapper() {
        fflush(fp);
        fsync(fileno(fp)); // make sure writes go to disk for benchmarking!
        fclose(fp);
    }
    
};

void write_png(const char* filename,
               const IImage& image) {

    OutputFileWrapper w(filename);

    EPixelType ptype = image.GetPixelType();

    size_t stride = image.GetWidth() * BitPerPixel(ptype) / 8 + image.GetPaddingX();
    
    png_pixel_format format = ( SamplesPerPixel(ptype) == 3 ?
                                PNG_PIXEL_FORMAT_RGB :
                                PNG_PIXEL_FORMAT_GRAY );
    
    write_png(w.fp, format, 1,
              image.GetWidth(), image.GetHeight(), stride,
              (const uint8_t*)image.GetBuffer());

}
                           
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

        std::cout << "loaded YUV & Bayer input images with size "
                  << width << "x" << height << "\n";

        PROFILE("wrote ycbr422.jpg") {
            OutputFileWrapper wrapper("ycbr422.jpg");
            ycbcr422_to_jpeg(wrapper.fp, 100,
                             width, height, 
                             2*width + p_yuv.GetPaddingX(),
                             (const uint8_t*)p_yuv.GetBuffer());
        }

        CImageFormatConverter fc;
        fc.OutputPixelFormat = PixelType_RGB8packed;

        CPylonImage p_yuv2rgb;

        PROFILE("converted YUV -> RGB") {
            fc.Convert(p_yuv2rgb, p_yuv);
        }

        PROFILE("wrote ycbr422.png via libpng") {
            write_png("ycbr422.png", p_yuv2rgb);
        }
        
        CPylonImage p_bayer2 = CPylonImage::Create(PixelType_Mono8,
                                                   width, height);

        CPylonImage p_bayer3 = CPylonImage::Create(PixelType_Mono8,
                                                   width, height);

        PROFILE ("wrote bayerBG8_raw.png") {
            write_png("bayerBG8_raw.png", p_bayer);
        }

        PROFILE("de-interleaved bayer data") {
            bayer_deinterleave(width, height,
                               (const uint8_t*)p_bayer.GetBuffer(), width,
                               (uint8_t*)p_bayer2.GetBuffer(), width);
        }
        
        PROFILE("wrote bayerBG8_raw2.png") {
            write_png("bayerBG8_raw2.png", p_bayer2);
        }

        PROFILE("re-interleaved bayer data (same as orig)") {
            bayer_interleave(width, height,
                             (const uint8_t*)p_bayer2.GetBuffer(), width,
                             (uint8_t*)p_bayer3.GetBuffer(), width);

        }

        PROFILE("wrote bayerBG8_raw3.png") {
            write_png("bayerBG8_raw3.png", p_bayer3);
        }

        CPylonImage p_bayer2rgb;
        
        PROFILE("used Pylon to convert Bayer -> RGB") {
            fc.Convert(p_bayer2rgb, p_bayer);
        }

        PROFILE("wrote bayerBG8_p.png") {
            write_png("bayerBG8_p.png", p_bayer2rgb);
        }
            

        cv::Mat cv_bayer2rgb_cv;

        PROFILE("used OpenCV to convert Bayer -> RGB") {
            cv::cvtColor(pylon_to_cv(p_bayer),
                         cv_bayer2rgb_cv, CV_BayerBG2RGB);
        }

        PROFILE ("wrote bayerBG8_cv.png") {
            cv::imwrite("bayerBG8_cv.png", cv_bayer2rgb_cv);
            sync(); // my write_png calls fsync() for benchmarking purposes, opencv doesn't
        }
        
    } catch (GenericException& e) {

        std::cerr << "error: " << e.GetDescription() << "\n";
        return 1;

    }

    return 0;
    
}
