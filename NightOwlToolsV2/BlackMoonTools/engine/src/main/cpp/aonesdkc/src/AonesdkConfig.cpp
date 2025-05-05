#include "AonesdkConfig.h"
#include "xcore_json.h"
#include "cpfile.h"
#include <sstream>

const char* AONESDK_CONFIG_FILE = "aonesdk.json";

static AonesdkConfig* s_sdkConfig = NULL;

AonesdkConfig* AonesdkConfig::getInstance()
{
	if (s_sdkConfig == NULL) {
		s_sdkConfig = new AonesdkConfig();
	}	
	return s_sdkConfig;
}

void AonesdkConfig::end()
{
	if (s_sdkConfig != NULL) {
		delete s_sdkConfig;
		s_sdkConfig = NULL;
	}
}

void AonesdkConfig::init()
{
	if (!m_sdkMaps.empty()) return;
//	unsigned long dsize;
	string fileData = (char*)cpfile_readstr(AONESDK_CONFIG_FILE);
    if (fileData.empty()) {
        AONESDK_CONFIG_FILE = "sdkConfig";
        fileData = (char*)cpfile_readstr(AONESDK_CONFIG_FILE);
    }
    m_sdkConfigString = fileData;
	loadData(fileData.c_str());
}

void AonesdkConfig::loadData(const char* data)
{
	XJsonValue value;
	XJsonReader reader;
	//string fileData = uuFileUtils::getInstance()->getStringFromFile(AONESDK_CONFIG_FILE);
	m_params.clear();
	if (data != NULL && reader.parse(data, value)) {
		if (value.type() == XJsonValue::objectValue) {
			XJsonValue::ObjectValues objectValues = value.as_objects();
			for (XJsonValue::ObjectIterator it = objectValues.begin(); it != objectValues.end(); ++it) {
				if (it->second.type() == XJsonValue::stringValue) {
					m_params[it->first] = it->second.as_string();
				}
			}
		}
	}
}

const char* AonesdkConfig::getDefaultUserSdk()
{
	if (m_defaultUserSdk != "") {
		return m_defaultUserSdk.c_str();
	}

	return NULL;
}

const char* AonesdkConfig::getDefaultIapSdk()
{
	if (m_defaultIapSdk != "") {
		return m_defaultIapSdk.c_str();
	}

	return NULL;
}

const char* AonesdkConfig::getType( const char* name )
{
	if (name == NULL)
		return NULL;

	if (m_sdkMaps.find(name) != m_sdkMaps.end()) {
		return m_sdkMaps[name].type.c_str();
	}
	uuLOG("failed to get the type of sdk : %s", name);
	return NULL;
}

ConfigMap AonesdkConfig::getInitParams( const char* name )
{
	ConfigMap temp;
	if (name == NULL)
		return temp;

	if (m_sdkMaps.find(name) != m_sdkMaps.end()) {
		return m_sdkMaps[name].initParams;
	}
	uuLOG("failed to get the initParams of sdk : %s", name);
	return temp;
}

ConfigMap AonesdkConfig::getContentParams( const char* name )
{
	ConfigMap temp;
	if (name == NULL)
		return temp;

	if (m_sdkMaps.find(name) != m_sdkMaps.end()) {
		return m_sdkMaps[name].contentParams;
	}
	uuLOG("failed to get the contentParams of sdk : %s", name);
	return temp;
}

vector<string> AonesdkConfig::getAllSdkNames()
{
/*
	vector<string> sdkNameVec;
	map<string, SdkInfo>::iterator it;
	for (it = m_sdkMaps.begin(); it != m_sdkMaps.end(); ++it) {
		sdkNameVec.push_back(it->first);
	}
	return sdkNameVec;
*/
	return m_sdkVector;
}

int AonesdkConfig::getSdkCount()
{
	return m_sdkMaps.size();
}

AonesdkConfig::AonesdkConfig()
{
	init();
}

string AonesdkConfig::getAoneConfigData(string key)
{
	if (m_params.find(key) != m_params.end())
		return m_params[key];

	return "";
}

string AonesdkConfig::getSdkConfigString()
{
    return m_sdkConfigString;
}

uint32 AonesdkConfig::getAppId()
{
  string ret = m_params["appId"];
  if (ret == "") {
    return 0;
  }
  return atoi(ret.c_str());
}

std::string AonesdkConfig::getAppKey()
{
  return m_params["appKey"];
}

std::string AonesdkConfig::getChannel()
{
  return m_params["channel"];
}

std::string AonesdkConfig::getPayChannel()
{
	if (m_params.find("payChannel") != m_params.end())
    {
		return m_params["payChannel"];
	}
	else return m_params["channel"];
}

std::string AonesdkConfig::getTestToken()
{
  return m_params["testAccount"];
}

std::string AonesdkConfig::getTestIp()
{
	return m_params["testIp"];
}

std::string AonesdkConfig::getHost()
{
	return m_params["host"];
}

std::string AonesdkConfig::getBackupIp()
{
	return m_params["backupIp"];
}

std::string AonesdkConfig::getPort()
{
	return m_params["port"];
}

std::string AonesdkConfig::getHttpPort()
{
    return m_params["http_port"];
}

std::string AonesdkConfig::getAgainstFakePay()
{
	return m_params["againstFakePay"];
}

std::string AonesdkConfig::getBuglyAppID()
{
    return m_params["buglyAppID"];
}

std::string AonesdkConfig::getAdChannel()
{
	return m_params["adChannel"];
}

std::string AonesdkConfig::getAdActivity()
{
	return m_params["adActivity"];
}



bool AonesdkConfig::getAppVersion(uint8& major, uint8& minor, uint8& revision)
{
	if (m_params.find("appVersion") != m_params.end())
	{
		string& appVersion = m_params["appVersion"];
		std::stringstream ss(appVersion);
		std::string item;
		std::vector<uint8*> numbers;

		major = 0;
		minor = 0;
		revision = 0;
		numbers.push_back(&major);
		numbers.push_back(&minor);
		numbers.push_back(&revision);

		int index = 0;
		while (std::getline(ss, item, '.'))
		{
			int number = strtol(item.c_str(), NULL, 10);
			*(numbers[index]) = number;
			index = index + 1;

			if (index >= numbers.size())
				break;
		}

		return true;
	}

	return false;
}

bool AonesdkConfig::isSavePicture()
{
    if (m_params["savePicture"] == "no") {
        return false;
    }
    return true;
}

