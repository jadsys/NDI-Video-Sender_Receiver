#pragma once

#include <vector>
#include "VideoSource.h"
#include "USBCamera.h"
#include "Realsense.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <mutex>
#include <boost/bimap/bimap.hpp>
#include <cmath>

#define ABS(X) (((X) > 0) ? (X): (-X))

#define COMPARE_EPSILON 1e-4

typedef boost::bimaps::bimap<int, int> bimap_t;
typedef bimap_t::value_type bimap_value_t;

/**
 * 映像ソースを管理し、切り替えるクラス
 */
class VideoSourceManager {
public:
    VideoSourceManager();

    ~VideoSourceManager();

    void addVideoSource(int camera_numbeer);

    void registSender(int sender_number);

    bool requestVideoSource(int sender_number, int camera_number);

    cv::Mat getFrame(int sender_number);

    void setRealSenseMode(int sender_number, cameraMode camera_mode);

private:
    std::mutex m;

    vector<VideoSource *> *sources;
    vector<bool> *flip_flags;
    vector<string> *descriptions;
    vector<double> *rotation_angles;
    // 双方向連想配列
    // スレッドとカメラを紐付ける
    bimap_t thread_camera_map;

    void rotateImage(cv::Mat &in, cv::Mat &out, double angle);

};

