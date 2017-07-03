#ifndef _BASLER_UTILS_H_
#define _BASLER_UTILS_H_

#include <pylon/PylonIncludes.h>

const char* type_str(GenApi::EInterfaceType type);
const char* pixeltype_str(Pylon::EPixelType ptype);

void save_image_raw(const char* filename, const Pylon::IImage& image);
bool load_image_raw(const char* filename, Pylon::CPylonImage& image);

void save_image_raw(std::ostream& ostr, const Pylon::IImage& image);
bool load_image_raw(std::istream& istr, Pylon::CPylonImage& image);
                


#endif
