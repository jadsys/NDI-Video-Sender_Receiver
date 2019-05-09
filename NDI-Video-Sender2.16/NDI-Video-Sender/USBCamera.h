#pragma once
#include "VideoSource.h"
class USBCamera :
    public VideoSource
{
public:
    USBCamera(int camera_number);
    ~USBCamera();
    cv::Mat getFrame() override;
    void setCameraMode(cameraMode _camera_mode) override;
private:
    int USBCam_number;
    cv::VideoCapture* capture;
};

