#include "cpfile.h"
#include "cplog.h"
#include <jni.h>
#include <cstring>
#include <android/log.h>
#include <android/asset_manager_jni.h>
#include "platform/android/CCFileUtils-android.h"

#define  LOG_TAG    "cputils"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

extern "C"
{
	JavaVM* _JAVA_VM = NULL;
	#define MAX_PATH 1024
	static char _WRITE_PATH[MAX_PATH];
	static char _EXTERNAL_WRITE_PATH[MAX_PATH];

	JavaVM* cputils_get_jvm()
	{
    	return _JAVA_VM;
	}

	JNIEXPORT void JNICALL Java_indi_cputils_Helper_SetAssetsMgr(JNIEnv*  env, jobject thiz, jobject assetMgr) 
	{
		cputils::FileUtilsAndroid::setassetmanager(AAssetManager_fromJava(env, assetMgr));
	}

	JNIEXPORT void JNICALL Java_indi_cputils_Helper_SetWritablePath(JNIEnv*  env, jobject thiz, jstring jWritePath) 
	{
		int status = env->GetJavaVM(&_JAVA_VM);
		if(status != 0) {
			LOGD("get JavaVM failed!!!");
		}

        const char* tmpPath = env->GetStringUTFChars(jWritePath, NULL);
        strcpy(_WRITE_PATH, tmpPath);
        if (strlen(_WRITE_PATH) > 0)
        {
        	strcat(_WRITE_PATH, "/");
        }
        env->ReleaseStringUTFChars(jWritePath, tmpPath);
    }

    JNIEXPORT void JNICALL Java_indi_cputils_Helper_SetExternalWritablePath(JNIEnv*  env, jobject thiz, jstring jWritePath) 
	{
        const char* tmpPath = env->GetStringUTFChars(jWritePath, NULL);
        strcpy(_EXTERNAL_WRITE_PATH, tmpPath);
        if (strlen(_EXTERNAL_WRITE_PATH) > 0)
        {
        	strcat(_EXTERNAL_WRITE_PATH, "/");
        }
        env->ReleaseStringUTFChars(jWritePath, tmpPath);
    }

    JNIEXPORT void JNICALL Java_indi_cputils_Helper_InitLog(JNIEnv*  env, jobject thiz) 
	{
        cplog_init();
    }

	const char* cpfile_write_path()
	{
		return _WRITE_PATH;
	}

	const char* cpfile_external_path()
	{
		return _EXTERNAL_WRITE_PATH;
	}
}