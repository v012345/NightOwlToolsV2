#include <android/log.h>
#include "uuDevice.h"
#include "PluginJniHelper.h"
#include <string>
#include<stdlib.h>
#include <vector>
#include "aonesdk.h"
using namespace aonesdk;

#define  LOG_TAG    "uuDevice-android"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define CLASS_NAME "com/aonesoft/lib/AoneDevice"
#define PIC_CLASS_NAME "com/aonesoft/lib/AoneQuickRegisterUtils"
#define UPDATE_CLASS_NAME "com/aonesoft/lib/AoneUpdateWithAppVersion"

using namespace std;

static UpdateCallBack m_update_cb;
static AnnounceCallBack m_announce_cb;
static RPEI_CB m_rpei_cb;
std::string getDeviceInfo(const char* methodName) {
	PluginJniMethodInfo methodInfo;
	if (!PluginJniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, methodName, "()Ljava/lang/String;")) {
		return "";
	}
	jobject object = methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	if (object == NULL) {
		LOGD("uuDevice getInfo: null");
		return "";
	}
	jclass class_object = methodInfo.env->FindClass("java/lang/Object");
    jmethodID method_toString = methodInfo.env->GetMethodID(class_object, "toString", "()Ljava/lang/String;");
    jstring jstr = (jstring)methodInfo.env->CallObjectMethod(object, method_toString);
	const char* str = methodInfo.env->GetStringUTFChars(jstr, 0);
    LOGD("uuDevice getInfo: %s", str);
    std::string ret(str);
    methodInfo.env->ReleaseStringUTFChars(jstr, str);
    
	return ret;
}

std::string uuDevice::getDeviceId() {
	return getDeviceInfo("getDeviceId");
}

std::string uuDevice::getCountry() {
	return getDeviceInfo("getCountry");
}

std::string uuDevice::getLanguage() {
	return getDeviceInfo("getLanguage");
}

std::string uuDevice::getModel() {
	return getDeviceInfo("getModel");
}

std::string uuDevice::getOS() {
	return getDeviceInfo("getOS");
}

std::string uuDevice::getIMEI() {
	return getDeviceInfo("getIMEI");
}

std::string uuDevice::getIp() {
	return getDeviceInfo("getIp");
}

std::string uuDevice::getMac() {
	return getDeviceInfo("getMac");
}

std::string uuDevice::getNetworkType() {
	return getDeviceInfo("getNetworkType");
}

std::string uuDevice::getOpertorCode() {
	return getDeviceInfo("getOpertorCode");
}

std::string uuDevice::getIdfa() {
	return getDeviceInfo("getIdfa");
}

std::string callStringFunc(const char *className,const char *methodName, const char *paramCode,...){
	aonesdk::PluginJniMethodInfo methodInfo;
	std::string ret = "";
	if(aonesdk::PluginJniHelper::getStaticMethodInfo(methodInfo, className, methodName, paramCode)){
		jstring strRet = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID,methodInfo.methodID);
		ret = aonesdk::PluginJniHelper::jstring2string(strRet);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
	return ret;
}


void uuDevice::saveQuickRegisterPic(const std::string& aoneAccount, const std::string& aonePassword, const std::string& currentLang)
{
	LOGD("hhh---------account = %s", aoneAccount.c_str());
	LOGD("hhh---------password = %s", aonePassword.c_str());
	LOGD("hhh---------currentLang = %s", currentLang.c_str());

	PluginJniMethodInfo methodInfo;
	if (!PluginJniHelper::getStaticMethodInfo(methodInfo, PIC_CLASS_NAME, "saveStrsToPhotoAlbum", "(Ljava/lang/String;Ljava/lang/String;)V")) return;

	// 构建参数
	jstring account = methodInfo.env->NewStringUTF(aoneAccount.c_str());
	jstring password = methodInfo.env->NewStringUTF(aonePassword.c_str());
	jstring curLang = methodInfo.env->NewStringUTF(currentLang.c_str());
	
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, account, password);

	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	
	if (account) methodInfo.env->DeleteLocalRef(account);
	if (password) methodInfo.env->DeleteLocalRef(password);
	if (curLang) methodInfo.env->DeleteLocalRef(curLang);
}

std::vector<std::string> splitEx(const std::string& src, std::string separate_character)   
{   
    vector<string> strs;   
       
    int separate_characterLen = separate_character.size();//分割字符串的长度,这样就可以支持如“,,”多字符串的分隔符   
    int lastPosition = 0,index = -1;   
    while (-1 != (index = src.find(separate_character,lastPosition)))   
    {   
        strs.push_back(src.substr(lastPosition,index - lastPosition));   
        lastPosition = index + separate_characterLen;   
    }   
    string lastString = src.substr(lastPosition);//截取最后一个分隔符后的内容   
    if (!lastString.empty())   
        strs.push_back(lastString);//如果最后一个分隔符后还有内容就入队   
    return strs;   
}   

void uuDevice::getAppVersion(unsigned char& majorVersion, unsigned char& minorVersion, unsigned char& revisionVersion){
	std::string versionName = callStringFunc(UPDATE_CLASS_NAME,"getAppVersion","()Ljava/lang/String;");
	std::vector<std::string> versionSplits = splitEx(versionName,".");
	
	for ( unsigned int i = 0; i < versionSplits.size(); i++){     
        LOGD("getAppVersion--------- = %d",atoi(versionSplits[i].c_str()));
		if(i == 0){
			majorVersion = atoi(versionSplits[i].c_str());
			continue;
		}
		if(i == 1){
			minorVersion = atoi(versionSplits[i].c_str());
			continue;
		}
		if(i == 2){
			revisionVersion = atoi(versionSplits[i].c_str());
		}
		
    }   
}

void uuDevice::showUpdateDialog(const std::string& updateURL, const std::string& updateText, UPDATE_TYPE updateType, UpdateCallBack cb){
	
	m_update_cb = cb;
	
	aonesdk::PluginJniMethodInfo methodInfo;
	
	if(aonesdk::PluginJniHelper::getStaticMethodInfo(methodInfo, UPDATE_CLASS_NAME, "showUpdateDialog", "(Ljava/lang/String;Ljava/lang/String;I)V")){
		
		// 构建参数
		jstring update_url = methodInfo.env->NewStringUTF(updateURL.c_str());
		jstring update_text = methodInfo.env->NewStringUTF(updateText.c_str());
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID,methodInfo.methodID,update_url,update_text,updateType);
		
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void uuDevice::aoneUpdateCallback(){
	m_update_cb();
}

void uuDevice::showAnnounceDialog(const std::string& announceURL,AnnounceCallBack cb){
	m_announce_cb = cb;
	//callVoidFunc(UPDATE_CLASS_NAME,"showAnnounceDialog","(Ljava/lang/String;)V");
	aonesdk::PluginJniMethodInfo methodInfo;
	
	if(aonesdk::PluginJniHelper::getStaticMethodInfo(methodInfo, UPDATE_CLASS_NAME, "showAnnounceDialog", "(Ljava/lang/String;)V")){
		
		// 构建参数
		jstring announce_url = methodInfo.env->NewStringUTF(announceURL.c_str());
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID,methodInfo.methodID,announce_url);
		
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void uuDevice::aoneAnnounceCallback(){
	m_announce_cb();
}

void uuDevice::openUrl(const std::string& url)
{
	aonesdk::PluginJniMethodInfo methodInfo;
	
	if(aonesdk::PluginJniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "openUrl", "(Ljava/lang/String;)V")){
		jstring open_url = methodInfo.env->NewStringUTF(url.c_str());
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, open_url);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void uuDevice::copyToClipboard(const std::string& text)
{
	aonesdk::PluginJniMethodInfo methodInfo;
	
	if(aonesdk::PluginJniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "copyToClipboard", "(Ljava/lang/String;)V")){
		jstring j_text = methodInfo.env->NewStringUTF(text.c_str());
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, j_text);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void uuDevice::joinQGroup(const std::string& uin,const std::string& key)
{
	aonesdk::PluginJniMethodInfo methodInfo;
	
	if(aonesdk::PluginJniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "joinQGroup", "(Ljava/lang/String;)V")){
		jstring j_key = methodInfo.env->NewStringUTF(key.c_str());
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, j_key);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void uuDevice::starInApp()
{
	aonesdk::PluginJniMethodInfo methodInfo;
	
	if(aonesdk::PluginJniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "starInApp", "()V")){
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void uuDevice::reviewInApp(const std::string& appId)
{
	//
}

void uuDevice::starReviewInApp(const std::string& appId)
{
	
}
int uuDevice::isInstallApp(const std::string& packageName)
{
	aonesdk::PluginJniMethodInfo methodInfo;
	jint j_result = 1;
	if(aonesdk::PluginJniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "isInstallApp", "(Ljava/lang/String;)I")){
		jstring j_packageName = methodInfo.env->NewStringUTF(packageName.c_str());
		j_result = methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID,j_packageName);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
	return j_result;
}

void uuDevice::showIdcardUI()
{
	aonesdk::PluginJniMethodInfo methodInfo;
	if(aonesdk::PluginJniHelper::getStaticMethodInfo(methodInfo, "com/aonesoft/lib/AoneUiUtils", "showIdcardUI", "()V")){
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void uuDevice::requestPlayerExtraInfo(RPEI_CB cb)
{
	m_rpei_cb = cb;
	string className = AoneSDK::getInstance()->callStringFunc("getClassName");
	aonesdk::PluginJniMethodInfo methodInfo;
	if(aonesdk::PluginJniHelper::getStaticMethodInfo(methodInfo, className.c_str(), "requestPlayerExtraInfo", "()V")){
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void uuDevice::requestPlayerExtraInfoCallback(int code, std::map < std::string, std::string> msg){
	m_rpei_cb(code,msg);
}


