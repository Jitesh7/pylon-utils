#ifndef _IMAGE_UTILS_H_
#define _IMAGE_UTILS_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum png_pixel_format {
    PNG_PIXEL_FORMAT_GRAY = 0,
    PNG_PIXEL_FORMAT_BGR = 1,
    PNG_PIXEL_FORMAT_RGB = 2
} png_pixel_format_t;

// Directly save YCbCr 422 data to JPG without intermediate conversion
// to RGB.

void ycbcr422_to_jpeg(FILE* fp,
                      int quality,
                      size_t width,
                      size_t height,
                      size_t stride,
                      const uint8_t* data);

void write_png(FILE* fp,
               png_pixel_format_t format,
               int compression_level, // 0-9
               size_t width,
               size_t height,
               size_t stride,
               const uint8_t* data);

// Downsample from 444 to 422 by discarding alternate Cr and Cb samples.
// Does not perform any interpolation.

void ycbcr444_to_ycbcr_422(const uint8_t* src,
                           size_t width, size_t height, size_t src_stride,
                           uint8_t* dst, size_t dst_stride);

// Deinterleave a raw Bayer pattern image to separate each
// of the four pixel positions into its own quadrant of the image --
// generates PNG files that compress slightly better (approx 15% in my
// limited experiments).

void bayer_deinterleave(size_t width, size_t height, 
                        const uint8_t* src, size_t src_stride,
                        uint8_t* dst, size_t dst_stride);

// Convert back from planar-ized to interleaved.

void bayer_interleave(size_t width, size_t height, 
                      const uint8_t* src, size_t src_stride,
                      uint8_t* dst, size_t dst_stride);

#endif
