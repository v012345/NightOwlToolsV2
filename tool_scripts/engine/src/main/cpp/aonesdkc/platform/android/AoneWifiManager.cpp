#include "AoneWifiManager.h"
#include "PluginJniHelper.h"

#define WIFI_MANAGER_CLASS_NAME "com/aonesoft/lib/AoneWifiManager"

int callIntFunc(){
	aonesdk::PluginJniMethodInfo methodInfo;
	
	if (aonesdk::PluginJniHelper::getStaticMethodInfo(methodInfo, WIFI_MANAGER_CLASS_NAME, "getWifiRssi", "()I")){
		return methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID);
	}
	return 0;
}

int AoneWifiManager::getWifiRssi(){
	return callIntFunc();
}
