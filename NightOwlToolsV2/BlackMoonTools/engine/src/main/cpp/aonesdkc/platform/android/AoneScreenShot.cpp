#include "AoneScreenShot.h"
#include "PluginJniHelper.h"

#define SCREEN_SHOOT_CLASS_NAME "com/aonesoft/lib/AoneScreenShot"
AoneScreenShot::AoneScreenShot(){
}

AoneScreenShot::~AoneScreenShot(){}

void AoneScreenShot::takeScreenShot(const char* path){
	callFuncWithStringParam(path);
}

void AoneScreenShot::callFuncWithStringParam(const char* path){
	aonesdk::PluginJniMethodInfo methodInfo;
	
	if (!aonesdk::PluginJniHelper::getStaticMethodInfo(methodInfo, SCREEN_SHOOT_CLASS_NAME, "takeScreenShot", "(Ljava/lang/String;)V")) return;
	
	// 构建参数
	jstring path_str = methodInfo.env->NewStringUTF(path);
	
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, path_str);
	
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	
	if (path_str) methodInfo.env->DeleteLocalRef(path_str);
}