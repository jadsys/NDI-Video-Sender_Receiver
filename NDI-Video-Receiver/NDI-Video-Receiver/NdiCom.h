#pragma once
#include <malloc.h>
#include <time.h>
#include <iostream>
#include <chrono>
#include <string>
#include <thread>
#include <atomic>
#include <opencv/cv.hpp>    // cvtColor
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Processing.NDI.Lib.h>

/* linux動作用ヘッダ */
#ifdef __linux__
#include <csignal>
#include <cstddef>
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#endif // __linux__

/* Windows動作用ヘッダ */
#ifdef _WIN32
#include <time.h>
#include <signal.h>
#endif


#define NDI_REC_MAX     6



class NdiCom
{
public:
	NdiCom(const int channel_no, const NDIlib_source_t & p_source);
	~NdiCom();
	bool GetIsRecFlg();
	void creatRecVideoThread();
	void DeleteRecVideoThread();

private:
	bool m_exit_rec_loop;//受信ループ終了フラグ
	bool m_isRecv;//受信フラグ
	bool m_isHighest;//高解像度フラグ
	std::string m_str_resname;//リソース名格納用
	std::thread m_receive_thread;//受信Thread用変数

	NDIlib_recv_instance_t m_pNDI_recv;//受信用インスタンス

	cv::Mat m_rcvframe;//受信用フレーム
	int m_frame_xres, m_frame_yres, m_frame_xy;//受信フレームの解像度格納用
	const int m_channel_no;//チャンネル数格納用

	void recVideo();
};

