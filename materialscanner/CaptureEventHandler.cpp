#include "CaptureEventHandler.h"

void CaptureEventHandler::DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam)
{
    std::cout << "CaptureEventHandler" << std::endl;
    if (objImageDataPointer->GetStatus() == GX_FRAME_STATUS_SUCCESS)
    //if (objImageDataPointer->GetStatus() == GX_FRAME_STATUS_SUCCESS)
    {

        /*

        If the image acquisition is a full frame, then you can read the image width,height,data

        format and etc.

        */

        uint64_t nWidth = objImageDataPointer->GetWidth();

        uint64_t nHeight = objImageDataPointer->GetHeight();

        GX_PIXEL_FORMAT_ENTRY emPixelFormat = objImageDataPointer->GetPixelFormat();

        /* More images information please refer the IImageData interface define.*/

        std::cout << "Bild erhalten" << std::endl;
        std::cout << "ImageInfo: " << objImageDataPointer->GetStatus() << std::endl;
        std::cout << "ImageInfo: " << nWidth << std::endl;
        std::cout << "ImageInfo: " << nHeight << std::endl;
        std::cout << "ImageInfo: " << objImageDataPointer->GetPayloadSize() << std::endl;
        cv::Mat img;

        img.create(nHeight, nWidth, CV_8UC3);
        void* pRGB24Buffer = NULL;
        // Angenommen, die Originaldaten sind ein BayerRG8-Bild
        //pRGB24Buffer = objImageDataPtr->ConvertToRaw8(GX_BIT_0_7);
        pRGB24Buffer = objImageDataPointer->ConvertToRGB24(GX_BIT_0_7, GX_RAW2RGB_NEIGHBOUR, true);
        memcpy(img.data, pRGB24Buffer, (nHeight) * (nWidth) * 3);
        cv::flip(img, img, 0);
        cv::rotate(img, img, cv::ROTATE_90_COUNTERCLOCKWISE);
        cv::imshow("sss", img);
        cv::waitKey(1);

        QImage imgIn = QImage((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
        // TODO imgIn in GUI anzeigen
        bool imageSaved = false;
        if (!imageSaved) {
            imgIn.save(QString("01.bmp"), "bmp");
            imageSaved = true;
        }

    }

}

