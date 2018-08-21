#pragma once
#include <thread>
#include <atomic>
#include <time.h>
#include <malloc.h>
#include <string>
#include <iostream>
#include <assert.h>
#include <Processing.NDI.Lib.h>

/* linux動作用ヘッダ */
#ifdef __linux__
#include <csignal>
#include <cstddef>
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <atomic>
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

#endif

#define NDI_SND_MAX     6


class NdiCom
{
public:
	NdiCom();
	~NdiCom();

	bool GetUseFlg();
	bool GetWebcamFlg();
	bool GetIsRecflg();

	void SetUseFlg(bool str_use_flg);
	void SetResourcesID(std::string resources_id);
	void SetVideoSz(int xres, int yres);
	void SetFps(int sndfps);
	void SetColor(int sndColor);
	void SetUsetPreViewFlg(bool view_flg);
	void SetUseSndTimeCodeFlg(bool timecode_flg);
	void SetUseWebCameraFlg(bool webcam_flg);
	void SetCamera(int cam_no);
	void SetCamera(std::string cam_no);
	void SndVideo();
	void CretateSndVideoThread();
	void DeleteSndVideoThread();

private:
	NDIlib_FourCC_type_e m_sndNDIColor;//送信カラーフォーマット
	NDIlib_send_instance_t m_pNDI_send; //送信インスタンス
	int m_sndCVColor;//変換するカラーフォーマットの指定
	int m_xres, m_yres;//解像度
	int m_sndfps;//FPS指定用
	std::string m_resources_id;//リソース番号格納用
	int m_webcam_no;//カメラ番号格納用
	std::string m_ipcam_no;
	int m_setCVCamColor;//カメラデバイスのカラーフォーマット
	bool m_use_flg;//使用フラグ
	bool m_webcam_flg;//WEBカメラフラグ
	bool m_preview_flg;//imshowフラグ
	bool m_sndtmcode_flg;//タイムスタンプ挿入フラグ
	bool m_exit_snd_loop;//送信ループ終了フラグ
	bool m_isRecflg;//送信処理フラグ
	std::string m_strres;//送信リソース名格納用
	std::thread m_thread_video;//送信用Thread
	std::string getCurrentTime();
	void sendCreate();
};

