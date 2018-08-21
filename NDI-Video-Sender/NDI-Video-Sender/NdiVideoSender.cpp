// NDI-Video-Sender-win.cpp : アプリケーションのエントリ ポイントを定義します。
#include "NdiCom.h"
#include "ConfigRead.h"

using namespace std;

/* グローバル変数 */
static atomic<bool> exit_main_loop(false);// メインループの終了フラグ
NdiCom *ndi_video_snd = NULL;

/* シグナルハンドラ */
void sigint_handler(int)
{
	exit_main_loop = true;
}

/* インスタンス初期化 */
bool SetValue(int argv_no)
{
	ConfigRead *config_read = new ConfigRead();//コンフィグファイル読み込み処理用インスタンス
	string str_int;//起動引数で指定した値からカメラ番号を設定するための変数
	str_int = "_CAM";
	str_int += to_string(argv_no);//指定したカメラ番号を格納

	
	ndi_video_snd->SetUseFlg(config_read->GetBoolProperty("SwitchCamera" + str_int));//カメラを使用するかをセット
	//初期化判定（使用しない設定なら失敗）
	if (!ndi_video_snd->GetUseFlg())
	{
		return (false);
	}
	ndi_video_snd->SetResourcesID(config_read->GetStringProperty("Resources_ID" + str_int));//リソース名をセット
	ndi_video_snd->SetUseWebCameraFlg(config_read->GetBoolProperty("Camera_Flag" + str_int));//WEBカメラかどうかセット
	//WEBカメラならデバイス番号、IPカメラならURLの指定
	if (ndi_video_snd->GetWebcamFlg())
	{
		ndi_video_snd->SetCamera(config_read->GetIntProperty("Camera_ID" + str_int));//WEBカメラのデバイス番号のセット
	}
	else
	{
		ndi_video_snd->SetCamera(config_read->GetStringProperty("Camera_ID" + str_int));//IPカメラURLのセット
	}
	
	ndi_video_snd->SetVideoSz(config_read->GetIntProperty("xres" + str_int), config_read->GetIntProperty("yres" + str_int));//解像度をセット
	ndi_video_snd->SetFps(config_read->GetIntProperty("FPS" + str_int));//FPS値をセット
	ndi_video_snd->SetColor(config_read->GetIntProperty("ColorFormat" + str_int));//送信するカラーフォーマットをセット
	ndi_video_snd->SetUseSndTimeCodeFlg(config_read->GetBoolProperty("Timecode_Show_Flag" + str_int));//タイムスタンプを使用フラグのセット
	ndi_video_snd->SetUsetPreViewFlg(config_read->GetBoolProperty("Preview_Show_Flag" + str_int));//imshow使用フラグのセット
	
	delete config_read;//コンフィグファイル読み込みインスタンスの削除

	return (true);//初期化成功
}



int main(int argc, char *argv[])
{
	//シグナルハンドラ
	signal(SIGINT, sigint_handler);//SIGINTはCtrl+C押されたとき
	puts("Press <Ctrl> + <C> to exit...");

	//NDIの初期化
	if (!NDIlib_initialize())
	{
		printf("Cannot run NDI.\n");
		return (0);//CPUがNDIに対応していなければ終了
	}

	//ローカル変数宣言
	int argv_no = 0;

	//起動オプションの引数チェック
	if (argc != 2 || (atoi(argv[1]) <= 0 || NDI_SND_MAX < atoi(argv[1])))
	{
		printf("起動オプション（カメラ番号1～6）を指定して下さい。プログラムを終了します。\n");
		return (0);
	}

	argv_no = atoi(argv[1]);//起動引数をint型に変換して代入

	//NDI送信インスタンス生成
	ndi_video_snd = new NdiCom();

	//NDI送信インスタンスの初期化
	if (!SetValue(argv_no))
	{
		printf("カメラ%dのSwitchCameraの設定が使用しない（false）になっているため終了致します。\n", argv_no);

		return (0);
	}

	//送信スレッド開始
	ndi_video_snd->CretateSndVideoThread();//送信用Thread作成

	//メインループ
	while (!exit_main_loop)
	{
		// Sleep
		std::chrono::milliseconds dura(100);
		std::this_thread::sleep_for(dura);

		// Ctrl+Cが押されたか監視
		if (!ndi_video_snd->GetIsRecflg())
		{
			exit_main_loop = true;
		}

	}

	//終了処理
	printf("FREE NDI FRAME...\n");
	if (ndi_video_snd)
	{
		ndi_video_snd->DeleteSndVideoThread();//受信Threadの削除
		delete ndi_video_snd;//インスタンス開放
	}
	//NDIの削除
	NDIlib_destroy();

	return (0);
}

