#pragma once

#include <thread>
#include <opencv2/highgui.hpp>
#include <Processing.NDI.Lib.h>
#include "VideoSourceManager.h"

/***
 * NDIで映像送信を行うクラス。コンストラクタ実行時に別スレッドを作成しそこで送信処理を行う
 */
class NDISender {
public:
    /***
     * 送信を行うコンストラクタ。コンストラクタ内で別スレッドを作成
     * 実行時はスレッド番号と同じ番号のカメラの映像が送信される。
     * @param vsm 映像ソースマネージャへのポインタ
     * @param sender_number この送信スレッドの番号。プロセス内でユニークである必要がある。
     */
    NDISender(VideoSourceManager *vsm, int sender_number);

    ~NDISender();

private:
    /***
     * 送信処理
     */
    void sendThread();

    /***
     * この送信スレッドの番号
     */
    int sender_number;
    std::thread ndi_send_thread;
    VideoSourceManager *vsm;
    /***
     * 送信プロパティ構造体
     */
    NDIlib_send_create_t NDI_send_create_desc;
    /***
    * 送信用インスタンス
    */
    NDIlib_send_instance_t m_pNDI_send;
};



