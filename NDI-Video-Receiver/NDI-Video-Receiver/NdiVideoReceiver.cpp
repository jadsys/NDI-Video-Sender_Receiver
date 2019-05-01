// NDIVideoReceiver.cpp 　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　: コンソール アプリケーションのエントリ ポイントを定義します。
// 
#include "NdiCom.h"
#include "ConfigRead.h"

using namespace std;

/* グローバル変数 */
static atomic<bool> exit_main_loop(false); // メインループの終了フラグ
static atomic<bool> exit_find_loop(false); // 検索ループの終了フラグ
NdiCom* ndi_video_rec = NULL; // 受信インスタンス変数の宣言と初期化

/* シグナルハンドラ */
void sigint_handler(int)
{
    // 終了フラグをオン
    exit_find_loop = true;
    exit_main_loop = true;
}

/* リソース名を取得 */
string getResourceName(int argv_no)
{
    string str_value; // 取得したリソース名を格納する変数
    ConfigRead* config_read = new ConfigRead(); // コンフィグ読み込み用インスタンスの生成
    str_value = config_read->GetStringProperty("CH" + to_string(argv_no)); // リソース名の取得
    delete config_read; // インスタンスの開放

    return (str_value);
}

int main(int argc, char* argv[])
{
    // シグナルハンドラ
    signal(SIGINT, sigint_handler); // SIGINTはCtrl+C押されたとき
    cout << "Press <Ctrl> + <C> to exit..." << endl;

    // NDIの初期化
    if (!NDIlib_initialize())
    {
        cerr << "Cannot run NDI." << endl;
        return (0); // CPUがNDIに対応していなければ終了
    }

    // ローカル変数宣言
    int argv_no = 0; // 起動引数格納用
    uint32_t no_sources; // 発見したリソース数格納用
    string str_readname;; // configファイルから読み込んだリソース名の格納用
    string str_findname; // 発見したリソース名の格納用
    NDIlib_find_create_t NDI_find_create_desc; // 検索に必要な構造体定義(デフォルト値)
    const NDIlib_source_t* p_sources; // 検索したNDIリソース情報格納用
    NDIlib_find_instance_t pNDI_find = NDIlib_find_create_v2(&NDI_find_create_desc); // 検索インスタンス生成

    // 生成チェック
    if (!pNDI_find)
    {
        cerr << "NDIlib_find_instance_t create failure." << endl;
        return EXIT_FAILURE;
    }

    // 起動オプションの引数チェック
    if (argc != 2 || atoi(argv[1]) <= 0 || NDI_REC_MAX < atoi(argv[1]))
    {
        cerr << "起動オプション（カメラ番号1～6）を指定して下さい。プログラムを終了します。" << endl;
        return EXIT_FAILURE;
    }

    argv_no = atoi(argv[1]); // 起動引数をint型に変換して代入

    // リソース名の取得
    str_readname = getResourceName(argv_no);

    // コンフィグファイル読み込み失敗時
    if (str_readname == "")
    {
        cerr << "コンフィグファイルの読み込みに失敗しました。プログラムを終了します。" << endl;
        return EXIT_FAILURE;
    }

    // 検索処理開始
    cout << "SEARCHING..." << endl;
    while (!exit_find_loop)
    {
        NDIlib_find_wait_for_sources(pNDI_find, 1000); // 見つかるまで待機（タイムアウト1000msec）
        p_sources = NDIlib_find_get_current_sources(pNDI_find, &no_sources); // リソース情報の取得
        for (int i = 0; i < (int)no_sources; i++)
        {
            str_findname = string(p_sources[i].p_ndi_name); // 発見したリソース名の格納

            // 発見したリソース名にconfigから読み込んだリソース名が存在するか？見つからなければ-1が返る
            if (str_findname.find(str_readname) != -1)
            {
                cout << "HIT" << endl;
                ndi_video_rec = new NdiCom(argv_no, p_sources[i]); // 受信インスタンスの生成
                exit_find_loop = true; // ループ終了フラグオン
                break;
            }
        }
    }
    NDIlib_find_destroy(pNDI_find); // ファインダの削除

    // メインループ
    while (!exit_main_loop)
    {
        // Sleep
        chrono::milliseconds dura(100); // ミリセカンドを表現
        this_thread::sleep_for(dura); // 100msの間Sleep

        // Ctrl+Cが押されたか監視
        if (!ndi_video_rec->GetIsRecFlg())
        {
            exit_main_loop = true; // メインループ終了フラグをオン
        }
    }

    // 終了処理
    cout << "FREE NDI FRAME..." << endl;
    if (ndi_video_rec)
    {
        ndi_video_rec->DeleteRecVideoThread(); // 受信Threadの削除
        delete ndi_video_rec; // インスタンスの削除
    }

    // NDIの削除
    NDIlib_destroy();

    return (0);
}
