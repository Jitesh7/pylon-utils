#ifndef _BASLER_OPENCV_UTILS_H_
#define _BASLER_OPENCV_UTILS_H_

#include <pylon/PylonIncludes.h>
#include <opencv2/core/core.hpp>

// Convert a Pylon image to an OpenCV matrix
cv::Mat pylon_to_cv(const Pylon::CPylonImage& pylon_image);

// Convert an OpenCV matrix to a Pylon image
void cv_to_pylon(const cv::Mat& cv_src,
                 Pylon::EPixelType ptype,
                 Pylon::CPylonImage& pylon_dst);

#endif
