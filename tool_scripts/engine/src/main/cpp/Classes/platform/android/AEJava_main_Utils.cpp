#include "platform/AEStdC.h"
#if AE_TARGET_PLATFORM == AE_PLATFORM_ANDROID
#include <stdlib.h>
#include <jni.h>
#include <android/log.h>
#include <string>
#include "platform/android/jni/JniHelper.h"
#include "platform/android/AEJava_main_Utils.h"

#define  LOG_TAG    "AEJava_main_Utils.cpp"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
static const std::string className = "main/Utils";
std::string getAndroidSdcardPath()
{
	return JniHelper::callStaticStringMethod(className, "getSdcardPath");
}
#endif