#include <android/log.h>
#include "UUGenerateQRCode.h"
#include "PluginJniHelper.h"
using namespace aonesdk;

#define  LOG_TAG    "aonesdk"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define CALSS_NAME "com/aonesoft/lib/GenerateQRCode"

const char * generateQRCode(string srcImagePath , string content , int x , int y , int width , int height)
{
	LOGD("generateQRCode , path = %s, info = %s, x = %d, y = %d, w = %d , h = %d", srcImagePath.c_str(), content.c_str(), x, y, width, height);

	PluginJniMethodInfo methodInfo;
	if (!PluginJniHelper::getStaticMethodInfo(methodInfo, CALSS_NAME, "EmQRCode", "(Ljava/lang/String;Ljava/lang/String;IIII)Ljava/lang/String;"))
	{
		return "";
	}

    jstring src_image_path = methodInfo.env->NewStringUTF(srcImagePath.c_str());
    jstring content_info = methodInfo.env->NewStringUTF(content.c_str());

    jstring jstr = (jstring)methodInfo.env -> CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID, src_image_path, content_info , x, y, width, height);

    methodInfo.env -> DeleteLocalRef(methodInfo.classID);
    if (src_image_path) methodInfo.env -> DeleteLocalRef(src_image_path);
	if (content_info) methodInfo.env -> DeleteLocalRef(content_info);

    const char* str = methodInfo.env->GetStringUTFChars(jstr, 0);
//    LOGD("generateQRCode restult: %s", str);
	return str;
}