#include "image_utils.h"
#include <stdio.h>
#include <jpeglib.h>
#include <string.h>
#include <vector>
#include <stdexcept>

void ycbcr422_to_jpeg(const char* filename,
                      int quality,
                      size_t width,
                      size_t height,
                      size_t stride,
                      const uint8_t* yuv) {

    if (width % 8) {
        throw std::runtime_error("ycbcr_422_to_jpeg: image width must be divisible by 8!");
    }

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    FILE* outfile = fopen(filename, "wb");

    memset(&cinfo, 0, sizeof(cinfo));
    
    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    //////////////////////////////////////////////////////////////////////

    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_YCbCr;
    
    jpeg_set_defaults(&cinfo);
    
    jpeg_set_colorspace(&cinfo, JCS_YCbCr);
    jpeg_set_quality(&cinfo, quality, TRUE);

    //////////////////////////////////////////////////////////////////////

    cinfo.raw_data_in = TRUE;

#if JPEG_LIB_VERSION >= 70
    cinfo.do_fancy_downsampling = FALSE;
#endif    
    
    cinfo.comp_info[0].h_samp_factor = 2;
    cinfo.comp_info[0].v_samp_factor = 2;
    cinfo.comp_info[1].h_samp_factor = 1;
    cinfo.comp_info[1].v_samp_factor = 2;
    cinfo.comp_info[2].h_samp_factor = 1;
    cinfo.comp_info[2].v_samp_factor = 2;

    //////////////////////////////////////////////////////////////////////

    jpeg_start_compress(&cinfo, TRUE);
    
    JSAMPROW y[16], cb[16], cr[16];
    JSAMPARRAY planes[3] = { y, cb, cr };

    size_t ystride = width;
    size_t uvstride = width/2;
    
    std::vector<uint8_t> y_data(16 * ystride);
    std::vector<uint8_t> cb_data(16 * uvstride);
    std::vector<uint8_t> cr_data(16 * uvstride);
    
    for (size_t i = 0; i < 16; i++) {
        y[i] = &(y_data[0]) + i*ystride;
        cb[i] = &(cb_data[0]) + i*uvstride;
        cr[i] = &(cr_data[0]) + i*uvstride;
    }

    JDIMENSION src_row = 0;

    // TODO: deal with padding in y to 16 rows and padding in x to 8 cols
    while (cinfo.next_scanline < cinfo.image_height) {

        uint8_t* y_dst = y[0];
        uint8_t* cb_dst = cb[0];
        uint8_t* cr_dst = cr[0];

        for (size_t i=0; i<16; ++i) {
            
            const uint8_t* yuv_px = yuv;

            for (size_t x=0; x<uvstride; ++x) {

                *y_dst++ = yuv_px[0];
                *y_dst++ = yuv_px[2];

                *cb_dst++ = yuv_px[1];
                *cr_dst++ = yuv_px[3];
                
                yuv_px += 4;

            }

            // pad MCU by duplicating last row

            if (src_row+1 < cinfo.image_height) {
                yuv += stride;
                ++src_row;
            }
            
        }
        
        jpeg_write_raw_data(&cinfo, planes, 16);
        
    }
    
    jpeg_finish_compress(&cinfo);

    fclose(outfile);

}




void ycbcr444_to_ycbcr_422(const uint8_t* src,
                           size_t width, size_t height, size_t src_stride,
                           uint8_t* dst, size_t dst_stride) {

    if (width % 2) {
        throw std::runtime_error("ycbcr444_to_ycbcr_422: image width must be divisible by 2!");
    }


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

void bayer_shuffle(size_t width, size_t height, 
                   const uint8_t* src, size_t src_stride,
                   uint8_t* dst, size_t dst_stride) {

    if (width % 2 || height % 2) {
        throw std::runtime_error("image dimensions must be divisible by 2!\n");
    }

    size_t hh = height/2;
    size_t hw = width/2;

    for (int dy=0; dy<2; ++dy) {
        for (int dx=0; dx<2; ++dx) {

            uint8_t* dst_row = dst + dst_stride*dy*hh + dx*hw;
            const uint8_t* src_row = src + dx + dy*src_stride;
            
            for (size_t i=0; i<hh; ++i) {
                
                for (size_t j=0; j<hw; ++j) {
                    dst_row[j] = src_row[2*j];
                }
                
                dst_row += dst_stride;
                src_row += 2*src_stride;
                
            }
        }
    }

}

void bayer_unshuffle(size_t width, size_t height, 
                     const uint8_t* src, size_t src_stride,
                     uint8_t* dst, size_t dst_stride) {

    if (width % 2 || height % 2) {
        throw std::runtime_error("image dimensions must be divisible by 2!\n");
    }

    size_t hh = height/2;
    size_t hw = width/2;

    for (int dy=0; dy<2; ++dy) {
        for (int dx=0; dx<2; ++dx) {

            const uint8_t* src_row = src + src_stride*dy*hh + dx*hw;
            uint8_t* dst_row = dst + dx + dy*dst_stride;
            
            for (size_t i=0; i<hh; ++i) {
                
                for (size_t j=0; j<hw; ++j) {
                    dst_row[2*j] = src_row[j];
                }
                
                src_row += src_stride;
                dst_row += 2*dst_stride;
                
            }
        }
    }

}
