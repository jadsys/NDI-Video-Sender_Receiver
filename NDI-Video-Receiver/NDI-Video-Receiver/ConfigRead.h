#pragma once
#include <string>
#include <fstream>
#include <sstream>
/* linux動作用ヘッダ */
#ifdef __linux__

#endif // __linux__
/* Windows動作用ヘッダ */
#ifdef _WIN32

#endif // _WIN32

#define CONFIG_FILE_NAME "NdiVideoReceiver.config"

class ConfigRead
{
public:
	ConfigRead();
	~ConfigRead();
	std::string GetStringProperty(std::string op_name);

private:
	std::string m_defvalue; // デフォルト値格納用
	int m_int_defvalue;
	// 設定項目番号格納用
	std::string getDefaultValue();
	void setDefaltValue(std::string op_name);
	std::string getProperty(std::string conf_data, std::string op_name);
	std::string readConfigFile(std::string op_name);
};

