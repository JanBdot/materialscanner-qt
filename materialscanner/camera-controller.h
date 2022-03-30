#pragma once
#include "IGXFactory.h"
#include "GalaxyException.h"

class cameraController
{

public:
    cameraController();
    ~cameraController();

private:
    bool initCamera();
    
};