#include"GalaxyIncludes.h"
#include<QWidget>
#include<QImage>
#include<QObject>
#include "camera.h"
//#include<CSampleCaptureEventHandler.h>


using namespace std;

camera::camera() {

}

camera::~camera() {

}

void camera::DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam) // Callback function processing
{
    cout << "Bild erhalten" << endl;
    cout << "ImageInfo: " << objImageDataPointer->GetStatus() << endl;
    cout << "ImageInfo: " << objImageDataPointer->GetWidth() << endl;
    cout << "ImageInfo: " << objImageDataPointer->GetHeight() << endl;
    cout << "ImageInfo: " << objImageDataPointer->GetPayloadSize() << endl;
}



bool camera::run()
{
    cout << "Run code reached..." << endl;
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
        cout << "No device available!" << endl;

        // Desinicializar la biblioteca
        IGXFactory::GetInstance().Uninit();
        return false;
    }

    // Abre el primer dispositivo y la primera secuencia debajo del dispositivo
    CGXDevicePointer objDevicePtr;

    GxIAPICPP::gxstring strSN = vectorDeviceInfo[0].GetSN();

    objDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(strSN, GX_ACCESS_EXCLUSIVE);

    uint32_t nStreamNum = objDevicePtr->GetStreamCount();
    if (nStreamNum > 0)
    {
        CGXStreamPointer objStreamPtr = objDevicePtr->OpenStream(0);
        /*Stream object control or acquisition.*/

        // Abrufen des Remotegeräteattributtreibers
        CGXFeatureControlPointer ObjFeatureControlPtr = objDevicePtr->GetRemoteFeatureControl();
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
            cout << "Bild erhalten!" << endl;
            cout << "ImageInfo: " << objImageDataPtr->GetStatus() << endl;
            cout << "ImageInfo: " << objImageDataPtr->GetWidth() << endl;
            cout << "ImageInfo: " << objImageDataPtr->GetHeight() << endl;
            cout << "ImageInfo: " << objImageDataPtr->GetPayloadSize() << endl;

            void* pRGB24Buffer = NULL;
            // Angenommen, die Originaldaten sind ein BayerRG8-Bild
            pRGB24Buffer = objImageDataPtr->ConvertToRGB24(GX_BIT_0_7, GX_RAW2RGB_NEIGHBOUR, true);

            QImage* image = new QImage(objImageDataPtr->GetWidth(), objImageDataPtr->GetHeight(), QImage::Format_ARGB32);
        }
        

        //cout << "Sleep started!" << endl;
        //Sleep(10000);
        //cout << "Sleep ended!" << endl;

        //Befehl "Miningstopp senden"
        ObjFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();
        ObjFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("Off");

        objStreamPtr->StopGrab();

        //Rückruf beim Abmelden der Sammlung
        objStreamPtr->UnregisterCaptureCallback();

        //Ressourcen freisetzen
        objStreamPtr->Close();
    }

    objDevicePtr->Close();

    //Deinitialisieren der Bibliothek
    IGXFactory::GetInstance().Uninit();

    //Zerstört den Ereignisrückrufzeiger
    if (NULL != pCaptureEventHandler)
    {
        delete pCaptureEventHandler;
        pCaptureEventHandler = NULL;
    }

    return true;
}