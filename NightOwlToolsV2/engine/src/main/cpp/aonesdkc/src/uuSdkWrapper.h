#ifndef __UU_SDKWRAPPER_HH__
#define __UU_SDKWRAPPER_HH__

#include "uuSdkDef.h"
#include "PluginProtocol.h"
#include "ProtocolUser.h"
#include "ProtocolIAP.h"
#include "ProtocolShare.h"
//#include "ProtocolSocial.h"
#include "ProtocolAds.h"
#include "ProtocolAnalytics.h"
#include "uuSdkResultListener.h"
#include "ProtocolPlayer.h"
#include "ProtocolStream.h"
#include "ProtocolVoice.h"
#include "ProtocolException.h"
#include "ProtocolSupport.h"

#include <map>
#include <string>
using namespace std;

using namespace aonesdk::plugin;

typedef map<string, string> uuParamInfo;

typedef enum _uuESdkType {
	eSDK_UNKNOWN = 0,
	eSDK_USER,
	eSDK_PAY,
	eSDK_SHARE,
	eSDK_ANALYTICS,
	eSDK_ADS,
	eSDK_PUSH,
	eSDK_LIVESTREAM,
	eSDK_PLAYER,
	eSDK_VOICE,
    eSDK_EXCEPTION,
	eSDK_SUPPORT,
} uuESdkType;

class uuSdkProtocol;
class uuSdkWrapper
{
public:
	virtual ~uuSdkWrapper();
	uuSdkWrapper();

	static uuSdkWrapper* create(string sdkName);

	void configProtocol(uuObject* sender, uuSdkResultHandler handler,uuSdkResultHandler payHandler);
    
	//string getName();

	void share(uuParamInfo info, uuObject* sender, uuSdkResultHandler handler);
	
	void pay(uuParamInfo info, uuObject* sender, uuSdkResultHandler handler);
	
	void login(uuObject* sender, uuSdkResultHandler handler);
	void logout(uuObject* sender, uuSdkResultHandler handler);
	bool isLogined();
  
	void showAds(uuParamInfo info, int pos, uuObject* sender, uuSdkResultHandler handler);
	void hideAds(uuParamInfo info, uuObject* sender, uuSdkResultHandler handler);
	void queryPoints(uuObject* sender, uuSdkResultHandler handler);
	void spendPoints(int points, uuObject* sender, uuSdkResultHandler handler);

	void logPageStart(const char* pageName);
	void logPageEnd(const char* pageName);
	void logEvent(const char* eventId);
	void logEvent(const char* eventId, uuParamInfo info);
	void logEventBegin(const char* eventId);
	void logEventEnd(const char* eventId);
	void logKVEventBegin(const char* eventId, uuParamInfo info);
	void logKVEventEnd(const char* eventId);
	void logError(const char* errorId, const char* message);
	std::string getOnlineParams(const char* key);

	void startPush(uuObject* sender, uuSdkResultHandler handler);
	void closePush(uuObject* sender, uuSdkResultHandler handler);
	void setAlias(const char *alias, uuObject* sender, uuSdkResultHandler handler);
	void delAlias(const char *alias, uuObject* sender, uuSdkResultHandler handler);
	void setTags(const char *tags, uuObject* sender, uuSdkResultHandler handler);
	void delTags(const char *tags, uuObject* sender, uuSdkResultHandler handler);


	void setDebugMode(bool isDebug);
    
	void callFunc(const char* funcName);
	void callFuncWithParam(const char* funcName, std::vector<PluginParam*> params);
	std::string callStringFunc(const char* funcName);
	std::string callStringFuncWithParam(const char* funcName, std::vector<PluginParam*> params);
	bool callBoolFunc(const char* funcName);
	bool callBoolFuncWithParam(const char* funcName, std::vector<PluginParam*> params);
	int callIntFuncWithParam(const char* funcName, std::vector<PluginParam*> params);


    PluginProtocol* getSdk() { return m_sdk;}
	static void end();

private:
	static uuESdkType getSdkType(string sdkName);

	void setType(uuESdkType type) { m_type = type; }
	uuESdkType getType() { return m_type; }
	
	void setSdk(string name, PluginProtocol* protocol, uuESdkType type);

	bool isSdkAndTypeLegal(uuESdkType type);

private:
	uuESdkType m_type;
	string m_name;
	PluginProtocol* m_sdk;
	
	uuSdkResultListener* m_resultListener;

};

#endif // __UU_SDKWRAPPER_HH__
