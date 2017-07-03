#include "basler_utils.h"
#include <vector>
#include <string>
#include <fstream>

using namespace Pylon;
using namespace GenApi;

const char* type_str(EInterfaceType type) {

    switch (type) {
#define TOK_PASTE(x, y) x##y
#define HANDLE_CASE(x) case TOK_PASTE(intf, x): return #x;
        HANDLE_CASE(IValue);
        HANDLE_CASE(IBase);
        HANDLE_CASE(IInteger);
        HANDLE_CASE(IBoolean);
        HANDLE_CASE(ICommand);
        HANDLE_CASE(IFloat);
        HANDLE_CASE(IString);
        HANDLE_CASE(IRegister);
        HANDLE_CASE(ICategory);
        HANDLE_CASE(IEnumeration);
        HANDLE_CASE(IEnumEntry);
        HANDLE_CASE(IPort);
#undef TOK_PASTE
#undef HANDLE_CASE
    default: return "UNKNOWN";
    }

}

const char* pixeltype_str(EPixelType ptype) {
#define TOK_PASTE(x, y) x##y
#define HANDLE_CASE(x) case TOK_PASTE(PixelType_,x): return #x;
    switch (ptype) {
    HANDLE_CASE(Mono1packed);
    HANDLE_CASE(Mono2packed);
    HANDLE_CASE(Mono4packed);
    HANDLE_CASE(Mono8);
    HANDLE_CASE(Mono8signed);
    HANDLE_CASE(Mono10);
    HANDLE_CASE(Mono10packed);
    HANDLE_CASE(Mono10p);
    HANDLE_CASE(Mono12);
    HANDLE_CASE(Mono12packed);
    HANDLE_CASE(Mono12p);
    HANDLE_CASE(Mono16);
    HANDLE_CASE(BayerGR8);
    HANDLE_CASE(BayerRG8);
    HANDLE_CASE(BayerGB8);
    HANDLE_CASE(BayerBG8);
    HANDLE_CASE(BayerGR10);
    HANDLE_CASE(BayerRG10);
    HANDLE_CASE(BayerGB10);
    HANDLE_CASE(BayerBG10);
    HANDLE_CASE(BayerGR12);
    HANDLE_CASE(BayerRG12);
    HANDLE_CASE(BayerGB12);
    HANDLE_CASE(BayerBG12);
    HANDLE_CASE(RGB8packed);
    HANDLE_CASE(BGR8packed);
    HANDLE_CASE(RGBA8packed);
    HANDLE_CASE(BGRA8packed);
    HANDLE_CASE(RGB10packed);
    HANDLE_CASE(BGR10packed);
    HANDLE_CASE(RGB12packed);
    HANDLE_CASE(BGR12packed);
    HANDLE_CASE(RGB16packed);
    HANDLE_CASE(BGR10V1packed);
    HANDLE_CASE(BGR10V2packed);
    HANDLE_CASE(YUV411packed);
    HANDLE_CASE(YUV422packed);
    HANDLE_CASE(YUV444packed);
    HANDLE_CASE(RGB8planar);
    HANDLE_CASE(RGB10planar);
    HANDLE_CASE(RGB12planar);
    HANDLE_CASE(RGB16planar);
    HANDLE_CASE(YUV422_YUYV_Packed);
    HANDLE_CASE(BayerGR12Packed);
    HANDLE_CASE(BayerRG12Packed);
    HANDLE_CASE(BayerGB12Packed);
    HANDLE_CASE(BayerBG12Packed);
    HANDLE_CASE(BayerGR10p);
    HANDLE_CASE(BayerRG10p);
    HANDLE_CASE(BayerGB10p);
    HANDLE_CASE(BayerBG10p);
    HANDLE_CASE(BayerGR12p);
    HANDLE_CASE(BayerRG12p);
    HANDLE_CASE(BayerGB12p);
    HANDLE_CASE(BayerBG12p);
    HANDLE_CASE(BayerGR16);
    HANDLE_CASE(BayerRG16);
    HANDLE_CASE(BayerGB16);
    HANDLE_CASE(BayerBG16);
    HANDLE_CASE(RGB12V1packed);
    HANDLE_CASE(Double);
#undef HANDLE_CASE
#undef TOK_PASTE
    default: return "UNKNOWN";
    }
            
}

template <class Tval>
void write_raw(std::ostream& ostr, const Tval tval) {
    ostr.write((const char*)&tval, sizeof(Tval));
}

template <class Tval>
bool read_raw(std::istream& istr, Tval& tval) {
    return istr.read((char*)&tval, sizeof(Tval));
}

void save_image_raw(std::ostream& ostr, const IImage& image) {

    // pixel type
    ostr.write("PYLONRAW", 8);
    write_raw(ostr, uint64_t(image.GetPixelType()));
    write_raw(ostr, uint64_t(image.GetImageSize()));

    write_raw(ostr, uint32_t(image.GetWidth()));
    write_raw(ostr, uint32_t(image.GetHeight()));
    write_raw(ostr, uint32_t(image.GetPaddingX()));
    write_raw(ostr, uint32_t(image.GetOrientation()));

    ostr.write((const char*)image.GetBuffer(),
               image.GetImageSize());
    
}


bool load_image_raw(std::istream& istr, CPylonImage& image) {

    char header[9];
    uint64_t pixel_type, image_size;
    uint32_t width, height, padding_x, orientation;

    istr.read(header, 8);
    header[8] = 0;

    if (header != std::string("PYLONRAW")) {
        return false;
    }

    if (!(read_raw(istr, pixel_type) &&
          read_raw(istr, image_size) &&
          read_raw(istr, width) &&
          read_raw(istr, height) &&
          read_raw(istr, padding_x) &&
          read_raw(istr, orientation))) {

        return false;
        
    }

    std::cout << "read image size " << image_size << " width " << width << " height " << height << "\n";

    std::vector<uint8_t> buf(image_size);

    istr.read((char*)&buf[0], image_size);

    image.CopyImage(&buf[0], image_size, EPixelType(pixel_type),
                    width, height, padding_x, EImageOrientation(orientation));
    
    return true;
    
}


void save_image_raw(const char* filename, const Pylon::IImage& image) {
    std::ofstream ostr(filename);
    save_image_raw(ostr, image);
}
    
bool load_image_raw(const char* filename, Pylon::CPylonImage& image) {
    std::ifstream istr(filename);
    if (!istr.is_open()) { return false; }
    return load_image_raw(istr, image);
}
