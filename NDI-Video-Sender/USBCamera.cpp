#include "USBCamera.h"


/**
 * USBカメラデバイスを開く
 * @param camera_number カメラ番号
 */
USBCamera::USBCamera(int camera_number):VideoSource(camera_number)
{
    auto *config_read = new ConfigRead(); // コンフィグファイル読み込み処理用インスタンス
    string str_int; // 起動引数で指定した値からカメラ番号を設定するための変数
    str_int = "_CAM";
    str_int += to_string(camera_number); // 指定したカメラ番号を格納

    USBCam_path = config_read->GetStringProperty("Camera_ID" + str_int);
    delete config_read;

    //デバイスを開く
    capture = new cv::VideoCapture();
    capture->open(USBCam_path, cv::CAP_V4L);

    if (!capture->isOpened()) {
        cerr << "cannot open No." << camera_number << " camera" << endl;
        open_failed = true;
    }else{
        open_failed = false;
    }

    capture->set(cv::CAP_PROP_FRAME_WIDTH, m_xres);  // 幅
    capture->set(cv::CAP_PROP_FRAME_HEIGHT, m_yres); // 高さ
    capture->set(cv::CAP_PROP_FPS, m_sndfps); // フレームレート

    // フレームサイズ更新（カメラの性能以上要求によるエラー落ち対策）
    m_xres = (int)capture->get(cv::CAP_PROP_FRAME_WIDTH); // 横方向
    m_yres = (int)capture->get(cv::CAP_PROP_FRAME_HEIGHT); // 縦方向
    cv::Mat frame;
    while(frame.empty()){
        capture->read(frame);
    }
}



USBCamera::~USBCamera()
{
    capture->release();
    delete capture;
}

/**
 * フレームを得る
 * @return カメラ画像のフレーム
 */
cv::Mat USBCamera::getFrame() {
    if(!open_failed) {
        cv::Mat frame;
        cv::Mat bgra_frame;
        while (frame.empty()) {
            capture->read(frame);
        }
        cv::cvtColor(frame, bgra_frame, cv::COLOR_BGR2BGRA);
        return bgra_frame;
    }else{
        return cv::Mat(cv::Size(m_xres, m_yres), CV_8UC4, cv::Scalar(0, 0, 255, 255));
    }
}

void USBCamera::setCameraMode(cameraMode _camera_mode)
{
    return;
}
