
#include "PluginJniHelper.h"
#define FIX_IMAGE_CLASS_NAME "com/aonesoft/lib/AoneFixHeadImg"
#include "AoneAndroidImagePicker.h"

void callFuncWithParams(const char * path, int widthPixel, int heightPixel){
	aonesdk::PluginJniMethodInfo methodInfo;
	if (!aonesdk::PluginJniHelper::getStaticMethodInfo(methodInfo, 
		FIX_IMAGE_CLASS_NAME, "getHeadImage", 
		"(Ljava/lang/String;II)V")){
		return;
	}
	//构建参数
	jstring jCropPath = methodInfo.env->NewStringUTF(path);
	
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID,methodInfo.methodID,jCropPath,widthPixel,heightPixel);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	if (jCropPath) methodInfo.env->DeleteLocalRef(jCropPath);
	
}

void AoneAndroidImagePicker::start(const char * path, int widthPixel, int heightPixel, AONEPICKER_CB cb){
	m_cb = cb;
	callFuncWithParams(path,widthPixel,heightPixel);
}

void AoneAndroidImagePicker::AonePickerCallback(int code){
	
	m_cb(AonePickerStatus(code),NULL);
}

AoneAndroidImagePicker* AoneAndroidImagePicker::getInstance()
{
	static AoneAndroidImagePicker instance_;
	return &instance_;
}