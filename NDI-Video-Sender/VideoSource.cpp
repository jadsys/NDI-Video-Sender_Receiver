#include "VideoSource.h"



VideoSource::VideoSource(int camera_number)
{
    auto *config_read = new ConfigRead(); // コンフィグファイル読み込み処理用インスタンス
    string str_int; // 起動引数で指定した値からカメラ番号を設定するための変数
    str_int = "_CAM";
    str_int += to_string(camera_number); // 指定したカメラ番号を格納

    //x方向解像度取得
    m_xres = config_read->GetIntProperty("xres" + str_int);
    //y方向解像度取得
    m_yres = config_read->GetIntProperty("yres" + str_int);
    //fps取得
    m_sndfps = config_read->GetIntProperty("FPS" + str_int);

    delete config_read; // コンフィグファイル読み込みインスタンスの削除
}


VideoSource::~VideoSource()
{
}
