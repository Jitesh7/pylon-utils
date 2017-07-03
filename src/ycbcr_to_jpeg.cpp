#include "ycbcr_to_jpeg.h"
#include <stdio.h>
#include <jpeglib.h>
#include <string.h>


void compress(jpeg_compress_struct* cinfo, const uint8_t* yuv, int stride) {

    for (int i=0; i<3; ++i) {
        printf("inputting %d sample rows of channel %d\n",
               (int)cinfo->comp_info[i].v_samp_factor*DCTSIZE, (int)i);
        printf("channel %d has %d samples in row\n",
               i, (int)(cinfo->comp_info[i].width_in_blocks*DCTSIZE));
        printf("channel %d has %d rows in image\n",
               i, (int)(cinfo->comp_info[i].height_in_blocks*DCTSIZE));

        printf("channel %d has downsampled width %d\n",
               i, (int)(cinfo->comp_info[i].downsampled_width));

        printf("channel %d has h_samp_factor %d\n",
               i, (int)(cinfo->comp_info[i].h_samp_factor));

        printf("channel %d has MCU_sample_width %d\n",
               i, (int)(cinfo->comp_info[i].MCU_sample_width));


        printf("channel %d has DCT_h_scaled_size %d\n",
               i, (int)(cinfo->comp_info[i].DCT_h_scaled_size));
        
    }

    JSAMPROW y[16];
    JSAMPROW cb[16];
    JSAMPROW cr[16];
    JSAMPARRAY planes[3];
    
    planes[0] = y;
    planes[1] = cb;
    planes[2] = cr;
    
    JDIMENSION width = cinfo->image_width;
    JDIMENSION height = cinfo->image_height;
    
    uint8_t* y_data = new uint8_t [16 * width];
    uint8_t* cb_data = new uint8_t [16 * width];
    uint8_t* cr_data = new uint8_t [16 * width];

    memset(cb_data, 128, 16*width);
    memset(cr_data, 128, 16*width);
    
    
    // Jpeg library ignores the rows whose indices are greater than height.
    for (size_t i = 0; i < 16; i++) {
        // y row
        y[i] = y_data + i*width;
        cb[i] = cb_data + i*width;
        cr[i] = cr_data + i*width;
    }

    while (cinfo->next_scanline < cinfo->image_height) {

        for (size_t i=0; i<16; ++i) {
            
            const uint8_t* yuv_px = yuv;

            //cb[i] = (JSAMPLE*)yuv_px+1;
            //cr[i] = (JSAMPLE*)yuv_px+3;
            
            for (size_t x=0; x<width/2; ++x) {

                y[i][2*x+0] = yuv_px[0];
                y[i][2*x+1] = yuv_px[2];

                cb[i][2*x+0] = yuv_px[1];
                cb[i][2*x+1] = yuv_px[1];

                cr[i][2*x+0] = yuv_px[3];
                cr[i][2*x+1] = yuv_px[3];
                
                yuv_px += 4;

            }
            
            yuv += stride;
            
        }

        
        jpeg_write_raw_data(cinfo, planes, 16);
        
    }
    
    delete [] y_data;
    delete [] cb_data;
    delete [] cr_data;

}


void ycbcr_422_to_jpeg(const char* filename,
                       int quality,
                       size_t width,
                       size_t height,
                       size_t stride,
                       const uint8_t* data) {

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

    cinfo.dct_method = JDCT_IFAST;

    jpeg_set_quality(&cinfo, quality, TRUE);


    //////////////////////////////////////////////////////////////////////

    cinfo.raw_data_in = TRUE;
    
    cinfo.comp_info[0].h_samp_factor = 2;
    cinfo.comp_info[0].v_samp_factor = 2;
    cinfo.comp_info[1].h_samp_factor = 1;
    cinfo.comp_info[1].v_samp_factor = 2;
    cinfo.comp_info[2].h_samp_factor = 1;
    cinfo.comp_info[2].v_samp_factor = 2;

    //////////////////////////////////////////////////////////////////////

    jpeg_start_compress(&cinfo, TRUE);
    
    compress(&cinfo, data, stride);
    
    jpeg_finish_compress(&cinfo);

    fclose(outfile);

}


