#ifndef _YCBCR_TO_JPEG_H_
#define _YCBCR_TO_JPEG_H_

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

void ycbcr_422_to_jpeg(const char* filename,
                       int quality,
                       size_t width,
                       size_t height,
                       size_t stride,
                       const uint8_t* data);

#ifdef __cplusplus
}
#endif

#endif
