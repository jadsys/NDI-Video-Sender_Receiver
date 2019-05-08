#pragma once
#include <vector>
#include "VideoSource.h"
#include "USBCamera.h"
#include "RealSense.h"
#include <opencv2/core.hpp>
#include <mutex>
#include <boost/bimap/bimap.hpp>

typedef boost::bimaps::bimap<int, int> bimap_t;
typedef bimap_t::value_type bimap_value_t;

class VideoSourceManager
{
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
    typedef enum _camera_type {
        REALSENSE,
        USBCAMERA
    }cameraType;
    typedef struct _video_source {
        VideoSource* videosource;
        cameraType camera_type;
    }video_source;
    vector<video_source>* sources;
    vector<int>* using_flag;
    bimap_t thread_camera_map;
};

