#include "uuSdkConfig.h"
#include "xcore_json.h"
#include "cpfile.h"

const char* SDK_CONFIG_FILE = "uuSdkConfig.json";
static uuSdkConfig* s_sdkConfig = NULL;

uuSdkConfig* uuSdkConfig::getInstance()
{
	if (s_sdkConfig == NULL) {
		s_sdkConfig = new uuSdkConfig();
	}	
	return s_sdkConfig;
}

void uuSdkConfig::end()
{
	if (s_sdkConfig != NULL) {
		delete s_sdkConfig;
		s_sdkConfig = NULL;
	}
}

void uuSdkConfig::init()
{
	if (!m_sdkMaps.empty()) return;
	string fileData = (char*)cpfile_readstr(SDK_CONFIG_FILE);
    if (fileData.empty()) {
        SDK_CONFIG_FILE = "pluginConfig";
        fileData = (char*)cpfile_readstr(SDK_CONFIG_FILE);
    }
    m_pluginConfigString = fileData;
	loadData(fileData.c_str());
}

void uuSdkConfig::checkDefaultSdk() {
	map<string, SdkInfo>::iterator it;
	if (m_defaultUserSdk == "") {
		for (it = m_sdkMaps.begin(); it != m_sdkMaps.end(); ++it) {
			if (it->second.type == "user") {
				m_defaultUserSdk = it->first;
				break;
			}
		}
	}
	if (m_defaultIapSdk == "") {
		for (it = m_sdkMaps.begin(); it != m_sdkMaps.end(); ++it) {
			if (it->second.type == "pay" || it->second.type == "userPay") {
				m_defaultIapSdk = it->first;
				break;
			}
		}
	}
	if (m_defaultAnalyticsSdk == "") {
		for (it = m_sdkMaps.begin(); it != m_sdkMaps.end(); ++it) {
			if (it->second.type == "analytics") {
				m_defaultAnalyticsSdk = it->first;
				break;
			}
		}
	}
	if (m_defaultAdsSdk == "") {
		for (it = m_sdkMaps.begin(); it != m_sdkMaps.end(); ++it) {
			if (it->second.type == "ads") {
				m_defaultAdsSdk = it->first;
				break;
			}
		}
	}
	if (m_defaultPushSdk == "") {
		for (it = m_sdkMaps.begin(); it != m_sdkMaps.end(); ++it) {
			if (it->second.type == "push") {
				m_defaultPushSdk = it->first;
				break;
			}
		}
	}
	if (m_defaultShareSdk == "") {
		for (it = m_sdkMaps.begin(); it != m_sdkMaps.end(); ++it) {
			if (it->second.type == "share") {
				m_defaultShareSdk = it->first;
				break;
			}
		}
	}
}

void uuSdkConfig::loadData(const char* data)
{
	XJsonValue value;
	XJsonReader reader;
	clear();
	if (data == NULL || !reader.parse(data, value) || value.type() != XJsonValue::objectValue) {
		return;
	}
	XJsonValue::ObjectValues objectValues = value.as_objects();
	for (XJsonValue::ObjectIterator it = objectValues.begin(); it != objectValues.end(); ++it) {
		if (it->second.type() == XJsonValue::stringValue) {
			if (it->first == "defalutUserSdk" || it->first == "defaultUserSdk") {
				m_defaultUserSdk = it->second.as_string();
			} else if (it->first == "defaultIapSdk") {
				m_defaultIapSdk = it->second.as_string();
			} else if (it->first == "defaultShareSdk") {
				m_defaultShareSdk = it->second.as_string();
			} else if (it->first == "defaultAnalyticsSdk") {
				m_defaultAnalyticsSdk = it->second.as_string();
			} else if (it->first == "defaultAdsSdk") {
				m_defaultAdsSdk = it->second.as_string();
			} else if (it->first == "defaultPushSdk") {
				m_defaultPushSdk = it->second.as_string();
			} else {
				//uuLOG("load sdk config error! the value is not 'defaultUserSdk' or 'defaultIapSdk'.");
			}

		} else if (it->second.type() == XJsonValue::arrayValue && it->first == "sdks") {
			XJsonValue::ArrayValues arrayValues = it->second.as_array();

			for (XJsonValue::ArrayIterator sdkIt = arrayValues.begin(); sdkIt != arrayValues.end(); ++sdkIt) {
				SdkInfo sdkInfo;
				XJsonValue::ObjectValues objectValues2 = (*sdkIt).as_objects();
				for (XJsonValue::ObjectIterator it2 = objectValues2.begin(); it2 != objectValues2.end(); ++it2) {
					if (it2->second.type() == XJsonValue::stringValue) {
						if (it2->first == "name") {
							//sdkName = it2->second.as_string();
							sdkInfo.name = it2->second.as_string();
						} else if (it2->first == "type") {
							sdkInfo.type = it2->second.as_string();
						} else {
							uuLOG("%s loadData, load sdk config error! the value is not 'name' or 'type'.", LOG_TAG);
						}
					} else if (it2->second.type() == XJsonValue::objectValue) {
						XJsonValue::ObjectValues objectValues3 = it2->second.as_objects();
						for (XJsonValue::ObjectIterator it3 = objectValues3.begin(); it3 != objectValues3.end(); ++it3) {
							if (it3->second.type() == XJsonValue::stringValue) {
								if (it2->first == "init_params") {
									sdkInfo.initParams[it3->first] = it3->second.as_string();
								} else if (it2->first == "content_params") {
									sdkInfo.contentParams[it3->first] = it3->second.as_string();
								} else {
									uuLOG("%s loadData, load sdk config error! the param is not 'init_params' or 'content_params'.", LOG_TAG);
								}
							}
						}
					}
				}
				if (sdkInfo.name != "") {
					m_sdkMaps[sdkInfo.name] = sdkInfo;
					m_sdkVector.push_back(sdkInfo.name);
                    uuLOG("%s loadData, uuSdk-Config cpfile_readstr fileData", LOG_TAG);
				}
			}
		}
	}
	checkDefaultSdk();
}

void uuSdkConfig::setDefaultUserSdk(const char *sdkName)
{
    m_defaultUserSdk = sdkName;
}
string uuSdkConfig::getPluginConfig()
{
    return m_pluginConfigString;
}
const char* uuSdkConfig::getDefaultUserSdk()
{
	return m_defaultUserSdk.c_str();
}

const char* uuSdkConfig::getDefaultIapSdk()
{
	return m_defaultIapSdk.c_str();
}

const char* uuSdkConfig::getType( const char* name )
{
	if (name == NULL)
		return NULL;

	if (m_sdkMaps.find(name) != m_sdkMaps.end()) {
		return m_sdkMaps[name].type.c_str();
	}
	uuLOG("failed to get the type of sdk : %s", name);
	return NULL;
}

ConfigMap uuSdkConfig::getInitParams( const char* name )
{
	if (name == NULL)
		return map<string, string>();

	if (m_sdkMaps.find(name) != m_sdkMaps.end()) {
		return m_sdkMaps[name].initParams;
	}
	uuLOG("failed to get the initParams of sdk : %s", name);
	return map<string, string>();
}

ConfigMap uuSdkConfig::getContentParams( const char* name )
{
	if (name == NULL)
		return map<string, string>();

	if (m_sdkMaps.find(name) != m_sdkMaps.end()) {
		return m_sdkMaps[name].contentParams;
	}
	uuLOG("failed to get the contentParams of sdk : %s", name);
	return map<string, string>();

}

vector<string> uuSdkConfig::getAllSdkNames()
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

vector<string> uuSdkConfig::getAllAnalyticsNames()
{
	vector<string> analyticsNamesVec;
	map<string, SdkInfo>::iterator it;
	for (it = m_sdkMaps.begin(); it != m_sdkMaps.end(); ++it)
	{
		if (it->second.type == "analytics")
		{
			uuLOG("hhh---------analytics name is :%s", (it->first).c_str());
			analyticsNamesVec.push_back(it->first);
		}
	}

	return analyticsNamesVec;
}

int uuSdkConfig::getSdkCount()
{
	return m_sdkMaps.size();
}

uuSdkConfig::uuSdkConfig()
{
	init();
}

void uuSdkConfig::setInitParams( ConfigMap params )
{
	map<string, SdkInfo>::iterator it;
	ConfigMap::iterator paramIt;
	for (it = m_sdkMaps.begin(); it != m_sdkMaps.end(); ++it) {
		for (paramIt = params.begin(); paramIt != params.end(); ++paramIt) {
			//use local init param first
			if (it->second.initParams.find(paramIt->first) == it->second.initParams.end())
			{
				(it->second.initParams)[paramIt->first] = paramIt->second;
			}
		}
	}
}

void uuSdkConfig::setContentParams( ConfigMap params )
{
	map<string, SdkInfo>::iterator it;
	ConfigMap::iterator paramIt;
	for (it = m_sdkMaps.begin(); it != m_sdkMaps.end(); ++it) {
		for (paramIt = params.begin(); paramIt != params.end(); ++paramIt) {
			(it->second.contentParams)[paramIt->first] = paramIt->second;
		}
	}
}

const char* uuSdkConfig::getDefaultShareSdk()
{
	return m_defaultShareSdk.c_str();
}

const char* uuSdkConfig::getDefaultAnalyticsSdk()
{
	return m_defaultAnalyticsSdk.c_str();
}

const char* uuSdkConfig::getDefaultAdsSdk()
{
	return m_defaultAdsSdk.c_str();
}

const char* uuSdkConfig::getDefaultPushSdk()
{
	return m_defaultPushSdk.c_str();
}

void uuSdkConfig::clear()
{
	m_defaultUserSdk = "";
	m_defaultIapSdk = "";
	m_defaultShareSdk = "";
	m_defaultAnalyticsSdk = "";
	m_defaultAdsSdk = "";
	m_defaultPushSdk = "";
	m_sdkMaps.clear();
	m_sdkVector.clear();
}

const char* uuSdkConfig::getSdkNameByParam(const char* key, const char* value)
{
	map<string, SdkInfo>::iterator it;
	ConfigMap::iterator paramIt;
	for (it = m_sdkMaps.begin(); it != m_sdkMaps.end(); ++it) {
		ConfigMap params = it->second.initParams;
		if (params.find(key) != params.end()) {
			string strVal = params[key];
			if (strVal == value) {
				return it->first.c_str();
			}
		}
	}
	return "";
}

vector<string> uuSdkConfig::getAllSdkNamesByType( const char* type )
{
	vector<string> ret;
	map<string, SdkInfo>::iterator it;
	ConfigMap::iterator paramIt;
	for (it = m_sdkMaps.begin(); it != m_sdkMaps.end(); ++it) {
		if (it->second.type == type) {
			ret.push_back(it->first);
		}
	}
	return ret;
}

const char* uuSdkConfig::getInitValueByKey( const char* name, const char* key )
{
	map<string, string> initParams = getInitParams(name);
	if (initParams.find(key) != initParams.end()) {
		string val = initParams[key];
		return val.c_str();
	} else {
		return NULL;
	}
}

const char* uuSdkConfig::getContentValueByKey( const char* name, const char* key )
{
	map<string, string> contentParams = getContentParams(name);
	if (contentParams.find(key) != contentParams.end()) {
		string val = contentParams[key];
		return val.c_str();
	} else {
		return NULL;
	}
}

