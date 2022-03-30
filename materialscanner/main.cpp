#include "materialscanner.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "camera-controller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    materialscanner w;
    cameraController camera = cameraController::cameraController();
    w.show();
    return a.exec();
}
