#include "NDISender.h"


NDISender::NDISender(VideoSourceManager* vsm, int sender_number)
{
    this->sender_number = sender_number;
    this->vsm = vsm;
    vsm->registSender(sender_number);
    string sender_name = "Sender" + to_string(sender_number);
    NDI_send_create_desc.p_ndi_name = sender_name.c_str(); // 送信インスタンス
    NDI_send_create_desc.clock_video = true; // 送信時間を同期させるかどうか
    NDI_send_create_desc.clock_audio = false; // 送信時間を同期させるかどうか
    m_pNDI_send = NDIlib_send_create(&NDI_send_create_desc); // NDI送信インスタンス生成
    if (m_pNDI_send == NULL) {
        cerr << "cannot create NDI sender instance" << endl;
        throw runtime_error("cannot create NDI sender instance");
    }
    ndi_send_thread = thread(&NDISender::sendThread, this);
}

void NDISender::sendThread()
{
    cv::Mat frame;
    NDIlib_video_frame_v2_t video_frame;
    NDIlib_metadata_frame_t metadata_desc;
    while(true) {
        
        if (NDIlib_send_capture(m_pNDI_send, &metadata_desc, 0))
        {
            string camera_change = metadata_desc.p_data; // メタデータの本体をstring型の変数に格納

            // カメラモードのチェンジ
            if (camera_change.find("<RGB_mode enabled=\"true\"/>") != string::npos)
            {
                vsm->setRealSenseMode(sender_number, cameraMode::RGB);
                cout << "RGB mode enabled" << endl;
            }
            else if (camera_change.find("<Depth_mode enabled=\"true\"/>") != string::npos)
            {
                vsm->setRealSenseMode(sender_number, cameraMode::DEPTH);
                cout << "Depth mode enabled" << endl;
            }
            else if (camera_change.find("<IR_left_mode enabled=\"true\"/>") != string::npos)
            {
                vsm->setRealSenseMode(sender_number, cameraMode::IR_LEFT);
                cout << "IR left mode enabled" << endl;
            }
            else if (camera_change.find("<IR_right_mode enabled=\"true\"/>") != string::npos)
            {
                vsm->setRealSenseMode(sender_number, cameraMode::IR_RIGHT);
                cout << "IR right mode enabled" << endl;
            }
            else if (camera_change.find("Request_CAM1") != string::npos) {
                vsm->requestVideoSource(sender_number, 1);
            }
            else if (camera_change.find("Request_CAM2") != string::npos) {
                vsm->requestVideoSource(sender_number, 2);
            }
            else if (camera_change.find("Request_CAM3") != string::npos) {
                vsm->requestVideoSource(sender_number, 3);
            }
            else if (camera_change.find("Request_CAM4") != string::npos) {
                vsm->requestVideoSource(sender_number, 4);
            }
            else if (camera_change.find("Request_CAM5") != string::npos) {
                vsm->requestVideoSource(sender_number, 5);
            }
            else if (camera_change.find("Request_CAM6") != string::npos) {
                vsm->requestVideoSource(sender_number, 6);
            

            }

            // ここでデータを解放する必要があります
            NDIlib_send_free_metadata(m_pNDI_send, &metadata_desc);
        }

        frame = vsm->getFrame(sender_number);
        if (frame.empty()) {
            continue;
        }

        video_frame.xres = frame.cols; // 横方向解像度の指定
        video_frame.yres = frame.rows; // 縦方向解像度の指定
        video_frame.FourCC = NDIlib_FourCC_type_BGRA; // m_sndNDIColor指定先。フレームのカラーフォーマット指定
        video_frame.frame_format_type = NDIlib_frame_format_type_interleaved;
        video_frame.p_data = (uint8_t*)malloc((uint64_t)frame.rows * (long long)frame.cols * 4); // データサイズの指定
        video_frame.line_stride_in_bytes = frame.rows * 4;
        if (video_frame.p_data == NULL) {
            throw bad_alloc();
        }
        memcpy((void*)video_frame.p_data, frame.data, ((size_t)frame.rows * (size_t)frame.cols * 4)); // OpenCVのフレームをNDIフレームデータにコピー

        NDIlib_send_send_video_v2(m_pNDI_send, &video_frame);

        cv::imshow("sender" + to_string(sender_number), frame);
        switch (cv::waitKey(1))
        {
        case '1':
            vsm->requestVideoSource(sender_number, 1);
            break;
        case '2':
            vsm->requestVideoSource(sender_number, 2);
            break;
        default:
            break;
        }
        free(video_frame.p_data);
    }

}

NDISender::~NDISender()
{
    NDIlib_send_destroy(m_pNDI_send);
}
