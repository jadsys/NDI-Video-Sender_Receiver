#pragma once
#include <thread>
#include <opencv2/highgui.hpp>
#include <Processing.NDI.Lib.h>
#include "VideoSourceManager.h"
class NDISender
{
public:
    NDISender(VideoSourceManager *vsm, int sender_number);
    ~NDISender();

private:
    void sendThread();
    int sender_number;
    std::thread ndi_send_thread;
    VideoSourceManager* vsm;
    NDIlib_send_create_t NDI_send_create_desc;
    NDIlib_send_instance_t m_pNDI_send;
};



