#include "AoneDeepLink.h"
#include "PluginJniHelper.h"

#define DEEP_LINK_CLASS_NAME "com/aonesoft/lib/AoneDeepLink"
AoneDeepLink::AoneDeepLink(){
}

AoneDeepLink::~AoneDeepLink(){}

std::string callStringFuncWithStringParam(){
	aonesdk::PluginJniMethodInfo methodInfo;
	std::string ret = "";
	if(aonesdk::PluginJniHelper::getStaticMethodInfo(methodInfo, DEEP_LINK_CLASS_NAME, "getDeepLinkData", "()Ljava/lang/String;")){
		jstring strRet = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID,methodInfo.methodID);
		ret = aonesdk::PluginJniHelper::jstring2string(strRet);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
	
	printf("ret===platform===%s",ret.c_str());
	
	return ret;
}

std::string AoneDeepLink::getDeepLinkData(){
	return callStringFuncWithStringParam();
}



