#include "Realsense.h"


/**
 * RealSenseの設定を行う
 * @param camera_number カメラ番号
 */
RealSense::RealSense(int camera_number):VideoSource(camera_number)
{
    camera_mode = RGB; //初期状態ではRGBカメラを使用

    ConfigRead* config_read = new ConfigRead(); // コンフィグファイル読み込み処理用インスタンス
    string str_int; // 起動引数で指定した値からカメラ番号を設定するための変数
    str_int = "_CAM";
    str_int += to_string(camera_number); // 指定したカメラ番号を格納

    rs2::config config; // RealSenseカメラの設定用
    rs2::pipeline_profile pipeline_profile; // パイプラインの中身参照用

    // コンフィグの中身設定
    config.enable_device(config_read->GetStringProperty("Camera_ID" + str_int)); // デバイスのシリアル指定
    config.enable_stream(rs2_stream::RS2_STREAM_COLOR, m_xres, m_yres, rs2_format::RS2_FORMAT_BGR8, m_sndfps); // RGBカメラ有効化
    config.enable_stream(rs2_stream::RS2_STREAM_DEPTH, m_xres, m_yres, rs2_format::RS2_FORMAT_Z16, m_sndfps); // Depthカメラ有効化
    config.enable_stream(rs2_stream::RS2_STREAM_INFRARED, 1, m_xres, m_yres, rs2_format::RS2_FORMAT_Y8, m_sndfps); // IRカメラ有効化
    config.enable_stream(rs2_stream::RS2_STREAM_INFRARED, 2, m_xres, m_yres, rs2_format::RS2_FORMAT_Y8, m_sndfps); // IRカメラ有効化


    // パイプラインスタート
    pipeline_profile = m_pipeline.start(config);

    delete config_read;
}


RealSense::~RealSense()
{
    m_pipeline.stop();
}

/**
 * カメラモードを設定する
 * @param _camera_mode カメラモード
 */
void RealSense::setCameraMode(cameraMode _camera_mode)
{
    this->camera_mode = _camera_mode;
}

/**
 * 映像を取得する。カメラモードに合わせて種類がかわる
 * @return 取得したカメラフレーム
 */
cv::Mat RealSense::getFrame()
{
    rs2::frameset frameset;
    rs2::frame frame;
    //フレームを得る
    frameset = m_pipeline.wait_for_frames();
    //空フレーム時に単色を出力する
    cv::Mat mat_frame(cv::Size(m_xres, m_yres), CV_8UC3, cv::Scalar(0, 0, 255));
    cv::Mat rgba_frame;
    switch (camera_mode) {
        case cameraMode::RGB:
            frame = frameset.get_color_frame();
            mat_frame = cv::Mat(m_yres, m_xres, CV_8UC3, const_cast<void*>(frame.get_data()));
            cv::cvtColor(mat_frame, rgba_frame, cv::COLOR_BGR2BGRA);
            break;
        case cameraMode::DEPTH:
            frame = frameset.get_depth_frame();
            frame = frame.apply_filter(color_map);
            mat_frame = cv::Mat(m_yres, m_xres, CV_8UC3, const_cast<void*>(frame.get_data()));
            cv::cvtColor(mat_frame, rgba_frame, cv::COLOR_BGR2BGRA);
            break;
        case cameraMode::IR_LEFT:
            frame = frameset.get_infrared_frame(1);
            mat_frame = cv::Mat(m_yres, m_xres, CV_8UC1, const_cast<void*>(frame.get_data()));
            cv::cvtColor(mat_frame, rgba_frame, cv::COLOR_GRAY2BGRA);
            break;
        case cameraMode::IR_RIGHT:
            frame = frameset.get_infrared_frame(2);
            mat_frame = cv::Mat(m_yres, m_xres, CV_8UC1, const_cast<void*>(frame.get_data()));
            cv::cvtColor(mat_frame, rgba_frame, cv::COLOR_GRAY2BGRA);
            break;

        default:
            cv::cvtColor(mat_frame, rgba_frame, cv::COLOR_BGR2BGRA);
            break;
    }
    return rgba_frame;
}
