#ifndef __UU_SDKRESULTLISTENER_HH__
#define __UU_SDKRESULTLISTENER_HH__

#include "uuSdkDef.h"
#include "PluginProtocol.h"
#include "ProtocolUser.h"
#include "ProtocolIAP.h"
#include "ProtocolShare.h"
#include "ProtocolAds.h"
#include "ProtocolPush.h"

using namespace aonesdk::plugin;

enum uuESdkResultType
{
	eSDK_RESULT_SUCCESS = 0,
	eSDK_RESULT_FAILED,
	eSDK_RESULT_CANCEL,
	eSDK_RESULT_TIMEOUT,
	eSDK_RESULT_LOGOUT,
	eSDK_RESULT_SWITCHACCOUNT,
	eSDK_RESULT_UNKNOWERROR
};

typedef void (uuObject::*uuSdkResultHandler)(uuESdkResultType, std::map<std::string, std::string>);

class uuSdkResultListener
{
public:
	uuSdkResultListener() 
	{ 
		m_listener = NULL;
		m_handler = NULL;
    }
	virtual ~uuSdkResultListener() {}

	void setHandler(uuObject* listener, uuSdkResultHandler handler);
    
protected:
	uuObject* m_listener;
	uuSdkResultHandler m_handler;
};
class uuShareResultListener : public ShareResultListener, public uuSdkResultListener
{
public:
	virtual void onShareResult(ShareResultCode ret, const char* msg);
};

class uuPayResult : public PayResultListener, public uuSdkResultListener
{
public:
	virtual void onPayResult(PayResultCode ret, const char* msg, TProductInfo info);
};

class uuLoginResult : public UserActionListener, public uuSdkResultListener
{
public:
	virtual void onActionResult(ProtocolUser* pPlugin, UserActionResultCode code, const char* msg);
};

class uuUserInitResult : public UserActionListener, public uuSdkResultListener
{
public:
    uuUserInitResult() { isInited = false; }
	virtual void onActionResult(ProtocolUser* pPlugin, UserActionResultCode code, const char* msg);
    bool isInited;
};

class uuLogoutResult : public UserActionListener, public uuSdkResultListener
{
public:
	virtual void onActionResult(ProtocolUser* pPlugin, UserActionResultCode code, const char* msg);
};

class uuAdsResult : public AdsListener, public uuSdkResultListener
{
public:
	virtual void onAdsResult(AdsResultCode code, const char* msg);
};

class uuPushResult : public PushResultListener, public uuSdkResultListener
{
public:
	virtual void onPushResult(ProtocolPush* pPlugin, PushResultCode code, const char* msg);
};

#endif // __UU_SDKRESULTLISTENER_HH__
