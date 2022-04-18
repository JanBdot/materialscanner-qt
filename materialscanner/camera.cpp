#include"GalaxyIncludes.h"
#include<QWidget>
#include<QImage>
#include<QObject>
//#include<CSampleCaptureEventHandler.h>


using namespace std;

class camera : public ICaptureEventHandler
{
public:
    void DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam) // Callback function processing
    {
        cout << "Bild erhalten" << endl;
        cout << "ImageInfo: " << objImageDataPointer->GetStatus() << endl;
        cout << "ImageInfo: " << objImageDataPointer->GetWidth() << endl;
        cout << "ImageInfo: " << objImageDataPointer->GetHeight() << endl;
        cout << "ImageInfo: " << objImageDataPointer->GetPayloadSize() << endl;
    }



    bool run()
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
        CGXDevicePointer ObjDevicePtr =
            IGXFactory::GetInstance().OpenDeviceBySN(vectorDeviceInfo[0].GetSN(), GX_ACCESS_EXCLUSIVE);
        CGXStreamPointer ObjStreamPtr = ObjDevicePtr->OpenStream(0);

        // Abrufen des Remotegeräteattributtreibers
        CGXFeatureControlPointer ObjFeatureControlPtr = ObjDevicePtr->GetRemoteFeatureControl();
        ObjFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("On");
        ObjFeatureControlPtr->GetEnumFeature("TriggerSource")->SetValue("Line0");
        ObjFeatureControlPtr->GetEnumFeature("TriggerActivation")->SetValue("RisingEdge");
        ObjFeatureControlPtr->GetFloatFeature("TriggerDelay")->SetValue(500000);
        ObjFeatureControlPtr->GetFloatFeature("TriggerFilterRaisingEdge")->SetValue(5000);

        // Callback Collection registrieren
        pCaptureEventHandler = new camera();
        ObjStreamPtr->RegisterCaptureCallback(pCaptureEventHandler, NULL);

        // Senden von Miningbefehlen
        ObjStreamPtr->StartGrab();
        ObjFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();

        cout << "Sleep started!" << endl;
        Sleep(10000);
        cout << "Sleep ended!" << endl;

        //Befehl "Miningstopp senden"
        ObjFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();
        ObjFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("Off");

        ObjStreamPtr->StopGrab();

        //Rückruf beim Abmelden der Sammlung
        ObjStreamPtr->UnregisterCaptureCallback();

        //Ressourcen freisetzen
        ObjStreamPtr->Close();
        ObjDevicePtr->Close();

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

};