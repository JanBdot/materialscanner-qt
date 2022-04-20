#pragma once

class camera : ICaptureEventHandler
{
public:
    camera();
    ~camera();

    void DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam);
    bool run();
};