#include "camera-controller.h"
#include<qdebug.h>

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
        IGXFactory::GetInstance().Init();

        do {
            gxdeviceinfo_vector vectorDeviceInfo;
            IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);
            if (0 == vectorDeviceInfo.size())
            {
                cout << "Kein Geraet verfuegbar " << endl;
                break;
            }

            ObjDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(
                vectorDeviceInfo[0].GetSN(),
                GX_ACCESS_EXCLUSIVE);
            bIsDeviceOpen = true;

            //get remote device property controller
            ObjFeatureControlPtr = ObjDevicePtr->GetRemoteFeatureControl();

            int nStreamCount = ObjDevicePtr->GetStreamCount();

            if (nStreamCount > 0)
            {
                ObjStreamPtr = ObjDevicePtr->OpenStream(0);
                bIsStreamOpen = true;
            }
            else
            {
                cout << "Stream Count = 0" << endl;
            }


            GX_DEVICE_CLASS_LIST objDeviceClass = ObjDevicePtr->GetDeviceInfo().GetDeviceClass();
            if (GX_DEVICE_CLASS_GEV == objDeviceClass)
            {
                // determine whether the device supports the streaming channel packet function
                if (true == ObjFeatureControlPtr->IsImplemented("GevSCPSPacketSize"))
                {
                    int nPacketSize = ObjStreamPtr->GetOptimalPacketSize();
                    ObjFeatureControlPtr->GetIntFeature("GevSCPSPacketSize")->SetValue(nPacketSize);
                }
            }
            bool bBalanceWhiteAutoRead = false;

            
            ObjFeatureControlPtr->GetEnumFeature("AcquisitionMode")->SetValue("Continuous");
            
            m_bTriggerMode = ObjFeatureControlPtr->IsImplemented("TriggerMode");
            if (m_bTriggerMode)
            {
                ObjFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("Off");
            }

 

        } while (0);
    }
    catch (CGalaxyException& e)
    {
        cout << "error code: " << e.GetErrorCode() << endl;
        cout << "error message: " << e.what() << endl;
        return false;
    }
    return true;
}
