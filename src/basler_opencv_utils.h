#ifndef _BASLER_OPENCV_UTILS_H_
#define _BASLER_OPENCV_UTILS_H_

#include <pylon/PylonIncludes.h>
#include <opencv2/core/core.hpp>

cv::Mat pylon_to_cv(const Pylon::CPylonImage& pylon_image);

void cv_to_pylon(const cv::Mat& cv_src,
                 Pylon::EPixelType ptype,
                 Pylon::CPylonImage& pylon_dst);


#endif
