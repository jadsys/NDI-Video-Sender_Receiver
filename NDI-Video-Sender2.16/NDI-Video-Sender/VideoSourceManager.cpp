#include "VideoSourceManager.h"



VideoSourceManager::VideoSourceManager()
{
    sources = new vector<video_source>();
    using_flag = new vector<int>();
}


VideoSourceManager::~VideoSourceManager()
{
    delete sources;
    delete using_flag;
}

void VideoSourceManager::addVideoSource(int camera_numbeer)
{
    ConfigRead* config_read = new ConfigRead(); // コンフィグファイル読み込み処理用インスタンス
    string str_int; // 起動引数で指定した値からカメラ番号を設定するための変数
    str_int = "_CAM";
    str_int += to_string(camera_numbeer); // 指定したカメラ番号を格納
    string type = config_read->GetStringProperty("Camera_Types" + str_int);
    if (type == "web") {
        USBCamera* usb = new USBCamera(camera_numbeer);
        video_source vs;
        vs.videosource = usb;
        vs.camera_type = cameraType::USBCAMERA;
        sources->push_back(vs);
    }
    else if(type == "rs" || type == "realsense"){
        RealSense* rs = new RealSense(camera_numbeer);
        video_source vs;
        vs.videosource = rs;
        vs.camera_type = cameraType::REALSENSE;
        sources->push_back(vs);
    }

}

void VideoSourceManager::registSender(int sender_number)
{
    thread_camera_map.insert(bimap_value_t(sender_number, sender_number - 1));
}

bool VideoSourceManager::requestVideoSource(int sender_number, int camera_number)
{

    m.lock();
    bimap_t::right_iterator itr = thread_camera_map.right.find(thread_camera_map.left.at(sender_number));
    const bool successful_replace = thread_camera_map.right.replace_key(itr, camera_number - 1);
    m.unlock();
    return successful_replace;
}

cv::Mat VideoSourceManager::getFrame(int sender_number)
{
    int camera_number = thread_camera_map.left.at(sender_number);
    return sources->at(camera_number).videosource->getFrame();
}

void VideoSourceManager::setRealSenseMode(int sender_number, cameraMode camera_mode)
{
    int camera_number = thread_camera_map.left.at(sender_number);
    sources->at(camera_number).videosource->setCameraMode(camera_mode);
}
