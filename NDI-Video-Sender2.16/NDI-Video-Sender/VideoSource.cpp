#include "VideoSource.h"



VideoSource::VideoSource(int camera_number)
{
    ConfigRead* config_read = new ConfigRead(); // コンフィグファイル読み込み処理用インスタンス
    string str_int; // 起動引数で指定した値からカメラ番号を設定するための変数
    str_int = "_CAM";
    str_int += to_string(camera_number); // 指定したカメラ番号を格納
 
    resource_name = config_read->GetStringProperty("Resources_ID" + str_int);

    m_xres = config_read->GetIntProperty("xres" + str_int);
    m_yres = config_read->GetIntProperty("yres" + str_int);
    m_sndfps = config_read->GetIntProperty("FPS" + str_int);
    is_insert_time_stamp = config_read->GetBoolProperty("Timecode_Show_Flag" + str_int);

    delete config_read; // コンフィグファイル読み込みインスタンスの削除
}

VideoSource::~VideoSource()
{
}
