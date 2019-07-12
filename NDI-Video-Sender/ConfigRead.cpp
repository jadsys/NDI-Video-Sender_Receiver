#include "ConfigRead.h"

using namespace std;

/****************************************************************************
*
*　コンストラクタ・デストラクタ
*
*****************************************************************************/
/* コンストラクタ */
ConfigRead::ConfigRead() {
    // 初期化
    m_defvalue = "";
    m_int_defvalue = 0;
}

/* デストラクタ */
ConfigRead::~ConfigRead() {
    // 処理なし
}

/****************************************************************************
*
*　getterメソッド
*
*****************************************************************************/
/* コンフィグからの値取得(String型) */
string ConfigRead::GetStringProperty(string op_name) {
    string str_read_property = readConfigFile(op_name); // configファイルから読み込んだ生の値を格納用

    // configから読み込んだ値が想定された値かチェック
    if (!checkValue(str_read_property)) {
        str_read_property = getDefaultValue(op_name); // デフォルトを読み込む
    }
    std::transform(str_read_property.cbegin(), str_read_property.cend(), str_read_property.begin(),
                   [](unsigned char c) -> unsigned char { return std::tolower(c); });
    return (str_read_property);
}

/* コンフィグからの値取得(bool型) */
bool ConfigRead::GetBoolProperty(string op_name) {
    string str_read_property = readConfigFile(op_name); // configファイルから読み込んだ生の値を格納用
    bool bool_read_property = false; // bool型に変換した値の格納用

    // configから読み込んだ値が想定された値かチェック
    if (!checkValue(str_read_property)) {
        // デフォルトの値を読み込む
        str_read_property = getDefaultValue(op_name);
    }

    // string型→bool型へ変換(デフォルトがfalseのため、falseの変換は必要なし)
    // windowsでは標準関数がstricmpになる
    // 機能はどちらも大文字小文字関係なく比較を行う
    if (strEqualCaseInsensitive(str_read_property, "true") == 0) {
        bool_read_property = true;
    }

    return (bool_read_property);
}

/* コンフィグからの値取得(int型) */
int ConfigRead::GetIntProperty(string op_name) {
    string str_read_property = readConfigFile(op_name); // configファイルから読み込んだ生の値を格納用
    int int_read_property; // int型に変換した値の格納用

    // configから読み込んだ値が想定された値かチェック
    if (!checkValue(str_read_property)) {
        // デフォルトの値を読み込む
        str_read_property = getDefaultValue(op_name);
    }

    // string型→int型へ変換
    int_read_property = stoi(str_read_property);

    return (int_read_property);
}

/* コンフィグからの値取得(double型) */
double ConfigRead::GetDoubleProperty(std::string op_name) {

    string str_read_property = readConfigFile(op_name); // configファイルから読み込んだ生の値を格納用
    double double_read_property; // int型に変換した値の格納用


    // string型→int型へ変換
    try {
        double_read_property = stod(str_read_property);
    } catch (const std::invalid_argument &e) {
        std::cerr << e.what() << "in" << op_name << endl;
        double_read_property = 0.0;
    }

    return (double_read_property);
}


/* デフォルト値の読み込み */
string ConfigRead::getDefaultValue(string op_name) {
    return (m_defvalue);
}

/****************************************************************************
*
*　setterメソッド
*
*****************************************************************************/
/* デフォルト値の設定 */
void ConfigRead::setDefaultValue(string op_name) {
    // op_nameに入ってきたオプションに一致するデフォルト値が入る
    if (!op_name.find("SwitchCamera", 0)) {
        m_defvalue = "false";
        m_int_defvalue = 1;
    } else if (!op_name.find("Resources")) {
        m_defvalue = "Camera_default";
        m_int_defvalue = 2;
    } else if (!op_name.find("Camera_Types")) {
        m_defvalue = "web";
        m_int_defvalue = 3;
    } else if (!op_name.find("Camera_ID")) {
        m_defvalue = "0";
        m_int_defvalue = 4;
    } else if (!op_name.find("xres")) {
        m_defvalue = "640";
        m_int_defvalue = 5;
    } else if (!op_name.find("yres")) {
        m_defvalue = "480";
        m_int_defvalue = 6;
    } else if (!op_name.find("FPS")) {
        m_defvalue = "30";
        m_int_defvalue = 7;
    } else if (!op_name.find("ColorFormat")) {
        m_defvalue = "1";
        m_int_defvalue = 8;
    } else if (!op_name.find("Preview_Show_Flag")) {
        m_defvalue = "true";
        m_int_defvalue = 9;
    } else if (!op_name.find("Timecode_Show_Flag")) {
        m_defvalue = "true";
        m_int_defvalue = 10;
    }

}

/****************************************************************************
*
*　ファイル読み込み
*
*****************************************************************************/
string ConfigRead::readConfigFile(string op_name) {
    string str_cnf; // 1行分のコンフィグデータ格納
    string str_property; // プロパティ格納用

    // 検索前の初期化
    setDefaultValue(op_name);

    // configファイル読み込み
    ifstream iconfig_stream(CONFIG_FILE_NAME);
    if (!iconfig_stream) {
        return (getDefaultValue(op_name)); // 読み込み失敗時defaultを読み込む
    } else {
        while (true) {
            // 一行読み出す
            if (getline(iconfig_stream, str_cnf)) {
                // コメントと不正な値を弾く
                if (str_cnf.find("#") == -1 && str_cnf.find(op_name) != -1) {
                    // 読み出した1行からプロパティだけをトリミング
                    str_property = getProperty(str_cnf, op_name);
                    break;
                }
            } else {
                str_property = getDefaultValue(op_name); // 読み込み失敗時defaultを読み込む
                break;
            }
        }
    }
    return (str_property);
}

bool ConfigRead::strEqualCaseInsensitive(const std::string &a, const std::string &b) {
    return std::equal(a.begin(), a.end(),
                      b.begin(), b.end(),
                      [](char a, char b) {
                          return tolower(a) == tolower(b);
                      });
}

/****************************************************************************
*
*　プロパティ値取り出し
*
*****************************************************************************/
string ConfigRead::getProperty(string conf_data, string op_name) {
    /* プロパティの値を抜き出す */
    const char *trimCharacterList = " \t\v\r\n";
    // ex)lineStr = "Option1 = true  \n"
    // 右側から最初の文字列を検索
    size_t p_end = conf_data.find_last_not_of(trimCharacterList); // lineStr = "Option1 = true"
    // ＝の位置検索
    size_t p_start = conf_data.find("=") + 1; // lineStr = " true"
    // オプション値の空欄を検出
    if (!(p_start < p_end)) {
        return (getDefaultValue(op_name)); // defaultを読み込む
    }
    // 左側から最初の文字列を検索
    p_start = conf_data.find_first_not_of(trimCharacterList, p_start); // lineStr = "true"
    // プロパティの値の文字の長さ
    size_t p_leng = p_end - p_start + 1;

    // プロパティ値だけ抜き出し返す
    return (conf_data.substr(p_start, p_leng));
}

/****************************************************************************
*
*　中身のチェック
*
*****************************************************************************/
bool ConfigRead::checkValue(string value) {
    switch (m_int_defvalue) {
        case 1: // カメラを使用するか
            // windowsでは標準関数がstricmpになる
            // 機能はどちらも大文字小文字関係なく比較を行う
            if (!strEqualCaseInsensitive(value, "true") == 0) {
                return (false);
            }
            break;

        case 9: // 送信側のタイムコードを表示するか
        case 10: // プレビューを表示するか
            // windowsでは標準関数がstricmpになる
            // 機能はどちらも大文字小文字関係なく比較を行う
            if (!strEqualCaseInsensitive(value, "false") == 0) {
                return (false);
            }
            break;
        case 3: // リソースのカメラの種類
            if ((strEqualCaseInsensitive(value, "web") != 0) &&
                (strEqualCaseInsensitive(value, "ip") != 0) &&
                (strEqualCaseInsensitive(value, "realsense") != 0) &&
                (strEqualCaseInsensitive(value, "rs") != 0)) {
                return (false);
            }
            break;
        case 2: // リソース名
            break;
        case 4: // リソース先
            try {
                // string→int変換
                if (!(0 <= stoi(value))) {
                    return (false);
                }
                break;
            }
            catch (invalid_argument) {
                break;
            }
            catch (out_of_range) {
                break;
            }
        case 5: // フレームサイズxres
            try {
                if (!(0 < stoi(value))) {
                    return (false);
                }
            }
            catch (invalid_argument) {
                // 数値以外入ってきた場合
                return (false);
            }
            break;
        case 6: // フレームサイズyres
            try {
                if (!(0 < stoi(value))) {
                    return (false);
                }
            }
            catch (invalid_argument) {
                // 数値以外入ってきた場合
                return (false);
            }
            break;
        case 7: // フレームレート
            try {
                if (!(0 < stoi(value) && stoi(value) <= 250)) {
                    return (false);
                }
            }
            catch (invalid_argument) {
                // 数値以外入ってきた場合
                return (false);
            }
            break;
        case 8: // 色のフォーマット
            try {
                if (!(0 < stoi(value) && stoi(value) < 6)) {
                    return (false);
                }
                break;
            }
            catch (invalid_argument) {
                // 数値以外入ってきた場合
                return (false);
            }
            break;
    }
    return (true);
}


