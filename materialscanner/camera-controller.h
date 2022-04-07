#pragma once
#include "IGXFactory.h"
#include "GalaxyException.h"

class cameraController
{

public:
    cameraController();
    ~cameraController();

    bool isFinished = false;


private:
    bool initCamera();

    //GX_FEATURE_CALLBACK_HANDLE hFeatureEvent;
    GX_DEVICE_OFFLINE_CALLBACK_HANDLE hDeviceOffline;
    bool isOpenWhiteBalance = false;
    bool m_bTriggerMode;

    IDeviceOfflineEventHandler* pDeviceOfflineEventHandler;
    IFeatureEventHandler* pFeatureEventHandler;

    CGXDevicePointer ObjDevicePtr;
    CGXStreamPointer ObjStreamPtr;
    CGXFeatureControlPointer ObjFeatureControlPtr;
    CGXFeatureControlPointer ObjStreamFeatureControlPtr;
    
};