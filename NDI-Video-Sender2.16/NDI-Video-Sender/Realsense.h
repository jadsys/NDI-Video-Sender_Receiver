#pragma once
#include "NdiCom.h"
#include <librealsense2/rs.hpp>

#define RGB_CAMERA_VIDEOS  0
#define DEPTH_CAMERA_VIDEOS 1
#define IR_LEFT_CAMERA_VIDEOS 2
#define IR_RIGHT_CAMERA_VIDEOS 3

class Realsense :
    public NdiCom
{
public:
    Realsense();
    Realsense(int argv_no);
    ~Realsense();
    void initializeSensor();
    void sndVideo() override;
    void CretateSndVideoThread() override;

private:
    // RGBカメラ設定値
    uint32_t m_color_width; // 横方向解像度の格納用
    uint32_t m_color_height; // 縦方向解像度の格納用
    uint32_t m_color_fps; // フレームレートの格納用

    // デプスカメラ設定値
    uint32_t m_depth_width; // 横方向解像度の格納用
    uint32_t m_depth_height; // 縦方向解像度の格納用
    uint32_t m_depth_fps; // フレームレートの格納用

    // IRカメラ設定値
    uint32_t m_IR_width; // 横方向解像度の格納用
    uint32_t m_IR_height; // 縦方向解像度の格納用
    uint32_t m_IR_fps; // フレームレートの格納用

    rs2::pipeline m_pipeline; // パイプライン
};

