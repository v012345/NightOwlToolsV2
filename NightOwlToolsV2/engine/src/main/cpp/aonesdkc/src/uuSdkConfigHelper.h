#ifndef __UU_SDKCONFIG_HELPER_HH__
#define __UU_SDKCONFIG_HELPER_HH__

#include "uuSdkConfig.h"
#include <vector>
#include <string>
#include <map>
using namespace std;

class uuSdkConfigHelper
{
public:
    static const void setDefaultUserSdkName(const char* name);
	static const char* getDefaultUserSdkName();
	static const char* getDefaultIapSdkName();
	static const char* getDefalutShareSdkName();
	static const char* getDefalutAnalyticsSdkName();
	static const char* getDefalutAdsSdkName();
	static const char* getDefalutPushSdkName();

	static const char* getSdkType(string sdkName);

	static vector<string> getAllSdkNames();
	static vector<string> getAllAnalyticsNames();
	static vector<string> getAllSdkNamesByType(const char* type);

	static ConfigMap getInitParams(string name);
	static string getInitParams(string sdkName,string itemName);
	static ConfigMap getContentParams(string name, ConfigMap info);
	//static ConfigMap getShareParams();
	//static ConfigMap getLoginParams();
	static void setInitParams(ConfigMap params);
	static void setContentParams(ConfigMap params);

	static void init();

	static void setConfigData(const char* data);
    static string getPluginConfigString();

	static const char* getSdkNameByParam(const char* key, const char* value);

private:
	static ConfigMap getContentParams(string name);
};


#endif // __UU_SDKCONFIG_HELPER_HH__

