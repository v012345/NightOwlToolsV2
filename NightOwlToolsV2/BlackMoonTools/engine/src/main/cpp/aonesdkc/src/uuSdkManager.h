#ifndef __UU_SDKMANAGER_HH__
#define __UU_SDKMANAGER_HH__

#include "uuSdkDef.h"
#include "uuSdkWrapper.h"
#include <map>
#include <string>

class uuSdkManager : public uuObject
{
public:
	virtual ~uuSdkManager();
	static uuSdkManager* getInstance();
	static void end();

	/* the default sdk login
	* return it's name, if the value is null, it is a common app which not add any user sdk
	*/
	const char* login(uuObject* sender, uuSdkResultHandler handler);
	void login(std::string sdkName, uuObject* sender, uuSdkResultHandler handler);
	bool isLogined();

	void logout(uuObject* sender, uuSdkResultHandler handler);
	void logout(std::string sdkName, uuObject* sender, uuSdkResultHandler handler);

	void share(std::string sdkName, uuParamInfo info, uuObject* sender, uuSdkResultHandler handler);

	void pay(uuParamInfo info, uuObject* sender, uuSdkResultHandler handler);
	void pay(std::string sdkName, uuParamInfo info, uuObject* sender, uuSdkResultHandler handler);

	void showAds(std::string sdkName, uuParamInfo info, int pos, uuObject* sender, uuSdkResultHandler handler);
	void hideAds(std::string sdkName, uuParamInfo info, uuObject* sender, uuSdkResultHandler handler);
	void queryPoints(std::string sdkName, uuObject* sender, uuSdkResultHandler handler);
	void spendPoints(std::string sdkName, int points, uuObject* sender, uuSdkResultHandler handler);

	void logPageStart(const char* sdkName, const char* pageName);
	void logPageEnd(const char* sdkName, const char* pageName);
	void logEvent(const char* sdkName, const char* eventId);
	void logEvent(const char* eventId);
	void logEvent(const char* sdkName, const char* eventId, uuParamInfo info);
	void logEvent(const char* eventId, uuParamInfo info);
	void logEventBegin(const char* sdkName, const char* eventId);
	void logEventEnd(const char* sdkName, const char* eventId);
	void logKVEventBegin(const char* sdkName, const char* eventId, uuParamInfo info);
	void logKVEventEnd(const char* sdkName, const char* eventId);
	void logError(const char* sdkName, const char* errorId, const char* message);
	std::string getOnlineParams(const char* sdkName, const char* key);
  
	void annalyticsEvent(std::string sdkName, uuParamInfo info);

	void startPush(std::string sdkName, uuObject* sender, uuSdkResultHandler handler);
	void closePush(std::string sdkName, uuObject* sender, uuSdkResultHandler handler);
	void setAlias(std::string sdkName, std::string alias, uuObject* sender, uuSdkResultHandler handler);
	void delAlias(std::string sdkName, std::string alias, uuObject* sender, uuSdkResultHandler handler);
	void setTags(std::string sdkName, std::string tags, uuObject* sender, uuSdkResultHandler handler);
	void delTags(std::string sdkName, std::string tags, uuObject* sender, uuSdkResultHandler handler);

	void initAllSdks(uuObject* sender = NULL, uuSdkResultHandler handler = NULL, uuSdkResultHandler payHandler = NULL);
	bool init( std::string sdkName, uuObject* sender, uuSdkResultHandler handler,uuSdkResultHandler payHandler);

	bool load(std::string sdkName);
	int loadAllSdks();
	void unloadSdk(std::string sdkName);
	void unloadAllSdks();

	uuSdkWrapper* getSdk(std::string sdkName);

	void setDebugMode(std::string sdkName, bool isDebug);

	const char* getDefaultUserSdkName();
	const char* getDefaultIapSdkName();

	void callFunc(const char* funcName);
	void callFunc(std::string sdkName, const char* funcName);
	void callFuncWithParam(const char* funcName, std::vector<PluginParam*> params);
	void callFuncWithParam(std::string sdkName, const char* funcName, std::vector<PluginParam*> params);
	
	std::string callStringFunc(const char* funcName);
	std::string callStringFunc(std::string sdkName, const char* funcName);
	std::string callStringFuncWithParam(const char* funcName, std::vector<PluginParam*> params);
	
	bool callBoolFunc(const char* funcName);
	bool callBoolFunc(std::string sdkName, const char* funcName);
	bool callBoolFuncWithParam(const char* funcName, std::vector<PluginParam*> params);
	bool callBoolFuncWithParam(std::string sdkName, const char* funcName, std::vector<PluginParam*> params);

	int callIntFuncWithParam(const char* funcName, std::vector<PluginParam*> params);
	int callIntFuncWithParam(std::string sdkName, const char* funcName, std::vector<PluginParam*> params);

private:
	uuSdkManager();
	void initCallback(uuESdkResultType type, map<string, string> params);
    void initPayCallback(uuESdkResultType type, map<string, string> params);

private:
    const char* LOG_TAG = "[uuSdkManager]";
	std::map<std::string, uuSdkWrapper*> m_sdkMaps;
	uuSdkWrapper* m_defaultUserSdk;
	uuSdkWrapper* m_defaultIapSdk;
	int m_initCounter;
	std::map<std::string, std::string> m_initParams;
	std::map<std::string, std::string> m_initErrorParams;
	int m_initNumbers;
	uuESdkResultType m_initErrorType;
	uuObject* m_sender;
	uuSdkResultHandler m_handler;
    uuSdkResultHandler m_payHandler;
	bool m_init_ok;
};

#endif // __UU_SDKMANAGER_HH__
