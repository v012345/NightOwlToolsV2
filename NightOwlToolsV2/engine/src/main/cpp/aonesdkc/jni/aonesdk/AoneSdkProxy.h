#ifndef __AONESDK_PROXY_HH__
#define __AONESDK_PROXY_HH__

#include <jni.h>
#include <android/log.h>
#include <map>
#include <string>
/*
 * AoneClient_Java.h
 */
#include "../AoneClient_Java.h"

class AoneSdkProxy
{
public:
	static void setJavaVM(JavaVM *javaVM);
	static void init(JNIEnv* env, jobject thiz, jobject listener, std::string methodName);
	static void login(JNIEnv* env, jobject thiz, jobject listener, std::string methodName);
	static void pay(JNIEnv* env, jobject thiz, jobject hashTable, jobject listener, std::string methodName);
	static void pay(JNIEnv* env, jobject thiz, jstring productId,jint purchaseNum, jstring cp_ext, jobject listener, std::string methodName);

	static void setOAuthType(JNIEnv* env, jobject thiz, jstring type);
	static void setPayChannel(JNIEnv* env, jobject thiz, jstring payChannel);

	static void initCallback(int type, std::map<std::string, std::string> dataMap);
	static void loginCallback(int type, std::map<std::string, std::string> dataMap);
	static void payCallback(int type, std::map<std::string, std::string> dataMap);

	// static void initForNative(JNIEnv* env, jobject thiz, jobject listener);
	// static void loginForNative(JNIEnv* env, jobject thiz, jobject listener);
	// static void payForNative(JNIEnv* env, jobject thiz, jobject hashTable, jobject listener);

	static void callback(int type, std::map<std::string, std::string> dataMap);

	/*
	 * AoneClient Method Field Begin
	 *
	 */
	static void NewListObjAndGetAddMethod(JNIEnv* env, jobject* obj, jmethodID* methodId);
	static void GetGameClassAndContructMethod(JNIEnv* env, jclass* gameClass, jmethodID* methodId, const char* className, const char* signs);
	static void aoneCallback(int type, std::map<std::string, std::string> dataMap);
	static void aoneCallJavaListener(JNIEnv* env, jobject listener, std::string methodName, int type, std::map<std::string, std::string> dataMap);
	static jobject aoneCreateJavaMapObject(JNIEnv* env, std::map<std::string, std::string>* paramMap);

	static void aoneInitCallback(int type, std::map<std::string, std::string> dataMap);
	static void aoneLoginCallback(int type, std::map<std::string, std::string> dataMap);
	static void aoneOauthLoginCallback(int type, std::map<std::string, std::string> dataMap);
	static void aonePayCallback(int type, std::map<std::string, std::string> dataMap);
	static void aoneCheckVersionCallback(int type, std::map<std::string, std::string> dataMap);
	static void aoneSelectRoleAndGroupListenerCallback(int type, std::map<std::string, std::string> dataMap);
	static void aoneQueryPaynoStatusCallback(int type, std::map<std::string, std::string> dataMap);

	static void aoneInit(JNIEnv* env, jobject thiz, jobject listener, std::string methodName);
	static jobject aoneAllProducts(JNIEnv* env, jobject thiz);
	static jobject aoneAllLangs(JNIEnv* env, jobject thiz);
	static jint aoneSelectLang(JNIEnv* env, jobject thiz, jstring lang);
	static void aoneSetOAuthType(JNIEnv* env, jobject thiz, jstring lang);
	static jobject aoneGetGameVersion(JNIEnv* env, jobject thiz);

	static void aoneLogin(JNIEnv* env, jobject thiz, jobject listener, std::string methodName);
	static void aoneOauthLogin(JNIEnv* env, jobject thiz, jobject listener, std::string methodName);
	static jint aoneUserID(JNIEnv* env, jobject thiz);
	static jint aoneUserToken(JNIEnv* env, jobject thiz);
	static jstring aoneToken(JNIEnv* env, jobject thiz);
	static jobject aoneAllGroups(JNIEnv* env, jobject thiz);
	static jobject aoneAllRoles(JNIEnv* env, jobject thiz);

	static jstring aoneAoneAccount(JNIEnv* env, jobject thiz);
	static jobject aoneGetSelectedRole(JNIEnv* env, jobject thiz);
	static jobject aoneGetSelectedGroup(JNIEnv* env, jobject thiz);
	static void aonePay(JNIEnv* env, jobject thiz, jstring productId,jint purchaseNum, jstring cp_ext, jobject listener, std::string methodName);

	static void aoneCheckVersion(JNIEnv* env, jobject thiz, jint maj, jint min, jint rev, jobject listener, std::string methodName);
	static void aoneCreateRole(JNIEnv* env, jobject thiz, jstring name, jint type, jobject features, jint gId, jobject listener, std::string methodName);
	static jint aoneCreateRoleCP(JNIEnv* env, jobject thiz, jstring roleID, jstring name, jint level, jobject features, jstring groupID, jstring groupName, jint roleVip);
	static void aoneSubmitRoleData(JNIEnv* env, jobject thiz, jstring roleID, jstring name, jint level, jstring groupID, jstring groupName, jint roleVip, jboolean isLogin, jstring action,jlong roleCTime);
	static void aoneSelectRoleAndGroup(JNIEnv* env, jobject thiz, jint roleId, jint gId, jobject listener, std::string methodName);

	static void aonePullGameGroup(JNIEnv* env, jobject thiz, jobject listener, std::string methodName);
	static void aoneLogout(JNIEnv* env, jobject thiz, jobject listener, std::string methodName);

	static void aoneShareCallback(int type, std::map<std::string, std::string> dataMap);
	static void aoneShare(JNIEnv *env, jobject thiz, jobject cpInfo, jobject listener, std::string methodName);

	static void aoneGetAdTrackLinkCallback(int type, std::map<std::string, std::string> dataMap);
	static void aoneGetAdTrackLink(JNIEnv *env, jobject thiz, jstring userData, jobject listener, std::string methodName);

	static jstring aoneGetAppLink(JNIEnv *env, jobject thiz);

	static void aoneGetAdSourceCallback(int type, std::map<std::string, std::string> dataMap);
	static void aoneGetAdSource(JNIEnv *env, jobject thiz, jstring userData,  jobject listener, std::string methodName);
    static jstring aoneEmbedQR(JNIEnv *env, jobject thiz, jstring src, jstring info, int x, int y, int w, int h);
    static void aoneImagePicker(int code);
    static void aoneUpdateCallback();
    static void aoneAnnounceCallback();

    static void aoneLogEvent(JNIEnv *env, jobject thiz, jstring eventId);
    static void aoneLogEvent(JNIEnv *env, jobject thiz, jstring eventId, jobject cpInfo);
    static void aoneQueryPaynoStatus(JNIEnv *env, jobject thiz, jint appId,jstring payno, jobject listener, std::string methodName);
    static void recordDevicePoint(JNIEnv *env, jobject thiz, jint sdkPoint);
    static void recordRolePoint(JNIEnv *env, jobject thiz, jint gamePoint);
    static void aoneAddNotify(JNIEnv *env, jobject thiz, jobject infos);
    static void aoneClearNotify(JNIEnv *env, jobject thiz, jint notifyId);
    static void aoneClearAllNotify(JNIEnv *env, jobject thiz);
    static jstring aoneGetChannelParam(JNIEnv *env, jobject thiz,jstring paramName);
    static void aoneJoinQGroup(JNIEnv *env, jobject thiz);
    static void aoneCallFunc(JNIEnv *env, jobject thiz,jstring funcName);
    static int aoneCallIntFunc(JNIEnv *env, jobject thiz,jstring funcName);
    static jstring aoneCallStringFunc(JNIEnv *env, jobject thiz,jstring funcName);
    static void aoneShowAds(JNIEnv* env, jobject thiz,jobject infos,jint pos, jobject listener, std::string methodName);
    static void aoneShowAdsCallback(int type, std::map<std::string, std::string> dataMap);
    static void aoneSetAdsSdk(JNIEnv *env, jobject thiz,jstring adsSdkName);
    static void aoneQuickRegisterAsync(JNIEnv *env, jobject thiz,jobject listener,std::string methodName);
    static void aoneQuickRegisterCallback(int type, std::map<std::string, std::string> dataMap);

    static void aoneBindOtherAccount(JNIEnv *env, jobject thiz,std::string newAccount,jobject listener,std::string methodName);
    static void aoneBindOtherAccountCallback(int type, std::map<std::string, std::string> dataMap);

    static void aoneUnbindOtherAccount(JNIEnv *env, jobject thiz,jobject listener,std::string methodName);
    static void aoneUnbindOtherAccountCallback(int type, std::map<std::string, std::string> dataMap);

    static jint aoneQueryBindOtherAccountSync(JNIEnv *env, jobject thiz);
    static void aoneQueryBindOtherAccount(JNIEnv *env, jobject thiz,jobject listener,std::string methodName);
    static void aoneQueryBindOtherAccountCallback(int type, std::map<std::string, std::string> dataMap);

    static void aoneChangePassword(JNIEnv *env, jobject thiz,std::string oldPassword,std::string newPassword,jobject listener,std::string methodName);
    static void aoneChangePasswordCallback(int type, std::map<std::string, std::string> dataMap);

    static void aoneThirdPartyLogin(JNIEnv *env, jobject thiz,jobject listener,std::string methodName);
    static void aoneThirdPartyLoginCallback(int type, std::map<std::string, std::string> dataMap);

    static void aoneCreateInheritance(JNIEnv *env, jobject thiz,std::string password,jobject listener,std::string methodName);
    static void aoneCreateInheritanceCallback(int type, std::map<std::string, std::string> dataMap);
    static void aoneLoginWithAccount(JNIEnv *env, jobject thiz,std::string account,std::string password,jobject listener,std::string methodName);
    static void aoneLoginWithAccountCallback(int type, std::map<std::string, std::string> dataMap);

    static void aoneCleanRoleData(JNIEnv *env, jobject thiz);

    static void aoneStarReviewInApp(JNIEnv *env, jobject thiz);
    static void realnameInfo(JNIEnv *env, jobject thiz,jobject listener,std::string methodName);
    static void realnameInfoCallback(int type, std::map<std::string, std::string> dataMap);

    static void realnameCertificate(JNIEnv *env, jobject thiz,jstring identity_number, jstring real_name,jobject listener,std::string methodName);
    static void realnameCertificateCallback(int type, std::map<std::string, std::string> dataMap);

    static void requestPlayerExtraInfo(JNIEnv *env, jobject thiz,jobject listener,std::string methodName);
    static void requestPlayerExtraInfoCallback(int type, std::map<std::string, std::string> dataMap);

	/*
	 * AoneClient Method Field End
	 */
private:
	
	static JNIEnv* s_env;
	static jobject s_initListener;
	static jobject s_loginListener;
	static jobject s_oauthLoginListener;
	static jobject s_payListener;
	static jobject s_checkVersionListener;
	static jobject s_selectRoleAndGroupListener;
	static jobject s_listener;
	static jobject s_queryPaynoStatusListener;
	static jobject s_quickRegisterListener;
	static jobject s_adsListener;
	static jobject s_shareListener;
	static jobject s_bindOtherAccountListener;
	static jobject s_unbindOtherAccountListener;
	static jobject s_queryBindOtherAccountListener;
	static jobject s_changePasswordListener;
	static jobject s_thirdPartyLoginListener;
	static jobject s_creatreInheritanceListener;
	static jobject s_aoneLoginListener;
	static jobject s_realnameInfoListener;
	static jobject s_realnameCertificateListener;
	static jobject s_requestPlayerExtraInfoListener;
	static std::string s_methodName;
	static jobject s_context;
};


#endif // __AONESDK_PROXY_HH__
