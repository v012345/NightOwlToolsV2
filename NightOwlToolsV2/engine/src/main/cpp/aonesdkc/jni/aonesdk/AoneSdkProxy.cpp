#include <jni.h>
#include <android/log.h>
#include "AoneSdkProxy.h"
#include "PluginJniHelper.h"
// #include "uuSdkDef.h"
#include "aonesdk.h"
#include "AoneClient.h"
#include "uuSdkManager.h"
#include "AoneAndroidImagePicker.h"
#include "uuDevice.h"

#define  LOG_TAG    "AoneSdkProxy"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

using namespace std;
using namespace aonesdk;

extern "C"
{
// jint JNI_OnLoad(JavaVM *vm, void *reserved)
// {
// 	LOGD("AoneSdkProxy JNI_OnLoad !!!");
//     // PluginJniHelper::setJavaVM(vm);

//     AoneSdkProxy::setJavaVM(vm);

//     return JNI_VERSION_1_4;
// }
static JavaVM* s_jvm = NULL;

std::map<std::string, std::string> hashtable2Map(JNIEnv * env,jobject cpInfo){
		jclass class_hashTable = env->GetObjectClass(cpInfo);
		jmethodID method_hashTable_get = env->GetMethodID(class_hashTable, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
		jmethodID method_keys = env->GetMethodID(class_hashTable, "keys", "()Ljava/util/Enumeration;");
		jclass class_object = env->FindClass("java/lang/Object");
		jmethodID method_toString = env->GetMethodID(class_object, "toString", "()Ljava/lang/String;");
		jclass class_enumeration = env->FindClass("java/util/Enumeration");
		jmethodID method_hasMoreElements = env->GetMethodID(class_enumeration, "hasMoreElements", "()Z");
		jmethodID method_nextElement = env->GetMethodID(class_enumeration, "nextElement", "()Ljava/lang/Object;");
		jobject jkey, jvalue;
		jstring jkeyStr, jvalueStr;
		jstring jkeyStrParam;
		jstring jvalueStrValue;
		const char *key = NULL;
		const char *value = NULL;
		std::map<std::string, std::string> info;
		jobject keys = env->CallObjectMethod(cpInfo, method_keys);
		while (env->CallBooleanMethod(keys, method_hasMoreElements)) {
			jkey = env->CallObjectMethod(keys, method_nextElement);
			jkeyStr = (jstring)env->CallObjectMethod(jkey, method_toString);
			jkeyStrParam = jkeyStr;
			key = env->GetStringUTFChars(jkeyStr, NULL);

			jvalue = env->CallObjectMethod(cpInfo, method_hashTable_get, jkeyStr);
			jvalueStr = (jstring)env->CallObjectMethod(jvalue, method_toString);
			jvalueStrValue = jvalueStr;
			value = env->GetStringUTFChars(jvalueStr, NULL);
			key = env->GetStringUTFChars(jkeyStrParam, NULL);
			value = env->GetStringUTFChars(jvalueStrValue, NULL);
			info[key] = value;
			env->ReleaseStringUTFChars(jvalueStr, value);
			env->ReleaseStringUTFChars(jkeyStr, key);
			env->DeleteLocalRef(jvalueStr);
			env->DeleteLocalRef(jkeyStr);
			env->DeleteLocalRef(jvalue);
			env->DeleteLocalRef(jkey);
		}
		env->DeleteLocalRef(keys);
		env->DeleteLocalRef(class_enumeration);
		env->DeleteLocalRef(class_object);
		env->DeleteLocalRef(class_hashTable);
		return info;
}

void set_listener(jobject listener, jobject* to, JNIEnv* env) {
	if(to != NULL && *to != NULL){
		env->DeleteGlobalRef(*to);
	}

	if(to != NULL){
		*to =  env->NewGlobalRef(listener);
	}
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneJni_nativeSetJavaVM(JNIEnv*  env, jobject thiz) {
	JavaVM *jvm;
	int status = env->GetJavaVM(&jvm);
	if(status != 0) {
		LOGD("get JavaVM failed!!!");
	}
	AoneSdkProxy::setJavaVM(jvm);
	s_jvm = jvm;
}

char* convertJByteaArrayToChars(JNIEnv *env, jbyteArray bytearray){
	char *chars = NULL;
	jbyte *bytes;
	bytes = env->GetByteArrayElements(bytearray, 0);
	int chars_len = env->GetArrayLength(bytearray);
	chars = new char[chars_len + 1];
	memset(chars,0,chars_len + 1);
	memcpy(chars, bytes, chars_len);
	chars[chars_len] = 0;

	env->ReleaseByteArrayElements(bytearray, bytes, 0);
	return chars;
}

//字节数组转换为HEX 字符串
const char* Byte2HexString(const char* input, const int datasize){
    char  output[datasize*2];
    for(int j = 0; j < datasize; j++ )
    {
        unsigned char b = *(input+j);
        snprintf( output+j * 2,3, "%02x",b);
    }
    return string(output).c_str() ;
}

void setLauncherActivity(JNIEnv*  env,jclass context_class,jobject context_obj,jstring packageName){
	jclass intent_class = env->FindClass("android/content/Intent");
	jmethodID intent_m_id = env->GetMethodID(intent_class,"<init>","(Ljava/lang/String;)V");
	jobject intent_obj = env->NewObject(intent_class,intent_m_id,env->NewStringUTF("android.intent.action.MAIN"));
	//
    //intent = intent.addCategory("android.intent.category.LAUNCHER");
	jmethodID addCategory_m_id = env->GetMethodID(intent_class,"addCategory","(Ljava/lang/String;)Landroid/content/Intent;");
	env->CallObjectMethod(intent_obj,addCategory_m_id,env->NewStringUTF("android.intent.category.LAUNCHER"));

	//PackageManager packageManager = activity.getPackageManager();
	jmethodID getPackageManager_m_id = env->GetMethodID(context_class,"getPackageManager","()Landroid/content/pm/PackageManager;");
	jobject packageManager_obj = env->CallObjectMethod(context_obj,getPackageManager_m_id);

	//List<ResolveInfo> appList =  packageManager.queryIntentActivities(intent, 0);
	jclass packageManager_class = env->GetObjectClass(packageManager_obj);
	jmethodID queryIntentActivities_m_id = env->GetMethodID(packageManager_class,"queryIntentActivities","(Landroid/content/Intent;I)Ljava/util/List;");
	jobject list_obj = env->CallObjectMethod(packageManager_obj,queryIntentActivities_m_id,intent_obj,64);

	//Iterator<ResolveInfo> it = appList.iterator();
	jclass list_class = env->GetObjectClass(list_obj);
	jmethodID iterator_m_id = env->GetMethodID(list_class,"iterator","()Ljava/util/Iterator;");
	jobject it_obj = env->CallObjectMethod(list_obj,iterator_m_id);
	// while(it.hasNext()){
	jclass it_class = env->GetObjectClass(it_obj);
	jmethodID hasNext_m_id = env->GetMethodID(it_class,"hasNext","()Z");
	while(env->CallBooleanMethod(it_obj,hasNext_m_id)){
		//ResolveInfo resolveInfo = it.next();
		jmethodID next_m_id = env->GetMethodID(it_class,"next","()Ljava/lang/Object;");
		jobject resolveInfo_obj = env->CallObjectMethod(it_obj,next_m_id);

		//ActivityInfo activityInfo = resolveInfo.activityInfo;
		jclass resolveInfo_class = env->GetObjectClass(resolveInfo_obj);
		jfieldID activityInfo_f_id = env->GetFieldID(resolveInfo_class,"activityInfo","Landroid/content/pm/ActivityInfo;");
		jobject activityInfo_obj = env->GetObjectField(resolveInfo_obj,activityInfo_f_id);
		//String packageStr = activityInfo.packageName;
		jclass activityInfo_class = env->GetObjectClass(activityInfo_obj);
		jfieldID packageName_f_id = env->GetFieldID(activityInfo_class,"packageName","Ljava/lang/String;");
		jobject packageName_obj = env->GetObjectField(activityInfo_obj,packageName_f_id);
		//if (packageStr.equals(packageName)) {
		jclass string_class = env->GetObjectClass(packageName_obj);
		jmethodID equals_m_id = env->GetMethodID(string_class,"equals","(Ljava/lang/Object;)Z");
		if(env->CallBooleanMethod(packageName_obj,equals_m_id,packageName)){
			jfieldID name_f_id = env->GetFieldID(activityInfo_class,"name","Ljava/lang/String;");
			jstring name_str = (jstring)env->GetObjectField(activityInfo_obj,name_f_id);
			const char* name = env->GetStringUTFChars(name_str, NULL);
			aonesdk::AoneSDK::getInstance()->setClassName(name);
			env->ReleaseStringUTFChars(name_str, name);//这行不注释，使用的时候得到是乱码
			env->DeleteLocalRef(name_str);
			env->DeleteLocalRef(string_class);
			env->DeleteLocalRef(packageName_obj);
			env->DeleteLocalRef(activityInfo_class);
			env->DeleteLocalRef(activityInfo_obj);
			env->DeleteLocalRef(resolveInfo_class);
			env->DeleteLocalRef(resolveInfo_obj);

			break;
		}

	}

	env->DeleteLocalRef(it_class);
	env->DeleteLocalRef(it_obj);
	env->DeleteLocalRef(list_class);
	env->DeleteLocalRef(list_obj);
	env->DeleteLocalRef(packageManager_class);
	env->DeleteLocalRef(packageManager_obj);
	env->DeleteLocalRef(intent_obj);
	env->DeleteLocalRef(intent_class);
}

void setAppName(JNIEnv*  env,jobject obj,jclass clazz,jclass context_class,jobject context_obj,jstring packageName ){//Open Declaration android.content.pm.ApplicationInfo
	setLauncherActivity(env,context_class,context_obj,packageName);
	jfieldID id_member = env->GetFieldID(clazz, "applicationInfo", "Landroid/content/pm/ApplicationInfo;");
	jobject applicationInfo_obj = env->GetObjectField(obj, id_member);
	jclass applicationInfo_clazz = env->GetObjectClass(applicationInfo_obj);
	jfieldID id_member2 = env->GetFieldID(applicationInfo_clazz, "labelRes", "I");
	jint labRes = env->GetIntField(applicationInfo_obj, id_member2);
//android.content.res.Resources
	jmethodID resources_m_id = env->GetMethodID(context_class, "getResources", "()Landroid/content/res/Resources;");
	jobject resources_obj = env->CallObjectMethod(context_obj, resources_m_id);

	jclass resources_clazz = env->GetObjectClass(resources_obj);

	jmethodID lable_m_id = env->GetMethodID(resources_clazz, "getString", "(I)Ljava/lang/String;");
	jstring label_string = (jstring)env->CallObjectMethod(resources_obj, lable_m_id,labRes);
	const char* app_name = env->GetStringUTFChars(label_string, NULL);
	aonesdk::AoneSDK::getInstance()->setAppName(app_name);
	env->DeleteLocalRef(applicationInfo_obj);
	env->DeleteLocalRef(applicationInfo_clazz);
	env->DeleteLocalRef(resources_obj);
	env->DeleteLocalRef(resources_clazz);

	env->ReleaseStringUTFChars(label_string, app_name);
	env->DeleteLocalRef(label_string);
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneJni_nativeSetContext(JNIEnv*  env, jobject thiz,jobject contextObject) {
	jclass native_class = env->GetObjectClass(contextObject);
	jmethodID pm_id = env->GetMethodID(native_class, "getPackageManager", "()Landroid/content/pm/PackageManager;");
	jobject pm_obj = env->CallObjectMethod(contextObject, pm_id);
	jclass pm_clazz = env->GetObjectClass(pm_obj);
	// 得到 getPackageInfo 方法的 ID
	jmethodID package_info_id = env->GetMethodID(pm_clazz, "getPackageInfo","(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
	jmethodID mId = env->GetMethodID(native_class, "getPackageName", "()Ljava/lang/String;");
	jstring pkg_str = static_cast<jstring>(env->CallObjectMethod(contextObject, mId));
	const char* packageName = env->GetStringUTFChars(pkg_str, NULL);
	aonesdk::AoneSDK::getInstance()->setPackageName(packageName);
	// 获得应用包的信息
	jobject pi_obj = env->CallObjectMethod(pm_obj, package_info_id, pkg_str, 64);
	// 获得 PackageInfo 类
	jclass pi_clazz = env->GetObjectClass(pi_obj);
	setAppName(env,pi_obj,pi_clazz,native_class,contextObject,pkg_str);
	// 获得签名数组属性的 ID
	jfieldID signatures_fieldId = env->GetFieldID(pi_clazz, "signatures", "[Landroid/content/pm/Signature;");
	jobject signatures_obj = env->GetObjectField(pi_obj, signatures_fieldId);
	jobjectArray signaturesArray = (jobjectArray)signatures_obj;
	jsize size = env->GetArrayLength(signaturesArray);
	jobject signature_obj = env->GetObjectArrayElement(signaturesArray, 0);
	jclass signature_clazz = env->GetObjectClass(signature_obj);
	jmethodID tem_method = env->GetMethodID(signature_clazz, "toByteArray", "()[B");

    jobject obj_sign_byte_array = env->CallObjectMethod(signature_obj, tem_method);// 这个就是拿到的签名byte数组

    //MessageDigest localMessageDigest = MessageDigest.getInstance("MD5");
    jclass class_MessageDigest = env->FindClass("java/security/MessageDigest");
    tem_method = env->GetStaticMethodID(class_MessageDigest, "getInstance", "(Ljava/lang/String;)Ljava/security/MessageDigest;");
    jobject obj_md5 = env->CallStaticObjectMethod(class_MessageDigest, tem_method, env->NewStringUTF("md5"));
    //localMessageDigest.update(localSignature.toByteArray());
    //tem_class = (*env)->GetObjectClass(env, obj_md5);
    tem_method = env->GetMethodID(class_MessageDigest, "update", "([B)V");// 这个函数的返回值是void，写V

    env->CallVoidMethod(obj_md5, tem_method, obj_sign_byte_array);
    // localMessageDigest.digest()
    tem_method = env->GetMethodID(class_MessageDigest, "digest", "()[B");
    // 这个是md5以后的byte数组，如今仅仅要将它转换成16进制字符串。就能够和之前的比較了
	jbyteArray obj_array_sign = (jbyteArray)env->CallObjectMethod(obj_md5, tem_method);
	jsize int_array_length = env->GetArrayLength(obj_array_sign);

	char* data_char = convertJByteaArrayToChars(env,obj_array_sign);
	const char* data = Byte2HexString(data_char, int_array_length);
	aonesdk::AoneSDK::getInstance()->setVerifyKey(data);
	
	env->ReleaseStringUTFChars(pkg_str, packageName);
	env->DeleteLocalRef(pkg_str);
	env->DeleteLocalRef(obj_array_sign);
	//env->ReleaseStringUTFChars(pkg_str, data_char);
	
	//--
	env->DeleteLocalRef(obj_md5);
	env->DeleteLocalRef(class_MessageDigest);
	env->DeleteLocalRef(obj_sign_byte_array);
	env->DeleteLocalRef(signature_clazz);
	env->DeleteLocalRef(signature_obj);
	env->DeleteLocalRef(signaturesArray);
	//env->DeleteLocalRef(signatures_obj);//signaturesArray是由signatures_obj强转而来
	//--
	env->DeleteLocalRef(pi_clazz);
	env->DeleteLocalRef(pi_obj);
	//--
	env->DeleteLocalRef(pm_clazz);
	env->DeleteLocalRef(pm_obj);
	env->DeleteLocalRef(native_class);
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneJni_nativeLoadSdk(JNIEnv*  env, jobject thiz) {
	uuSdkManager::getInstance()->loadAllSdks();
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneJni_nativeInit(JNIEnv*  env, jobject thiz, jobject listener)
{
	LOGD("aonejni native init !!!");
	AoneSdkProxy::init(env, thiz, listener, "onResult");
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneJni_nativeLogin(JNIEnv*  env, jobject thiz, jobject listener)
{
	LOGD("aonejni native login !!!");
	AoneSdkProxy::login(env, thiz, listener, "onResult");
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneJni_nativeSetOAuthType(JNIEnv*  env, jobject thiz, jstring type)
{
	LOGD("aonejni native setOAuthType !!!");
	AoneSdkProxy::setOAuthType(env, thiz, type);
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneJni_nativeSetPayChannel(JNIEnv*  env, jobject thiz, jstring payChannel)
{
	LOGD("aonejni native setPayChannel !!!");
	AoneSdkProxy::setPayChannel(env, thiz, payChannel);
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneJni_nativePay(JNIEnv* env, jobject thiz, jobject info, jobject listener)
{
	LOGD("aonejni native pay !!!");
	AoneSdkProxy::pay(env, thiz, info, listener, "onResult");
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneJni_nativeInitForNative(JNIEnv*  env, jobject thiz, jobject listener)
{
	LOGD("aonejni native init !!!");
	AoneSdkProxy::init(env, thiz, listener, "initCallback");
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneJni_nativeLoginForNative(JNIEnv*  env, jobject thiz, jobject listener)
{
	LOGD("aonejni native login !!!");
	AoneSdkProxy::login(env, thiz, listener, "loginCallback");
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneJni_nativePayForNative(JNIEnv* env, jobject thiz, jobject info, jobject listener)
{
	LOGD("aonejni native pay !!!");
	AoneSdkProxy::pay(env, thiz, info, listener, "payCallback");
}

/***********************
 * AoneClient_Java  Begin
 ***********************/

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeHasExitUI
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_aonesoft_lib_AoneClient_nativeHasExitUI
(JNIEnv* env, jobject thiz)
{
	LOGD("aonejni native has exit UI !!!");
	return AoneClient::hasExitUI();
}

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeExit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeExit
(JNIEnv* env, jobject thiz)
{
	LOGD("aonejni native exit !!!");
	AoneClient::Exit();
}

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeInitSDK
 * Signature: (Lcom/aonesoft/lib/AoneResultListener;)V
 */
JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeInitSDK
(JNIEnv* env, jobject thiz, jobject listener)
{
	LOGD("aonejni native init !!!");
	AoneSdkProxy::aoneInit(env, thiz, listener, "onResult");
}

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeAoneConfigData
 */
JNIEXPORT jstring JNICALL Java_com_aonesoft_lib_AoneClient_nativeAoneConfigData(JNIEnv*  env, jobject thiz, jstring key)
{
	LOGD("aonejni native aoneConfigData !!!");
	const char* aoneKey = env->GetStringUTFChars(key, NULL);

	jstring aoneValue = env->NewStringUTF(AoneClient::GetAoneConfigData(aoneKey).c_str());

	env->ReleaseStringUTFChars(key, aoneKey);

	return aoneValue;
}

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeAllProducts
 * Signature: ()Ljava/util/ArrayList;
 */
JNIEXPORT jobject JNICALL Java_com_aonesoft_lib_AoneClient_nativeAllProducts
(JNIEnv* env, jobject thiz)
{
	LOGD("aonejni native AllProducts !!!");
	return AoneSdkProxy::aoneAllProducts(env, thiz);
}

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeAllLangs
 * Signature: ()Ljava/util/Hashtable;
 */
JNIEXPORT jobject JNICALL Java_com_aonesoft_lib_AoneClient_nativeAllLangs
(JNIEnv* env, jobject thiz)
{
	LOGD("aonejni native AllLangs !!!");
	return AoneSdkProxy::aoneAllLangs(env, thiz);
}

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeSelectLang
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_aonesoft_lib_AoneClient_nativeSelectLang
(JNIEnv* env, jobject thiz, jstring lang)
{
	LOGD("aonejni native SelectLang !!!");
	return AoneSdkProxy::aoneSelectLang(env, thiz, lang);
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeSetOAuthType
(JNIEnv* env, jobject thiz, jstring type)
{
	LOGD("aonejni native setOAuthType !!!");
	AoneSdkProxy::aoneSetOAuthType(env, thiz, type);
}

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeCheckVersion
 * Signature: (LCCCLcom/aonesoft/lib/AoneResultListener;)V
 */
JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeCheckVersion
(JNIEnv* env, jobject thiz, jint maj, jint min, jint rev, jobject listener)
{
	LOGD("aonejni native CheckVersion !!!");
	AoneSdkProxy::aoneCheckVersion(env, thiz, maj, min, rev, listener, "onResult");
}

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeGetGameVersion
 * Signature: ()Lcom/aonesoft/lib/AoneClient/GameVersion;
 */
JNIEXPORT jobject JNICALL Java_com_aonesoft_lib_AoneClient_nativeGetGameVersion
(JNIEnv* env, jobject thiz)
{
	LOGD("aonejni native GetGameVersion !!!");
	return AoneSdkProxy::aoneGetGameVersion(env, thiz);
}
/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeLogin
 * Signature: (Lcom/aonesoft/lib/AoneResultListener;)V
 */
JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeLogin
(JNIEnv* env, jobject thiz, jobject listener)
{
	LOGD("aonejni native login !!!");
	aonesdk::AoneSDK::getInstance()->setUseUserLogin(false);
	AoneSdkProxy::aoneLogin(env, thiz, listener, "onResult");
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeOauthLogin
(JNIEnv* env, jobject thiz, jobject listener)
{
	LOGD("aonejni native oauth login !!!");
	aonesdk::AoneSDK::getInstance()->setUseUserLogin(true);
	AoneSdkProxy::aoneOauthLogin(env, thiz, listener, "onResult");
}

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeUserID
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_aonesoft_lib_AoneClient_nativeUserID
(JNIEnv* env, jobject thiz)
{
	LOGD("aonejni native UserID !!!");
	return AoneSdkProxy::aoneUserID(env, thiz);
}

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeUserToken
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jint JNICALL Java_com_aonesoft_lib_AoneClient_nativeUserToken
(JNIEnv* env, jobject thiz)
{
	LOGD("aonejni native UserToken !!!");
	return AoneSdkProxy::aoneUserToken(env, thiz);
}

JNIEXPORT jstring JNICALL Java_com_aonesoft_lib_AoneClient_nativeToken
(JNIEnv* env, jobject thiz)
{
	LOGD("aonejni native Token !!!");
	return AoneSdkProxy::aoneToken(env, thiz);
}

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeAllGroups
 * Signature: ()Ljava/util/ArrayList;
 */
JNIEXPORT jobject JNICALL Java_com_aonesoft_lib_AoneClient_nativeAllGroups
(JNIEnv* env, jobject thiz)
{
	LOGD("aonejni native AllGroups !!!");
	return AoneSdkProxy::aoneAllGroups(env, thiz);
}

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeAllRoles
 * Signature: ()Ljava/util/ArrayList;
 */
JNIEXPORT jobject JNICALL Java_com_aonesoft_lib_AoneClient_nativeAllRoles
(JNIEnv* env, jobject thiz)
{
	LOGD("aonejni native AllRoles !!!");
	return AoneSdkProxy::aoneAllRoles(env, thiz);
}

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeAoneAccount
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_aonesoft_lib_AoneClient_nativeAoneAccount
(JNIEnv* env, jobject thiz)
{
	LOGD("aonejni native AoneAccount !!!");
	return AoneSdkProxy::aoneAoneAccount(env, thiz);
}

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeCreateRole
 * Signature: (Ljava/lang/String;ILjava/util/Hashtable;ILcom/aonesoft/lib/AoneResultListener;)V
 */
JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeCreateRole
(JNIEnv* env, jobject thiz, jstring name, jint type, jobject features, jint gId, jobject listener)
{
	LOGD("aonejni native CreateRole !!!");
	AoneSdkProxy::aoneCreateRole(env, thiz, name, type, features, gId, listener, "onResult");
}

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeCreateRoleCP
 * Signature: (Ljava/lang/String;Ljava/lang/String;ILjava/util/Hashtable;Ljava/lang/String;Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_com_aonesoft_lib_AoneClient_nativeCreateRoleCP
(JNIEnv* env, jobject thiz, jstring roleID, jstring name, jint level, jobject features, jstring groupID, jstring groupName, jint roleVip)
{
	LOGD("aonejni native CreateRole !!!");
	return AoneSdkProxy::aoneCreateRoleCP(env, thiz, roleID, name, level, features, groupID, groupName, roleVip);
}

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeSubmitRoleData
 * Signature:  (Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;I)I
 */
JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeSubmitRoleData
(JNIEnv* env, jobject thiz, jstring roleID, jstring name, jint level, jstring groupID, jstring groupName, jint roleVip, jboolean isLogin, jstring action,jlong roleCTime)
{
	LOGD("aonejni native SubmitRoleData !!!");
	AoneSdkProxy::aoneSubmitRoleData(env, thiz, roleID, name, level, groupID, groupName, roleVip, isLogin, action,roleCTime);
}

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeSelectRoleAndGroup
 * Signature: (JILcom/aonesoft/lib/AoneResultListener;)V
 */
JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeSelectRoleAndGroup
(JNIEnv* env, jobject thiz, jint roleId, jint gId, jobject listener)
{
	LOGD("aonejni native SelectRoleAndGroup !!!");
	AoneSdkProxy::aoneSelectRoleAndGroup(env, thiz, roleId, gId, listener, "onResult");
}

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativePullGameGroup
 * Signature: (Lcom/aonesoft/lib/AoneResultListener;)V
 */
JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativePullGameGroup
(JNIEnv* env, jobject thiz, jobject listener)
{
	LOGD("aonejni native PullGameGroup !!!");
	AoneSdkProxy::aonePullGameGroup(env, thiz, listener, "onResult");
}

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeGetSelectedRole
 * Signature: ()Lcom/aonesoft/lib/AoneClient/GameRole;
 */
JNIEXPORT jobject JNICALL Java_com_aonesoft_lib_AoneClient_nativeGetSelectedRole
(JNIEnv* env, jobject thiz)
{
	LOGD("aonejni native GetSelectedRole !!!");
	return AoneSdkProxy::aoneGetSelectedRole(env, thiz);
}

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeGetSelectedGroup
 * Signature: ()Lcom/aonesoft/lib/AoneClient/GameGroup;
 */
JNIEXPORT jobject JNICALL Java_com_aonesoft_lib_AoneClient_nativeGetSelectedGroup
(JNIEnv* env, jobject thiz)
{
	LOGD("aonejni native GetSelectedGroup !!!");
	return AoneSdkProxy::aoneGetSelectedGroup( env, thiz);
}

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativePay
 * Signature: (Ljava/lang/String;Ljava/lang/String;I;Lcom/aonesoft/lib/AoneResultListener;)V
 */
JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativePay
(JNIEnv* env, jobject thiz, jstring productId,jint purchaseNum, jstring cpExt, jobject listener)
{
	LOGD("aonejni native Pay !!!");
	AoneSdkProxy::aonePay(env, thiz, productId,purchaseNum, cpExt, listener, "onResult");
}

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeLogout
 * Signature: (Lcom/aonesoft/lib/AoneResultListener;)V
 */
JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeLogout
(JNIEnv* env, jobject thiz, jobject listener)
{
	LOGD("aonejni native Logout !!!");
	AoneSdkProxy::aoneLogout(env, thiz, listener, "onResult");
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeShare
(JNIEnv* env, jobject thiz, jobject cpInfo, jobject listener)
{
	LOGD("aonejni native share !!!");
	AoneSdkProxy::aoneShare(env, thiz, cpInfo, listener, "onResult");
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeGetAdTrackLink
(JNIEnv* env, jobject thiz, jstring userData, jobject listener)
{
	LOGI("aonejni native GetAdTrackLink !!!");
	AoneSdkProxy::aoneGetAdTrackLink(env, thiz, userData, listener, "onResult");
}

JNIEXPORT jstring JNICALL Java_com_aonesoft_lib_AoneClient_nativeGetAppLink
(JNIEnv* env, jobject thiz)
{
	LOGD("aonejni native GetAppLink !!!");

	return AoneSdkProxy::aoneGetAppLink(env, thiz);
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeGetAdSource
(JNIEnv* env, jobject thiz, jstring userData, jobject listener)
{
	LOGD("aonejni native GetAdSource !!!");
	AoneSdkProxy::aoneGetAdSource(env, thiz, userData, listener, "onResult");
}

JNIEXPORT jstring JNICALL Java_com_aonesoft_lib_AoneClient_nativeEmbedQR
(JNIEnv* env, jobject thiz, jstring src, jstring info, int x, int y, int w, int h)
{
	LOGD("aonejni native embedQR !!!");

	return AoneSdkProxy::aoneEmbedQR(env, thiz, src, info , x, y, w, h);
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeImagePicker
(JNIEnv* env, jobject thiz,jint code)
{
	LOGD("aonejni native ImagePicker !!!");

	AoneSdkProxy::aoneImagePicker(code);
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeUpdateCallback
(JNIEnv* env, jobject thiz)
{
	LOGD("aonejni update callback !!!");

	AoneSdkProxy::aoneUpdateCallback();
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeAnnounceCallback
(JNIEnv* env, jobject thiz)
{
	LOGD("aonejni announce callback !!!");

	AoneSdkProxy::aoneAnnounceCallback();
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeLogEvent
(JNIEnv* env, jobject thiz, jstring eventId)
{
	LOGD("aonejni native logEvent !!!");
	AoneSdkProxy::aoneLogEvent(env, thiz, eventId);
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeLogEventParams
(JNIEnv* env, jobject thiz, jstring eventId, jobject cpInfo)
{
	LOGD("aonejni native logEvent params !!!");
	AoneSdkProxy::aoneLogEvent(env, thiz, eventId, cpInfo);
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeOpenUrl
(JNIEnv* env, jobject thiz, jstring url)
{
	LOGD("aonejni native openurl !!!");
	const char* s_url = env->GetStringUTFChars(url, NULL);
	aonesdk::AoneClient::OpenUrl(s_url);
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeCopyToClipboard
(JNIEnv* env, jobject thiz, jstring text)
{
	LOGD("aonejni native copyToClipboard !!!");
	const char* s_text = env->GetStringUTFChars(text, NULL);
	aonesdk::AoneClient::copyToClipboard(s_text);
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeQueryPaynoStatus
(JNIEnv* env, jobject thiz, jint appId,jstring payno, jobject listener)
{
	LOGD("aonejni native query payno status !!!");
	AoneSdkProxy::aoneQueryPaynoStatus(env, thiz, appId,payno, listener, "onResult");
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeRecordDevicePoint
(JNIEnv* env, jobject thiz, jint sdkPoint)
{
	LOGD("aonejni RecordDevicePoint !!!");
	AoneSdkProxy::recordDevicePoint(env, thiz, sdkPoint);
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeRecordRolePoint
(JNIEnv* env, jobject thiz, jint gamePoint)
{
	LOGD("aonejni recordRolePoint !!!");
	AoneSdkProxy::recordRolePoint(env, thiz, gamePoint);
}

//push
JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeAddNotify
(JNIEnv* env, jobject thiz, jobject infos)
{
	LOGD("aonejni native nativeAddNotify !!!");
	AoneSdkProxy::aoneAddNotify(env, thiz, infos);
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeClearNotify
(JNIEnv* env, jobject thiz, jint notifyId)
{
	LOGD("aonejni native nativeClearNotify !!!");
	AoneSdkProxy::aoneClearNotify(env, thiz, notifyId);
}
JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeClearAllNotify
(JNIEnv* env, jobject thiz)
{
	LOGD("aonejni native nativeClearNotify !!!");
	AoneSdkProxy::aoneClearAllNotify(env, thiz);
}
JNIEXPORT jstring JNICALL Java_com_aonesoft_lib_AoneClient_nativeGetChannelParam
(JNIEnv* env, jobject thiz,jstring paramName)
{
	LOGD("aonejni native nativeGetChannelParam !!!");
	return AoneSdkProxy::aoneGetChannelParam(env, thiz,paramName);
}
JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeJoinQGroup
(JNIEnv* env, jobject thiz)
{
	LOGD("aonejni native nativeJoinQGroup !!!");
	AoneSdkProxy::aoneJoinQGroup(env, thiz);
}
JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeCallFunc
(JNIEnv* env, jobject thiz,jstring funcName)
{
	LOGD("aonejni native nativeCallFunc !!!");
	AoneSdkProxy::aoneCallFunc(env, thiz,funcName);
}
JNIEXPORT int JNICALL Java_com_aonesoft_lib_AoneClient_nativeCallIntFunc
(JNIEnv* env, jobject thiz,jstring funcName)
{
	LOGD("aonejni native nativeCallIntFunc !!!");
	return AoneSdkProxy::aoneCallIntFunc(env, thiz,funcName);
}
JNIEXPORT jstring JNICALL Java_com_aonesoft_lib_AoneClient_nativeCallStringFunc
(JNIEnv* env, jobject thiz,jstring funcName)
{
	LOGD("aonejni native nativeCallStringFunc !!!");
	return AoneSdkProxy::aoneCallStringFunc(env, thiz,funcName);
}
/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeShowAds
 * Signature: (Ljava/util/Hashtable;ILcom/aonesoft/lib/AoneResultListener;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeShowAds
(JNIEnv* env, jobject thiz,jobject infos,jint pos,jobject listener,jstring funcName)
{
	LOGD("aonejni native ShowAds !!!");
	return AoneSdkProxy::aoneShowAds(env, thiz,infos,pos, listener,"onResult");
}
/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeShowAds
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeSetAdsSdk
(JNIEnv* env, jobject thiz,jstring adsSdkName)
{
	LOGD("aonejni native SetAdsSdk !!!");
	AoneSdkProxy::aoneSetAdsSdk(env, thiz,adsSdkName);
}

/*
 * Class:     com_aonesoft_lib_AoneClient
 * Method:    nativeAoneQuickRegister
 * Signature: (ILcom/aonesoft/lib/AoneResultListener;)V
 */
JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeAoneQuickRegister
(JNIEnv* env, jobject thiz, jobject listener)
{
	LOGD("aonejni native AoneQuickRegister !!!");
	AoneSdkProxy::aoneQuickRegisterAsync(env, thiz, listener, "onResult");
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeSetUseUserLogin
(JNIEnv* env, jobject thiz, jboolean useUserLogin)
{
	LOGD("aonejni native set  UseUserLogin!!!");
	aonesdk::AoneClient::SetUseUserLogin(useUserLogin);
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeAoneBindOtherAccount
(JNIEnv* env, jobject thiz, jstring newAccount,jobject listener)
{
	LOGD("aonejni native aoneBindOtherAccount!!!");
	const char* newAccountStr;
	newAccountStr = env->GetStringUTFChars(newAccount, NULL);
	if(newAccountStr == NULL) {
		LOGD("AoneSdkProxy get newAccountStr null");
		return; /* OutOfMemoryError already thrown */
	}

	AoneSdkProxy::aoneBindOtherAccount(env, thiz,newAccountStr, listener, "onResult");
	env->ReleaseStringUTFChars(newAccount, newAccountStr);
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeAoneUnbindOtherAccount
(JNIEnv* env, jobject thiz,jobject listener)
{
	LOGD("aonejni native aoneUnBindOtherAccount!!!");
	AoneSdkProxy::aoneUnbindOtherAccount(env, thiz, listener, "onResult");
}

JNIEXPORT jint JNICALL Java_com_aonesoft_lib_AoneClient_nativeAoneQueryBindOtherAccountSync
(JNIEnv* env, jobject thiz)
{
	LOGD("aonejni native aone Query Bind Other Account!!!");
	return AoneSdkProxy::aoneQueryBindOtherAccountSync(env, thiz);
}
JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeAoneQueryBindOtherAccount
(JNIEnv* env, jobject thiz,jobject listener)
{
	LOGD("aonejni native aoneQueryBindOtherAccount!!!");
	AoneSdkProxy::aoneQueryBindOtherAccount(env, thiz, listener, "onResult");
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeAoneChangePassword
(JNIEnv* env, jobject thiz, jstring oldPassword,jstring newPassword,jobject listener)
{
	LOGD("aonejni native change password!!!");
	const char* oldPasswordStr;
	const char* newPasswordStr;
	oldPasswordStr = env->GetStringUTFChars(oldPassword,NULL);
	newPasswordStr = env->GetStringUTFChars(newPassword,NULL);

	if(oldPasswordStr == NULL) {
		LOGD("AoneSdkProxy get oldPasswordStr null");
		return; /* OutOfMemoryError already thrown */
	}
	if(newPasswordStr == NULL) {
		LOGD("AoneSdkProxy get newPasswordStr null");
		return; /* OutOfMemoryError already thrown */
	}
	AoneSdkProxy::aoneChangePassword(env, thiz,oldPasswordStr,newPasswordStr, listener, "onResult");
	env->ReleaseStringUTFChars(oldPassword, oldPasswordStr);
	env->ReleaseStringUTFChars(newPassword, newPasswordStr);
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeAoneThirdPartyLogin
(JNIEnv* env, jobject thiz ,jobject listener)
{
	LOGD("aonejni native thirdParty login!!!");
	AoneSdkProxy::aoneThirdPartyLogin(env, thiz,listener, "onResult");
}


JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeAoneCreateInheritance
(JNIEnv* env, jobject thiz,jstring password,jobject listener)
{
	LOGD("aonejni native inheritance code!!!");
	const char* passwordStr;
	passwordStr = env->GetStringUTFChars(password,NULL);

	if(passwordStr == NULL) {
		LOGD("AoneSdkProxy get passwordStr null");
		return; /* OutOfMemoryError already thrown */
	}
	AoneSdkProxy::aoneCreateInheritance(env, thiz,passwordStr, listener, "onResult");
	env->ReleaseStringUTFChars(password, passwordStr);
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeAoneLogin
(JNIEnv* env, jobject thiz,jstring account,jstring password,jobject listener)
{
	LOGD("aonejni native aone login with account!!!");
	const char* accountStr;
	const char* passwordStr;
	accountStr = env->GetStringUTFChars(account,NULL);
	passwordStr = env->GetStringUTFChars(password,NULL);

	if(accountStr == NULL) {
		LOGD("AoneSdkProxy get accountStr null");
		return; /* OutOfMemoryError already thrown */
	}
	if(passwordStr == NULL) {
		LOGD("AoneSdkProxy get passwordStr null");
		return; /* OutOfMemoryError already thrown */
	}
	AoneSdkProxy::aoneLoginWithAccount(env, thiz,accountStr,passwordStr, listener, "onResult");
	env->ReleaseStringUTFChars(account, accountStr);
	env->ReleaseStringUTFChars(password, passwordStr);
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeCleanRoleData
(JNIEnv* env, jobject thiz)
{
	LOGD("aonejni native clean role data!!!");
	AoneSdkProxy::aoneCleanRoleData(env, thiz);
}
JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeSetUserId
(JNIEnv* env, jobject thiz,jint userId)
{
	LOGD("aonejni native set user id!!!");
	aonesdk::AoneSDK::getInstance()->setUserId(userId);
}
JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeRealnameInfo
(JNIEnv* env, jobject thiz,jobject listener)
{
	LOGD("aonejni native realname info!!!");
	AoneSdkProxy::realnameInfo(env, thiz,listener,"onResult");
}

JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeRealnameCertificate
(JNIEnv* env, jobject thiz,jstring identity_number, jstring real_name,jobject listener)
{
	LOGD("aonejni native realname Certificate!!!");
	AoneSdkProxy::realnameCertificate(env, thiz,identity_number, real_name,listener,"onResult");
}
JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeRequestPlayerExtraInfo
(JNIEnv* env, jobject thiz,jobject listener)
{
	LOGD("aonejni native realname Certificate!!!");
	AoneSdkProxy::requestPlayerExtraInfo(env, thiz,listener,"onResult");
}
JNIEXPORT void JNICALL Java_com_aonesoft_lib_AoneClient_nativeCallRequestPlayerExtraInfoListener
(JNIEnv* env, jobject thiz,jint code,jobject infos)
{
	LOGD("aoneclient native CallRequestPlayerExtraInfoListener!!!");
	std::map<std::string, std::string> mapInfos = hashtable2Map(env,infos);
	uuDevice::requestPlayerExtraInfoCallback(code,mapInfos);
}


/********************
 * AoneClient_Java End
 ********************/
}

/*
 * AoneSdkProxy defination
 */
JNIEnv* AoneSdkProxy::s_env = NULL;
jobject AoneSdkProxy::s_initListener = NULL;
jobject AoneSdkProxy::s_loginListener = NULL;
jobject AoneSdkProxy::s_oauthLoginListener = NULL;
jobject AoneSdkProxy::s_payListener = NULL;
jobject AoneSdkProxy::s_checkVersionListener = NULL;
jobject AoneSdkProxy::s_selectRoleAndGroupListener = NULL;
jobject AoneSdkProxy::s_listener = NULL;
jobject AoneSdkProxy::s_queryPaynoStatusListener = NULL;
jobject AoneSdkProxy::s_quickRegisterListener = NULL;
jobject AoneSdkProxy::s_adsListener = NULL;
jobject AoneSdkProxy::s_shareListener = NULL;
jobject AoneSdkProxy::s_bindOtherAccountListener = NULL;
jobject AoneSdkProxy::s_unbindOtherAccountListener = NULL;
jobject AoneSdkProxy::s_queryBindOtherAccountListener = NULL;
jobject AoneSdkProxy::s_changePasswordListener = NULL;
jobject AoneSdkProxy::s_thirdPartyLoginListener = NULL;
jobject AoneSdkProxy::s_creatreInheritanceListener = NULL;
jobject AoneSdkProxy::s_aoneLoginListener = NULL;
jobject AoneSdkProxy::s_realnameInfoListener = NULL;
jobject AoneSdkProxy::s_realnameCertificateListener = NULL;
jobject AoneSdkProxy::s_requestPlayerExtraInfoListener = NULL;
std::string AoneSdkProxy::s_methodName = "";

jobject createJavaMapObject(JNIEnv* env, std::map<std::string, std::string>* paramMap)
{
	jclass class_Hashtable = env->FindClass("java/util/Hashtable");
	jmethodID construct_method = env->GetMethodID( class_Hashtable, "<init>","()V");
	jobject obj_Map = env->NewObject( class_Hashtable, construct_method, "");
	if (paramMap != NULL)
	{
		jmethodID add_method = env->GetMethodID( class_Hashtable,"put","(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
		for (std::map<std::string, std::string>::const_iterator it = paramMap->begin(); it != paramMap->end(); ++it)
		{
			jstring keyStr = env->NewStringUTF(it->first.c_str());
			jstring valStr = env->NewStringUTF(it->second.c_str());
			env->CallObjectMethod(obj_Map, add_method, keyStr, valStr);
			env->DeleteLocalRef(keyStr);
			env->DeleteLocalRef(valStr);
		}
	}
	env->DeleteLocalRef(class_Hashtable);
	return obj_Map;
}

void callJavaListener(JNIEnv* env, jobject listener, std::string methodName, int type, std::map<std::string, std::string> dataMap)
{
	if (listener == NULL) {
		LOGD("AoneSDK callJavaListener Error: the listener is NULL!");
		return;
	}

	// jclass class_listener = env->FindClass("com/aonesoft/lib/AoneResultListener");
	jclass class_listener = env->GetObjectClass(listener);
	jmethodID method_onResult = env->GetMethodID(class_listener, methodName.c_str(), "(ILjava/util/Hashtable;)V");
	jobject obj_map = createJavaMapObject(env, &dataMap);
	env->CallVoidMethod(listener, method_onResult, type, obj_map);
	env->DeleteLocalRef(obj_map);
}

void AoneSdkProxy::setJavaVM(JavaVM *javaVM)
{
	LOGD("AoneSdkProxy setJavaVM begin");
	aonesdk::PluginJniHelper::setJavaVM(javaVM);
	LOGD("AoneSdkProxy setJavaVM end");
}
// init
void AoneSdkProxy::initCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("AoneSDK initCallback, type: %d", type);
	callJavaListener(s_env, s_initListener, s_methodName, type, dataMap);
	s_env->DeleteGlobalRef(s_initListener);
	s_initListener = NULL;
}

void AoneSdkProxy::init(JNIEnv* env, jobject thiz, jobject listener, std::string methodName)
{
	LOGD("AoneSdkProxy init");
	s_env = env;
	s_listener = env->NewGlobalRef(listener);
	s_methodName = methodName;
	aonesdk::AoneSDK::getInstance()->initAsync(callback);
}

// login
void AoneSdkProxy::loginCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("AoneSDK loginCallback, type: %d", type);
	callJavaListener(s_env, s_loginListener, s_methodName, type, dataMap);
	s_env->DeleteGlobalRef(s_loginListener);
	s_loginListener = NULL;
}

void AoneSdkProxy::login(JNIEnv* env, jobject thiz, jobject listener, std::string methodName)
{
	LOGD("AoneSdkProxy login");
	s_env = env;
	s_listener = env->NewGlobalRef(listener);
	s_methodName = methodName;
	LOGD("not thirdPartyLogin");
	aonesdk::AoneSDK::getInstance()->login(callback);
}

// pay
void AoneSdkProxy::payCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("AoneSDK payCallback, type: %d", type);
	callJavaListener(s_env, s_payListener, s_methodName, type, dataMap);
	s_env->DeleteGlobalRef(s_payListener);
}

void AoneSdkProxy::pay(JNIEnv* env, jobject thiz, jstring productId,jint purchaseNum, jstring cp_ext, jobject listener, std::string methodName)
{
	LOGD("AoneSdkProxy pay begin");
	s_env = env;
	set_listener(listener, &s_payListener, env);
	s_methodName = methodName;

	const char* id = env->GetStringUTFChars(productId, NULL);
	const char* cp = env->GetStringUTFChars(cp_ext, NULL);
	aonesdk::AoneSDK::getInstance()->pay(id, purchaseNum, cp, aonePayCallback);

	env->ReleaseStringUTFChars(productId, id);
	env->ReleaseStringUTFChars(cp_ext, cp);

	LOGD("AoneSdkProxy pay end");
}

void AoneSdkProxy::pay(JNIEnv* env, jobject thiz, jobject hashTable, jobject listener, std::string methodName)
{
	LOGD("AoneSdkProxy pay begin");

	// jclass class_hashTable = env->GetObjectClass(hashTable);
	// jmethodID method_hashTable_get = env->GetMethodID(class_hashTable, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
	// jmethodID method_keys = env->GetMethodID(class_hashTable, "keys", "()Ljava/util/Enumeration;");
	// jclass class_object = env->FindClass("java/lang/Object");
	// jmethodID method_toString = env->GetMethodID(class_object, "toString", "()Ljava/lang/String;");
	// jclass class_enumeration = env->FindClass("java/util/Enumeration");
	// jmethodID method_hasMoreElements = env->GetMethodID(class_enumeration, "hasMoreElements", "()Z");
	// jmethodID method_nextElement = env->GetMethodID(class_enumeration, "nextElement", "()Ljava/lang/Object;");

	// jobject jkey, jvalue;
	// jstring jkeyStr, jvalueStr;
	// jstring jkeyStrParam, jvalueStrValue;
	// const char *key = NULL;
	// const char *value = NULL;
	// std::map<std::string, std::string> info;
	// jobject keys = env->CallObjectMethod(hashTable, method_keys);
	// while (env->CallBooleanMethod(keys, method_hasMoreElements)) {
	//     jkey = env->CallObjectMethod(keys, method_nextElement);
	//     jkeyStr = (jstring)env->CallObjectMethod(jkey, method_toString);
	//	   jkeyStrParam = jkeyStr;
	//     jvalue = env->CallObjectMethod(hashTable, method_hashTable_get, jkeyStr);
	//     jvalueStr = (jstring)env->CallObjectMethod(jvalue, method_toString);
	//	   jvalueStrValue = jvalueStr;
	//     key = env->GetStringUTFChars(jkeyStr, NULL);
	//     value = env->GetStringUTFChars(jvalueStr, NULL);
	//     key = env->GetStringUTFChars(jkeyStrParam, NULL);
	//     value = env->GetStringUTFChars(jvalueStrValue, NULL);
	//     info[key] = value;
	//     env->ReleaseStringUTFChars(jkeyStr, key);
	//     env->ReleaseStringUTFChars(jvalueStr, value);
	// }

	// s_env = env;
	// s_listener = env->NewGlobalRef(listener);
	// s_methodName = methodName;
	// aonesdk::AoneSDK::getInstance()->pay(info, callback);
	// aonesdk::AoneSDK::getInstance()->pay(info, callback);

	aonesdk::AoneSDK::getInstance()->pay("1",1, "", callback);
	LOGD("AoneSdkProxy pay end");
}

void AoneSdkProxy::callback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("AoneSDK callback, type: %d", type);
	callJavaListener(s_env, s_listener, s_methodName, type, dataMap);
	s_env->DeleteGlobalRef(s_listener);
}

void AoneSdkProxy::aoneShareCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("AoneSDK callback, type: %d", type);
	JNIEnv* env = NULL;
	s_jvm->AttachCurrentThread(&env, 0);
	aoneCallJavaListener(env, s_shareListener, s_methodName, type, dataMap);
}

void AoneSdkProxy::aoneGetAdTrackLinkCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGI("AoneSDK callback, type: %d", type);
	JNIEnv* env = NULL;
	s_jvm->AttachCurrentThread(&env, 0);
	aoneCallJavaListener(env, s_listener, s_methodName, type, dataMap);
}

void AoneSdkProxy::aoneGetAdSourceCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("AoneSDK callback, type: %d", type);
	JNIEnv* env = NULL;
	s_jvm->AttachCurrentThread(&env, 0);
	aoneCallJavaListener(env, s_listener, s_methodName, type, dataMap);
}

void AoneSdkProxy::setOAuthType(JNIEnv* env, jobject thiz, jstring type)
{
	LOGD("AoneSdkProxy setOAuthType begin");
	const char* str;
	str = env->GetStringUTFChars(type, NULL);
	if(str == NULL) {
		LOGD("AoneSdkProxy get str null");
		return; /* OutOfMemoryError already thrown */
	}

	aonesdk::AoneSDK::getInstance()->setOAuthType(str);
	env->ReleaseStringUTFChars(type, str);
}

void AoneSdkProxy::setPayChannel(JNIEnv* env, jobject thiz, jstring payChannel)
{
	LOGD("AoneSdkProxy setPayChannel begin");
	const char* str;
	str = env->GetStringUTFChars(payChannel, NULL);
	if(str == NULL) {
		LOGD("AoneSdkProxy get str null");
		return; /* OutOfMemoryError already thrown */
	}

	aonesdk::AoneSDK::getInstance()->setPayChannel(str);
	env->ReleaseStringUTFChars(payChannel, str);
}

/******************************
 * AoneClient Method Field Begin
 ******************************/

void AoneSdkProxy::NewListObjAndGetAddMethod(JNIEnv* env, jobject* obj, jmethodID* methodId) {
	jclass alClass = env->FindClass("java/util/ArrayList");
	jmethodID alConstructMethod = env->GetMethodID(alClass, "<init>", "()V");
	(*obj) = env->NewObject(alClass, alConstructMethod, "");
	(*methodId) = env->GetMethodID(alClass, "add", "(Ljava/lang/Object;)Z");
}

void AoneSdkProxy::GetGameClassAndContructMethod(JNIEnv* env, jclass* gameClass, jmethodID* methodId, const char* className, const char* signs) {
	(*gameClass) = env->FindClass(className);
	if (!(*gameClass)) {
		LOGD("failed to get class reference: %s", className);
		return;
	}
	(*methodId) = env->GetMethodID(*gameClass, "<init>", signs);
	if (!(*methodId)) {
		LOGD("failed to get method id: %s", className);
		return;
	}
}

void AoneSdkProxy::aoneCallJavaListener(JNIEnv* env, jobject listener, std::string methodName, int type, std::map<std::string, std::string> dataMap)
{
	if (listener == NULL) {
		LOGD("AoneSDK callJavaListener Error: the listener is NULL!");
		return;
	}

	LOGD("AoneSDK callJavaListener!");
	jclass class_listener = env->GetObjectClass(listener);
	jmethodID method_onResult = env->GetMethodID(class_listener, methodName.c_str(), "(ILjava/util/Hashtable;)V");
	jobject obj_map = aoneCreateJavaMapObject(env, &dataMap);
	env->CallVoidMethod(listener, method_onResult, type, obj_map);
	env->DeleteLocalRef(obj_map);
}

jobject AoneSdkProxy::aoneCreateJavaMapObject(JNIEnv* env, std::map<std::string, std::string>* paramMap)
{
	jclass class_Hashtable = env->FindClass("java/util/Hashtable");
	jmethodID construct_method = env->GetMethodID( class_Hashtable, "<init>","()V");
	jobject obj_Map = env->NewObject( class_Hashtable, construct_method, "");
	if (paramMap != NULL)
	{
		jmethodID add_method = env->GetMethodID( class_Hashtable,"put","(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
		for (std::map<std::string, std::string>::const_iterator it = paramMap->begin(); it != paramMap->end(); ++it)
		{
			jstring keyStr = env->NewStringUTF(it->first.c_str());
			jstring valStr = env->NewStringUTF(it->second.c_str());
			env->CallObjectMethod(obj_Map, add_method, keyStr, valStr);
			env->DeleteLocalRef(keyStr);
			env->DeleteLocalRef(valStr);
		}
	}
	env->DeleteLocalRef(class_Hashtable);
	return obj_Map;
}

void AoneSdkProxy::aoneCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("AoneSDK callback, type: %d", type);
	JNIEnv* env = NULL;
	s_jvm->AttachCurrentThread(&env, 0);
	aoneCallJavaListener(env, s_listener, s_methodName, type, dataMap);
}

void AoneSdkProxy::aoneInitCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("AoneSDK aoneInit callback, type: %d", type);
	JNIEnv* env = NULL;
	s_jvm->AttachCurrentThread(&env, 0);
	aoneCallJavaListener(env, s_initListener, s_methodName, type, dataMap);
}

void AoneSdkProxy::aoneLoginCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("AoneSDK login callback, type: %d", type);
	JNIEnv* env = NULL;
	s_jvm->AttachCurrentThread(&env, 0);
	aoneCallJavaListener(env, s_loginListener, s_methodName, type, dataMap);
}

void AoneSdkProxy::aoneOauthLoginCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("AoneSDK oauth login callback, type: %d", type);
	JNIEnv* env = NULL;
	s_jvm->AttachCurrentThread(&env, 0);
	aoneCallJavaListener(env, s_oauthLoginListener, s_methodName, type, dataMap);
}

void AoneSdkProxy::aonePayCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("AoneSDK callback, type: %d", type);
	JNIEnv* env = NULL;
	s_jvm->AttachCurrentThread(&env, 0);
	aoneCallJavaListener(env, s_payListener, s_methodName, type, dataMap);
}

void AoneSdkProxy::aoneCheckVersionCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("AoneSDK callback, type: %d", type);
	JNIEnv* env = NULL;
	s_jvm->AttachCurrentThread(&env, 0);
	aoneCallJavaListener(env, s_checkVersionListener, s_methodName, type, dataMap);
}
void AoneSdkProxy::aoneSelectRoleAndGroupListenerCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("AoneSDK callback, type: %d", type);
	JNIEnv* env = NULL;
	s_jvm->AttachCurrentThread(&env, 0);
	aoneCallJavaListener(env, s_selectRoleAndGroupListener, s_methodName, type, dataMap);
}

void AoneSdkProxy::aoneInit(JNIEnv* env, jobject thiz, jobject listener, std::string methodName)
{
	LOGD("AoneSdkProxy init begin");
	s_env = env;
	s_initListener = env->NewGlobalRef(listener);
	s_methodName = methodName;
	aonesdk::AoneSDK::getInstance()->setUseUserLogin(false);
	aonesdk::AoneSDK::getInstance()->initAsync(aoneInitCallback);
}

jobject AoneSdkProxy::aoneAllProducts(JNIEnv* env, jobject thiz)
{
	LOGD("AoneSdkProxy getAllProducts begin");

	map<std::string, aonesdk::GameProduct> products = aonesdk::AoneSDK::getInstance()->getAllProducts();

	LOGD("###game products list (%u):", (unsigned int)products.size());

	jobject alObj;
	jmethodID alAddMethod;
	AoneSdkProxy::NewListObjAndGetAddMethod(env, &alObj, &alAddMethod);

	jclass gameGroupClass;
	jmethodID gameGroupConstructMethod;
	AoneSdkProxy::GetGameClassAndContructMethod(env, &gameGroupClass,
			&gameGroupConstructMethod, "com/aonesoft/lib/AoneClient$GameProduct",
			"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;DDILjava/lang/String;)V");

	jstring jid = NULL;
	jstring jname = NULL;
	jstring jdescribe = NULL;
	jstring jcurrency = NULL;
	jstring jtype = NULL;
	jobject productObj = NULL;
	for (map<std::string, aonesdk::GameProduct>::const_iterator it = products.begin(); it != products.end(); ++it)
	{
		LOGD("######id: %s, name: %s", it->second.ID.c_str(), it->second.Name.c_str());
		jid = env->NewStringUTF(it->second.ID.c_str());
		jname = env->NewStringUTF(it->second.Name.c_str());
		jdescribe = env->NewStringUTF(it->second.Describe.c_str());
		jcurrency = env->NewStringUTF(it->second.Currency.c_str());
		jtype = env->NewStringUTF(it->second.Type.c_str());
		productObj = env->NewObject(gameGroupClass, gameGroupConstructMethod, jid,
				jname, jdescribe, jcurrency, it->second.Amount, it->second.Amount_usd, it->second.Coin, jtype);
		env->CallBooleanMethod(alObj, alAddMethod, productObj);
	}
	if (jname) {
		env->DeleteLocalRef(jid);
		env->DeleteLocalRef(jname);
		env->DeleteLocalRef(jdescribe);
		env->DeleteLocalRef(jcurrency);
		env->DeleteLocalRef(productObj);
	}
	// env->DeleteLocalRef(alObj);
	LOGD("AoneSdkProxy getAllProducts end");
	return alObj;
}

jobject AoneSdkProxy::aoneAllLangs(JNIEnv* env, jobject thiz)
{
	LOGD("AoneSdkProxy getAllLangs begin");
	map<std::string, std::string> langs = aonesdk::AoneSDK::getInstance()->getAllAppLangs();

	LOGD("###game Langs list (%u):", (unsigned int)langs.size());

	jclass class_hashTable = env->FindClass("java/util/Hashtable");
	jmethodID method_hashTable_put = env->GetMethodID(class_hashTable, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
	jmethodID construct_method = env->GetMethodID(class_hashTable, "<init>","()V");
	jobject obj_Map = env->NewObject(class_hashTable, construct_method, "");

	for (std::map<std::string, std::string>::const_iterator it = langs.begin(); it != langs.end(); ++it)
	{
		env->CallObjectMethod(obj_Map, method_hashTable_put, env->NewStringUTF(it->first.c_str()), env->NewStringUTF(it->second.c_str()));
	}

	env->DeleteLocalRef(class_hashTable);
	LOGD("AoneSdkProxy getAllLangs end");
	return obj_Map;
}

jint AoneSdkProxy::aoneSelectLang(JNIEnv* env, jobject thiz, jstring lang)
{
	LOGD("AoneSdkProxy SelectLang begin");

	const char* langStr = env->GetStringUTFChars(lang, NULL);
	jint status = aonesdk::AoneSDK::getInstance()->setAppLang(langStr);
	env->ReleaseStringUTFChars(lang, langStr);
	LOGD("AoneSdkProxy SelectLang end");
	return status;
}

void AoneSdkProxy::aoneSetOAuthType(JNIEnv* env, jobject thiz, jstring type)
{
	LOGD("AoneSdkProxy SetOAuthType begin");

	const char* typeStr = env->GetStringUTFChars(type, NULL);
	aonesdk::AoneSDK::getInstance()->setOAuthType(typeStr);
	env->ReleaseStringUTFChars(type, typeStr);

	LOGD("AoneSdkProxy SetOAuthType end");
}

jobject AoneSdkProxy::aoneGetGameVersion(JNIEnv* env, jobject thiz)
{
	LOGD("AoneSdkProxy GetGameVersion begin");

	GameVersion gameVersion = aonesdk::AoneSDK::getInstance()->getGameVersion();

	jobject Obj;
	jmethodID alAddMethod;
	jclass alClass = env->FindClass("com/aonesoft/lib/AoneClient$GameVersion");
	jmethodID alConstructMethod = env->GetMethodID(alClass, "<init>",
			"(CCCLjava/lang/String;Ljava/lang/String;Ljava/lang/String;D)V");

	jstring Notice = NULL;
	jstring DownloadURL = NULL;
	jstring UpdateURL = NULL;

	Notice = env->NewStringUTF(gameVersion.Notice.c_str());
	DownloadURL = env->NewStringUTF(gameVersion.DownloadURL.c_str());
	UpdateURL = env->NewStringUTF(gameVersion.UpdateURL.c_str());
	Obj = env->NewObject(alClass, alConstructMethod, gameVersion.MajorVersion,
			gameVersion.MinorVersion, gameVersion.RevisionVersion, Notice, DownloadURL, UpdateURL, gameVersion.Status);
	if (Notice) {
		env->DeleteLocalRef(Notice);
	}
	if (DownloadURL) {
		env->DeleteLocalRef(DownloadURL);
	}
	if (UpdateURL) {
		env->DeleteLocalRef(UpdateURL);
	}
	LOGD("AoneSdkProxy GetGameVersion end");
	return Obj;
}

void AoneSdkProxy::aoneLogin(JNIEnv* env, jobject thiz, jobject listener, std::string methodName)
{
	LOGD("AoneSdkProxy login begin");

	s_env = env;
	set_listener(listener, &s_loginListener, env);
	s_methodName = methodName;
	aonesdk::AoneSDK::getInstance()->login(aoneLoginCallback);
}

void AoneSdkProxy::aoneOauthLogin(JNIEnv* env, jobject thiz, jobject listener, std::string methodName)
{
	LOGD("AoneSdkProxy oauth login begin");

	s_env = env;
	set_listener(listener, &s_oauthLoginListener, env);
	s_methodName = methodName;
	aonesdk::AoneSDK::getInstance()->OAuthLoginAsync(aoneOauthLoginCallback);
}

jint AoneSdkProxy::aoneUserID(JNIEnv* env, jobject thiz)
{
	return aonesdk::AoneSDK::getInstance()->getUserId();
}

jint AoneSdkProxy::aoneUserToken(JNIEnv* env, jobject thiz)
{
	return aonesdk::AoneSDK::getInstance()->getUserToken();
}

jstring AoneSdkProxy::aoneToken(JNIEnv* env, jobject thiz)
{
	return env->NewStringUTF(aonesdk::AoneSDK::getInstance()->getToken().c_str());
}

jobject AoneSdkProxy::aoneAllGroups(JNIEnv* env, jobject thiz)
{
	LOGD("AoneSdkProxy getAllGroups begin");

	std::vector<GameGroup> groups = aonesdk::AoneSDK::getInstance()->getAllGroups();

	LOGD("###game AllGroups list (%u):", (unsigned int)groups.size());

	jobject alObj;
	jmethodID alAddMethod;
	AoneSdkProxy::NewListObjAndGetAddMethod(env, &alObj, &alAddMethod);

	jclass gameGroupClass;
	jmethodID gameGroupConstructMethod;
	AoneSdkProxy::GetGameClassAndContructMethod(env, &gameGroupClass,
			&gameGroupConstructMethod, "com/aonesoft/lib/AoneClient$GameGroup",
			"(ILjava/lang/String;ILjava/lang/String;IZZ)V");

	jstring jname = NULL;
	jstring jServerIP = NULL;
	jobject groupObj;
	for (std::vector<GameGroup>::const_iterator it = groups.begin(); it != groups.end(); ++it)
	{
		LOGD("######id: %d, name: %s", it->ID, it->Name.c_str());
		jname = env->NewStringUTF(it->Name.c_str());
		jServerIP = env->NewStringUTF(it->ServerIP.c_str());
		groupObj = env->NewObject(gameGroupClass, gameGroupConstructMethod, it->ID, jname, it->Status, jServerIP, it->ServerPort, it->IsRecommend, it->IsInitialed);
		env->CallBooleanMethod(alObj, alAddMethod, groupObj);
	}
	if (jname) {
		env->DeleteLocalRef(jname);
		env->DeleteLocalRef(jServerIP);
	}
	// env->DeleteLocalRef(alObj);
	LOGD("AoneSdkProxy getAllGroups end");
	return alObj;
}

jobject AoneSdkProxy::aoneAllRoles(JNIEnv* env, jobject thiz)
{
	LOGD("AoneSdkProxy getAllRoles begin");

	const std::vector<GameRole> roles = aonesdk::AoneSDK::getInstance()->getAllRoles();

	LOGD("###game AllRoles list (%u):", (unsigned int)roles.size());

	jobject alObj;
	jmethodID alAddMethod;
	AoneSdkProxy::NewListObjAndGetAddMethod(env, &alObj, &alAddMethod);

	jclass gameRoleClass;
	jmethodID gameGroupConstructMethod;
	AoneSdkProxy::GetGameClassAndContructMethod(env, &gameRoleClass,
			&gameGroupConstructMethod, "com/aonesoft/lib/AoneClient$GameRole",
			"(ILjava/lang/String;IIILjava/lang/String;Ljava/lang/String;IILjava/util/Hashtable;)V");

	jstring jname = NULL;
	jstring jCreateTime = NULL;
	jstring jLastLoginTime = NULL;
	std::map<std::string, std::string> info;

	jclass class_hashTable = env->FindClass("java/util/Hashtable");
	jmethodID method_hashTable_put = env->GetMethodID(class_hashTable, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
	jmethodID construct_method = env->GetMethodID( class_hashTable, "<init>","()V");
	jobject obj_Map = env->NewObject(class_hashTable, construct_method, "");
	jobject roleObj;
	for (std::vector<GameRole>::const_iterator itVec = roles.begin(); itVec != roles.end(); ++itVec)
	{
		LOGD("######id: %d, name: %s", itVec->ID, itVec->Name.c_str());
		jname = env->NewStringUTF(itVec->Name.c_str());
		jCreateTime = env->NewStringUTF(itVec->CreateTime.c_str());
		jLastLoginTime = env->NewStringUTF(itVec->LastLoginTime.c_str());
		info = itVec->Features;
		for (std::map<std::string, std::string>::const_iterator it = info.begin(); it != info.end(); ++it)
		{
			env->CallObjectMethod(obj_Map, method_hashTable_put, env->NewStringUTF(it->first.c_str()), env->NewStringUTF(it->second.c_str()));
		}
		roleObj = env->NewObject(gameRoleClass, gameGroupConstructMethod, itVec->ID, jname, itVec->Type, itVec->Level, itVec->VIP, jCreateTime, jLastLoginTime, itVec->OwnerGroupID, itVec->LastGroupID, obj_Map);
		env->CallBooleanMethod(alObj, alAddMethod, roleObj);
		info.clear();
	}
	if (jname) {
		env->DeleteLocalRef(jname);
		env->DeleteLocalRef(jCreateTime);
		env->DeleteLocalRef(jLastLoginTime);
		env->DeleteLocalRef(class_hashTable);
	}
	// env->DeleteLocalRef(alObj);
	LOGD("AoneSdkProxy getAllRoles end");
	return alObj;
}

jstring AoneSdkProxy::aoneAoneAccount(JNIEnv* env, jobject thiz)
{
	return env->NewStringUTF(aonesdk::AoneSDK::getInstance()->getAoneAccount().c_str());
}

jobject AoneSdkProxy::aoneGetSelectedRole(JNIEnv* env, jobject thiz)
{
	LOGD("AoneSdkProxy GetSelectedRole begin");
	aonesdk::GameRole gameRole = aonesdk::AoneSDK::getInstance()->getSelectedRole();
	jclass gameRoleClass;
	jmethodID gameGroupConstructMethod;
	AoneSdkProxy::GetGameClassAndContructMethod(env, &gameRoleClass,
			&gameGroupConstructMethod, "com/aonesoft/lib/AoneClient$GameRole",
			"(ILjava/lang/String;IIILjava/lang/String;Ljava/lang/String;IILjava/util/Hashtable;)V");
	jclass class_hashTable = env->FindClass("java/util/Hashtable");
	jmethodID method_hashTable_put = env->GetMethodID(class_hashTable, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
	jmethodID construct_method = env->GetMethodID( class_hashTable, "<init>", "()V");
	jobject obj_Map = env->NewObject(class_hashTable, construct_method, "");
	jstring jname = NULL;
	jstring jCreateTime = NULL;
	jstring jLastLoginTime = NULL;
	std::map<std::string, std::string> info;
	jname = env->NewStringUTF(gameRole.Name.c_str());
	jCreateTime = env->NewStringUTF(gameRole.CreateTime.c_str());
	jLastLoginTime = env->NewStringUTF(gameRole.LastLoginTime.c_str());
	info = gameRole.Features;
	jobject roleObj = NULL;
	for (std::map<std::string, std::string>::const_iterator it = info.begin(); it != info.end(); ++it)
	{
		env->CallObjectMethod(obj_Map, method_hashTable_put, env->NewStringUTF(it->first.c_str()), env->NewStringUTF(it->second.c_str()));
	}
	roleObj = env->NewObject(gameRoleClass, gameGroupConstructMethod, gameRole.ID, jname, gameRole.Type, gameRole.Level, gameRole.VIP, jCreateTime, jLastLoginTime, gameRole.OwnerGroupID, gameRole.LastGroupID, obj_Map);
	if (jname) {
		env->DeleteLocalRef(jname);
		env->DeleteLocalRef(jCreateTime);
		env->DeleteLocalRef(jLastLoginTime);
		env->DeleteLocalRef(class_hashTable);
	}
	// env->DeleteLocalRef(alObj);
	LOGD("AoneSdkProxy GetSelectedRole end");
	return roleObj;
}

jobject AoneSdkProxy::aoneGetSelectedGroup(JNIEnv* env, jobject thiz)
{
	LOGD("AoneSdkProxy GetSelectedGroup begin");

	aonesdk::GameGroup gameGroup = aonesdk::AoneSDK::getInstance()->getSelectedGroup();

	jclass gameGroupClass;
	jmethodID gameGroupConstructMethod;
	AoneSdkProxy::GetGameClassAndContructMethod(env, &gameGroupClass,
			&gameGroupConstructMethod, "com/aonesoft/lib/AoneClient$GameGroup",
			"(ILjava/lang/String;ILjava/lang/String;IZZ)V");

	jstring jname = NULL;
	jstring jServerIP = NULL;
	jobject groupObj = NULL;

	jname = env->NewStringUTF(gameGroup.Name.c_str());
	jServerIP = env->NewStringUTF(gameGroup.ServerIP.c_str());
	groupObj = env->NewObject(gameGroupClass, gameGroupConstructMethod, gameGroup.ID, jname, jServerIP, gameGroup.ServerPort, gameGroup.IsRecommend, gameGroup.IsInitialed);

	if (jname) {
		env->DeleteLocalRef(jname);
		env->DeleteLocalRef(jServerIP);
	}
	// env->DeleteLocalRef(alObj);
	LOGD("AoneSdkProxy GetSelectedGroup end");
	return groupObj;
}

void AoneSdkProxy::aonePay(JNIEnv* env, jobject thiz, jstring productId,jint purchaseNum, jstring cp_ext, jobject listener, std::string methodName)
{
	LOGD("AoneSdkProxy Pay begin");

	s_env = env;
	set_listener(listener, &s_payListener, env);
	s_methodName = methodName;

	const char* id = env->GetStringUTFChars(productId, NULL);
	const char* cp = env->GetStringUTFChars(cp_ext, NULL);
	aonesdk::AoneSDK::getInstance()->pay(id, purchaseNum, cp, aonePayCallback);

	env->ReleaseStringUTFChars(productId, id);
	env->ReleaseStringUTFChars(cp_ext, cp);

}

void AoneSdkProxy::aoneCheckVersion(JNIEnv* env, jobject thiz, jint maj, jint min, jint rev, jobject listener, std::string methodName)
{
	LOGD("AoneSdkProxy CheckVersion begin");

	s_env = env;
	set_listener(listener, &s_checkVersionListener, env);
	s_methodName = methodName;

	aonesdk::AoneSDK::getInstance()->checkVersionAsync(maj, min, rev, aoneCheckVersionCallback);

}

void AoneSdkProxy::aoneCreateRole(JNIEnv* env, jobject thiz, jstring name, jint type, jobject feature, jint gId, jobject listener, std::string methodName)
{
	LOGD("AoneSdkProxy CreateRole begin");

	jclass class_hashTable = env->GetObjectClass(feature);
	jmethodID method_hashTable_get = env->GetMethodID(class_hashTable, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
	jmethodID method_keys = env->GetMethodID(class_hashTable, "keys", "()Ljava/util/Enumeration;");
	jclass class_object = env->FindClass("java/lang/Object");
	jmethodID method_toString = env->GetMethodID(class_object, "toString", "()Ljava/lang/String;");
	jclass class_enumeration = env->FindClass("java/util/Enumeration");
	jmethodID method_hasMoreElements = env->GetMethodID(class_enumeration, "hasMoreElements", "()Z");
	jmethodID method_nextElement = env->GetMethodID(class_enumeration, "nextElement", "()Ljava/lang/Object;");

	jobject jkey, jvalue;
	jstring jkeyStr, jvalueStr;
	const char *key = NULL;
	const char *value = NULL;
	std::map<std::string, std::string> info;
	jobject keys = env->CallObjectMethod(feature, method_keys);
	while (env->CallBooleanMethod(keys, method_hasMoreElements)) {
		jkey = env->CallObjectMethod(keys, method_nextElement);
		jkeyStr = (jstring)env->CallObjectMethod(jkey, method_toString);
		jvalue = env->CallObjectMethod(feature, method_hashTable_get, jkeyStr);
		jvalueStr = (jstring)env->CallObjectMethod(jvalue, method_toString);
		key = env->GetStringUTFChars(jkeyStr, NULL);
		value = env->GetStringUTFChars(jvalueStr, NULL);
		info[key] = value;
		env->ReleaseStringUTFChars(jkeyStr, key);
		env->ReleaseStringUTFChars(jvalueStr, value);
	}

	const char* nameCstr = env->GetStringUTFChars(name, NULL);
	s_env = env;
	set_listener(listener, &s_listener, env);
	s_methodName = methodName;
	aonesdk::AoneSDK::getInstance()->createRoleAsync(nameCstr, type, info, gId, aoneCallback);
	env->ReleaseStringUTFChars(name, nameCstr);

}

jint AoneSdkProxy::aoneCreateRoleCP(JNIEnv* env, jobject thiz, jstring roleID, jstring name, jint level, jobject feature, jstring groupID, jstring groupName, jint roleVip)
{
	LOGD("AoneSdkProxy CreateRole begin");

	jclass class_hashTable = env->GetObjectClass(feature);
	jmethodID method_hashTable_get = env->GetMethodID(class_hashTable, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
	jmethodID method_keys = env->GetMethodID(class_hashTable, "keys", "()Ljava/util/Enumeration;");
	jclass class_object = env->FindClass("java/lang/Object");
	jmethodID method_toString = env->GetMethodID(class_object, "toString", "()Ljava/lang/String;");
	jclass class_enumeration = env->FindClass("java/util/Enumeration");
	jmethodID method_hasMoreElements = env->GetMethodID(class_enumeration, "hasMoreElements", "()Z");
	jmethodID method_nextElement = env->GetMethodID(class_enumeration, "nextElement", "()Ljava/lang/Object;");

	jobject jkey, jvalue;
	jstring jkeyStr, jvalueStr;
	const char *key = NULL;
	const char *value = NULL;
	std::map<std::string, std::string> info;
	jobject keys = env->CallObjectMethod(feature, method_keys);
	while (env->CallBooleanMethod(keys, method_hasMoreElements)) {
		jkey = env->CallObjectMethod(keys, method_nextElement);
		jkeyStr = (jstring)env->CallObjectMethod(jkey, method_toString);
		jvalue = env->CallObjectMethod(feature, method_hashTable_get, jkeyStr);
		jvalueStr = (jstring)env->CallObjectMethod(jvalue, method_toString);
		key = env->GetStringUTFChars(jkeyStr, NULL);
		value = env->GetStringUTFChars(jvalueStr, NULL);
		info[key] = value;
		env->ReleaseStringUTFChars(jkeyStr, key);
		env->ReleaseStringUTFChars(jvalueStr, value);
	}

	const char* nameCstr = env->GetStringUTFChars(name, NULL);
	const char* groupNameCstr = env->GetStringUTFChars(groupName, NULL);
	const char* roleIDCstr = env->GetStringUTFChars(roleID, NULL);
	const char* groupIDCstr = env->GetStringUTFChars(groupID, NULL);
	return aonesdk::AoneSDK::getInstance()->createRoleCP(roleIDCstr, nameCstr, level, info, groupIDCstr, groupNameCstr, roleVip);
	env->ReleaseStringUTFChars(name, nameCstr);
	env->ReleaseStringUTFChars(groupName, groupNameCstr);
	env->ReleaseStringUTFChars(roleID, roleIDCstr);
	env->ReleaseStringUTFChars(groupID, groupIDCstr);
}

void AoneSdkProxy::aoneSubmitRoleData(JNIEnv* env, jobject thiz, jstring roleID, jstring name, jint level, jstring groupID, jstring groupName, jint roleVip, jboolean isLogin, jstring action,jlong roleCTime)
{
	LOGD("AoneSdkProxy SubmitRoleData begin");
	const char* nameCstr = env->GetStringUTFChars(name, NULL);
	LOGD("%s", nameCstr);
	const char* groupNameCstr = env->GetStringUTFChars(groupName, NULL);
	LOGD("%s", groupNameCstr);
	const char* roleIDCstr = env->GetStringUTFChars(roleID, NULL);
	const char* groupIDCstr = env->GetStringUTFChars(groupID, NULL);
	const char* actionCstr = env->GetStringUTFChars(action, NULL);
	aonesdk::AoneSDK::getInstance()->thirdPartySubmitRoleData(roleIDCstr, nameCstr, level, groupIDCstr, groupNameCstr, roleVip, isLogin, actionCstr,roleCTime);
	env->ReleaseStringUTFChars(name, nameCstr);
	env->ReleaseStringUTFChars(groupName, groupNameCstr);
	env->ReleaseStringUTFChars(roleID, roleIDCstr);
	env->ReleaseStringUTFChars(groupID, groupIDCstr);
}

void AoneSdkProxy::aoneSelectRoleAndGroup(JNIEnv* env, jobject thiz, jint roleId, jint gId, jobject listener, std::string methodName)
{
	LOGD("AoneSdkProxy SelectRoleAndGroup begin");

	s_env = env;
	set_listener(listener, &s_selectRoleAndGroupListener, env);
	s_methodName = methodName;

	aonesdk::AoneSDK::getInstance()->selectRoleAndGroupAsync(roleId, gId, aoneSelectRoleAndGroupListenerCallback);

}

void AoneSdkProxy::aonePullGameGroup(JNIEnv* env, jobject thiz, jobject listener, std::string methodName)
{
	LOGD("AoneSdkProxy PullGameGroup begin");

	s_env = env;
	set_listener(listener, &s_listener, env);
	s_methodName = methodName;

	aonesdk::AoneSDK::getInstance()->pullGameGroupAsync(aoneCallback);

}

void AoneSdkProxy::aoneLogout(JNIEnv* env, jobject thiz, jobject listener, std::string methodName)
{
	LOGD("AoneSdkProxy logout begin");

	s_env = env;
	set_listener(listener, &s_listener, env);
	s_methodName = methodName;

	aonesdk::AoneSDK::getInstance()->logout(aoneCallback);

}

void AoneSdkProxy::aoneShare(JNIEnv *env, jobject thiz, jobject cpInfo, jobject listener, std::string methodName)
{
	LOGD("AoneSdkProxy share begin");
	s_env = env;
	set_listener(listener, &s_shareListener, env);
	s_methodName = methodName;
	jclass class_hashTable = env->GetObjectClass(cpInfo);
	jmethodID method_hashTable_get = env->GetMethodID(class_hashTable, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
	jmethodID method_keys = env->GetMethodID(class_hashTable, "keys", "()Ljava/util/Enumeration;");
	jclass class_object = env->FindClass("java/lang/Object");
	jmethodID method_toString = env->GetMethodID(class_object, "toString", "()Ljava/lang/String;");
	jclass class_enumeration = env->FindClass("java/util/Enumeration");
	jmethodID method_hasMoreElements = env->GetMethodID(class_enumeration, "hasMoreElements", "()Z");
	jmethodID method_nextElement = env->GetMethodID(class_enumeration, "nextElement", "()Ljava/lang/Object;");
	jobject jkey, jvalue;
	jstring jkeyStr, jvalueStr;
	const char *key = NULL;
	const char *value = NULL;
	std::map<std::string, std::string> info;
	jobject keys = env->CallObjectMethod(cpInfo, method_keys);
	while (env->CallBooleanMethod(keys, method_hasMoreElements)) {
		jkey = env->CallObjectMethod(keys, method_nextElement);
		jkeyStr = (jstring)env->CallObjectMethod(jkey, method_toString);
		jvalue = env->CallObjectMethod(cpInfo, method_hashTable_get, jkeyStr);
		jvalueStr = (jstring)env->CallObjectMethod(jvalue, method_toString);
		key = env->GetStringUTFChars(jkeyStr, NULL);
		value = env->GetStringUTFChars(jvalueStr, NULL);
		info[key] = value;
		env->ReleaseStringUTFChars(jkeyStr, key);
		env->ReleaseStringUTFChars(jvalueStr, value);
	}
	LOGD("AoneSdkProxy share begin111");
	aonesdk::AoneSDK::getInstance()->share(info, aoneShareCallback);
}


void AoneSdkProxy::aoneGetAdTrackLink(JNIEnv *env, jobject thiz, jstring userData, jobject listener,std::string methodName)
{
	LOGI("AoneSdkProxy aoneGetAdTrackLink begin");
	s_env = env;
	set_listener(listener, &s_listener, env);
	s_methodName = methodName;

	const char * user_data = env -> GetStringUTFChars(userData, NULL);

	aonesdk::AoneClient::GetAdTrackLink(user_data, aoneGetAdTrackLinkCallback);
}

jstring AoneSdkProxy::aoneGetAppLink(JNIEnv *env, jobject thiz)
{
	LOGD("AoneSdkProxy aoneGetAppLink begin");
	string link = aonesdk::AoneClient::GetAppLink();
	return env -> NewStringUTF(link.c_str());
}

void AoneSdkProxy::aoneGetAdSource(JNIEnv *env, jobject thiz, jstring userData,  jobject listener, std::string methodName)
{
	LOGD("AoneSdkProxy GetAdSource begin");
	s_env = env;
	set_listener(listener, &s_listener, env);
	s_methodName = methodName;

	string user_data = env -> GetStringUTFChars(userData, NULL);
	aonesdk::AoneClient::GetAdSource(user_data,aoneGetAdSourceCallback);

}

jstring AoneSdkProxy::aoneEmbedQR(JNIEnv *env, jobject thiz, jstring src, jstring info, int x, int y, int w, int h)
{
	LOGD("AoneSdkProxy embedQR begin");
	const char* s_path = env -> GetStringUTFChars(src, NULL);
	const char* s_info = env -> GetStringUTFChars(info, NULL);

	//string ret_path = aonesdk::AoneClient::EmbedQR(s_path, s_info, x, y, w, h);
	string ret_path = "";

	return env -> NewStringUTF(ret_path.c_str());
}
void AoneSdkProxy::aoneImagePicker(int code)
{
	LOGD("AoneSdkProxy ImagePicker begin");
	AoneAndroidImagePicker::getInstance()->AonePickerCallback(code);
}

void AoneSdkProxy::aoneUpdateCallback()
{
	LOGD("AoneSdkProxy update force begin");
	uuDevice::aoneUpdateCallback();
}

void AoneSdkProxy::aoneAnnounceCallback()
{
	LOGD("AoneSdkProxy announce  begin");
	uuDevice::aoneAnnounceCallback();
}

void AoneSdkProxy::aoneLogEvent(JNIEnv *env, jobject thiz, jstring eventId)
{
	LOGD("AoneSdkProxy aoneLogEvent  begin");

	const char* s_eventId = env->GetStringUTFChars(eventId, NULL);
	aonesdk::AoneClient::LogEvent(s_eventId);
}

void AoneSdkProxy::aoneLogEvent(JNIEnv *env, jobject thiz, jstring eventId, jobject cpInfo)
{
	LOGD("AoneSdkProxy aoneLogEvent params  begin");

	jclass class_hashTable = env->GetObjectClass(cpInfo);
	jmethodID method_hashTable_get = env->GetMethodID(class_hashTable, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
	jmethodID method_keys = env->GetMethodID(class_hashTable, "keys", "()Ljava/util/Enumeration;");
	jclass class_object = env->FindClass("java/lang/Object");
	jmethodID method_toString = env->GetMethodID(class_object, "toString", "()Ljava/lang/String;");
	jclass class_enumeration = env->FindClass("java/util/Enumeration");
	jmethodID method_hasMoreElements = env->GetMethodID(class_enumeration, "hasMoreElements", "()Z");
	jmethodID method_nextElement = env->GetMethodID(class_enumeration, "nextElement", "()Ljava/lang/Object;");

	jobject jkey, jvalue;
	jstring jkeyStr, jvalueStr;
	const char *key = NULL;
	const char *value = NULL;
	std::map<std::string, std::string> info;
	jobject keys = env->CallObjectMethod(cpInfo, method_keys);
	while (env->CallBooleanMethod(keys, method_hasMoreElements)) {
		jkey = env->CallObjectMethod(keys, method_nextElement);
		jkeyStr = (jstring)env->CallObjectMethod(jkey, method_toString);
		jvalue = env->CallObjectMethod(cpInfo, method_hashTable_get, jkeyStr);
		jvalueStr = (jstring)env->CallObjectMethod(jvalue, method_toString);
		key = env->GetStringUTFChars(jkeyStr, NULL);
		value = env->GetStringUTFChars(jvalueStr, NULL);
		info[key] = value;
		env->ReleaseStringUTFChars(jkeyStr, key);
		env->ReleaseStringUTFChars(jvalueStr, value);
	}

	const char* s_eventId = env->GetStringUTFChars(eventId, NULL);
	aonesdk::AoneClient::LogEvent(s_eventId, info);
}

void AoneSdkProxy::aoneQueryPaynoStatusCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("AoneSDK callback, type: %d", type);
	JNIEnv* env = NULL;
	s_jvm->AttachCurrentThread(&env, 0);
	aoneCallJavaListener(env, s_queryPaynoStatusListener, s_methodName, type, dataMap);
}

void AoneSdkProxy::aoneQueryPaynoStatus(JNIEnv *env, jobject thiz, jint appId,jstring payno, jobject listener, std::string methodName){
	s_env = env;
	set_listener(listener, &s_queryPaynoStatusListener, env);
	s_methodName = methodName;
	const char* paynoCstr = env->GetStringUTFChars(payno, NULL);
	aonesdk::AoneSDK::getInstance()->queryPayNoStatus(appId,paynoCstr, aoneQueryPaynoStatusCallback);
	env->ReleaseStringUTFChars(payno, paynoCstr);
}

void AoneSdkProxy::recordDevicePoint(JNIEnv *env, jobject thiz, jint sdkPoint){
	s_env = env;
	aonesdk::AoneSDK::getInstance()->recordDevicePoint(sdkPoint);
}

void AoneSdkProxy::recordRolePoint(JNIEnv *env, jobject thiz, jint gamePoint){
	s_env = env;
	aonesdk::AoneSDK::getInstance()->recordRolePoint(gamePoint);
}

void AoneSdkProxy::aoneAddNotify(JNIEnv* env, jobject thiz, jobject infos){
	LOGD("aoneAddNotify");
	jclass class_hashTable = env->GetObjectClass(infos);
	jmethodID method_hashTable_get = env->GetMethodID(class_hashTable, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
	jmethodID method_keys = env->GetMethodID(class_hashTable, "keys", "()Ljava/util/Enumeration;");
	jclass class_object = env->FindClass("java/lang/Object");
	jmethodID method_toString = env->GetMethodID(class_object, "toString", "()Ljava/lang/String;");
	jclass class_enumeration = env->FindClass("java/util/Enumeration");
	jmethodID method_hasMoreElements = env->GetMethodID(class_enumeration, "hasMoreElements", "()Z");
	jmethodID method_nextElement = env->GetMethodID(class_enumeration, "nextElement", "()Ljava/lang/Object;");

	jobject jkey, jvalue;
	jstring jkeyStr, jvalueStr;
	const char *key = NULL;
	const char *value = NULL;
	std::map<std::string, std::string> info;
	jobject keys = env->CallObjectMethod(infos, method_keys);
	while (env->CallBooleanMethod(keys, method_hasMoreElements)) {
		jkey = env->CallObjectMethod(keys, method_nextElement);
		jkeyStr = (jstring)env->CallObjectMethod(jkey, method_toString);
		jvalue = env->CallObjectMethod(infos, method_hashTable_get, jkeyStr);
		jvalueStr = (jstring)env->CallObjectMethod(jvalue, method_toString);
		key = env->GetStringUTFChars(jkeyStr, NULL);
		value = env->GetStringUTFChars(jvalueStr, NULL);
		info[key] = value;
		env->ReleaseStringUTFChars(jkeyStr, key);
		env->ReleaseStringUTFChars(jvalueStr, value);
	}

	aonesdk::AoneClient::AddNotify(info);
}

void AoneSdkProxy::aoneClearNotify(JNIEnv *env, jobject thiz, jint notifyId){
	aonesdk::AoneClient::ClearNotify(notifyId);
}
void AoneSdkProxy::aoneClearAllNotify(JNIEnv *env, jobject thiz){
	aonesdk::AoneClient::ClearAllNotify();
}
jstring AoneSdkProxy::aoneGetChannelParam(JNIEnv *env, jobject thiz,jstring paramName){
	const char *key = env->GetStringUTFChars(paramName, NULL);
	jstring value = env->NewStringUTF(aonesdk::AoneClient::GetChannelParam(key).c_str());
	env->ReleaseStringUTFChars(paramName, key);
	return value;
}
void AoneSdkProxy::aoneJoinQGroup(JNIEnv *env, jobject thiz){
	aonesdk::AoneClient::joinQGroup();
}

void AoneSdkProxy::aoneCallFunc(JNIEnv *env, jobject thiz,jstring FuncName){
	const char *key = env->GetStringUTFChars(FuncName, NULL);
	aonesdk::AoneClient::callFunc(key);
	env->ReleaseStringUTFChars(FuncName, key);
}
int AoneSdkProxy::aoneCallIntFunc(JNIEnv *env, jobject thiz,jstring FuncName){
	const char *key = env->GetStringUTFChars(FuncName, NULL);
	int data = aonesdk::AoneClient::callIntFunc(key,NULL);
	env->ReleaseStringUTFChars(FuncName, key);
	return data;
}
jstring AoneSdkProxy::aoneCallStringFunc(JNIEnv *env, jobject thiz,jstring FuncName){
	const char *key = env->GetStringUTFChars(FuncName, NULL);
	jstring data = env->NewStringUTF(aonesdk::AoneClient::callStringFunc(key).c_str());
	env->ReleaseStringUTFChars(FuncName, key);
	return data;
}

void AoneSdkProxy::aoneShowAds(JNIEnv* env, jobject thiz,jobject infos,jint pos, jobject listener, std::string methodName)
{
	LOGD("AoneSdkProxy ShowAds begin");
	//*******************infos*******************
	jclass class_hashTable = env->GetObjectClass(infos);
	jmethodID method_hashTable_get = env->GetMethodID(class_hashTable, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
	jmethodID method_keys = env->GetMethodID(class_hashTable, "keys", "()Ljava/util/Enumeration;");
	jclass class_object = env->FindClass("java/lang/Object");
	jmethodID method_toString = env->GetMethodID(class_object, "toString", "()Ljava/lang/String;");
	jclass class_enumeration = env->FindClass("java/util/Enumeration");
	jmethodID method_hasMoreElements = env->GetMethodID(class_enumeration, "hasMoreElements", "()Z");
	jmethodID method_nextElement = env->GetMethodID(class_enumeration, "nextElement", "()Ljava/lang/Object;");

	jobject jkey, jvalue;
	jstring jkeyStr, jvalueStr;
	const char *key = NULL;
	const char *value = NULL;
	std::map<std::string, std::string> info;
	jobject keys = env->CallObjectMethod(infos, method_keys);
	while (env->CallBooleanMethod(keys, method_hasMoreElements)) {
		jkey = env->CallObjectMethod(keys, method_nextElement);
		jkeyStr = (jstring)env->CallObjectMethod(jkey, method_toString);
		jvalue = env->CallObjectMethod(infos, method_hashTable_get, jkeyStr);
		jvalueStr = (jstring)env->CallObjectMethod(jvalue, method_toString);
		key = env->GetStringUTFChars(jkeyStr, NULL);
		value = env->GetStringUTFChars(jvalueStr, NULL);
		info[key] = value;
		env->ReleaseStringUTFChars(jkeyStr, key);
		env->ReleaseStringUTFChars(jvalueStr, value);
	}
	//*******************infos*******************

	s_env = env;
	set_listener(listener, &s_adsListener, env);
	s_methodName = methodName;
	aonesdk::AoneSDK::getInstance()->showAds(info, pos, aoneShowAdsCallback);
}

void AoneSdkProxy::aoneShowAdsCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("AoneSDK login callback, type: %d", type);
	JNIEnv* env = NULL;
	s_jvm->AttachCurrentThread(&env, 0);
	aoneCallJavaListener(env, s_adsListener, s_methodName, type, dataMap);
}
//static void aoneSetAdsSdkName(JNIEnv *env, jobject thiz,jstring adsSdkName);
void AoneSdkProxy::aoneSetAdsSdk(JNIEnv *env, jobject thiz,jstring adsSdkName)
{
	const char *key = env->GetStringUTFChars(adsSdkName, NULL);
	aonesdk::AoneClient::SetAdsSdk(key);
	env->ReleaseStringUTFChars(adsSdkName, key);
}

void AoneSdkProxy::aoneQuickRegisterAsync(JNIEnv *env, jobject thiz,jobject listener, std::string methodName)
{
	s_env = env;
	//s_quickRegisterListener = env->NewGlobalRef(listener);
	set_listener(listener, &s_quickRegisterListener, env);
	s_methodName = methodName;
	aonesdk::AoneClient::QuickRegister(aoneQuickRegisterCallback);
}

void AoneSdkProxy::aoneQuickRegisterCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("AoneSDK quick register callback, type: %d", type);
	JNIEnv* env = NULL;
	s_jvm->AttachCurrentThread(&env, 0);
	aoneCallJavaListener(env, s_quickRegisterListener, s_methodName, type, dataMap);
}

void AoneSdkProxy::aoneBindOtherAccount(JNIEnv *env, jobject thiz, std::string newAccount,jobject listener, std::string methodName)
{
	s_env = env;
	set_listener(listener, &s_bindOtherAccountListener, env);
	s_methodName = methodName;

	aonesdk::AoneClient::AoneBindOtherAccount(newAccount,aoneBindOtherAccountCallback);
}

void AoneSdkProxy::aoneBindOtherAccountCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("aone bindOtherAccount callback, type: %d", type);
	JNIEnv* env = NULL;
	s_jvm->AttachCurrentThread(&env, 0);
	aoneCallJavaListener(env, s_bindOtherAccountListener, s_methodName, type, dataMap);
}

void AoneSdkProxy::aoneUnbindOtherAccount(JNIEnv *env, jobject thiz,jobject listener, std::string methodName)
{
	s_env = env;
	set_listener(listener, &s_unbindOtherAccountListener, env);
	s_methodName = methodName;

	aonesdk::AoneClient::AoneUnbindOtherAccount(aoneUnbindOtherAccountCallback);
}

void AoneSdkProxy::aoneUnbindOtherAccountCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("aone unbindOtherAccount callback, type: %d", type);
	JNIEnv* env = NULL;
	s_jvm->AttachCurrentThread(&env, 0);
	aoneCallJavaListener(env, s_unbindOtherAccountListener, s_methodName, type, dataMap);
}
jint AoneSdkProxy::aoneQueryBindOtherAccountSync(JNIEnv *env, jobject thiz)
{
	s_env = env;
	return aonesdk::AoneClient::AoneQueryBindOtherAccount();
}
void AoneSdkProxy::aoneQueryBindOtherAccount(JNIEnv *env, jobject thiz,jobject listener, std::string methodName)
{
	s_env = env;
	set_listener(listener, &s_queryBindOtherAccountListener, env);
	s_methodName = methodName;

	aonesdk::AoneClient::AoneQueryBindOtherAccount(aoneQueryBindOtherAccountCallback);
}

void AoneSdkProxy::aoneQueryBindOtherAccountCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("aone unbindOtherAccount callback, type: %d", type);
	JNIEnv* env = NULL;
	s_jvm->AttachCurrentThread(&env, 0);
	aoneCallJavaListener(env, s_queryBindOtherAccountListener, s_methodName, type, dataMap);
}

void AoneSdkProxy::aoneChangePassword(JNIEnv *env, jobject thiz, std::string oldPassword, std::string newPassword,jobject listener, std::string methodName)
{
	s_env = env;
	set_listener(listener, &s_changePasswordListener, env);
	s_methodName = methodName;

	aonesdk::AoneClient::AoneChangePassword(oldPassword,newPassword,aoneChangePasswordCallback);
}

void AoneSdkProxy::aoneChangePasswordCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("aone change password callback, type: %d", type);
	JNIEnv* env = NULL;
	s_jvm->AttachCurrentThread(&env, 0);
	aoneCallJavaListener(env, s_changePasswordListener, s_methodName, type, dataMap);
}

void AoneSdkProxy::aoneThirdPartyLogin(JNIEnv *env, jobject thiz,jobject listener, std::string methodName)
{
	s_env = env;
	set_listener(listener, &s_thirdPartyLoginListener, env);
	s_methodName = methodName;

	aonesdk::AoneClient::ThirdParthLogin(aoneThirdPartyLoginCallback);
}

void AoneSdkProxy::aoneThirdPartyLoginCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("aone thirdparty login callback, type: %d", type);
	JNIEnv* env = NULL;
	s_jvm->AttachCurrentThread(&env, 0);
	aoneCallJavaListener(env, s_thirdPartyLoginListener, s_methodName, type, dataMap);
}


void AoneSdkProxy::aoneCreateInheritance(JNIEnv *env, jobject thiz, std::string password,jobject listener, std::string methodName)
{
	s_env = env;
	set_listener(listener, &s_creatreInheritanceListener, env);
	s_methodName = methodName;

	aonesdk::AoneClient::AoneCreateInheritance(password,aoneCreateInheritanceCallback);
}

void AoneSdkProxy::aoneCreateInheritanceCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("aone inheritance code callback, type: %d", type);
	JNIEnv* env = NULL;
	s_jvm->AttachCurrentThread(&env, 0);
	aoneCallJavaListener(env, s_creatreInheritanceListener, s_methodName, type, dataMap);
}


void AoneSdkProxy::aoneLoginWithAccount(JNIEnv *env, jobject thiz, std::string account,std::string password,jobject listener, std::string methodName)
{
	s_env = env;
	set_listener(listener, &s_aoneLoginListener, env);
	s_methodName = methodName;

	aonesdk::AoneClient::AoneLogin(account,password,aoneLoginWithAccountCallback);
}

void AoneSdkProxy::aoneLoginWithAccountCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("aone inheritance code callback, type: %d", type);
	JNIEnv* env = NULL;
	s_jvm->AttachCurrentThread(&env, 0);
	aoneCallJavaListener(env, s_aoneLoginListener, s_methodName, type, dataMap);
}

void AoneSdkProxy::aoneCleanRoleData(JNIEnv *env, jobject thiz)
{
	LOGD("aone clean role data");
	aonesdk::AoneSDK::getInstance()->cleanRoleData();
}

void AoneSdkProxy::aoneStarReviewInApp(JNIEnv *env, jobject thiz)
{
	LOGD("aone star review in app");
	aonesdk::AoneClient::starReviewInApp("");
}

void AoneSdkProxy::realnameInfo(JNIEnv *env, jobject thiz,jobject listener,std::string methodName)
{
	LOGD("aone Account Identity info");
	s_env = env;
	set_listener(listener, &s_realnameInfoListener, env);
	s_methodName = methodName;
	aonesdk::AoneClient::realnameInfo(realnameInfoCallback);
}

void AoneSdkProxy::realnameInfoCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("aone Account Identity info callback, type: %d", type);
	JNIEnv* env = NULL;
	s_jvm->AttachCurrentThread(&env, 0);
	aoneCallJavaListener(env, s_realnameInfoListener, s_methodName, type, dataMap);
}

void AoneSdkProxy::realnameCertificate(JNIEnv *env, jobject thiz,jstring identity_number,
		jstring real_name,jobject listener,std::string methodName)
{
	LOGD("aone realname");
	s_env = env;
	set_listener(listener, &s_realnameCertificateListener, env);
	s_methodName = methodName;
	const char* identity_numberStr = env->GetStringUTFChars(identity_number, NULL);
	const char* real_nameStr = env->GetStringUTFChars(real_name, NULL);
	aonesdk::AoneClient::realnameCertificate(identity_numberStr,real_nameStr,realnameCertificateCallback);
	env->ReleaseStringUTFChars(identity_number, identity_numberStr);
	env->ReleaseStringUTFChars(real_name, real_nameStr);
}


void AoneSdkProxy::realnameCertificateCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("aone realname certificate callback, type: %d", type);
		JNIEnv* env = NULL;
		s_jvm->AttachCurrentThread(&env, 0);
		aoneCallJavaListener(env, s_realnameCertificateListener, s_methodName, type, dataMap);
}

void AoneSdkProxy::requestPlayerExtraInfo(JNIEnv *env, jobject thiz,jobject listener,std::string methodName)
{
	LOGD("request PlayerExtraInfo");
	s_env = env;
	set_listener(listener, &s_requestPlayerExtraInfoListener, env);
	s_methodName = methodName;
	aonesdk::AoneClient::requestPlayerExtraInfo(requestPlayerExtraInfoCallback);
}

void AoneSdkProxy::requestPlayerExtraInfoCallback(int type, std::map<std::string, std::string> dataMap)
{
	LOGD("request PlayerExtraInfo callback, type: %d", type);
		JNIEnv* env = NULL;
		s_jvm->AttachCurrentThread(&env, 0);
		aoneCallJavaListener(env, s_requestPlayerExtraInfoListener, s_methodName, type, dataMap);
}

/*****************************
 * AoneClient Method Field End
 ****************************/
