#include "materialscanner.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "camera-controller.h"
#include "lights-controller.h"
#include "mqtt/client.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    materialscanner w;
    lightsController lights = lightsController::lightsController();
    cameraController camera = cameraController::cameraController();
    w.show();
    return a.exec();
}
