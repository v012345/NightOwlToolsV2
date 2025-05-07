#include "uuSdkWrapper.h"
#include "uuSdkConfigHelper.h"
#include "PluginManager.h"
#include "uuSdkDef.h"

using namespace aonesdk;

static uuSdkResultListener* s_initResultListener;
static uuSdkResultListener* s_payInitResultListener;

void uuSdkWrapper::configProtocol(uuObject* sender, uuSdkResultHandler handler,uuSdkResultHandler payHandler)
{
	if (m_name == "" || m_sdk == NULL) {
		uuLOG("config protocol failed, sdk name or protocol is NULL");
		return;
	}
	//m_sdk->setDebugMode(false);
	map<string, string> configMap;
	uuLOG("config protocol, the type is %d", m_type);
	switch (m_type)
	{
	case eSDK_USER:
		configMap = uuSdkConfigHelper::getInitParams(m_name);
        if (s_initResultListener == NULL) {
            s_initResultListener = new uuUserInitResult();
        }
        s_initResultListener->setHandler(sender, handler);
        (dynamic_cast<plugin::ProtocolUser*>(m_sdk))->setActionListener(dynamic_cast<UserActionListener*>(s_initResultListener));
		(dynamic_cast<plugin::ProtocolUser*>(m_sdk))->configDeveloperInfo(configMap);
    break;
	case eSDK_PAY:
        configMap = uuSdkConfigHelper::getInitParams(m_name);
        
        if (s_payInitResultListener == NULL) {
            s_payInitResultListener = new uuPayResult();
        }
        s_payInitResultListener->setHandler(sender, payHandler);
        (dynamic_cast<plugin::ProtocolIAP*>(m_sdk))->setResultListener(dynamic_cast<PayResultListener*>(s_payInitResultListener));
        (dynamic_cast<plugin::ProtocolIAP*>(m_sdk))->configDeveloperInfo(configMap);
//        (dynamic_cast<plugin::ProtocolUser*>(m_sdk))->setActionListener(dynamic_cast<UserActionListener*>(s_initResultListener));
//        (dynamic_cast<plugin::ProtocolUser*>(m_sdk))->configDeveloperInfo(configMap);
		
//		(dynamic_cast<plugin::ProtocolIAP*>(m_sdk))->configDeveloperInfo(configMap);
		(sender->*handler)(uuESdkResultType::eSDK_RESULT_SUCCESS, map<string, string>());
		break;
	case eSDK_SHARE:
		configMap = uuSdkConfigHelper::getInitParams(m_name);
		(dynamic_cast<plugin::ProtocolShare*>(m_sdk))->configDeveloperInfo(configMap);
		(sender->*handler)(uuESdkResultType::eSDK_RESULT_SUCCESS, map<string, string>());
		break;
    case eSDK_ANALYTICS:
		configMap = uuSdkConfigHelper::getInitParams(m_name);
		(dynamic_cast<plugin::ProtocolAnalytics*>(m_sdk))->configDeveloperInfo(configMap);
		(sender->*handler)(uuESdkResultType::eSDK_RESULT_SUCCESS, map<string, string>());
		break;
	case eSDK_ADS:
		configMap = uuSdkConfigHelper::getInitParams(m_name);
		(dynamic_cast<plugin::ProtocolAds*>(m_sdk))->configDeveloperInfo(configMap);
		(sender->*handler)(uuESdkResultType::eSDK_RESULT_SUCCESS, map<string, string>());
		break;
	case eSDK_PUSH:
		configMap = uuSdkConfigHelper::getInitParams(m_name);
		(dynamic_cast<plugin::ProtocolPush*>(m_sdk))->configDeveloperInfo(configMap);
		(sender->*handler)(uuESdkResultType::eSDK_RESULT_SUCCESS, map<string, string>());
		break;
	case eSDK_LIVESTREAM:
		configMap = uuSdkConfigHelper::getInitParams(m_name);
		(dynamic_cast<plugin::ProtocolStream*>(m_sdk))->configDeveloperInfo(configMap);
		(sender->*handler)(uuESdkResultType::eSDK_RESULT_SUCCESS, map<string, string>());
		break;
	case eSDK_PLAYER:
		configMap = uuSdkConfigHelper::getInitParams(m_name);
		(dynamic_cast<plugin::ProtocolPlayer*>(m_sdk))->configDeveloperInfo(configMap);
		(sender->*handler)(uuESdkResultType::eSDK_RESULT_SUCCESS, map<string, string>());
		break;
	case eSDK_VOICE:
		configMap = uuSdkConfigHelper::getInitParams(m_name);
		(dynamic_cast<plugin::ProtocolVoice*>(m_sdk))->configDeveloperInfo(configMap);
		(sender->*handler)(uuESdkResultType::eSDK_RESULT_SUCCESS, map<string, string>());
		break;
	case eSDK_SUPPORT:
		configMap = uuSdkConfigHelper::getInitParams(m_name);
		(dynamic_cast<plugin::ProtocolSupport*>(m_sdk))->configDeveloperInfo(configMap);
		(sender->*handler)(uuESdkResultType::eSDK_RESULT_SUCCESS, map<string, string>());
		break;
    case eSDK_EXCEPTION:
//        configMap = uuSdkConfigHelper::getInitParams(m_name);
//        (dynamic_cast<plugin::ProtocolException*>(m_sdk))->configDeveloperInfo(configMap);
//        (sender->*handler)(uuESdkResultType::eSDK_RESULT_SUCCESS, map<string, string>());
		break;
	case eSDK_UNKNOWN:
		uuAssert(0, "unknown protocol type!");
		(sender->*handler)(uuESdkResultType::eSDK_RESULT_FAILED, map<string, string>());
		break;
	default:
		uuAssert(0, "excepted protocol type!");
		(sender->*handler)(uuESdkResultType::eSDK_RESULT_FAILED, map<string, string>());
		break;
	}
	uuLOG("config protocol end");
}

uuSdkWrapper::uuSdkWrapper()
{
	m_type = eSDK_UNKNOWN;
	m_sdk = NULL;
	m_resultListener = NULL;
}

uuSdkWrapper::~uuSdkWrapper()
{
	if (m_sdk != NULL) {
		delete m_sdk;
		m_sdk = NULL;
	}

	if (m_resultListener != NULL) {
		delete m_resultListener;
		m_resultListener = NULL;
	}
	if (s_initResultListener != NULL) {
		delete s_initResultListener;
		s_initResultListener = NULL;
	}
}
void uuSdkWrapper::end ()
{
	PluginManager *plugin=PluginManager::getInstance();
	plugin->end();
}
void uuSdkWrapper::setSdk( string name, PluginProtocol* protocol, uuESdkType type )
{
	m_name = name;
	m_sdk = protocol;
	m_type = type;
}

uuSdkWrapper* uuSdkWrapper::create( string sdkName)
{
	if (sdkName == "") {
		uuLOG("create sdk failed, sdk name is null !");
		return NULL;
	}
	// get sdk type
	uuESdkType sdkType = getSdkType(sdkName);
	if (sdkType == eSDK_UNKNOWN) {
		uuLOG("create sdk failed, sdk type is unknown!");
		return NULL;
	}

	// load plugin
	plugin::PluginProtocol* protocol = plugin::PluginManager::getInstance()->loadPlugin(sdkName.c_str());
	if (protocol == NULL) {
		uuLOG("create sdk failed, the sdk name is %s", sdkName.c_str());
		return NULL;
	}

	// init sdk
	uuSdkWrapper* sdk = new uuSdkWrapper();
    //sdk->configProtocol(sdkName, protocol, sdkType, sender, handler);
	sdk->setSdk(sdkName, protocol, sdkType);

	return sdk;
}

uuESdkType uuSdkWrapper::getSdkType( string sdkName )
{
	const char* type = uuSdkConfigHelper::getSdkType(sdkName);

	uuRETURN_RET_IF_COND(type== NULL, eSDK_UNKNOWN);

	if (strcmp(type, "user") == 0) {
		return eSDK_USER;
	} else if (strcmp(type, "share") == 0){
		return eSDK_SHARE;
	} else if (strcmp(type, "pay") == 0 || strcmp(type, "userPay") == 0){
		return eSDK_PAY;
	} else if (strcmp(type, "analytics") == 0){
		return eSDK_ANALYTICS;
	} else if (strcmp(type, "ads") == 0){
		return eSDK_ADS;
	} else if (strcmp(type, "push") == 0){
		return eSDK_PUSH;
	}
	else if (strcmp(type, "stream") == 0){
		return eSDK_LIVESTREAM;
	}
	else if (strcmp(type, "player") == 0){
		return eSDK_PLAYER;
	}
	else if (strcmp(type, "voice") == 0){
		return eSDK_VOICE;
	}
	else if (strcmp(type, "exception")==0){
        return eSDK_EXCEPTION;
    }else if (strcmp(type, "support") == 0) {
        return eSDK_SUPPORT;
    }
	else {
		return eSDK_UNKNOWN;
	}

	return eSDK_UNKNOWN;
}

void uuSdkWrapper::share( uuParamInfo info, uuObject* sender, uuSdkResultHandler handler )
{
	if (!isSdkAndTypeLegal(eSDK_SHARE))
		return;

	if (m_resultListener == NULL) {
		m_resultListener = new uuShareResultListener();
	}
	m_resultListener->setHandler(sender, handler);

	plugin::ProtocolShare* sdk = dynamic_cast<plugin::ProtocolShare*>(m_sdk);
	sdk->setResultListener(dynamic_cast<uuShareResultListener*>(m_resultListener));
	
	uuParamInfo params = uuSdkConfigHelper::getContentParams(m_name.c_str(), info);
	sdk->share(params);
}

void uuSdkWrapper::pay( uuParamInfo info, uuObject* sender, uuSdkResultHandler handler )
{
	if (!isSdkAndTypeLegal(eSDK_PAY))
		return;

	if (m_resultListener == NULL) {
		m_resultListener = new uuPayResult();
	}
	m_resultListener->setHandler(sender, handler);

	plugin::ProtocolIAP* sdk = dynamic_cast<plugin::ProtocolIAP*>(m_sdk);
	sdk->setResultListener(dynamic_cast<uuPayResult*>(m_resultListener));

	uuParamInfo params = uuSdkConfigHelper::getContentParams(m_name.c_str(), info);
	sdk->payForProduct(params);
}

void uuSdkWrapper::login( uuObject* sender, uuSdkResultHandler handler )
{
	if (!isSdkAndTypeLegal(eSDK_USER))
		return;

	if (m_resultListener == NULL) {
		m_resultListener = new uuLoginResult();
	}
	m_resultListener->setHandler(sender, handler);

	plugin::ProtocolUser* sdk = dynamic_cast<plugin::ProtocolUser*>(m_sdk);
	sdk->setActionListener(dynamic_cast<uuLoginResult*>(m_resultListener));

	sdk->login();
}

void uuSdkWrapper::logout( uuObject* sender, uuSdkResultHandler handler )
{
  if (!isSdkAndTypeLegal(eSDK_USER))
    return;

  if (m_resultListener == NULL) {
	  m_resultListener = new uuLoginResult();
  }
  m_resultListener->setHandler(sender, handler);

  plugin::ProtocolUser* sdk = dynamic_cast<plugin::ProtocolUser*>(m_sdk);
  sdk->setActionListener(dynamic_cast<uuLoginResult*>(m_resultListener));

  sdk->logout();
}

bool uuSdkWrapper::isLogined()
{
  if (!isSdkAndTypeLegal(eSDK_USER))
    return false;

  plugin::ProtocolUser* sdk = dynamic_cast<plugin::ProtocolUser*>(m_sdk);

  return sdk->isLogined();
}

bool uuSdkWrapper::isSdkAndTypeLegal( uuESdkType type )
{
	if (m_sdk == NULL) {
		uuLOG("the sdk is not legal, it must be not null!");
		return false;
	}
	if (getType() != type) {
		uuLOG("the type is not legal, it must be uuESdkType::%d", getType());
		return false;
	}

	return true;
}

void uuSdkWrapper::setDebugMode( bool isDebug )
{
	if (m_sdk != NULL) {
		m_sdk->setDebugMode(isDebug);
	}
}

void uuSdkWrapper::callFunc(const char* funcName)
{
	if (m_sdk != NULL) {
		m_sdk->callFuncWithParam(funcName, NULL);
	}
}

void uuSdkWrapper::callFuncWithParam(const char* funcName, std::vector<PluginParam*> params)
{
	if (m_sdk != NULL) {
		m_sdk->callFuncWithParam(funcName, params);
	}
}

int uuSdkWrapper::callIntFuncWithParam(const char* funcName, std::vector<PluginParam*> params)
{
	if (m_sdk != NULL) {
		return m_sdk->callIntFuncWithParam(funcName, params);
	}
	return 0;
}

bool uuSdkWrapper::callBoolFuncWithParam(const char* funcName, std::vector<PluginParam*> params)
{
	if (m_sdk != NULL) {
		return m_sdk->callBoolFuncWithParam(funcName, params);
	}
	return false;
}

std::string uuSdkWrapper::callStringFunc(const char* funcName)
{
	if (m_sdk != NULL) {
		return m_sdk->callStringFuncWithParam(funcName, NULL);
	}
	return "";
}

std::string uuSdkWrapper::callStringFuncWithParam(const char* funcName, std::vector<PluginParam*> params)
{
	if (m_sdk != NULL) {
		return m_sdk->callStringFuncWithParam(funcName, params);
	}
	return "";
}

bool uuSdkWrapper::callBoolFunc(const char* funcName)
{
	if (m_sdk != NULL) {
		return m_sdk->callBoolFuncWithParam(funcName, NULL);
	}
	return false;
}

void uuSdkWrapper::logPageStart( const char* pageName )
{
  if (!isSdkAndTypeLegal(eSDK_ANALYTICS))
    return;

  plugin::ProtocolAnalytics* sdk = dynamic_cast<plugin::ProtocolAnalytics*>(m_sdk);
  sdk->logPageStart(pageName);
}

void uuSdkWrapper::logPageEnd( const char* pageName )
{
  if (!isSdkAndTypeLegal(eSDK_ANALYTICS))
    return;

  plugin::ProtocolAnalytics* sdk = dynamic_cast<plugin::ProtocolAnalytics*>(m_sdk);
  sdk->logPageEnd(pageName);
}

void uuSdkWrapper::logEvent( const char* eventId )
{
  //uuLOG("uuSdkWrapper::logEvent begin null %s", eventId);
  if (!isSdkAndTypeLegal(eSDK_ANALYTICS))
    return;

  plugin::ProtocolAnalytics* sdk = dynamic_cast<plugin::ProtocolAnalytics*>(m_sdk);
  sdk->logEvent(eventId, NULL);
  //uuLOG("uuSdkWrapper::logEvent end");
}

void uuSdkWrapper::logEvent( const char* eventId, uuParamInfo info )
{
  //uuLOG("uuSdkWrapper::logEvent begin %s", eventId);
  if (!isSdkAndTypeLegal(eSDK_ANALYTICS))
    return;

  plugin::ProtocolAnalytics* sdk = dynamic_cast<plugin::ProtocolAnalytics*>(m_sdk);
  sdk->logEvent(eventId, &info);
  //uuLOG("uuSdkWrapper::logEvent end");
}

void uuSdkWrapper::logEventBegin( const char* eventId )
{
  if (!isSdkAndTypeLegal(eSDK_ANALYTICS))
    return;

  plugin::ProtocolAnalytics* sdk = dynamic_cast<plugin::ProtocolAnalytics*>(m_sdk);
  sdk->logTimedEventBegin(eventId);
}

void uuSdkWrapper::logEventEnd( const char* eventId )
{
  if (!isSdkAndTypeLegal(eSDK_ANALYTICS))
    return;

  plugin::ProtocolAnalytics* sdk = dynamic_cast<plugin::ProtocolAnalytics*>(m_sdk);
  sdk->logTimedEventEnd(eventId);
}

void uuSdkWrapper::logKVEventBegin( const char* eventId, uuParamInfo info)
{
  if (!isSdkAndTypeLegal(eSDK_ANALYTICS))
    return;

  plugin::ProtocolAnalytics* sdk = dynamic_cast<plugin::ProtocolAnalytics*>(m_sdk);
  sdk->logTimedKVEventBegin(eventId, &info);
}

void uuSdkWrapper::logKVEventEnd( const char* eventId )
{
  if (!isSdkAndTypeLegal(eSDK_ANALYTICS))
    return;

  plugin::ProtocolAnalytics* sdk = dynamic_cast<plugin::ProtocolAnalytics*>(m_sdk);
  sdk->logTimedKVEventEnd(eventId);
}

void uuSdkWrapper::logError( const char* errorId, const char* message )
{
  if (!isSdkAndTypeLegal(eSDK_ANALYTICS))
    return;

  plugin::ProtocolAnalytics* sdk = dynamic_cast<plugin::ProtocolAnalytics*>(m_sdk);
  sdk->logError(errorId, message);
}

std::string uuSdkWrapper::getOnlineParams( const char* key )
{
  if (!isSdkAndTypeLegal(eSDK_ANALYTICS))
    return "";
  plugin::ProtocolAnalytics* sdk = dynamic_cast<plugin::ProtocolAnalytics*>(m_sdk);
  std::string strKey = key;
  return sdk->getConfigParams(key);
}

void uuSdkWrapper::showAds( uuParamInfo info, int pos, uuObject* sender, uuSdkResultHandler handler )
{
	if (!isSdkAndTypeLegal(eSDK_ADS))
		return;

	if (m_resultListener == NULL) {
		m_resultListener = new uuAdsResult();
	}
	m_resultListener->setHandler(sender, handler);
	
	plugin::ProtocolAds* sdk = dynamic_cast<plugin::ProtocolAds*>(m_sdk);
	sdk->setAdsListener(dynamic_cast<uuAdsResult*>(m_resultListener));

	sdk->showAds(info, (ProtocolAds::AdsPos)pos);
}

void uuSdkWrapper::hideAds( uuParamInfo info, uuObject* sender, uuSdkResultHandler handler )
{
	if (!isSdkAndTypeLegal(eSDK_ADS))
		return;

	if (m_resultListener == NULL) {
		m_resultListener = new uuAdsResult();
	}
	m_resultListener->setHandler(sender, handler);

	plugin::ProtocolAds* sdk = dynamic_cast<plugin::ProtocolAds*>(m_sdk);
	sdk->setAdsListener(dynamic_cast<uuAdsResult*>(m_resultListener));

	sdk->hideAds(info);
}

void uuSdkWrapper::queryPoints( uuObject* sender, uuSdkResultHandler handler )
{
	if (!isSdkAndTypeLegal(eSDK_ADS))
		return;

	if (m_resultListener == NULL) {
		m_resultListener = new uuAdsResult();
	}
	m_resultListener->setHandler(sender, handler);

	plugin::ProtocolAds* sdk = dynamic_cast<plugin::ProtocolAds*>(m_sdk);
	sdk->setAdsListener(dynamic_cast<uuAdsResult*>(m_resultListener));

	sdk->queryPoints();
}

void uuSdkWrapper::spendPoints( int points, uuObject* sender, uuSdkResultHandler handler )
{
	if (!isSdkAndTypeLegal(eSDK_ADS))
		return;

	if (m_resultListener == NULL) {
		m_resultListener = new uuAdsResult();
	}
	m_resultListener->setHandler(sender, handler);

	plugin::ProtocolAds* sdk = dynamic_cast<plugin::ProtocolAds*>(m_sdk);
	sdk->setAdsListener(dynamic_cast<uuAdsResult*>(m_resultListener));

	sdk->spendPoints(points);
}

void uuSdkWrapper::startPush(uuObject* sender, uuSdkResultHandler handler)
{
	if (!isSdkAndTypeLegal(eSDK_PUSH))
		return;

	if (m_resultListener == NULL) {
		m_resultListener = new uuPushResult();
	}
	m_resultListener->setHandler(sender, handler);

	plugin::ProtocolPush* sdk = dynamic_cast<plugin::ProtocolPush*>(m_sdk);
	sdk->setPushResultListener(dynamic_cast<uuPushResult*>(m_resultListener));

	sdk->startPush();
}

void uuSdkWrapper::closePush(uuObject* sender, uuSdkResultHandler handler)
{
	if (!isSdkAndTypeLegal(eSDK_PUSH))
		return;

	if (m_resultListener == NULL) {
		m_resultListener = new uuPushResult();
	}
	m_resultListener->setHandler(sender, handler);

	plugin::ProtocolPush* sdk = dynamic_cast<plugin::ProtocolPush*>(m_sdk);
	sdk->setPushResultListener(dynamic_cast<uuPushResult*>(m_resultListener));

	sdk->closePush();
}

void uuSdkWrapper::setAlias(const char *alias, uuObject* sender, uuSdkResultHandler handler)
{
	if (!isSdkAndTypeLegal(eSDK_PUSH))
		return;

	if (m_resultListener == NULL) {
		m_resultListener = new uuPushResult();
	}
	m_resultListener->setHandler(sender, handler);

	plugin::ProtocolPush* sdk = dynamic_cast<plugin::ProtocolPush*>(m_sdk);
	sdk->setPushResultListener(dynamic_cast<uuPushResult*>(m_resultListener));

	sdk->setAlias(alias);
}

void uuSdkWrapper::delAlias(const char *alias, uuObject* sender, uuSdkResultHandler handler)
{
	if (!isSdkAndTypeLegal(eSDK_PUSH))
		return;

	if (m_resultListener == NULL) {
		m_resultListener = new uuPushResult();
	}
	m_resultListener->setHandler(sender, handler);

	plugin::ProtocolPush* sdk = dynamic_cast<plugin::ProtocolPush*>(m_sdk);
	sdk->setPushResultListener(dynamic_cast<uuPushResult*>(m_resultListener));

	sdk->delAlias(alias);
}

void uuSdkWrapper::setTags(const char *tags, uuObject* sender, uuSdkResultHandler handler)
{
	if (!isSdkAndTypeLegal(eSDK_PUSH))
		return;

	if (m_resultListener == NULL) {
		m_resultListener = new uuPushResult();
	}
	m_resultListener->setHandler(sender, handler);

	plugin::ProtocolPush* sdk = dynamic_cast<plugin::ProtocolPush*>(m_sdk);
	sdk->setPushResultListener(dynamic_cast<uuPushResult*>(m_resultListener));

	sdk->setTags(tags);
}

void uuSdkWrapper::delTags(const char *tags, uuObject* sender, uuSdkResultHandler handler)
{
	if (!isSdkAndTypeLegal(eSDK_PUSH))
		return;

	if (m_resultListener == NULL) {
		m_resultListener = new uuPushResult();
	}
	m_resultListener->setHandler(sender, handler);

	plugin::ProtocolPush* sdk = dynamic_cast<plugin::ProtocolPush*>(m_sdk);
	sdk->setPushResultListener(dynamic_cast<uuPushResult*>(m_resultListener));

	sdk->delTags(tags);
}

