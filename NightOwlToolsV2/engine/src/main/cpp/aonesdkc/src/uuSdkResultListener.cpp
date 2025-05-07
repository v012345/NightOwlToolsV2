#include "uuSdkResultListener.h"
#include <string>
#include "xcore_json.h"
using namespace std;


void uuSdkResultListener::setHandler(uuObject *listener, uuSdkResultHandler handler)
{
    m_listener=listener;
    m_handler=handler;
}

void uuShareResultListener::onShareResult( ShareResultCode ret, const char* msg )
{
	uuLOG("call back share result!");

	char shareStatus[1024] = { 0 };
	sprintf(shareStatus, "Share %s\n", (ret == kShareSuccess)? "Successed" : "Failed");
	if (m_handler == NULL) {
		uuMessageBox(msg, shareStatus);
	} else {
		if (msg != NULL) {
			uuLOG("%s, %s", shareStatus, msg);
		} else {
			uuLOG("%s", shareStatus);
		}
		uuESdkResultType resultType;
		switch (ret)
		{
		case kShareSuccess:
			resultType = eSDK_RESULT_SUCCESS;
			break;
		case kShareFail:
			resultType = eSDK_RESULT_FAILED;
			break;
		case kShareCancel:
			resultType = eSDK_RESULT_CANCEL;
			break;
		case kShareTimeOut:
			resultType = eSDK_RESULT_TIMEOUT;
			break;
		default:
			resultType = eSDK_RESULT_FAILED;
			break;
		}
		std::map<std::string, std::string> resultMap;
		resultMap["share_msg"] = msg;
		(m_listener->*m_handler)(resultType, resultMap);
	}
}

void uuPayResult::onPayResult( PayResultCode ret, const char* msg, TProductInfo info )
{
	uuLOG("call back pay result!");

	char shareStatus[1024] = { 0 };
	sprintf(shareStatus, "Pay %s\n", (ret == kPaySuccess)? "Successed" : "Failed");
	if (m_handler == NULL) {
		uuMessageBox(msg, shareStatus);
	} else {
		if (msg != NULL) {
			uuLOG("%s, %s", shareStatus, msg);
			info["aone_msg"] = msg;
			// 解析json
			XJsonValue root(XJsonValue::objectValue);
			if (true == XJsonReader().parse(msg, root, false) && root.type() == XJsonValue::objectValue)
			{
				XJsonValue::ObjectValues& objs = root.as_objects();
				for (XJsonValue::ObjectIterator it = objs.begin(); it != objs.end(); ++it)
				{
					if (it->second.type() == XJsonValue::stringValue)
					{
						info[it->first] = it->second.as_string();
					}
				}
			}
		} else {
			uuLOG("%s", shareStatus);
		}
		uuESdkResultType resultType;
		switch (ret)
		{
		case kPaySuccess:
			resultType = eSDK_RESULT_SUCCESS;
			break;
		case kPayFail:
			resultType = eSDK_RESULT_FAILED;
			break;
		case kPayCancel:
			resultType = eSDK_RESULT_CANCEL;
			break;
		case kPayTimeOut:
			resultType = eSDK_RESULT_TIMEOUT;
			break;
		default:
			resultType = eSDK_RESULT_FAILED;
			break;
		}
		//std::map<std::string, std::string> resultMap;
		(m_listener->*m_handler)(resultType, info);
	}
}

void uuLoginResult::onActionResult( ProtocolUser* pPlugin, UserActionResultCode code, const char* msg )
{
	uuLOG("call back login result!");

	char shareStatus[1024] = { 0 };
	sprintf(shareStatus, "User of \"%s\" login %s\n", pPlugin->getPluginName(), (code == kLoginSucceed)? "Successed" : "Failed");
	if (m_handler == NULL) {
		uuMessageBox(msg, shareStatus);
	} else {
		if (msg != NULL) {
			uuLOG("%s, %s", shareStatus, msg);
		} else {
			uuLOG("%s", shareStatus);
		}
		uuESdkResultType resultType;
		switch (code)
		{
		case kLoginSucceed:
			resultType = eSDK_RESULT_SUCCESS;
			break;
		case kLoginFailed:
			resultType = eSDK_RESULT_FAILED;
			break;
        case kLogoutSucceed:
            resultType = eSDK_RESULT_LOGOUT;
            break;
        case kLoginCancel:
            resultType = eSDK_RESULT_CANCEL;
            break;
		case kSwitchAccountSucceed:
			resultType = eSDK_RESULT_SWITCHACCOUNT;
			break;
		default:
			resultType = eSDK_RESULT_FAILED;
			break;
		}

		// return json string
		std::map<std::string, std::string> resultMap;
		if (code == kLoginSucceed || code == kSwitchAccountSucceed)
		{
			string session = pPlugin->getSessionID();
            
			if (session == "") 
			{
				uuLOG("the login session id is empty!");
			}

			resultMap["session"] = session;
			uuLOG("the login session id is : %s", pPlugin->getSessionID().c_str());
			string uid = pPlugin->getUId();
			if (uid != "")
			{
				resultMap["uid"] = uid;
				uuLOG("the login uid id is : %s", pPlugin->getUId().c_str());
			}
		}

		(m_listener->*m_handler)(resultType, resultMap);
	}
}

void uuLogoutResult::onActionResult( ProtocolUser* pPlugin, UserActionResultCode code, const char* msg )
{
	uuLOG("call back logout result!");

	char shareStatus[1024] = { 0 };
	sprintf(shareStatus, "User of \"%s\" logout %s\n", pPlugin->getPluginName(), (code == kLogoutSucceed)? "Successed" : "Failed");
	if (m_handler == NULL) {
		uuMessageBox(msg, shareStatus);
	} else {
		if (msg != NULL) {
			uuLOG("%s, %s", shareStatus, msg);
		} else {
			uuLOG("%s", shareStatus);
		}
		uuESdkResultType resultType;
		switch (code)
		{
		case kLoginSucceed:
			resultType = eSDK_RESULT_SUCCESS;
			break;
		case kLoginFailed:
			resultType = eSDK_RESULT_FAILED;
			break;
		case kLogoutSucceed:
			resultType = eSDK_RESULT_LOGOUT;
			break;
        case kLoginCancel:
            resultType = eSDK_RESULT_CANCEL;
            break;
		default:
			resultType = eSDK_RESULT_FAILED;
			break;
		}

		std::map<std::string, std::string> resultMap;
		(m_listener->*m_handler)(resultType, resultMap);
	}
}

void uuUserInitResult::onActionResult( ProtocolUser* pPlugin, UserActionResultCode code, const char* msg )
{
	uuLOG("call back user init result!");
    
	char shareStatus[1024] = { 0 };
	sprintf(shareStatus, "User of \"%s\" init %s\n", pPlugin->getPluginName(), (code == kLoginSucceed)? "Successed" : "Failed");
	//if (isInited)
    //    return;
    
    //isInited = true;
    
    if (m_handler == NULL) {
		uuMessageBox(msg, shareStatus);
	} else {
		if (msg != NULL) {
			uuLOG("%s, %s", shareStatus, msg);
		} else {
			uuLOG("%s", shareStatus);
		}
		uuESdkResultType resultType;
		switch (code)
		{
            case kLoginSucceed:
                resultType = eSDK_RESULT_SUCCESS;
                break;
            case kLoginFailed:
            default:
				resultType = eSDK_RESULT_FAILED;
                break;
		}
        
		std::map<std::string, std::string> resultMap;
        
		(m_listener->*m_handler)(resultType, resultMap);
	}
}

void uuAdsResult::onAdsResult( AdsResultCode code, const char* msg )
{
	uuLOG("call back ads result!");
	uuLOG("code:%d", code);

	char shareStatus[1024] = { 0 };
	sprintf(shareStatus, "ads result: %d\n", code);
	std::map<std::string, std::string> resultMap;
	if (m_handler == NULL) {
		uuMessageBox(msg, shareStatus);
	} else {
		if (msg != NULL) {
			uuLOG("%s, %s", shareStatus, msg);
		} else {
			uuLOG("%s", shareStatus);
		}
		uuESdkResultType resultType;
		string message = "";
		switch (code)
		{//下面的赋值枚举只对应相应的数字大小，并不表示其中的含义
		case kAdsReceived:
			resultType = eSDK_RESULT_SUCCESS;//0
			message = "AdsReceived";
			break;
		case kAdsShown:
			resultType = eSDK_RESULT_FAILED;//1
			message = "AdsShown";
			break;
		case kAdsDismissed:
			resultType = eSDK_RESULT_CANCEL;//2
			message = "AdsDismissed";
			break;
		case kPointsSpendSucceed:
			resultType = eSDK_RESULT_TIMEOUT;//3
			message = "PointsSpendSucceed";
			break;
		//Reward rejected
		case kRewardRejected:
			resultType = eSDK_RESULT_UNKNOWERROR;
			message = "RewardRejected";
			break;

		//failed,need deal in aonesdk.cpp(adsCallback)
		case kPointsSpendFailed:
			resultType = eSDK_RESULT_LOGOUT;//4
			message = "PointsSpendFailed";
			break;
		case kNetworkError:
			resultType = eSDK_RESULT_SWITCHACCOUNT;//5
			message = "NetworkError";
			break;
		case kUnknownError:
		default:
			resultType = eSDK_RESULT_SWITCHACCOUNT;//5
			message = "UnknownError";
			break;
		}
		resultMap["message"] = message;
		(m_listener->*m_handler)(resultType, resultMap);
	}
}


void uuPushResult::onPushResult( ProtocolPush* pPlugin, PushResultCode code, const char* msg )
{
	uuLOG("call back push result!");

	char shareStatus[1024] = { 0 };
	sprintf(shareStatus, "push plugin %s, result: %d\n", pPlugin->getPluginName(), code);

	if (m_handler == NULL) {
		uuMessageBox(msg, shareStatus);
	} else {
		if (msg != NULL) {
			uuLOG("%s, %s", shareStatus, msg);
		} else {
			uuLOG("%s", shareStatus);
		}
		uuESdkResultType resultType;
		switch (code)
		{
		case kPushSuccess:
			resultType = eSDK_RESULT_SUCCESS;
			break;
		default:
			resultType = eSDK_RESULT_FAILED;
			break;
		}

		std::map<std::string, std::string> resultMap;

		(m_listener->*m_handler)(resultType, resultMap);
	}
}

