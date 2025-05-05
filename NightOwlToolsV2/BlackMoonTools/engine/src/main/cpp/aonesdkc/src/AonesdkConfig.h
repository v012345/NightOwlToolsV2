#ifndef __AONE_SDKCONFIG_HH__
#define __AONE_SDKCONFIG_HH__

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

class AonesdkConfig
{
public:
	~AonesdkConfig() {}
	static AonesdkConfig* getInstance();
	static void end();

	void init();
	
	const char* getDefaultUserSdk();
	const char* getDefaultIapSdk();
	
	const char* getType(const char* name);
	ConfigMap getInitParams(const char* name);
	ConfigMap getContentParams(const char* name);

	vector<string> getAllSdkNames();
	int getSdkCount();

	string getAoneConfigData(string key);
    string getSdkConfigString();

	uint32 getAppId();
	string getAppKey();
	string getChannel();
	string getPayChannel();
	string getTestToken();
	string getTestIp();
	string getHost();
	string getBackupIp();
	string getPort();
    string getHttpPort();
	string getAgainstFakePay();
    string getBuglyAppID();
	string getAdChannel();
	string getAdActivity();
	bool getAppVersion(uint8& major, uint8& minor, uint8& revision);
    bool isSavePicture();
    
	void loadData(const char* data);

private:
	AonesdkConfig();

	struct SdkInfo
	{
		string name;
		string type;
		ConfigMap initParams;
		ConfigMap contentParams;
	};
	string m_defaultUserSdk;
	string m_defaultIapSdk;
	map<string, SdkInfo> m_sdkMaps;
	vector<string> m_sdkVector;
    string m_sdkConfigString;
	map<string, string> m_params;
};

#endif // __UU_SDKCONFIG_HH__

