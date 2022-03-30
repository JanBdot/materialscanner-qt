#include "camera-controller.h"

using namespace std;

cameraController::cameraController()
{
    this->initCamera();
}

cameraController::~cameraController()
{
}

bool cameraController::initCamera()
{
    try {
        cout << "Init camera..." << endl;
        IGXFactory::GetInstance().Init();
    }
    catch (CGalaxyException& e)
    {
        cout << "error code: " << e.GetErrorCode() << endl;
        cout << "error message: " << e.what() << endl;
        return false;
    }
    return true;
}
