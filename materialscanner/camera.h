#pragma once
#include"GalaxyIncludes.h"
#include<QWidget>
#include<QImage>
#include<QObject>
#include "opencv2/opencv.hpp"
#include "opencv2/core/mat.hpp"

class camera : ICaptureEventHandler
{
public:
    camera();
    ~camera();

    void DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam);
    bool run();
};