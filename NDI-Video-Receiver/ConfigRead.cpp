#include "ConfigRead.h"

using namespace std;

/****************************************************************************
*																			
*　コンストラクタ・デストラクタ												
*																			
*****************************************************************************/
/* コンストラクタ */
ConfigRead::ConfigRead()
{
	// 初期化
	m_defvalue = "";
	m_int_defvalue = 0;
}

/* デストラクタ */
ConfigRead::~ConfigRead()
{
	// 処理なし
}

/****************************************************************************
*																			
*　getterメソッド															
*																			
*****************************************************************************/
/* コンフィグからの値取得 */
string ConfigRead::GetStringProperty(string op_name)
{
	return (readConfigFile(op_name));
}

/* デフォルト値の読み込み */
string ConfigRead::getDefaultValue()
{
	return (m_defvalue);
}

/****************************************************************************
*																			
*　setterメソッド															
*																			
*****************************************************************************/
/* デフォルト値の設定 */
void ConfigRead::setDefaltValue(string op_name)
{
	// op_nameに入ってきたオプションに一致するデフォルト値が入る
	if (!op_name.find("Resources_ID_CH"))
	{
		m_defvalue = ""; // デフォルトは空文字
		m_int_defvalue = 1; // 設定項目番号
	}
}

/****************************************************************************
*																			
*　ファイル読み込み															
*																			
*****************************************************************************/
string ConfigRead::readConfigFile(string op_name)
{
	string str_cnf; // 1行分のコンフィグデータ格納
	string str_property; // プロパティ格納用

	// 検索前の初期化
	setDefaltValue(op_name);

	// configファイル読み込み
	ifstream iconfig_stream(CONFIG_FILE_NAME);

	// 読み込みチェック
	if (!iconfig_stream)
	{
        std::cout << "ファイル名が" << CONFIG_FILE_NAME << "と一致しているか確認して下さい。" << endl;
		return (getDefaultValue()); // 読み込み失敗時defaultを読み込む
	}
	else
	{
		while (true)
		{
			// 一行読み出す
			if (getline(iconfig_stream, str_cnf))
			{
				// コメントと不正な値を弾く
				if (str_cnf.find("#") == -1 && str_cnf.find(op_name) != -1)
				{
					// 読み出した1行からプロパティだけをトリミング
					str_property = getProperty(str_cnf, op_name);
					break;
				}
			}
			else
			{
				str_property = getDefaultValue(); // 読み込み失敗時defaultを読み込む
				break;
			}
		}
	}
	return (str_property);
}

/****************************************************************************
*																			
*　プロパティ値取り出し														
*																			
*****************************************************************************/
string ConfigRead::getProperty(string conf_data, string op_name)
{
	/* プロパティの値を抜き出す */
	const char* trimCharacterList = " \t\v\r\n"; // 各特殊文字のトリム用
	// ex)lineStr:"Option1 = true  \n"
	// 右側から最初の文字列を検索
	size_t p_end = conf_data.find_last_not_of(trimCharacterList); // lineStr:"Option1 = true"
	// =の位置検索
	size_t p_start = conf_data.find("=") + 1; // lineStr:" true"
	// オプション値の空欄を検出
	if (!(p_start < p_end))
	{
		return (getDefaultValue()); // defaultを読み込む
	}
	// 左側から最初の文字列を検索
	p_start = conf_data.find_first_not_of(trimCharacterList, p_start); // lineStr = "true"

	// プロパティの値の文字の長さ
	size_t p_leng = p_end - p_start + 1;

	// プロパティ値だけ抜き出し返す
	return (conf_data.substr(p_start, p_leng));
}

