#pragma once
#include "ConfigRead.h"
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <stdexcept>
using namespace std;

typedef enum _camera_mode {
    RGB,
    DEPTH,
    IR_LEFT,
    IR_RIGHT
}cameraMode;

class VideoSource
{
public:
    VideoSource(int camera_number);
    ~VideoSource();
    virtual cv::Mat getFrame() = 0;
    virtual void setCameraMode(cameraMode _camera_mode) = 0;
protected:
    int m_xres; // x解像度
    int m_yres; // y解像度
    int m_sndfps; // FPS指定用
    bool is_insert_time_stamp;
    string resource_name;
};

