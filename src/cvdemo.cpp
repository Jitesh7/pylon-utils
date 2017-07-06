#include "basler_opencv_utils.h"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace Pylon;
using namespace GenApi;

int main(int argc, char** argv) {
 
    PylonInitialize();

    cv::namedWindow("display");

    try {

        CInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice());

        camera.Open();
    
        std::cout << "opened camera.\n";

        camera.StartGrabbing();

        std::cout << "started grabbing.\n";

        CGrabResultPtr ptrGrabResult;

        CImageFormatConverter fc;
        fc.OutputPixelFormat = PixelType_BGR8packed;

        CPylonImage image;    

        while (camera.IsGrabbing()) {
      
            camera.RetrieveResult(5000, ptrGrabResult, 
                                  TimeoutHandling_ThrowException);

            if (ptrGrabResult->GrabSucceeded()) {

                uint32_t width = ptrGrabResult->GetWidth();
                uint32_t height = ptrGrabResult->GetHeight();

                size_t stride;
                bool ok = ptrGrabResult->GetStride(stride);

                std::cout << "got a frame with size " << width << "x" << height << "\n";

                fc.Convert(image, ptrGrabResult);

                cv::imshow("display", pylon_to_cv(image));
                
                int k = cv::waitKey(5);
                
                if (k == 27) {
                    break;
                }
        
            }

        }

    } catch (const GenericException& e) {

        std::cerr << "error: " << e.GetDescription() << "\n";
        return 1;

    }

    return 0;

}
