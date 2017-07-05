#include "basler_opencv_utils.h"
#include <stdexcept>

cv::Mat pylon_to_cv(const Pylon::CPylonImage& pylon_image) {
                    

    Pylon::EPixelType ptype = pylon_image.GetPixelType();

    int bpp = Pylon::BitPerPixel(ptype);
    int spp = Pylon::SamplesPerPixel(ptype);

    int base_type = -1;

    if (bpp == 8*spp) {
        base_type = CV_8U;
    } else if (bpp == 16*spp) {
        base_type = CV_16U;
    } else {
        throw std::runtime_error("unsupported BitPerPixel in pylon_to_cv");
    }

    int bytes_per_pixel = bpp / 8;

    uint32_t width = pylon_image.GetWidth();
    uint32_t height = pylon_image.GetHeight();
    uint32_t padding = pylon_image.GetPaddingX();
    
    if (padding % bytes_per_pixel != 0) {
        throw std::runtime_error("unsupported padding X in pylon_to_cv");
    }
    
    uint32_t computed_width = width + padding / bytes_per_pixel;

    cv::Mat rval(height, computed_width,
                 CV_MAKETYPE(base_type, spp),
                 (uint8_t*)pylon_image.GetBuffer());

    if (computed_width == pylon_image.GetWidth()) {

        return rval;

    } else {

        cv::Rect roi(0, 0, width, height);
        return cv::Mat(rval, roi);

    }
    
}

void cv_to_pylon(const cv::Mat& cv_src,
                 Pylon::EPixelType ptype,
                 Pylon::CPylonImage& pylon_dst) {

    int depth = cv_src.depth();
    int bits_per_sample;

    if (depth == CV_8U) {
        bits_per_sample = 8;
    } else if (depth == CV_16U) {
        bits_per_sample = 16;
    } else {
        throw std::runtime_error("unsupported depth in cv_to_pylon");
    }

    int bits_per_pixel = bits_per_sample * cv_src.channels();
    int bytes_per_pixel = bits_per_pixel / 8;

    if (bits_per_pixel != Pylon::BitPerPixel(ptype)) {
        throw std::runtime_error("pixel format mismatch in cv_to_pylon");
    }



    uint32_t width = cv_src.cols;
    uint32_t height = cv_src.rows;
    uint32_t padding = cv_src.step[0] - width * bytes_per_pixel;

    pylon_dst.AttachUserBuffer(cv_src.data,
                               height * (width*bytes_per_pixel + padding),
                               ptype,
                               width, height, padding);
    
}
