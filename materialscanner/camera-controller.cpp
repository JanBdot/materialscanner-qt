#include "camera-controller.h"
#include<qdebug.h>
#include "camera.h"

using namespace std;

cameraController::cameraController()
{
    this->initCamera();
    
    pDeviceOfflineEventHandler = nullptr;
    pFeatureEventHandler = nullptr;
    hDeviceOffline = nullptr;
}

cameraController::~cameraController()
{
}

bool cameraController::initCamera()
{
    isFinished = false;

    //exposure time
    double EPSTime = 0.0;

   
    bool bIsDeviceOpen = false;         
    bool bIsStreamOpen = false;         

    try {
        cout << "Init camera..." << endl;
        camera cameraInstance = camera();  //create instance, assuming it has default constructor!
        cameraInstance.run();
    }
    catch (CGalaxyException& e)
    {
        cout << "error code: " << e.GetErrorCode() << endl;
        cout << "error message: " << e.what() << endl;
        return false;
    }
    return true;
}
