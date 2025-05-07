#include "uuSdkManager.h"
#include "uuSdkConfigHelper.h"

static uuSdkManager* s_sdkManager = NULL;

uuSdkManager::uuSdkManager()
{
	m_defaultUserSdk = NULL;
	m_defaultIapSdk = NULL;
	m_initCounter = 0;
	m_init_ok = false;
}

uuSdkManager::~uuSdkManager()
{
	unloadAllSdks();
	uuSdkConfig::end();
	uuSdkWrapper::end();
}

uuSdkManager* uuSdkManager::getInstance()
{
	if (s_sdkManager == NULL) {
		s_sdkManager = new uuSdkManager();
	}
	return s_sdkManager;
}

void uuSdkManager::end()
{
	if (s_sdkManager != NULL) {
		delete s_sdkManager;
		s_sdkManager = NULL;
	}
}

const char* uuSdkManager::login(uuObject* sender, uuSdkResultHandler handler)
{
	const char* sdkName = getDefaultUserSdkName();
	if (sdkName == NULL) {
		uuLOG("no default user sdk!, it maybe a platform that not need a user sdk.");
		return NULL;
	}

	uuSdkWrapper* sdk = getSdk(sdkName);
	if (sdk == NULL) {
		uuLOG("default login failed, can not find the default sdk name!");
    std::map<std::string, std::string> empty;
    (sender->*handler)(eSDK_RESULT_FAILED, empty);
		return NULL;
	} else {
		sdk->login(sender, handler);
		return sdkName;
	}
}

void uuSdkManager::login(std::string sdkName, uuObject* sender, uuSdkResultHandler handler)
{
	if (sdkName == "") {
		uuLOG("login failed, the sdk name is null.");
    std::map<std::string, std::string> empty;
    (sender->*handler)(eSDK_RESULT_FAILED, empty);
		return;
	}

	uuSdkWrapper* sdk = getSdk(sdkName);
	if (sdk == NULL) {
		uuLOG("login failed, can not find the sdk, name:%s", sdkName.c_str());
    std::map<std::string, std::string> empty;
    (sender->*handler)(eSDK_RESULT_FAILED, empty);
	} else {
		sdk->login(sender, handler);
	}
}

bool uuSdkManager::isLogined()
{
    const char* sdkName = getDefaultUserSdkName();
    if (sdkName == NULL) {
        uuLOG("no default user sdk!, it maybe a platform that not need a user sdk.");
        return false;
    }
    
    uuSdkWrapper* sdk = getSdk(sdkName);
    if (sdk == NULL) {
        uuLOG("default login failed, can not find the default sdk name!");
        return false;
    } else {
        return sdk->isLogined();
    }
}

void uuSdkManager::logout(uuObject* sender, uuSdkResultHandler handler)
{
	const char* sdkName = getDefaultUserSdkName();
	if (sdkName == NULL) {
		uuLOG("default logout failed, can not find the default sdk name!");
		return;
	}

	uuSdkWrapper* sdk = getSdk(sdkName);
	std::map<std::string, std::string> empty;
	if (sdk == NULL) {
		uuLOG("default logout failed, can not find the default sdk name!");
    (sender->*handler)(eSDK_RESULT_FAILED, empty);
	} else {
		sdk->logout(sender, handler);
	}
}

void uuSdkManager::logout(std::string sdkName, uuObject* sender, uuSdkResultHandler handler)
{
	if (sdkName == "") {
		uuLOG("logout failed,  the sdk name is null.");
		return;
	}

	uuSdkWrapper* sdk = getSdk(sdkName);
    std::map<std::string, std::string> empty;
	if (sdk == NULL) {
		uuLOG("default logout failed, can not find the sdk name!");
        (sender->*handler)(eSDK_RESULT_FAILED, empty);
	} else
    {
        sdk->logout(sender, handler);
	}
}

void uuSdkManager::share( std::string sdkName, uuParamInfo info, uuObject* sender, uuSdkResultHandler handler )
{
	uuSdkWrapper* sdk = getSdk(sdkName);
	if (sdk == NULL) {
		uuLOG("share failed, can not find sdk name!");
        std::map<std::string, std::string> empty;
        (sender->*handler)(eSDK_RESULT_FAILED, empty);
	} else {
		sdk->share(info, sender, handler);
	}
}

void uuSdkManager::pay( uuParamInfo info, uuObject* sender, uuSdkResultHandler handler )
{
	if (m_defaultIapSdk == NULL)
    {
		uuLOG("pay failed, the default iap sdk is none, you must set a iap sdk name!");
		std::map<std::string, std::string> empty;
		(sender->*handler)(eSDK_RESULT_FAILED, empty);
	} else {
		m_defaultIapSdk->pay(info, sender, handler);
	}
}

void uuSdkManager::pay( std::string sdkName, uuParamInfo info, uuObject* sender, uuSdkResultHandler handler )
{
	uuSdkWrapper* sdk = getSdk(sdkName);
	if (sdk == NULL) {
		uuLOG("pay failed, can not find sdk name!");
		std::map<std::string, std::string> empty;
		(sender->*handler)(eSDK_RESULT_FAILED, empty);
	} else {
		sdk->pay(info, sender, handler);
	}
}

bool uuSdkManager::init( std::string sdkName, uuObject* sender, uuSdkResultHandler handler,uuSdkResultHandler payHandler)
{
	if (sdkName == "" || sdkName == "UserTest" || sdkName == "IAPTest") {
		(sender->*handler)((uuESdkResultType::eSDK_RESULT_SUCCESS), map<string, string>());
		return true;
	}

	uuSdkWrapper* sdk = NULL;
	std::map<std::string, uuSdkWrapper*>::iterator itr = m_sdkMaps.find(sdkName);
	if (itr != m_sdkMaps.end()) {
		sdk = itr->second;
	} else {
		sdk = uuSdkWrapper::create(sdkName);
		m_sdkMaps[sdkName.c_str()] = sdk;
	}

	if (sdk == NULL) {
		(sender->*handler)((uuESdkResultType::eSDK_RESULT_FAILED), map<string, string>());
		uuLOG("%s init, sdk %s init failed.",LOG_TAG, sdkName.c_str());
		return false;
	}

	sdk->configProtocol(sender, handler,payHandler);

	const char* defaultUser = uuSdkConfigHelper::getDefaultUserSdkName();
	if (m_defaultUserSdk == NULL && defaultUser != NULL && sdkName.compare(defaultUser) == 0) {
		uuLOG("%s init, the default user sdk is : %s",LOG_TAG, sdkName.c_str());
		m_defaultUserSdk = sdk;
	}
	const char* defaultIap = uuSdkConfigHelper::getDefaultIapSdkName();
	if (m_defaultIapSdk  == NULL && defaultIap != NULL && sdkName.compare(defaultIap) == 0) {
		uuLOG("%s init, the default iap sdk is : %s",LOG_TAG, sdkName.c_str());
		m_defaultIapSdk = sdk;
	}

	uuLOG("%s init, sdk %s init success.",LOG_TAG, sdkName.c_str());
	return true;
}

void uuSdkManager::initAllSdks(uuObject* sender , uuSdkResultHandler handler, uuSdkResultHandler payHandler)
{
	if (m_init_ok)
	{
		(sender->*m_handler)(m_initErrorType, m_initParams);
        return;
	}

	std::vector<std::string> vecNames = uuSdkConfigHelper::getAllSdkNames();	
	uuLOG("%s initAllSdks, begin to init all sdks, the number is: %d",LOG_TAG, vecNames.size());

	m_initCounter = 0;
	m_sender = sender;
	m_handler = handler;
    m_payHandler = payHandler;
	m_initParams.clear();
	m_initErrorParams.clear();
	m_initErrorType = uuESdkResultType::eSDK_RESULT_SUCCESS;
	m_initNumbers = (int)vecNames.size();
	for (std::vector<std::string>::iterator itr = vecNames.begin(); itr != vecNames.end(); ++itr) {
		init(*itr, this, (uuSdkResultHandler)&uuSdkManager::initCallback, (uuSdkResultHandler)&uuSdkManager::initPayCallback);
	}
	if (vecNames.size() == 0)
	{
		(sender->*m_handler)(m_initErrorType, m_initParams);
	}

	m_init_ok = true;
}

bool uuSdkManager::load(std::string sdkName)
{
	if (sdkName == "")
		return false;

	if (sdkName == "UserTest" || sdkName == "IAPTest") {
		uuLOG("load test sdk.");
		return true;
	}

	uuSdkWrapper* sdk = NULL;
	std::map<std::string, uuSdkWrapper*>::iterator itr = m_sdkMaps.find(sdkName);
	if (itr != m_sdkMaps.end()) {
		sdk = itr->second;
	} else {
		sdk = uuSdkWrapper::create(sdkName);
		m_sdkMaps[sdkName.c_str()] = sdk;
	}

	if (sdk == NULL) {
		uuLOG("load sdk failed, sdk name: %s", sdkName.c_str());
		return false;
	}

	//sdk->configProtocol(sender, handler);

	const char* defaultUser = uuSdkConfigHelper::getDefaultUserSdkName();
	if (defaultUser != NULL && sdkName.compare(defaultUser) == 0) {
		uuLOG("the default user sdk is : %s", sdkName.c_str());
		m_defaultUserSdk = sdk;
	} 
	const char* defaultIap = uuSdkConfigHelper::getDefaultIapSdkName();
	if (defaultIap != NULL && sdkName.compare(defaultIap) == 0) {
		uuLOG("the default iap sdk is : %s", sdkName.c_str());
		m_defaultIapSdk = sdk;
	}

	return true;
}

int uuSdkManager::loadAllSdks()
{
	std::vector<std::string> vecNames = uuSdkConfigHelper::getAllSdkNames();
	std::vector<std::string>::iterator itr;
	uuLOG("begin to load all sdks, the number is: %u", vecNames.size());
	for (itr = vecNames.begin(); itr != vecNames.end(); ++itr)
    {
		if (load(*itr))
        {
			uuLOG("sdk [%s] loaded.", (*itr).c_str());
		} else
        {
			uuLOG("failed to load [sdk] %s", (*itr).c_str());
            return -4;
		};
	}
	uuLOG("load all sdks end.");
    return 0;
}

void uuSdkManager::unloadSdk( std::string sdkName )
{
	if (sdkName == "")
		return;

	std::map<std::string, uuSdkWrapper*>::iterator itr = m_sdkMaps.find(sdkName.c_str());
	if (itr != m_sdkMaps.end()) {
		if (itr->second != NULL) {
			delete itr->second;
			itr->second = NULL;
		}
		m_sdkMaps.erase(itr);
	}
}

void uuSdkManager::unloadAllSdks()
{
	std::map<std::string, uuSdkWrapper*>::iterator itr = m_sdkMaps.begin();
	for(; itr != m_sdkMaps.end(); ++itr) {
		if (itr->second != NULL) {
			delete itr->second;
			itr->second = NULL;
		}
	}
	m_sdkMaps.clear();
}

const char* uuSdkManager::getDefaultUserSdkName()
{
	return uuSdkConfigHelper::getDefaultUserSdkName();
}

const char* uuSdkManager::getDefaultIapSdkName()
{
	return uuSdkConfigHelper::getDefaultIapSdkName();
}

void uuSdkManager::setDebugMode( std::string sdkName, bool isDebug )
{
	uuSdkWrapper* sdk = getSdk(sdkName);

	if (sdk != NULL) {
		sdk->setDebugMode(isDebug);
	}
}

uuSdkWrapper* uuSdkManager::getSdk( std::string sdkName )
{
	if (sdkName == "")
		return NULL;

	if (m_sdkMaps.find(sdkName) != m_sdkMaps.end()) {
		return m_sdkMaps[sdkName];
	}
	uuLOG("getSdk failed, can not find sdk name: %s", sdkName.c_str());
	return NULL;
}

void uuSdkManager::callFunc(const char* funcName)
{
	std::map<std::string, uuSdkWrapper*>::iterator itr = m_sdkMaps.begin();
	for(; itr != m_sdkMaps.end(); ++itr) {
		if (itr->second != NULL) {
			itr->second->callFunc(funcName);
		}
	}
}

void uuSdkManager::callFunc(std::string sdkName, const char* funcName)
{
  uuSdkWrapper* sdk = getSdk(sdkName);
  if (sdk != NULL) {
    sdk->callFunc(funcName);
  }
}

void uuSdkManager::callFuncWithParam(const char* funcName, std::vector<PluginParam*> params)
{
	std::map<std::string, uuSdkWrapper*>::iterator itr = m_sdkMaps.begin();
	for(; itr != m_sdkMaps.end(); ++itr) {
		if (itr->second != NULL) {
			itr->second->callFuncWithParam(funcName, params);
		}
	}
}

void uuSdkManager::callFuncWithParam(std::string sdkName, const char* funcName, std::vector<PluginParam*> params)
{
  uuSdkWrapper* sdk = getSdk(sdkName);
  if (sdk != NULL) {
    sdk->callFuncWithParam(funcName, params);
  }
}

std::string uuSdkManager::callStringFunc(const char* funcName)
{
	std::map<std::string, uuSdkWrapper*>::iterator itr = m_sdkMaps.begin();
	string temp = "";
	for(; itr != m_sdkMaps.end(); ++itr) {
		if (itr->second != NULL) {
			temp = itr->second->callStringFunc(funcName);
			if (temp != "") return temp;
		}
	}
	return temp;
}

std::string uuSdkManager::callStringFunc(std::string sdkName, const char* funcName)
{
  uuSdkWrapper* sdk = getSdk(sdkName);
  if (sdk != NULL) {
    return sdk->callStringFunc(funcName);
  } else {
    return "";
  }
}

std::string uuSdkManager::callStringFuncWithParam(const char* funcName, std::vector<PluginParam*> params){
	std::map<std::string, uuSdkWrapper*>::iterator itr = m_sdkMaps.begin();
	string temp = "";
	for(; itr != m_sdkMaps.end(); ++itr) {
		if (itr->second != NULL) {
			temp = itr->second->callStringFuncWithParam(funcName, params);
			if (temp != "") return temp;
		}
	}
	return temp;
}

bool uuSdkManager::callBoolFunc(const char* funcName)
{
	bool temp = false;
	std::map<std::string, uuSdkWrapper*>::iterator itr = m_sdkMaps.begin();
	for(; itr != m_sdkMaps.end(); ++itr) {
		if (itr->second != NULL) {
			temp = itr->second->callBoolFunc(funcName);
			if (temp) return temp;
		}
	}
	return false;
}

bool uuSdkManager::callBoolFunc(std::string sdkName, const char* funcName)
{
	uuSdkWrapper* sdk = getSdk(sdkName);
	if (sdk != NULL) {
		return sdk->callBoolFunc(funcName);
	} else {
		return false;
	}
}

bool uuSdkManager::callBoolFuncWithParam(const char* funcName, std::vector<PluginParam*> params)
{
	std::map<std::string, uuSdkWrapper*>::iterator itr = m_sdkMaps.begin();
	bool ret = 0;
	for(; itr != m_sdkMaps.end(); ++itr) {
		if (itr->second != NULL) {
			ret = itr->second->callBoolFuncWithParam(funcName, params);
			if (ret) return ret;
		}
	}
	return ret;
}

bool uuSdkManager::callBoolFuncWithParam(std::string sdkName, const char* funcName, std::vector<PluginParam*> params)
{
	uuSdkWrapper* sdk = getSdk(sdkName);
	if (sdk != NULL) {
		return sdk->callBoolFuncWithParam(funcName, params);
	}
	return false;
}

int uuSdkManager::callIntFuncWithParam(const char* funcName, std::vector<PluginParam*> params)
{
	std::map<std::string, uuSdkWrapper*>::iterator itr = m_sdkMaps.begin();
	int ret = 0;
	for(; itr != m_sdkMaps.end(); ++itr) {
		if (itr->second != NULL) {
			ret = itr->second->callIntFuncWithParam(funcName, params);
			if (ret != 0) return ret;
		}
	}
	return ret;
}

int uuSdkManager::callIntFuncWithParam(std::string sdkName, const char* funcName, std::vector<PluginParam*> params)
{
	uuSdkWrapper* sdk = getSdk(sdkName);
	if (sdk != NULL) {
		return sdk->callIntFuncWithParam(funcName, params);
	}
	return 0;
}


void uuSdkManager::logPageStart( const char* sdkName, const char* pageName )
{
  uuSdkWrapper* sdk = getSdk(sdkName);
  if (sdk == NULL) {
    uuLOG("logPageStart failed, can not find sdk name!");
  } else {
    sdk->logPageStart(pageName);
  }
}

void uuSdkManager::logPageEnd( const char* sdkName, const char* pageName )
{
  uuSdkWrapper* sdk = getSdk(sdkName);
  if (sdk == NULL) {
    uuLOG("logPageEnd failed, can not find sdk name!");
  } else {
    sdk->logPageEnd(pageName);
  }
}

void uuSdkManager::logEvent( const char* sdkName, const char* eventId )
{
  uuSdkWrapper* sdk = getSdk(sdkName);
  if (sdk == NULL) {
    //uuLOG("logEvent failed, can not find sdk name!");
	 vector<string> sdknames = uuSdkConfig::getInstance()->getAllSdkNamesByType("analytics");
	 vector<string>::iterator it;
	 for (it = sdknames.begin(); it != sdknames.end(); ++it) {
		 sdk = getSdk((*it).c_str());
		 if (sdk != NULL) {
			 sdk->logEvent(eventId); 
		 }
	 }
	 
  } else {
    sdk->logEvent(eventId);
  }
}

void uuSdkManager::logEvent( const char* sdkName, const char* eventId, uuParamInfo info )
{
  uuSdkWrapper* sdk = getSdk(sdkName);
  if (sdk == NULL) {
    //uuLOG("logEvent failed, can not find sdk name!");
	  vector<string> sdknames = uuSdkConfig::getInstance()->getAllSdkNamesByType("analytics");
	  vector<string>::iterator it;
	  for (it = sdknames.begin(); it != sdknames.end(); ++it) {
		  sdk = getSdk((*it).c_str());
		  if (sdk != NULL) {
			  sdk->logEvent(eventId, info); 
		  }
	  }
  } else {
    sdk->logEvent(eventId, info);
  }
}

void uuSdkManager::logEventBegin( const char* sdkName, const char* eventId )
{
  uuSdkWrapper* sdk = getSdk(sdkName);
  if (sdk == NULL) {
    uuLOG("logEventBegin failed, can not find sdk name!");
  } else {
    sdk->logEventBegin(eventId);
  }
}

void uuSdkManager::logEventEnd( const char* sdkName, const char* eventId )
{
  uuSdkWrapper* sdk = getSdk(sdkName);
  if (sdk == NULL) {
    uuLOG("pay failed, can not find sdk name!");
  } else {
    sdk->logEventEnd(eventId);
  }
}

void uuSdkManager::logKVEventBegin( const char* sdkName, const char* eventId, uuParamInfo info )
{
  uuSdkWrapper* sdk = getSdk(sdkName);
  if (sdk == NULL) {
    uuLOG("logEventBegin failed, can not find sdk name!");
  } else {
    sdk->logKVEventBegin(eventId, info);
  }
}

void uuSdkManager::logKVEventEnd( const char* sdkName, const char* eventId )
{
  uuSdkWrapper* sdk = getSdk(sdkName);
  if (sdk == NULL) {
    uuLOG("pay failed, can not find sdk name!");
  } else {
    sdk->logKVEventEnd(eventId);
  }
}

void uuSdkManager::logError( const char* sdkName, const char* errorId, const char* message )
{
  uuSdkWrapper* sdk = getSdk(sdkName);
  if (sdk == NULL) {
    uuLOG("logError failed, can not find sdk name!");
  } else {
    sdk->logError(errorId, message);
  }
}

std::string uuSdkManager::getOnlineParams( const char* sdkName, const char* key )
{
  uuSdkWrapper* sdk = getSdk(sdkName);
  if (sdk == NULL) {
    uuLOG("getOnlineParams failed, can not find sdk name!");
    return "";
  } else {
    return sdk->getOnlineParams(key);
  }
}

void uuSdkManager::showAds( std::string sdkName, uuParamInfo info, int pos, uuObject* sender, uuSdkResultHandler handler )
{
	uuSdkWrapper* sdk = getSdk(sdkName);
	if (sdk == NULL) {
		uuLOG("ads failed, can not find sdk name!");
		std::map<std::string, std::string> empty;
		(sender->*handler)(eSDK_RESULT_FAILED, empty);
	} else {
		sdk->showAds(info, pos, sender, handler);
	}
}

void uuSdkManager::hideAds( std::string sdkName, uuParamInfo info, uuObject* sender, uuSdkResultHandler handler )
{
	uuSdkWrapper* sdk = getSdk(sdkName);
	if (sdk == NULL) {
		uuLOG("ads failed, can not find sdk name!");
		std::map<std::string, std::string> empty;
		(sender->*handler)(eSDK_RESULT_FAILED, empty);
	} else {
		sdk->hideAds(info, sender, handler);
	}
}

void uuSdkManager::queryPoints( std::string sdkName, uuObject* sender, uuSdkResultHandler handler )
{
	uuSdkWrapper* sdk = getSdk(sdkName);
	if (sdk == NULL) {
		uuLOG("ads failed, can not find sdk name!");
		std::map<std::string, std::string> empty;
		(sender->*handler)(eSDK_RESULT_FAILED, empty);
	} else {
		sdk->queryPoints(sender, handler);
	}
}

void uuSdkManager::spendPoints( std::string sdkName, int points, uuObject* sender, uuSdkResultHandler handler )
{
	uuSdkWrapper* sdk = getSdk(sdkName);
	if (sdk == NULL) {
		uuLOG("ads failed, can not find sdk name!");
		std::map<std::string, std::string> empty;
		(sender->*handler)(eSDK_RESULT_FAILED, empty);
	} else {
		sdk->spendPoints(points, sender, handler);
	}
}

void uuSdkManager::startPush( std::string sdkName, uuObject* sender, uuSdkResultHandler handler )
{
	uuSdkWrapper* sdk = getSdk(sdkName);
	if (sdk == NULL) {
		uuLOG("start push failed, can not find sdk name!");
		std::map<std::string, std::string> empty;
		(sender->*handler)(eSDK_RESULT_FAILED, empty);
	} else {
		sdk->startPush(sender, handler);
	}
}

void uuSdkManager::closePush( std::string sdkName, uuObject* sender, uuSdkResultHandler handler )
{
	uuSdkWrapper* sdk = getSdk(sdkName);
	if (sdk == NULL) {
		uuLOG("close push failed, can not find sdk name!");
		std::map<std::string, std::string> empty;
		(sender->*handler)(eSDK_RESULT_FAILED, empty);
	} else {
		sdk->closePush(sender, handler);
	}
}

void uuSdkManager::setAlias( std::string sdkName, std::string alias, uuObject* sender, uuSdkResultHandler handler )
{
	uuSdkWrapper* sdk = getSdk(sdkName);
	if (sdk == NULL) {
		uuLOG("set alias failed, can not find sdk name!");
		std::map<std::string, std::string> empty;
		(sender->*handler)(eSDK_RESULT_FAILED, empty);
	} else {
		sdk->setAlias(alias.c_str(), sender, handler);
	}
}

void uuSdkManager::delAlias( std::string sdkName, std::string alias, uuObject* sender, uuSdkResultHandler handler )
{
	uuSdkWrapper* sdk = getSdk(sdkName);
	if (sdk == NULL) {
		uuLOG("del alias failed, can not find sdk name!");
		std::map<std::string, std::string> empty;
		(sender->*handler)(eSDK_RESULT_FAILED, empty);
	} else {
		sdk->delAlias(alias.c_str(), sender, handler);
	}
}

void uuSdkManager::setTags( std::string sdkName, std::string tags, uuObject* sender, uuSdkResultHandler handler )
{
	uuSdkWrapper* sdk = getSdk(sdkName);
	if (sdk == NULL) {
		uuLOG("set tags failed, can not find sdk name!");
		std::map<std::string, std::string> empty;
		(sender->*handler)(eSDK_RESULT_FAILED, empty);
	} else {
		sdk->setTags(tags.c_str(), sender, handler);
	}
}


void uuSdkManager::delTags( std::string sdkName, std::string tags, uuObject* sender, uuSdkResultHandler handler )
{
	uuSdkWrapper* sdk = getSdk(sdkName);
	if (sdk == NULL) {
		uuLOG("del tags failed, can not find sdk name!");
		std::map<std::string, std::string> empty;
		(sender->*handler)(eSDK_RESULT_FAILED, empty);
	} else {
		sdk->delTags(tags.c_str(), sender, handler);
	}
}

void uuSdkManager::initPayCallback( uuESdkResultType type, map<string, string> params )
{
    (m_sender->*m_payHandler)(type, params);
}

void uuSdkManager::initCallback( uuESdkResultType type, map<string, string> params )
{
	m_initCounter++;
	uuLOG("m_initCounter: %d, %d, type:%d", m_initCounter, m_initNumbers, type);
	if (type != uuESdkResultType::eSDK_RESULT_SUCCESS) {
		m_initErrorType = type;
		m_initErrorParams = params;
	} else {
		for (map<string, string>::iterator it = params.begin(); it != params.end(); it++) {
			m_initParams[it->first] = it->second;
		}
	}
	if (m_initCounter == m_initNumbers) {
		if (m_initErrorType == uuESdkResultType::eSDK_RESULT_SUCCESS) {
			(m_sender->*m_handler)(m_initErrorType, m_initParams);
		} else {
			(m_sender->*m_handler)(m_initErrorType, m_initErrorParams);
		}
	}
}

