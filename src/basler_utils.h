#ifndef _BASLER_UTILS_H_
#define _BASLER_UTILS_H_

#include <pylon/PylonIncludes.h>

// Convert GenApi Interface Type Enum to string
const char* type_str(GenApi::EInterfaceType type);

// Conert Pylon Pixel Type Enum to string
const char* pixeltype_str(Pylon::EPixelType ptype);

// Save a raw Pylon image to a file/stream
void save_image_raw(const char* filename, const Pylon::IImage& image);
void save_image_raw(std::ostream& ostr, const Pylon::IImage& image);

// Load a raw Pylon image from a file/stream
bool load_image_raw(const char* filename, Pylon::CPylonImage& image);
bool load_image_raw(std::istream& istr, Pylon::CPylonImage& image);


#endif
