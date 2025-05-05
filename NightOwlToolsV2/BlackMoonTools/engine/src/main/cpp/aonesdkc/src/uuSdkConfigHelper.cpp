#include "uuSdkConfigHelper.h"

const char* uuSdkConfigHelper::getSdkType( string sdkName )
{
	return uuSdkConfig::getInstance()->getType(sdkName.c_str());
}

const void uuSdkConfigHelper::setDefaultUserSdkName(const char *name)
{
    uuSdkConfig::getInstance() -> setDefaultUserSdk(name);
}

const char* uuSdkConfigHelper::getDefaultUserSdkName()
{
	return uuSdkConfig::getInstance()->getDefaultUserSdk();
}

const char* uuSdkConfigHelper::getDefaultIapSdkName()
{
	return uuSdkConfig::getInstance()->getDefaultIapSdk();
}

vector<string> uuSdkConfigHelper::getAllSdkNames()
{
	return uuSdkConfig::getInstance()->getAllSdkNames();
}

vector<string> uuSdkConfigHelper::getAllAnalyticsNames()
{
	return uuSdkConfig::getInstance()->getAllAnalyticsNames();
}

vector<string> uuSdkConfigHelper::getAllSdkNamesByType(const char* type)
{
	return uuSdkConfig::getInstance()->getAllSdkNamesByType(type);
}

map<string, string> uuSdkConfigHelper::getContentParams(string name, map<string, string> info)
{
	ConfigMap params = getContentParams(name);
	ConfigMap additionParams;
	bool isFind = false;
	ConfigMap::iterator itr1;
	ConfigMap::iterator itr2;
	for (itr1 = info.begin(); itr1 != info.end(); ++itr1) {
		isFind = false;
		for (itr2 = params.begin(); itr2 != params.end();) {
			if (itr1->first != "" && itr1->first.compare(itr2->second) == 0) {
				isFind = true;
				if (itr1->second == "null") {
					params.erase(itr2++);
				} else {
					itr2->second = itr1->second;
					++itr2;
				}
			} else {
				++itr2;
			}
		}
		if (!isFind) {
			additionParams[itr1->first] = itr1->second;
		}
	}
	for (itr1 = additionParams.begin(); itr1 != additionParams.end(); ++itr1) {
		params[itr1->first] = itr1->second;
	}

	return params;
}

ConfigMap uuSdkConfigHelper::getContentParams(string name)
{
	return uuSdkConfig::getInstance()->getContentParams(name.c_str());
}

ConfigMap uuSdkConfigHelper::getInitParams( string name )
{
	return uuSdkConfig::getInstance()->getInitParams(name.c_str());
}

string uuSdkConfigHelper::getInitParams(string sdkName,string itemName)
{
	string itemValue("");
	ConfigMap initParams = uuSdkConfigHelper::getInitParams(sdkName);
	ConfigMap::iterator iter = initParams.find(itemName);

	if (iter != initParams.end())
		itemValue = iter->second;

	return itemValue;
}

void uuSdkConfigHelper::setInitParams( ConfigMap params )
{
  uuSdkConfig::getInstance()->setInitParams(params);
}

void uuSdkConfigHelper::setContentParams( ConfigMap params )
{
  uuSdkConfig::getInstance()->setContentParams(params);
}

void uuSdkConfigHelper::init()
{
  uuSdkConfig::getInstance()->init();
}

void uuSdkConfigHelper::setConfigData( const char* data )
{
	uuSdkConfig::getInstance()->loadData(data);
}

string uuSdkConfigHelper::getPluginConfigString()
{
    return uuSdkConfig::getInstance()->getPluginConfig();
}

const char* uuSdkConfigHelper::getDefalutShareSdkName()
{
	return uuSdkConfig::getInstance()->getDefaultShareSdk();
}

const char* uuSdkConfigHelper::getDefalutAnalyticsSdkName()
{
	return uuSdkConfig::getInstance()->getDefaultAnalyticsSdk();
}

const char* uuSdkConfigHelper::getDefalutAdsSdkName()
{
	return uuSdkConfig::getInstance()->getDefaultAdsSdk();
}

const char* uuSdkConfigHelper::getDefalutPushSdkName()
{
	return uuSdkConfig::getInstance()->getDefaultPushSdk();
}

const char* uuSdkConfigHelper::getSdkNameByParam( const char* key, const char* value )
{
	return uuSdkConfig::getInstance()->getSdkNameByParam(key, value);
}

