#include "camera.h"
//#include<CSampleCaptureEventHandler.h>

camera::camera() {

}

camera::~camera() {

}


bool camera::initDevice()
{
    std::cout << "Initialize camera..." << std::endl;
    try
    {
        // Declare event callback object pointer
        IDeviceOfflineEventHandler* pDeviceOfflineEventHandler = NULL;/// Callback object for offline event
        IFeatureEventHandler* pFeatureEventHandler = NULL;/// remote device event callback object
        ICaptureEventHandler* pCaptureEventHandler = NULL;/// fetch callback object

            // Inicializar
        IGXFactory::GetInstance().Init();

        // Enumerar dispositivos
        gxdeviceinfo_vector vectorDeviceInfo;
        IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);

        int a = 0;
        if (0 == vectorDeviceInfo.size())
        {
            IGXFactory::GetInstance().Uninit();
            throw std::exception("No device available!");
        }

        GxIAPICPP::gxstring strSN = vectorDeviceInfo[0].GetSN();

        objDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(strSN, GX_ACCESS_EXCLUSIVE);

        std::cout << "Initialize Camera finished!" << std::endl;
        return true;
    }
    catch (const std::exception& e)
    {
        std::cout << "!!!Initialize Camera failed!!! " << e.what() << std::endl;
        return false;
    }  
}

bool camera::initStream() {
    try
    {
        std::cout << "Init Stream and DeviceControl..." << std::endl;
        uint32_t nStreamNum = objDevicePtr->GetStreamCount();
        if (nStreamNum > 0)
        {
            objStreamPtr = objDevicePtr->OpenStream(0);
            /*Stream object control or acquisition.*/

            // Abrufen des Remotegeräteattributtreibers
            ObjFeatureControlPtr = objDevicePtr->GetRemoteFeatureControl();
            /*

            ObjFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("On");
            ObjFeatureControlPtr->GetEnumFeature("TriggerSource")->SetValue("Line0");
            ObjFeatureControlPtr->GetEnumFeature("TriggerActivation")->SetValue("RisingEdge");
            ObjFeatureControlPtr->GetFloatFeature("TriggerDelay")->SetValue(500000);
            ObjFeatureControlPtr->GetFloatFeature("TriggerFilterRaisingEdge")->SetValue(5000);
            */

            /*Improve the acquisition performance of the network camera
            [currently only the Gigabit network camera supports the optimal packet length]*/
            GX_DEVICE_CLASS_LIST objDeviceClass = objDevicePtr->GetDeviceInfo().GetDeviceClass();

            if (GX_DEVICE_CLASS_U3V == objDeviceClass)
            {
                /*Determine whether the device supports the stream channel packet function.*/
                if (true == ObjFeatureControlPtr->IsImplemented("GevSCPSPacketSize"))
                {
                    /*Get the optimal packet length value of the current network environment*/
                    int nPacketSize = objStreamPtr->GetOptimalPacketSize();

                    /*Set the optimal packet length value to the stream channel packet length of
                    the current device.*/

                    ObjFeatureControlPtr->GetIntFeature("GevSCPSPacketSize")->SetValue(nPacketSize);

                }
            }
        }
        std::cout << "Init Stream and DeviceControl finished!" << std::endl;
        return true;
    }
    catch (const std::exception&)
    {
        std::cout << "Init Stream and DeviceControl failed!" << std::endl;
        return false;
    }
}

bool camera::takePictureEvent() {
    try
    {
        std::cout << "Take Picture Event..." << std::endl;
        // Callback Collection registrieren
        ICaptureEventHandler* pCaptureEventHandler = new CaptureEventHandler();
        objStreamPtr->RegisterCaptureCallback(pCaptureEventHandler, NULL);

        // Senden von Miningbefehlen
        objStreamPtr->StartGrab();
        ObjFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();

        std::getchar();

        //Befehl "Miningstopp senden"
        ObjFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();
        ObjFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("Off");

        objStreamPtr->StopGrab();

        std::cout << "Picture event saved!" << std::endl;
        return true;
    }
    catch (const std::exception&)
    {
        std::cout << "Take Picture failed!" << std::endl;
        return false;
    }
}

bool camera::takePicture() {
    try
    {
        std::cout << "Take Picture..." << std::endl;
        // Callback Collection registrieren
        //pCaptureEventHandler = new camera();
        //ObjStreamPtr->RegisterCaptureCallback(pCaptureEventHandler, NULL);

        // Senden von Miningbefehlen
        objStreamPtr->StartGrab();
        ObjFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();

        CImageDataPointer objImageDataPtr;

        objImageDataPtr = objStreamPtr->GetImage(500);/* Set timeout is 500ms, users can set the value by themselves.*/

        if (objImageDataPtr->GetStatus() == GX_FRAME_STATUS_SUCCESS)
        {
            cv::Mat img;
            std::cout << "Bild erhalten!" << std::endl;
            std::cout << "ImageInfo: " << objImageDataPtr->GetStatus() << std::endl;
            std::cout << "ImageInfo: " << objImageDataPtr->GetWidth() << std::endl;
            std::cout << "ImageInfo: " << objImageDataPtr->GetHeight() << std::endl;
            std::cout << "ImageInfo: " << objImageDataPtr->GetPayloadSize() << std::endl;

    //        /*
    //        void* pRGB24Buffer = NULL;
    //        // Angenommen, die Originaldaten sind ein BayerRG8-Bild
    //        pRGB24Buffer = objImageDataPtr->ConvertToRGB24(GX_BIT_0_7, GX_RAW2RGB_NEIGHBOUR, true);

    //        int width = objImageDataPtr->GetWidth();
    //        int height = objImageDataPtr->GetHeight();

    //        // Welche konvertierungsmatrix braucht man?
    //        cv::Mat openCvImage(height, width, CV_16UC1, pRGB24Buffer);
    //        openCvImage.convertTo(openCvImage, CV_8UC1);

    //        QImage qImage = QImage(
    //            openCvImage.data,
    //            width,
    //            height,
    //            QImage::Format_ARGB32
    //        );
    //        */

            img.create(objImageDataPtr->GetHeight(), objImageDataPtr->GetWidth(), CV_8UC3);
            void* pRGB24Buffer = NULL;
            // Angenommen, die Originaldaten sind ein BayerRG8-Bild
            //pRGB24Buffer = objImageDataPtr->ConvertToRaw8(GX_BIT_0_7);
            pRGB24Buffer = objImageDataPtr->ConvertToRGB24(GX_BIT_0_7, GX_RAW2RGB_NEIGHBOUR, true);
            memcpy(img.data, pRGB24Buffer, (objImageDataPtr->GetHeight()) * (objImageDataPtr->GetWidth()) * 3);
            cv::flip(img, img, 0);
            cv::rotate(img, img, cv::ROTATE_90_COUNTERCLOCKWISE);
            //cv::imshow("sss", img);
            cv::waitKey(1);
            cv::imwrite("03.bmp", img);

            QImage imgIn = QImage((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
            imgIn.save(QString("01.bmp"), "bmp");
        }

        //Befehl "Miningstopp senden"
        ObjFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();
        ObjFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("Off");

        objStreamPtr->StopGrab();

        std::cout << "Picture saved!" << std::endl;
        return true;
    }
    catch (const std::exception&)
    {
        std::cout << "Take Picture failed!" << std::endl;
        return false;
    }
}

bool camera::unInit() {
    try
    {
        std::cout << "UnInit..." << std::endl;
        objDevicePtr->Close();

        //Deinitialisieren der Bibliothek
        IGXFactory::GetInstance().Uninit();

        /*
        //Zerstört den Ereignisrückrufzeiger
        if (NULL != pCaptureEventHandler)
        {
            delete pCaptureEventHandler;
            pCaptureEventHandler = NULL;
        }
        */

        //Rückruf beim Abmelden der Sammlung
        objStreamPtr->UnregisterCaptureCallback();

        //Ressourcen freisetzen
        objStreamPtr->Close();

        std::cout << "UnInit complete!" << std::endl;
        return true;
    }
    catch (const std::exception&)
    {
        std::cout << "UnInit failed!" << std::endl;
    }
}