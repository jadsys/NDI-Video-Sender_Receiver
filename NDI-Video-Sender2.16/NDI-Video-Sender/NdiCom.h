#pragma once
#include <thread>
#include <atomic>
#include <string>
#include <iostream>
#include <assert.h>
#include <time.h>
#include <malloc.h>
#include <string.h>
#include <Processing.NDI.Lib.h>
#include "ConfigRead.h"

/* linux動作用ヘッダ */
#ifdef __linux__
#include <csignal>
#include <cstddef>
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <strings.h>
#include <sys/time.h>
#include <opencv/cv.hpp>    // cv::cvtColor
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#endif // __linux__

/* Windows動作用ヘッダ */
#ifdef _WIN32
#include <sys/timeb.h>
#include <opencv2\opencv.hpp>
#include <signal.h>
#include <Windows.h>

#define strcasecmp stricmp // LinuxとWindowsで標準関数が違う
#endif

#define NDI_SND_MAX     6


class NdiCom
{
public:
	NdiCom();
	NdiCom(int argv_no);
	~NdiCom();
	bool GetUseFlg();
	bool GetIsRecflg();
	virtual void CretateSndVideoThread();
	void DeleteSndVideoThread();

protected:
	NDIlib_send_instance_t m_pNDI_send; // 送信インスタンス
	int m_xres, m_yres; // 解像度
	int m_sndfps; // FPS指定用
	bool m_use_flg; // 使用フラグ
	bool m_preview_flg; // imshowフラグ
	bool m_sndtmcode_flg; // タイムスタンプ挿入フラグ
	bool m_exit_snd_loop; // 送信ループ終了フラグ
	bool m_isRecflg; // 送信処理フラグ
	std::string m_resources_id; // リソース番号格納用
	std::string m_other_camera; // ipカメラ番号格納用
	std::string m_str_resname; // 送信リソース名格納用
	std::thread m_thread_video; // 送信用Thread

	virtual void sndVideo();
	std::string getCurrentTime();
	void sendCreate();

private:
	NDIlib_FourCC_type_e m_sndNDIColor; // 送信カラーフォーマット
	int m_sndCVColor; // 変換するカラーフォーマットの指定
	int m_setCVCamColor; // カメラデバイスのカラーフォーマット
	int m_webcam_no; // カメラ番号格納用
	std::string m_camera_types; // カメラの種類

	void initialize(int argv_no);
	bool getWebcamFlg();
	void setUseFlg(bool str_use_flg);
	void setResourcesID(std::string resources_id);
	void setVideoSz(int xres, int yres);
	void setFps(int sndfps);
	void setColor(int sndColor);
	void setUsetPreViewFlg(bool view_flg);
	void setUseSndTimeCodeFlg(bool timecode_flg);
	void setUseWebCameraFlg(std::string webcam_flg);
	void setCamera(int cam_no);
	void setCamera(std::string cam_no);
};

