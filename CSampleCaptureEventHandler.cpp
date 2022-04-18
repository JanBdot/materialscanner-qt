#include"GalaxyIncludes.h"
#include<QWidget>
#include<QImage>
#include<QObject>
//#include<CSampleCaptureEventHandler.h>


using namespace std;

class CSampleCaptureEventHandler : public ICaptureEventHandler
{
public:
    void DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam) // Callback function processing
    {
        cout << "¡Recibe una imagen!" << endl;
        cout << "ImageInfo: " << objImageDataPointer->GetStatus() << endl;
        cout << "ImageInfo: " << objImageDataPointer->GetWidth() << endl;
        cout << "ImageInfo: " << objImageDataPointer->GetHeight() << endl;
        cout << "ImageInfo: " << objImageDataPointer->GetPayloadSize() << endl;
    }



    void run()
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
        cout << "No team available!" << endl;
        cin >> a;

        // Desinicializar la biblioteca
        IGXFactory::GetInstance().Uninit();
        return;
    }

    // Abre el primer dispositivo y la primera secuencia debajo del dispositivo
    CGXDevicePointer ObjDevicePtr =
        IGXFactory::GetInstance().OpenDeviceBySN(vectorDeviceInfo[0].GetSN(), GX_ACCESS_EXCLUSIVE);
    CGXStreamPointer ObjStreamPtr = ObjDevicePtr->OpenStream(0);

    // Obtener controlador de atributo de dispositivo remoto
    CGXFeatureControlPointer ObjFeatureControlPtr = ObjDevicePtr->GetRemoteFeatureControl();
    ObjFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("On");
    ObjFeatureControlPtr->GetEnumFeature("TriggerSource")->SetValue("Line0");
    ObjFeatureControlPtr->GetEnumFeature("TriggerActivation")->SetValue("RisingEdge");
    ObjFeatureControlPtr->GetFloatFeature("TriggerDelay")->SetValue(500000);
    ObjFeatureControlPtr->GetFloatFeature("TriggerFilterRaisingEdge")->SetValue(5000);

    // Registrar colección de devolución de llamada
    pCaptureEventHandler = new CSampleCaptureEventHandler();
    ObjStreamPtr->RegisterCaptureCallback(pCaptureEventHandler, NULL);

    // Enviar comandos de minería
    ObjStreamPtr->StartGrab();
    ObjFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();

    Sleep(10000);

    // Enviar comando de detención de minería
    ObjFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();
    ObjFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("Off");

    ObjStreamPtr->StopGrab();

    // Cerrar sesión de devolución de llamada de colección
    ObjStreamPtr->UnregisterCaptureCallback();

    // Liberar recursos
    ObjStreamPtr->Close();
    ObjDevicePtr->Close();

    // Desinicializar la biblioteca
    IGXFactory::GetInstance().Uninit();

    // Destruye el puntero de devolución de llamada de evento
    if (NULL != pCaptureEventHandler)
    {
        delete pCaptureEventHandler;
        pCaptureEventHandler = NULL;
    }
    cin >> a;
}

};