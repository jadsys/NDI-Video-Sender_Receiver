#pragma once
#include <string>
#include <string.h>
#include <fstream>
#include <sstream>
/* linux����p�w�b�_ */
#ifdef __linux__
#include <strings.h>

#endif // __linux__
/* Windows����p�w�b�_ */
#ifdef _WIN32
#define strcasecmp stricmp // LinuxとWindowsで標準関数が違う
#endif // _WIN32

#define CONFIG_FILE_NAME "NdiVideoSender.config"


class ConfigRead
{
public:
	ConfigRead();
	~ConfigRead();
	std::string GetStringProperty(std::string op_name);
	bool GetBoolProperty(std::string op_name);
	int GetIntProperty(std::string op_name);

private:
	std::string m_defvalue; // デフォルト値格納用
	int m_int_defvalue; // 設定項目番号格納用

	std::string getDefaultValue(std::string op_name);
	void setDefaltValue(std::string op_name);
	std::string getProperty(std::string conf_data, std::string op_name);
	bool checkValue(std::string value);
	std::string readConfigFile(std::string op_name);

};

