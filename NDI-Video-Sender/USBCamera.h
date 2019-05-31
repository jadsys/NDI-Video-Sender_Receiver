#pragma once
#include "VideoSource.h"
#include <opencv2/imgproc.hpp>
/**
 * USB接続カメラ用映像クラス
 */
class USBCamera :
        public VideoSource
{
public:
    USBCamera(int camera_number);

    ~USBCamera();

    cv::Mat getFrame() override;

    void setCameraMode(cameraMode _camera_mode) override;
private:
    std::string USBCam_path;
    //キャプチャハンドル
    cv::VideoCapture* capture;
};

