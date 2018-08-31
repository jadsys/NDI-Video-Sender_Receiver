// NDI-Video-Sender-win.cpp : アプリケーションのエントリ ポイントを定義します。
#include "NdiCom.h"
#include "ConfigRead.h"
#include "Realsense.h"

using namespace std;

/* グローバル変数 */
static atomic<bool> exit_main_loop(false); // メインループの終了フラグ
NdiCom *ndi_video_snd = NULL;

/* シグナルハンドラ */
void sigint_handler(int)
{
	exit_main_loop = true;
}




int main(int argc, char *argv[])
{
	// シグナルハンドラ
	signal(SIGINT, sigint_handler); // SIGINTはCtrl+C押されたとき
	puts("Press <Ctrl> + <C> to exit...");

	// NDIの初期化
	if (!NDIlib_initialize())
	{
		printf("Cannot run NDI.\n");
		return (0); // CPUがNDIに対応していなければ終了
	}

	// ローカル変数宣言
	int argv_no = 0;

	// 起動オプションの引数チェック
	if (argc != 2 || (atoi(argv[1]) <= 0 || NDI_SND_MAX < atoi(argv[1])))
	{
		printf("起動オプション（カメラ番号1～6）を指定して下さい。プログラムを終了します。\n");
		return (0);
	}

	argv_no = atoi(argv[1]); // 起動引数をint型に変換して代入

	// NDI送信インスタンス生成
	ConfigRead config_read;
	string camera_types = config_read.GetStringProperty("Camera_Types_CAM" + string(argv[1]));
	if (strcasecmp(camera_types.c_str(), "rs") == 0 || strcasecmp(camera_types.c_str(), "realsense") == 0)
	{
		ndi_video_snd = new Realsense(argv_no);
	}
	else
	{
		ndi_video_snd = new NdiCom(argv_no);
	}

	// 送信フラグチェック
	if(!ndi_video_snd->GetUseFlg())
	{
		printf("カメラ%dのSwitchCameraの設定が使用しない（false）になっているため終了致します。\n", argv_no);
		delete ndi_video_snd; // インスタンス削除
		return (0);
	}
	
	// 送信スレッド開始
	ndi_video_snd->CretateSndVideoThread(); // 送信用Thread作成

	// メインループ
	while (!exit_main_loop)
	{
		// Sleep
		chrono::milliseconds dura(1000);
		this_thread::sleep_for(dura);

		// Ctrl+Cが押されたか監視
		if (!ndi_video_snd->GetIsRecflg())
		{
			exit_main_loop = true; // メインループ終了
		}

	}

	// 終了処理
	printf("FREE NDI FRAME...\n");
	if (ndi_video_snd)
	{
		ndi_video_snd->DeleteSndVideoThread(); // 受信Threadの削除
		delete ndi_video_snd; // インスタンス開放
	}

	// NDIの削除
	NDIlib_destroy();

	return (0);
}

