#pragma once
#include"GalaxyIncludes.h"
#include<QWidget>
#include<QImage>
#include<QObject>
#include "opencv2/opencv.hpp"
#include "opencv2/core/mat.hpp"
#include "CaptureEventHandler.h"

class camera
{
public:
    camera();
    ~camera();

    bool initDevice();
    bool initStream();
    bool takePicture();
    bool unInit();

    bool takePictureEvent();

private:
    CGXDevicePointer objDevicePtr;
    CGXStreamPointer objStreamPtr;
    CGXFeatureControlPointer ObjFeatureControlPtr;
};