#include <android/log.h>
#include "uuNetAsync.h"
#include "PluginJniHelper.h"

#define  LOG_TAG    "aonesdk debug uuNetAsync"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define CALSS_NAME "com/aonesoft/lib/AoneNetAsync"
#define CALSS_NAME_2 "com/aonesoft/lib/AoneNetThread"

namespace aonesdk {

AoneSDK* uuNetAsync::mSdk;
NET_CB uuNetAsync::mCb;
AoneSDK* uuNetAsync::mHttpSdk;
HTTP_CB uuNetAsync::mHttpCb;
map<string, string> uuNetAsync::mParams;

vector<AoneSDK*> uuNetAsync::mSdks = vector<AoneSDK*>();
vector<NET_CB> uuNetAsync::mCbs = vector<NET_CB>();
int uuNetAsync::g_netNumber;

vector<AoneSDK*> uuNetAsync::mHttpSdks = vector<AoneSDK*>();
vector<HTTP_CB> uuNetAsync::mHttpCbs = vector<HTTP_CB>();
int uuNetAsync::g_httpNumber;
map<int, map<string, string> > uuNetAsync::mParamsMap = map<int, map<string, string> >();

extern "C" {
	JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneNetThread_nativeSendRecv(JNIEnv* env, jobject thiz, jobject netThread, jstring ip, jint port, jstring key, jbyteArray request, jint requestLen) {
        LOGD("AoneNetThread nativeSendRecv begin");
        unsigned char *response = NULL;
        unsigned int responseLen = 0;
        const char *str = env->GetStringUTFChars(ip, NULL);
        string c_ip = str;
        env->ReleaseStringUTFChars(ip, str);
        str = env->GetStringUTFChars(key, NULL);
        string c_key = str;
        env->ReleaseStringUTFChars(ip, str);
        unsigned char* c_request = (unsigned char*)env->GetByteArrayElements(request, NULL);

        int result = aonesdk::AoneSDK::getInstance()->send_recv_async(c_ip, (int)port, c_key, c_request, (int)requestLen, &response, responseLen);
        env->ReleaseByteArrayElements(request, (jbyte*)c_request, 0);

        PluginJniMethodInfo methodInfo;
		if (!PluginJniHelper::getMethodInfo(methodInfo, CALSS_NAME_2, "setResponse", "(I[BI)V")) {
			return;
		}

		jbyteArray jresponse = methodInfo.env->NewByteArray(responseLen);
		methodInfo.env->SetByteArrayRegion(jresponse, 0, responseLen, (jbyte*)response);
		methodInfo.env->CallVoidMethod(netThread, methodInfo.methodID, result, jresponse, responseLen);

		methodInfo.env->DeleteLocalRef(jresponse);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);

		if (response != NULL) {
			delete response;
		}
		LOGD("AoneNetThread nativeSendRecv end");
    }

    JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneNetResponse_nativeNetCallback(JNIEnv* env, jobject thiz, jint result, jbyteArray response, jint responseLen, jint cbNum) {
		LOGD("AoneNetThread nativeNetCallback begin");
		unsigned char* c_response = (unsigned char*)env->GetByteArrayElements(response, NULL);
		    
		unsigned char* c_response2 = (unsigned char*)malloc(responseLen);
		memcpy(c_response2, c_response, responseLen);
		if (cbNum < 0 || cbNum >= uuNetAsync::g_netNumber) {
			LOGD("AoneNetThread nativeNetCallback Error! wrong cb num!");
			return;
		}
		if (uuNetAsync::mSdks[cbNum] != NULL && uuNetAsync::mCbs[cbNum] != NULL) {
        	(uuNetAsync::mSdks[cbNum]->*uuNetAsync::mCbs[cbNum])(result, c_response2, responseLen);
		}

        env->ReleaseByteArrayElements(response, (jbyte*)c_response, 0);
        LOGD("AoneNetThread nativeNetCallback end");
    }

    JNIEXPORT int JNICALL Java_com_aonesoft_lib_AoneHttpThread_nativeSendRecv(JNIEnv* env, jobject thiz, jstring ip, jint port, jstring key, jbyteArray request, jint requestLen, jint cbNum) {
        LOGD("AoneHttpThread nativeSendRecv begin");
        const char *str = env->GetStringUTFChars(ip, NULL);
        string c_ip = str;
        env->ReleaseStringUTFChars(ip, str);
        str = env->GetStringUTFChars(key, NULL);
        string c_key = str;
        env->ReleaseStringUTFChars(ip, str);
        unsigned char* c_request = (unsigned char*)env->GetByteArrayElements(request, NULL);

        int result = aonesdk::AoneSDK::getInstance()->http_send_recv(c_ip, (int)port, c_key, c_request, (int)requestLen, uuNetAsync::mParams);
        uuNetAsync::mParamsMap[cbNum] = uuNetAsync::mParams;
        env->ReleaseByteArrayElements(request, (jbyte*)c_request, 0);
        LOGD("AoneHttpThread nativeSendRecv end");
        return result;
    }

    JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneNetResponse_nativeHttpCallback(JNIEnv* env, jobject thiz, jint result, jint cbNum) {
		LOGD("AoneNetResponse_nativeHttpCallback begin");
		// if (uuNetAsync::mHttpSdk != NULL && uuNetAsync::mHttpCb != NULL) {
  //       	(uuNetAsync::mHttpSdk->*uuNetAsync::mHttpCb)(result, uuNetAsync::mParams);
		// }
		if (cbNum < 0 || cbNum >= uuNetAsync::g_httpNumber) {
			LOGD("AoneNetResponse_nativeHttpCallback Error! wrong cb num!");
			return;
		}
		if (uuNetAsync::mHttpSdks[cbNum] != NULL && uuNetAsync::mHttpCbs[cbNum] != NULL && uuNetAsync::mParamsMap.find(cbNum) != uuNetAsync::mParamsMap.end()) {
        	(uuNetAsync::mHttpSdks[cbNum]->*uuNetAsync::mHttpCbs[cbNum])(result, uuNetAsync::mParamsMap[cbNum]);
		}

        LOGD("AoneNetResponse_nativeHttpCallback end");
    }
}

void uuNetAsync::send_recv(AoneSDK* sdk, NET_CB cb, const string ip, int port, const string key, unsigned char* request, int requestLen)
{
	PluginJniMethodInfo methodInfo;
	if (!PluginJniHelper::getStaticMethodInfo(methodInfo, CALSS_NAME, "sendRecvAsync", "(Ljava/lang/String;ILjava/lang/String;[BII)V")) {
		return;
	}

	mSdk = sdk;
	mCb = cb;

	mSdks.push_back(sdk);
	mCbs.push_back(cb);

	jstring jip = methodInfo.env->NewStringUTF(ip.c_str());
	jstring jkey = methodInfo.env->NewStringUTF(key.c_str());
	jbyteArray jrequest = methodInfo.env->NewByteArray(requestLen);
	methodInfo.env->SetByteArrayRegion(jrequest, 0, requestLen, (jbyte*)request);
	if (request) {
		delete[] request;
	}

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jip, port, jkey, jrequest, requestLen, g_netNumber++);

	methodInfo.env->DeleteLocalRef(jip);
	methodInfo.env->DeleteLocalRef(jrequest);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void uuNetAsync::http_send_recv(AoneSDK* sdk, HTTP_CB cb, const string ip, int port, const string key, const char* request, int requestLen)
{
	PluginJniMethodInfo methodInfo;
	if (!PluginJniHelper::getStaticMethodInfo(methodInfo, CALSS_NAME, "httpSendRecv", "(Ljava/lang/String;ILjava/lang/String;[BII)V")) {
		return;
	}

	mHttpSdk = sdk;
	mHttpCb = cb;

	mHttpSdks.push_back(sdk);
	mHttpCbs.push_back(cb);

	jstring jip = methodInfo.env->NewStringUTF(ip.c_str());
	jstring jkey = methodInfo.env->NewStringUTF(key.c_str());
	jbyteArray jrequest = methodInfo.env->NewByteArray(requestLen);
	methodInfo.env->SetByteArrayRegion(jrequest, 0, requestLen, (jbyte*)request);
	if (request) {
		delete[] request;
	}
	
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jip, port, jkey, jrequest, requestLen, g_httpNumber++);

	methodInfo.env->DeleteLocalRef(jip);
	methodInfo.env->DeleteLocalRef(jrequest);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

} // aonesdk