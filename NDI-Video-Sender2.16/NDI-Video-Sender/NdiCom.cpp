#include "NdiCom.h"

using namespace std;

/****************************************************************************
*
*　コンストラクタ・デストラクタ
*
*****************************************************************************/
/* コンストラクタ */
NdiCom::NdiCom()
{
    // 初期化
    m_use_flg = true;
    m_exit_snd_loop = true;
    m_preview_flg = true;
    m_sndtmcode_flg = true;
    m_isRecflg = false;
    m_pNDI_send = NULL;
    m_webcam_no = 0;


}

/* コンストラクタ */
NdiCom::NdiCom(int argv_no)
{
    initialize(argv_no);
}

/* デストラクタ */
NdiCom::~NdiCom()
{
    // 処理なし
}

/* メンバ初期化 */
void NdiCom::initialize(int argv_no)
{
    ConfigRead* config_read = new ConfigRead(); // コンフィグファイル読み込み処理用インスタンス
    string str_int; // 起動引数で指定した値からカメラ番号を設定するための変数
    str_int = "_CAM";
    str_int += to_string(argv_no); // 指定したカメラ番号を格納

    setUseFlg(config_read->GetBoolProperty("SwitchCamera" + str_int)); // カメラを使用するかをセット

    // 使用フラグ
    if (!GetUseFlg())
    {
        return;
    }

    setResourcesID(config_read->GetStringProperty("Resources_ID" + str_int)); // リソース名をセット
    setUseWebCameraFlg(config_read->GetStringProperty("Camera_Types" + str_int)); // WEBカメラかどうかセット

    // WEBカメラならデバイス番号、RealSenseならシリアル番号、IPカメラならURLの指定
    if (getWebcamFlg())
    {
        setCamera(config_read->GetIntProperty("Camera_ID" + str_int)); // WEBカメラのデバイス番号のセット
    }
    else
    {
        setCamera(config_read->GetStringProperty("Camera_ID" + str_int)); // Realsenseカメラのシリアル番号及びIPカメラURLのセット
    }

    setVideoSz(config_read->GetIntProperty("xres" + str_int), config_read->GetIntProperty("yres" + str_int)); // 解像度をセット
    setFps(config_read->GetIntProperty("FPS" + str_int)); // FPS値をセット
    setColor(config_read->GetIntProperty("ColorFormat" + str_int)); // 送信するカラーフォーマットをセット
    setUseSndTimeCodeFlg(config_read->GetBoolProperty("Timecode_Show_Flag" + str_int)); // タイムスタンプを使用フラグのセット
    setUsetPreViewFlg(config_read->GetBoolProperty("Preview_Show_Flag" + str_int)); // imshow使用フラグのセット

    delete config_read; // コンフィグファイル読み込みインスタンスの削除
}

/****************************************************************************
*
*　getterメソッド
*
*****************************************************************************/
/* 使用フラグの取得 */
bool NdiCom::GetUseFlg()
{
    return (m_use_flg);
}

/* ウェブカメラフラグの取得 */
bool NdiCom::getWebcamFlg()
{
    bool webcam_flg = true; // webカメラ使用フラグ
    if (m_camera_types == "web")
    {
        webcam_flg = false;
    }
    return (webcam_flg);
}

/* 送信フラグの取得 */
bool NdiCom::GetIsRecflg()
{
    return (m_isRecflg);
}

/****************************************************************************
*
*　setterメソッド
*
*****************************************************************************/
/* リソースの使用フラグの設定 */
void NdiCom::setUseFlg(bool str_use_flg)
{
    m_use_flg = str_use_flg;
}

/* リソースのIDを設定 */
void NdiCom::setResourcesID(string resources_id)
{
    m_resources_id = resources_id;
}

/* 取得する映像サイズの設定 */
void NdiCom::setVideoSz(int xres, int yres)
{
    m_xres = xres;
    m_yres = yres;
}

/* FPSの設定 */
void NdiCom::setFps(int sndfps)
{
    m_sndfps = sndfps;
}

/* 送信するカラーフォーマットの設定 */
void NdiCom::setColor(int sndColor)
{
	/*
	// 現状ではこの設定は固定である
	switch (sndColor)
	{
	case 1:
		m_sndNDIColor = NDIlib_FourCC_type_BGRX;
		m_sndCVColor = CV_BGR2BGRA;
		m_setCVCamColor = CV_FOURCC_DEFAULT;
		break;
	case 2:
		m_sndNDIColor = NDIlib_FourCC_type_BGRA;
		m_sndCVColor = CV_BGR2BGRA;
		m_setCVCamColor = CV_FOURCC_DEFAULT;
		break;
	case 3:
		m_sndNDIColor = NDIlib_FourCC_type_RGBX;
		m_sndCVColor = CV_BGR2RGBA;
		m_setCVCamColor = CV_FOURCC_DEFAULT;
		break;
	case 4:
		m_sndNDIColor = NDIlib_FourCC_type_RGBA;
		m_sndCVColor = CV_BGR2RGBA;
		m_setCVCamColor = CV_FOURCC_DEFAULT;
		break;
	case 5: // 利用不可
		m_sndNDIColor = NDIlib_FourCC_type_UYVY;
		m_sndCVColor = CV_YUV2BGRA_UYVY;
		m_setCVCamColor = CV_FOURCC('U', 'Y', 'V', 'Y');
		break;
	}
	*/

}

/* プレビュー画面の表示フラグのセット */
void NdiCom::setUsetPreViewFlg(bool view_flg)
{
    m_preview_flg = view_flg;
}

/* 送信側タイムコードの使用フラグのセット */
void NdiCom::setUseSndTimeCodeFlg(bool timecode_flg)
{
    m_sndtmcode_flg = timecode_flg;
}

/* カメラの種類のセット */
void NdiCom::setUseWebCameraFlg(string camera_types)
{
    m_camera_types = camera_types;
}

/* 使用するwebカメラのデバイス番号の設定 */
void NdiCom::setCamera(int cam_no)
{
    m_webcam_no = cam_no;
}

/* 使用するipカメラのURLの設定 */
void NdiCom::setCamera(string cam_no)
{
    m_other_camera = cam_no;
}

/****************************************************************************
*
*　送信処理
*
*****************************************************************************/
/* 送信準備 */
void NdiCom::sendCreate()
{
    // 送信用インスタンスの生成
    NDIlib_send_create_t NDI_send_create_desc;

    // WEBカメラかIPカメラか判定
    if (m_camera_types == "web")
    {
        // WEBカメラの場合
        m_str_resname = string("WebCameraID:") + m_resources_id; // カメラ番号指定
    }
    else if (m_camera_types == "ip")
    {
        // IPカメラの場合
        m_str_resname = string("IPCameraID:") + m_resources_id; // URL指定
    }
    else if (m_camera_types == "rs" || m_camera_types == "realsense")
    {
        // RealSenseカメラの場合
        m_str_resname = string("RealSenseID:") + m_resources_id; // シリアル番号指定
    }
    NDI_send_create_desc.p_ndi_name = m_str_resname.c_str(); // 送信インスタンス
    NDI_send_create_desc.clock_video = true; // 送信時間を同期させるかどうか
    NDI_send_create_desc.clock_audio = false; // 送信時間を同期させるかどうか
    m_pNDI_send = NDIlib_send_create(&NDI_send_create_desc); // NDI送信インスタンス生成

    // インスタンスの生成チェック
    if (!m_pNDI_send)
    {
        cout << "Generation failure of the instance." << endl;
    }
}

/* 時刻取得 */
string NdiCom::getCurrentTime()
{
    // ローカル変数宣言
    time_t crrent_time; // 時間格納用
    struct tm* current_time_t; // 暦時刻の格納用
    char crrent_time_c[20]; // 時刻格納用変数

    // 時間取得
    crrent_time = time(NULL); // 1970/1/1 00:00:00 から現在までの経過秒数を取得
    // 取得チェック
    if (crrent_time == -1)
    {
        return ("GET TIME ERROR");
    }

    // 日本時間に変更
    current_time_t = localtime(&crrent_time);

    // 構造体だと扱いづらいので所定の書式に変換
    if (!strftime(crrent_time_c, 128, "%Y/%m/%d %H:%M:%S", current_time_t))
    {
        return ("GET TIME ERROR"); // 変換失敗時
    }

    return (string(crrent_time_c));
}

/* 映像送信 */
void NdiCom::sndVideo()
{
    // ローカル変数宣言
    cv::VideoCapture cap; // カメラオープン用変数
    cv::Mat  myframe, sndframe; // カメラから取得したフレームと送信用フレームのデータを格納するする変数
    cv::Point point(30, 30); // フレーム上の座標を指定する
    string strTimecode; // 文字列に変換された時間を格納するための変数

    m_isRecflg = true; // 送信フラグオン



    // カメラオープン
    if (getWebcamFlg())
    {
        // Webカメラの場合
        cap.open(m_webcam_no); // stringをint型に変換
    }
    else
    {
        // IPカメラの場合
        cap.open(m_other_camera);
    }

	// オープンしたカメラデバイスの設定
	// cap.set(CV_CAP_PROP_FOURCC, setCVCamColor); // m_setCVCamColor指摘先
	cap.set(cv::CAP_PROP_FRAME_WIDTH, m_xres);  // 幅
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, m_yres); // 高さ
	cap.set(cv::CAP_PROP_FPS, m_sndfps); // フレームレート

	// フレームサイズ更新（カメラの性能以上要求によるエラー落ち対策）
	m_xres = (int)cap.get(cv::CAP_PROP_FRAME_WIDTH); // 横方向
	m_yres = (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT); // 縦方向

    // カメラオープン成功判定
    if (!cap.isOpened())
    {
        // カメラオープン失敗時
        if (getWebcamFlg())
        {
            cout << "Camera_ID " << m_webcam_no << " could not open." << endl; // WEBカメラならカメラ番号
        }
        else
        {
            cout << "Camera_ID " << m_other_camera << " could not open." << endl; // IPカメラならURL
        }
        m_isRecflg = false;
        return;
    }

    // 送信用インスタンス生成
    sendCreate();

    // 映像用フレームの生成と初期化
    NDIlib_video_frame_v2_t video_frame;
    video_frame.xres = m_xres; // 横方向解像度の指定
    video_frame.yres = m_yres; // 縦方向解像度の指定
    video_frame.FourCC = NDIlib_FourCC_type_BGRA; // m_sndNDIColor指定先。フレームのカラーフォーマット指定
    video_frame.frame_format_type = NDIlib_frame_format_type_interleaved;
    video_frame.p_data = (uint8_t*)malloc(m_xres * m_yres * 4); // データサイズの指定
    video_frame.line_stride_in_bytes = m_xres * 4;




    // Send Start
    cout << m_resources_id << "SENDING..." << endl;
    m_exit_snd_loop = false;

    // 送信処理
    while (!m_exit_snd_loop)
    {
        // カメラから読み込み
        cap.read(myframe);

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
				myframe, // 画像
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
				cv::LINE_AA // アンチエイリアス
			);
		}
		cv::cvtColor(myframe, sndframe, cv::COLOR_BGR2BGRA); // m_sndCVColorの指定先。色の変換(NDI送信用フレームに乗せるにはBGRXに変換する必要がある)
		
		memcpy((void*)video_frame.p_data, sndframe.data, (m_xres * m_yres * 4)); // OpenCVのフレームをNDIフレームデータにコピー
		
		// 送信
		NDIlib_send_send_video_v2(m_pNDI_send, &video_frame);

        if (m_preview_flg)
        {
            cv::imshow(m_str_resname, sndframe); // 画像を表示

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
    free(video_frame.p_data); // 映像フレームのデータ領域を解放
    NDIlib_send_destroy(m_pNDI_send); // 送信用フレームの開放
    m_isRecflg = false;
}

/* 送信Threadの作成 */
void NdiCom::CretateSndVideoThread()
{
    if (m_use_flg)
    {
        m_isRecflg = true; // 送信フラグオン
        m_thread_video = thread(&NdiCom::sndVideo, this);
    }
}

/* 送信Threadの開放 */
void NdiCom::DeleteSndVideoThread()
{
    for (;;) // 無限ループ
    {
        if (!m_exit_snd_loop)
        {
            m_exit_snd_loop = true; // 送信ループ終了フラグオン
        }

        // Sleep
        chrono::milliseconds dura(100);
        this_thread::sleep_for(dura);

        // 送信処理終了済みまで待機
        if (!GetIsRecflg())
        {
            if (!GetUseFlg())
            {
                break;
            }
            // join可能か判定
            if (m_thread_video.joinable())
            {
                m_thread_video.join(); // スレッドをjoinするまで待機
                break; // forループ抜ける
            }
        }
    }
}