#pragma once
#include "ConfigRead.h"
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <stdexcept>
using namespace std;

/***
 * カメラモード一覧
 */
typedef enum _camera_mode {
    RGB,
    DEPTH,
    IR_LEFT,
    IR_RIGHT
}cameraMode;

/***
 * 映像ソースの抽象クラス
 * ソースに合わせてオーバーライドする
 */
class VideoSource
{
public:
    VideoSource(int camera_number);

    ~VideoSource();

    virtual cv::Mat getFrame() = 0;

    // カメラモードの切り替えをすべてのソースに実装する
    //　必要ない場合は何もしない
    virtual void setCameraMode(cameraMode _camera_mode) = 0;
protected:
    int m_xres; // x解像度
    int m_yres; // y解像度
    int m_sndfps; // FPS指定用
    bool open_failed;
};

