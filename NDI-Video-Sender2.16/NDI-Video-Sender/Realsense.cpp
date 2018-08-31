#include "Realsense.h"


using namespace std;

/****************************************************************************
*
*　コンストラクタ・デストラクタ
*
*****************************************************************************/
/* コンストラクタ */
Realsense::Realsense()
{
	// 処理なし
}

/* コンストラクタ */
Realsense::Realsense(int argv_no) : NdiCom::NdiCom(argv_no)
{
	// カメラ初期化
	initializeSensor();
}

/* デストラクタ */
Realsense::~Realsense()
{
	// 処理なし
}

/* 初期化 */
void Realsense::initializeSensor()
{
	// RGBカメラ初期値
	m_color_width = m_xres; // 横方向解像度の指定
	m_color_height = m_yres; // 縦方向解像度の指定
	m_color_fps = m_sndfps; // フレームレートの指定

	// Depthカメラ初期値
	m_depth_width = m_xres; // 横方向解像度の指定
	m_depth_height = m_yres; // 縦方向解像度の指定
	m_depth_fps = m_sndfps; // フレームレートの指定

	// IRカメラ設定値
	m_IR_width = m_xres; // 横方向解像度の指定
	m_IR_height = m_yres; // 縦方向解像度の指定
	m_IR_fps = m_sndfps; // フレームレートの指定

	rs2::config config; // RealSenseカメラの設定用
	rs2::pipeline_profile pipeline_profile; // パイプラインの中身参照用

	// コンフィグの中身設定
	config.enable_device(NdiCom::m_other_camera); // デバイスのシリアル指定
	config.enable_stream(rs2_stream::RS2_STREAM_COLOR, m_xres, m_yres, rs2_format::RS2_FORMAT_BGR8, m_color_fps); // RGBカメラ有効化
	config.enable_stream(rs2_stream::RS2_STREAM_DEPTH, m_xres, m_yres, rs2_format::RS2_FORMAT_Z16, m_depth_fps); // Depthカメラ有効化
	config.enable_stream(rs2_stream::RS2_STREAM_INFRARED, 1, m_IR_width, m_IR_height, rs2_format::RS2_FORMAT_Y8, m_IR_fps); // IRカメラ有効化
	config.enable_stream(rs2_stream::RS2_STREAM_INFRARED, 2, m_IR_width, m_IR_height, rs2_format::RS2_FORMAT_Y8, m_IR_fps); // IRカメラ有効化


	// パイプラインスタート
	pipeline_profile = m_pipeline.start(config);
}

/****************************************************************************
*
*　送信処理
*
*****************************************************************************/
void Realsense::sndVideo() 
{
	// ローカル変数
	rs2::frame color_frame; // RGB映像用フレーム
	rs2::frame depth_frame; // Depth映像用フレーム
	rs2::frame ir1_frame; // IR映像用フレーム
	rs2::frame ir2_frame; // IR映像用フレーム
	cv::Mat myframe, sndframe; // OpenCV用フレーム
	cv::Point point(30, 30); // タイムスタンプ位置
	string strTimecode; // タイムスタンプ用時刻格納用変数
	rs2::frameset frameset; // フレームセット
	rs2::colorizer color_map; // カラー化ツール
	int camera_mode = RGB_CAMERA_VIDEOS; // RGBカメラか
	int cv_color; // 入力したキーの番号を格納するための変数
	NDIlib_metadata_frame_t metadata_desc; // メタ受信用フレーム

	// 送信用インスタンス生成
	sendCreate();

	// 映像用フレームの生成と初期化
	NDIlib_video_frame_v2_t video_frame; // 映像フレームの宣言
	video_frame.xres = m_xres; // 横方向解像度の指定
	video_frame.yres = m_yres; // 縦方向解像度の指定
	video_frame.FourCC = NDIlib_FourCC_type_BGRA; // m_sndNDIColor指定先。フレームのカラーフォーマット指定
	video_frame.frame_format_type = NDIlib_frame_format_type_interleaved;
	video_frame.p_data = (uint8_t*)malloc(m_xres * m_yres * 4); // データサイズの指定
	video_frame.line_stride_in_bytes = m_xres * 4;

	// Send Start!
	printf("%s SENDING...\n", m_resources_id.c_str());
	m_exit_snd_loop = false;

	// 送信処理
	while (!m_exit_snd_loop)
	{
		if (NDIlib_send_capture(m_pNDI_send, &metadata_desc, 0))
		{	
			string camera_change = metadata_desc.p_data; // メタデータの本体をstring型の変数に格納
			
			// カメラモードのチェンジ
			if (camera_change.find("<RGB_mode enabled=\"true\"/>") != -1)
			{
				camera_mode = RGB_CAMERA_VIDEOS;
				printf("RGB mode enabled\n");
			}
			else if (camera_change.find("<Depth_mode enabled=\"true\"/>") != -1)
			{
				camera_mode = DEPTH_CAMERA_VIDEOS;
				printf("Depth mode enabled\n");
			}
			else if (camera_change.find("<IR_left_mode enabled=\"true\"/>") != -1)
			{
				camera_mode = IR_LEFT_CAMERA_VIDEOS;
				printf("IR left mode enabled\n");
			}
			else if (camera_change.find("<IR_right_mode enabled=\"true\"/>") != -1)
			{
				camera_mode = IR_RIGHT_CAMERA_VIDEOS;
				printf("IR right mode enabled\n");
			}

			// ここでデータを解放する必要があります
			NDIlib_send_free_metadata(m_pNDI_send, &metadata_desc);
		}
		// カメラから読み込み処理
		frameset = m_pipeline.wait_for_frames(); // フレームの更新

		// カメラの動作モード切替
		switch (camera_mode)
		{
		case RGB_CAMERA_VIDEOS:
			// RGBフレーム取得
			color_frame = frameset.get_color_frame();

			// フレームサイズの更新
			m_color_width = color_frame.as<rs2::video_frame>().get_width();
			m_color_height = color_frame.as<rs2::video_frame>().get_height();

			// OpenCV用フレームに格納
			myframe = cv::Mat(m_color_height, m_color_width, CV_8UC3, const_cast<void *>(color_frame.get_data()));

			// 変換の色を変更
			cv_color = CV_BGR2BGRA;

			break;

		case DEPTH_CAMERA_VIDEOS:
			// Depthフレーム取得(色付け)
			depth_frame = frameset.get_depth_frame();
			depth_frame = depth_frame.apply_filter(color_map);

			// フレームサイズの更新
			m_depth_width = depth_frame.as<rs2::video_frame>().get_width();
			m_depth_height = depth_frame.as<rs2::video_frame>().get_height();

			// OpenCV用フレームに格納
			myframe = cv::Mat(m_depth_height, m_depth_width, CV_8UC3, const_cast<void*>(depth_frame.get_data()));

			// 変換の色を変更
			cv_color = CV_BGR2BGRA;
			
			break;

		case IR_LEFT_CAMERA_VIDEOS:
			// IRフレーム取得(色付け)
			ir1_frame = frameset.get_infrared_frame(1);

			// フレームサイズの更新
			m_IR_width = ir1_frame.as<rs2::video_frame>().get_width();
			m_IR_height = ir1_frame.as<rs2::video_frame>().get_height();

			// OpenCV用フレームに格納
			myframe = cv::Mat(m_IR_height, m_IR_width, CV_8UC1, const_cast<void*>(ir1_frame.get_data()));

			// 変換の色を変更
			cv_color = CV_GRAY2BGRA;
			
			break;

		case IR_RIGHT_CAMERA_VIDEOS:
			// IRフレーム取得(色付け)
			ir2_frame = frameset.get_infrared_frame(2);

			// フレームサイズの更新
			m_IR_width = ir2_frame.as<rs2::video_frame>().get_width();
			m_IR_height = ir2_frame.as<rs2::video_frame>().get_height();

			// OpenCV用フレームに格納
			myframe = cv::Mat(m_IR_height, m_IR_width, CV_8UC1, const_cast<void*>(ir2_frame.get_data()));

			// 変換の色を変更
			cv_color = CV_GRAY2BGRA;
			
			break;
		}

		// 読み込んだフレームが空か
		if (myframe.empty())
		{
			continue;
		}

		// タイムスタンプ挿入するか
		if (m_sndtmcode_flg)
		{
			// タイムスタンプ挿入
			strTimecode = getCurrentTime();

			cv::putText(
				myframe,  // 画像
				strTimecode, // 文字列
				point, // 座標
				cv::FONT_HERSHEY_SIMPLEX, // フォントの種類
				0.8, // 文字の大きさ
				cv::Scalar(255, 255, 255), // 文字の色
				3 // 線の太さ
			);

			cv::putText( // インラインフォント
				myframe, // 画像
				strTimecode, // 文字列
				point, // 座標
				cv::FONT_HERSHEY_SIMPLEX, // フォントの種類
				0.8, // 文字の大きさ
				cv::Scalar(0, 0, 0), // 文字の色
				1, // 線の太さ
				CV_AA // アンチエイリアス
			);
		}

		
		cv::cvtColor(myframe, sndframe, cv_color); // m_sndCVColorの指定先。色の変換(NDI送信用フレームに乗せるにはBGRXに変換する必要がある)

		
		memcpy((void*)video_frame.p_data, sndframe.data, (m_xres * m_yres * 4)); // OpenCVのフレームをNDIフレームデータにコピー

		// 送信
		NDIlib_send_send_video_v2(m_pNDI_send, &video_frame);

		if (m_preview_flg)
		{
			cv::imshow(m_str_resname, myframe); // 画像を表示

			// キー入力を待つ
			switch (cv::waitKey(1))
			{
			case 3: // imshow中にCtrl+cが入力されたら終了
			case 227: // windowsだとCtrl+cは3、LinuxだとCtrl+cが正しく認識できない。227が返る
				m_exit_snd_loop = true;
				cv::destroyWindow(m_str_resname);
				break;
			default:
				break;
			}
		}
	}
	m_pipeline.stop(); // パイプラインストップ
	free(video_frame.p_data); // 映像フレームのデータ領域を解放
	NDIlib_send_destroy(m_pNDI_send); // 送信用フレームの開放
	m_isRecflg = false;
	
}

/* 送信Threadの作成 */
void Realsense::CretateSndVideoThread() 
{
	if (m_use_flg) 
	{
		m_isRecflg = true; // 送信フラグオン
		m_thread_video = thread(&Realsense::sndVideo, this); // 送信スレッド開始
	}
}

