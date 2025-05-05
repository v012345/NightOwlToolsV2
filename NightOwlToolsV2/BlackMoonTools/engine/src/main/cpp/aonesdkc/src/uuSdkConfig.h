#ifndef __UU_SDKCONFIG_HH__
#define __UU_SDKCONFIG_HH__

/*
//const char* defaultSdk = "None";
static const char* s_defaultUserSdk = "UserUC";
static const char* s_defaultIapSdk = "IAPOnlineUC";

static const char* s_sdkConfigs[][2] = {
	{"ShareWeibo", "type_share"},
	{"IAPOnlineUC", "type_pay"},
	{"UserUC", "type_user"}
};
static const char* s_payInfo[][2] = {
	//{"none", "none"}
	{"UCCustomInfo", "orderNumber"}, 	// order number
	{"productPrice", "productPrice"},
	{"UCServerID", "1886"},
	{"UCRoleID", "100"}, // what ever you want
	{"UCRoleName", "101"}, // what ever you want
	{"UCRoleLevel", "102"}, // what ever you want
};
*/
#include "uuSdkDef.h"
#include <vector>
#include <map>
#include <string>
using namespace std;

typedef std::map<std::string, std::string> ConfigMap;

class uuSdkConfig
{
public:
	~uuSdkConfig() {}
	static uuSdkConfig* getInstance();
	static void end();

	void init();
	
    void setDefaultUserSdk(const char* sdkName);
    string getPluginConfig();
	const char* getDefaultUserSdk();
	const char* getDefaultIapSdk();
	const char* getDefaultShareSdk();
	const char* getDefaultAnalyticsSdk();
	const char* getDefaultAdsSdk();
	const char* getDefaultPushSdk();
	const char* getSdkNameByParam(const char* key, const char* value);
	
	const char* getType(const char* name);
	ConfigMap getInitParams(const char* name);
	ConfigMap getContentParams(const char* name);

	void setInitParams(ConfigMap params);
	void setContentParams(ConfigMap params);

	vector<string> getAllSdkNames();
	vector<string> getAllAnalyticsNames();
	vector<string> getAllSdkNamesByType(const char* type);
	int getSdkCount();

	const char* getInitValueByKey(const char* name, const char* key);
	const char* getContentValueByKey(const char* name, const char* key);

	void loadData(const char* data);

private:
	const char* LOG_TAG = "[uuSdkConfig]";

	uuSdkConfig();
	void clear();
	void checkDefaultSdk();

	struct SdkInfo
	{
		string name;
		string type;
		ConfigMap initParams;
		ConfigMap contentParams;
	};
    string m_pluginConfigString;
	string m_defaultUserSdk;
	string m_defaultIapSdk;
	string m_defaultShareSdk;
	string m_defaultAnalyticsSdk;
	string m_defaultAdsSdk;
	string m_defaultPushSdk;
	map<string, SdkInfo> m_sdkMaps;
	vector<string> m_sdkVector;

};

#endif // __UU_SDKCONFIG_HH__

