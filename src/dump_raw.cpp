#include "basler_utils.h"
#include <iostream>
#include <stdio.h>
#include <fstream>

using namespace Pylon;
using namespace GenApi;


int main(int argc, char** argv) {
 
    PylonInitialize();

    try {

        CInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice());

        camera.RegisterConfiguration( new Pylon::CAcquireSingleFrameConfiguration,
                                      RegistrationMode_ReplaceAll, Cleanup_Delete);

        camera.Open();

        INodeMap &control = camera.GetNodeMap();

        CValuePtr pfmt(control.GetNode("PixelFormat"));
        
        const char* formats[2] = {
            "BayerBG8", "YCbCr422_8"
        };


        std::cout << "sizeof(EPixelType) = " << sizeof(EPixelType) << "\n";
        
        for (int i=0; i<2; ++i) {
            
            pfmt->FromString(formats[i]);

            CGrabResultPtr ptrGrabResult;

            camera.GrabOne(5000, ptrGrabResult);

            bool can_save = CImagePersistence::CanSaveWithoutConversion(ImageFileFormat_Tiff, ptrGrabResult);
                                                                        
            
            std::cout << "got result with format " << pixeltype_str(ptrGrabResult->GetPixelType()) << ", "
                      << "height " << ptrGrabResult->GetHeight() << ", "
                      << "width " << ptrGrabResult->GetWidth() << ", "
                      << "and size " << ptrGrabResult->GetImageSize() << ". "
                      << "can save = " << (can_save ? "YES" : "NO") << "\n";

            char fn1[1024], fn2[1024];

            snprintf(fn1, 1024, "dump_%s.pylon_raw", formats[i]);
            snprintf(fn2, 1024, "dump_%s_copy.pylon_raw", formats[i]);

            save_image_raw(fn1, ptrGrabResult);

            CPylonImage tmp;

            load_image_raw(fn1, tmp);

            save_image_raw(fn2, tmp);
            
        }
        

    } catch (const GenericException& e) {

        std::cerr << "error: " << e.GetDescription() << "\n";
        return 1;

    } catch (std::exception& e) {

        std::cerr << "error: " << e.what() << "\n";
        return 1;
        
    }

    return 0;

}
