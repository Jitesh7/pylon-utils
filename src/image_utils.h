#ifndef _YCBCR_TO_JPEG_H_
#define _YCBCR_TO_JPEG_H_

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

void ycbcr422_to_jpeg(const char* filename,
                      int quality,
                      size_t width,
                      size_t height,
                      size_t stride,
                      const uint8_t* data);

void ycbcr444_to_ycbcr_422(const uint8_t* src,
                           size_t width, size_t height, size_t src_stride,
                           uint8_t* dst, size_t dst_stride);

void bayer_shuffle(size_t width, size_t height, 
                   const uint8_t* src, size_t src_stride,
                   uint8_t* dst, size_t dst_stride);

void bayer_unshuffle(size_t width, size_t height, 
                     const uint8_t* src, size_t src_stride,
                     uint8_t* dst, size_t dst_stride);

#ifdef __cplusplus
}
#endif

#endif
