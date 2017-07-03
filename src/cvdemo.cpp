#include <pylon/PylonIncludes.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
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


        /*

        INodeMap &control = camera.GetNodeMap();
        CValuePtr pfmt(control.GetNode("PixelFormat"));
        pfmt->FromString("YCbCr422_8");
        pfmt->FromString("BGR8");
        pfmt->FromString("BayerBG8");
        
        pfmt->FromString("YCbCr422_8");

        CValuePtr timg(control.GetNode("TestImageSelector"));
        timg->FromString("Testimage6");

        CIntegerPtr hdec(control.GetNode("DecimationHorizontal"));
        hdec->SetValue(1);

        CIntegerPtr vdec(control.GetNode("DecimationVertical"));
        vdec->SetValue(1);
        */


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

                size_t extra = 2*width-stride;

                std::cout << "got a frame with size " << width << "x" << height << "\n";
                std::cout << "ok is " << ok << " and stride is " << stride << "\n";
                std::cout << "I get extra is " << extra << " and result is " << ptrGrabResult->GetPaddingX() << "\n";

                fc.Convert(image, ptrGrabResult);

                cv::Mat cimg(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(),
                             CV_8UC3, (uint8_t *)image.GetBuffer());

                cv::imshow("display", cimg);
                cv::waitKey(5);
        

            }

        }


    } catch (const GenericException& e) {

        std::cerr << "error: " << e.GetDescription() << "\n";
        return 1;

    }

    return 0;

}
