
#include "NdiCom.h"


using namespace std;


/****************************************************************************
*
*　コンストラクタ・デストラクタ
*
*****************************************************************************/
/* コンストラクタ */
NdiCom::NdiCom(const int channel_no, const NDIlib_source_t& p_source)
    : m_pNDI_recv(NULL), m_channel_no(channel_no)
{
    // 初期化
    m_exit_rec_loop = true;
    m_isHighest = true;
    m_isRecv = true;
    m_is_realsense = false;

    // Display the frames per second
    cout << "Channel " << m_channel_no << " is connecting to " << p_source.p_ndi_name << endl;
    m_str_resname = "CH" + to_string(m_channel_no) + " " + p_source.p_ndi_name; // チャンネル番号＋リソース名の格納

    if (m_str_resname.find("RealSense") != -1)
    {
        m_is_realsense = true;
    }

    NDIlib_recv_create_t recv_create_desc = { p_source, NDIlib_recv_color_format_e_BGRX_BGRA, NDIlib_recv_bandwidth_highest, true }; // FULL
//	NDIlib_recv_create_t m_recv_create_desc = { p_source, NDIlib_recv_color_format_e_BGRX_BGRA, NDIlib_recv_bandwidth_lowest, true };  // 低解像度向け

    // 解像度設定
    if (recv_create_desc.bandwidth == NDIlib_recv_bandwidth_lowest)
    {
        m_isHighest = false; // 高解像度フラグオフ
    }

    // Create the receiver
    m_pNDI_recv = NDIlib_recv_create_v2(&recv_create_desc);

    // インスタンスの生成チェック
    if (!m_pNDI_recv)
    {
        cerr << "NDIlib_recv_instance_t create failure." << endl;
        return;
    }

    // Start a thread to receive frames
    creatRecVideoThread(); // 受信用Threadの作成
}

/* デストラクタ */
NdiCom::~NdiCom()
{
}


/****************************************************************************
*
*　getterメソッド
*
*****************************************************************************/
/* 受信フラグ */
bool NdiCom::GetIsRecFlg()
{
    return (m_isRecv);
}

/****************************************************************************
*
*　setterメソッド
*
*****************************************************************************/


/****************************************************************************
*
*　受信処理
*
*****************************************************************************/
/* 受信処理本体 */
void NdiCom::recVideo()
{
    // ローカル変数宣言
    string strFps; // FPS格納用
    char crrent_time_c[20]; // 時刻格納用変数
    int frame_no = 0; // フレーム番号格納用
    bool isInitialized = false; // 初期済みフラグ
    auto prev_time = std::chrono::high_resolution_clock::now(); // 現在時刻取得
    NDIlib_video_frame_v2_t video_frame; // 映像用フレームの生成
    cv::Point point(30, 30); // タイムスタンプの座標指定
    m_exit_rec_loop = false; // 受信終了フラグ

    while (!m_exit_rec_loop)
    {
        // 受信開始
        if (NDIlib_recv_capture_v2(m_pNDI_recv, &video_frame, NULL, NULL, 1000) == NDIlib_frame_type_video)
        {
            // Every 1000 frames we check how long it has taken (1000フレームごとに撮影時間を確認しています)
            if (frame_no == 200)
            {
                // Get the time (時間を得る)
                const auto this_time = chrono::high_resolution_clock::now();
                strFps = m_str_resname + " " + "FPS:" + to_string((float)frame_no / chrono::duration_cast<chrono::duration<float>>(this_time - prev_time).count());

                // 現在時刻取得
                const auto current_time = chrono::system_clock::now(); // 時間取得
                time_t current_time_t = std::chrono::system_clock::to_time_t(current_time); // time_t型変換
                struct tm* now_tm = localtime(&current_time_t); // 日本時間に変更
                strftime(crrent_time_c, 128, "%Y/%m/%d %H:%M:%S", now_tm);

                // Display the frames per second and current time.    (毎秒のフレームと現在時刻を表示する)
                printf("[%s] %s\n", crrent_time_c, strFps.c_str());

                // Cycle the timers and reset the count (タイマーを回してカウントをリセットする)
                prev_time = this_time;
                frame_no = 0;
            }
            else ++frame_no; // フレーム数加算

            // 映像フレームの縦横の解像度を取得
            m_frame_xres = video_frame.xres;
            m_frame_yres = video_frame.yres;
            m_frame_xy = m_frame_xres * m_frame_yres;

            // 1回目のみ処理を行う
            if (!isInitialized)
            {
                m_rcvframe = cv::Mat::zeros(cv::Size(m_frame_xres, m_frame_yres), CV_8UC4); // 初期化
                isInitialized = true; // 初期化完了
            }

            memcpy(m_rcvframe.data, video_frame.p_data, m_frame_xy * 4); // NDIフレームデータをOpenCVのフレームにコピー

            if (!m_isHighest)
            {
                // 画面のリサイズ
                resize(m_rcvframe, m_rcvframe, cv::Size(), 1, 2); // 低解像度向け
            }

            imshow(m_str_resname, m_rcvframe); // フレームの表示

            // キー入力を待つ
            switch (cv::waitKey(1))
            {
            case 3: // imshow中にCtrl+cが入力されたら終了
            case 227: // windowsだとCtrl+cは3、LinuxだとCtrl+cが正しく認識できない。227が返る
                m_exit_rec_loop = true;
                cv::destroyWindow(m_str_resname);
                break;
            case 49: // RGBモード
                if (m_is_realsense)
                {
                    printf("RGB mode\n");
                    camera_mode.p_data = "<RGB_mode enabled=\"true\"/>";
                    NDIlib_recv_send_metadata(m_pNDI_recv, &camera_mode); // メタデータ送信
                }
                break;
            case 50: // Depthモード
                if (m_is_realsense)
                {
                    printf("Depth mode\n");
                    camera_mode.p_data = "<Depth_mode enabled=\"true\"/>";
                    NDIlib_recv_send_metadata(m_pNDI_recv, &camera_mode); // メタデータ送信
                }
                break;
            case 51: // IR1モード
                if (m_is_realsense)
                {
                    printf("IR LEFT mode\n");
                    camera_mode.p_data = "<IR_left_mode enabled=\"true\"/>";
                    NDIlib_recv_send_metadata(m_pNDI_recv, &camera_mode); // メタデータ送信
                }
                break;
            case 52: // IR2モード
                if (m_is_realsense)
                {
                    printf("IR RIGHT mode\n");
                    camera_mode.p_data = "<IR_right_mode enabled=\"true\"/>";
                    NDIlib_recv_send_metadata(m_pNDI_recv, &camera_mode); // メタデータ送信
                }
                break;
            default:
                break;
            }


            NDIlib_recv_free_video_v2(m_pNDI_recv, &video_frame);// 映像受信用オブジェクトの破棄
        }
    }
    m_isRecv = false;
}

/* 受信用Thread作成 */
void NdiCom::creatRecVideoThread()
{
    m_receive_thread = thread(&NdiCom::recVideo, this); // 受信Thread開始
}

/* 受信用Thread解放 */
void NdiCom::DeleteRecVideoThread()
{
    for (;;)// 無限ループ
    {
        if (!m_exit_rec_loop)
        {
            m_exit_rec_loop = true; // 受信ループ終了フラグオン
        }

        // Sleep
        chrono::milliseconds dura(100);
        this_thread::sleep_for(dura);

        // 受信処理終了まで待機
        if (!GetIsRecFlg())
        {
            // Threadのjoin可能か判定
            if (m_receive_thread.joinable())
            {
                m_receive_thread.join(); // スレッドをjoinするまで待機
                break; // forループ抜ける
            }
        }
    }
}
