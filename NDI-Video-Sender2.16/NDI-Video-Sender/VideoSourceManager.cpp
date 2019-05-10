#include "VideoSourceManager.h"



VideoSourceManager::VideoSourceManager()
{
    sources = new vector<VideoSource *>();
    using_flag = new vector<int>();
}


VideoSourceManager::~VideoSourceManager()
{
    for (auto itr = sources->begin(); itr != sources->end(); ++itr) {
        delete *itr;
    }
    delete sources;
    delete using_flag;
}

void VideoSourceManager::addVideoSource(int camera_numbeer)
{
    auto *config_read = new ConfigRead(); // コンフィグファイル読み込み処理用インスタンス
    string str_int; // 起動引数で指定した値からカメラ番号を設定するための変数
    str_int = "_CAM";
    str_int += to_string(camera_numbeer); // 指定したカメラ番号を格納
    //カメラタイプに合わせてインスタンスを作成し登録
    string type = config_read->GetStringProperty("Camera_Types" + str_int);
    if (type == "web") {
        auto *usb = new USBCamera(camera_numbeer);
        sources->push_back((VideoSource *) usb);
    }
    else if(type == "rs" || type == "realsense"){
        auto *rs = new RealSense(camera_numbeer);
        sources->push_back((VideoSource *) rs);
    }

}

/***
 * 送信スレッドを登録する
 * @param sender_number  登録するスレッド番号
 */
void VideoSourceManager::registSender(int sender_number)
{
    //送信スレッドを登録する。初期状態ではスレッドと同じカメラ番号を登録しておく
    thread_camera_map.insert(bimap_value_t(sender_number, sender_number - 1));
}

/**
 * スレッドに紐付いたカメラを切り替える
 * 別のスレッドと紐付いているカメラには切り替えられずfalseが返る
 * @param sender_number スレッド番号
 * @param camera_number 受信したいカメラの番号
 * @return 切り替えに成功した場合true/失敗するとfalse
 */
bool VideoSourceManager::requestVideoSource(int sender_number, int camera_number)
{
    //同時書き込み防止のためロック
    m.lock();
    bimap_t::right_iterator itr = thread_camera_map.right.find(thread_camera_map.left.at(sender_number));
    const bool successful_replace = thread_camera_map.right.replace_key(itr, camera_number - 1);
    m.unlock();
    return successful_replace;
}

/**
 * 映像フレームを取得する
 * @param sender_number 取得するスレッド
 * @return フレーム
 */
cv::Mat VideoSourceManager::getFrame(int sender_number)
{
    int camera_number = thread_camera_map.left.at(sender_number);
    return sources->at(camera_number)->getFrame();
}

/**
 * RealSenseのモードを切り替える
 * もしそれ以外のカメラがつながっていた場合は引数にかかわらず何も起こらない
 * @param sender_number スレッド番号
 * @param camera_mode 変更先のカメラモード
 */
void VideoSourceManager::setRealSenseMode(int sender_number, cameraMode camera_mode)
{
    int camera_number = thread_camera_map.left.at(sender_number);
    sources->at(camera_number)->setCameraMode(camera_mode);
}
