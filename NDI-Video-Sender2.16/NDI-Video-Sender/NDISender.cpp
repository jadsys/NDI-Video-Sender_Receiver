#include "NDISender.h"


NDISender::NDISender(VideoSourceManager *vsm, int sender_number) {
    this->sender_number = sender_number;
    this->vsm = vsm;
    // 映像ソースマネージャに送信スレッドを登録
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
    //送信スレッド開始
    ndi_send_thread = thread(&NDISender::sendThread, this);
}


void NDISender::sendThread() {
    //カメラ画像フレーム
    cv::Mat frame;
    //NDI送信フレーム
    NDIlib_video_frame_v2_t video_frame;
    //NDIメタデータ受信用フレーム
    NDIlib_metadata_frame_t metadata_desc;
    while (true) {

        if (NDIlib_send_capture(m_pNDI_send, &metadata_desc, 0)) {
            string camera_change = metadata_desc.p_data; // メタデータの本体をstring型の変数に格納
            // カメラモードのチェンジ
            if (camera_change.find("<RGB_mode enabled=\"true\"/>") != string::npos) {
                vsm->setRealSenseMode(sender_number, cameraMode::RGB);
            } else if (camera_change.find("<Depth_mode enabled=\"true\"/>") != string::npos) {
                vsm->setRealSenseMode(sender_number, cameraMode::DEPTH);
            } else if (camera_change.find("<IR_left_mode enabled=\"true\"/>") != string::npos) {
                vsm->setRealSenseMode(sender_number, cameraMode::IR_LEFT);
            } else if (camera_change.find("<IR_right_mode enabled=\"true\"/>") != string::npos) {
                vsm->setRealSenseMode(sender_number, cameraMode::IR_RIGHT);
                // カメラ切り替え
            } else if (camera_change.find("<CAMERA=\"1\"/>") != string::npos) {
                vsm->requestVideoSource(sender_number, 1);
            } else if (camera_change.find("<CAMERA=\"2\"/>") != string::npos) {
                vsm->requestVideoSource(sender_number, 2);
            } else if (camera_change.find("<CAMERA=\"3\"/>") != string::npos) {
                vsm->requestVideoSource(sender_number, 3);
            } else if (camera_change.find("<CAMERA=\"4\"/>") != string::npos) {
                vsm->requestVideoSource(sender_number, 4);
            } else if (camera_change.find("<CAMERA=\"5\"/>") != string::npos) {
                vsm->requestVideoSource(sender_number, 5);
            } else if (camera_change.find("<CAMERA=\"6\"/>") != string::npos) {
                vsm->requestVideoSource(sender_number, 6);
            }

            // ここでデータを解放する必要があります
            NDIlib_send_free_metadata(m_pNDI_send, &metadata_desc);
        }
        // フレームを取得
        // フレームが到着するまでブロックされる
        frame = vsm->getFrame(sender_number);
        // フレームがからの場合は送信せず破棄
        if (frame.empty()) {
            continue;
        }

        video_frame.xres = frame.cols; // 横方向解像度の指定
        video_frame.yres = frame.rows; // 縦方向解像度の指定
        video_frame.FourCC = NDIlib_FourCC_type_BGRA; // m_sndNDIColor指定先。フレームのカラーフォーマット指定
        video_frame.frame_format_type = NDIlib_frame_format_type_interleaved;
        video_frame.p_data = (uint8_t *) malloc((uint64_t) frame.rows * (long long) frame.cols * 4); // データサイズの指定
        video_frame.line_stride_in_bytes = frame.cols * 4;
        if (video_frame.p_data == nullptr) {
            throw bad_alloc();
        }
        memcpy((void *) video_frame.p_data, frame.data,
               ((size_t) frame.rows * (size_t) frame.cols * 4)); // OpenCVのフレームをNDIフレームデータにコピー
        //送信
        NDIlib_send_send_video_v2(m_pNDI_send, &video_frame);
        //フレームサイズが変わる可能性があるため一度開放
        free(video_frame.p_data);
    }

}

NDISender::~NDISender() {
    ndi_send_thread.join();
    NDIlib_send_destroy(m_pNDI_send);
}
