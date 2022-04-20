#include "materialscanner.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "camera-controller.h"
#include "lights-controller.h"

#include "mqtt/client.h"
#include "opencv2/opencv.hpp"
#include<iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    materialscanner w;
    lightsController lightsController = lightsController::lightsController();
    cameraController cameraController = cameraController::cameraController();

    //string address = "./image/9demo.bmp";
    //cv::Mat src = cv::imread(address);

    w.show();
    return a.exec();
}
