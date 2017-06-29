#include <pylon/PylonIncludes.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>

using namespace Pylon;
using namespace GenApi;

int main(int argc, char** argv) {
 
  PylonInitialize();

  try {

    CInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice());

    camera.Open();

    std::cout << "opened camera.\n";
    
    camera.StartGrabbing();

    std::cout << "started grabbing.\n";

    CGrabResultPtr ptrGrabResult;

    while (camera.IsGrabbing()) {
      
      camera.RetrieveResult(5000, ptrGrabResult, 
                            TimeoutHandling_ThrowException);

      if (ptrGrabResult->GrabSucceeded()) {

        std::cout << "got a frame with size " << ptrGrabResult->GetWidth() << "x" << ptrGrabResult->GetHeight() << "\n";

      }

    }


  } catch (const GenericException& e) {

    std::cerr << "error: " << e.GetDescription() << "\n";
    return 1;

  }

  return 0;

}
