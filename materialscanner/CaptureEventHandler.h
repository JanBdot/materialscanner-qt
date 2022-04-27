#pragma once
#include "ICaptureEventHandler.h"
#include"GalaxyIncludes.h"
#include<QWidget>
#include<QImage>
#include<QObject>
#include "opencv2/opencv.hpp"
#include "opencv2/core/mat.hpp"

class CaptureEventHandler :
    public ICaptureEventHandler
{
public:
    void DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam);

};

