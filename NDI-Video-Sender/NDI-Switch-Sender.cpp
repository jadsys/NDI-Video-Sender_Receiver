#include "VideoSourceManager.h"
#include "NDISender.h"
#include <csignal>


int main(int argc, char *argv[]) {

    sigset_t ss;
    int signal_number;
    //引数解析
    if (argc != 3) {
        cerr << "第一引数にカメラ数、第二引数に送信スレッド数を指定してください" << endl;
        return EXIT_FAILURE;
    }
    //引数の文字列を数値に変換
    long camera_count = std::strtol(argv[1], NULL, 10);
    long thread_count = std::strtol(argv[2], NULL, 10);
    if (camera_count == 0 || thread_count == 0) {
        cerr << "第一引数にカメラ数、第二引数に送信スレッド数を指定してください" << endl;
        return EXIT_FAILURE;
    }
    if (camera_count < thread_count) {
        cerr << "送信スレッド数はカメラ数以下である必要があります" << endl;
        return EXIT_FAILURE;
    }

    //送信準備開始
    //映像ソースマネージャのインスタンスを作成
    auto *vsm = new VideoSourceManager();
    NDISender *sender[camera_count];
    //カメラを登録
    for (int i = 1; i <= camera_count; i++) {
        vsm->addVideoSource(i);
    }
    //送信スレッドを登録し順次送信開始
    for (int i = 1; i <= thread_count; i++) {
        vsm->registSender(i);
        sender[i] = new NDISender(vsm, i);
    }
    // SIGINTおよびSIGHUPを待機
    // 受信した場合には終了処理に移る
    bool end_flag = false;
    while (!end_flag) {
        if (sigwait(&ss, &signal_number) == 0) {
            switch (signal_number) {
                case SIGINT:
                case SIGHUP:
                    end_flag = true;
                    break;
                default:
                    break;
            }
        }
    }
    //送信スレッドと映像マネージャを削除
    for (int i = 1; i <= thread_count; i++) {
        delete sender[i];
    }
    delete vsm;
    return EXIT_SUCCESS;
}

