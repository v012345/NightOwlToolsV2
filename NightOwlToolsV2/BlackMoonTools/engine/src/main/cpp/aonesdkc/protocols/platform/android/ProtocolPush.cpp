#include "ProtocolPush.h"
#include "PluginJniHelper.h"
#include <android/log.h>
#include "PluginUtils.h"
#include "PluginJavaData.h"

namespace aonesdk { namespace plugin {

extern "C" {
	JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_PushWrapper_nativeOnPushResult(JNIEnv*  env, jobject thiz, jstring className, jint ret, jstring msg)
	{
		std::string strMsg = PluginJniHelper::jstring2string(msg);
		std::string strClassName = PluginJniHelper::jstring2string(className);
		PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
		PluginUtils::outputLog("aonesdk info ProtocolPush", "nativeOnPushResult(), Get plugin ptr : %p", pPlugin);
		if (pPlugin != NULL)
		{
			PluginUtils::outputLog("aonesdk info ProtocolPush", "nativeOnPushResult(), Get plugin name : %s", pPlugin->getPluginName());
			ProtocolPush* pPush = dynamic_cast<ProtocolPush*>(pPlugin);
			if (pPush != NULL)
			{
			    PushResultListener* listener = pPush->getPushResultListener();
			    if (NULL != listener)
			    {
			        listener->onPushResult(pPush, (PushResultCode)ret, strMsg.c_str());
			    }
			    else
			    {
			        PluginUtils::outputLog("aonesdk info ProtocolPush", "Listener of plugin %s not set correctly", pPlugin->getPluginName());
			    }
			}
		}
	}
}

ProtocolPush::ProtocolPush()
: _listener(NULL)
{
}

ProtocolPush::~ProtocolPush()
{
}

void ProtocolPush::configDeveloperInfo(TUserDeveloperInfo devInfo)
{
    if (devInfo.empty())
    {
        PluginUtils::outputLog("aonesdk info ProtocolPush", "The developer info is empty!");
        // return;
    }
    // else
    {
        PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
    	PluginJniMethodInfo t;
        if (PluginJniHelper::getMethodInfo(t
    		, pData->jclassName.c_str()
    		, "configDeveloperInfo"
    		, "(Ljava/util/Hashtable;)V"))
    	{
        	// generate the hashtable from map
        	jobject obj_Map = PluginUtils::createJavaMapObject(&devInfo);

            // invoke java method
            t.env->CallVoidMethod(pData->jobj, t.methodID, obj_Map);
            t.env->DeleteLocalRef(obj_Map);
            t.env->DeleteLocalRef(t.classID);
        }
    }
}

void ProtocolPush::startPush()
{
    PluginUtils::callJavaFunctionWithName(this, "startPush");
}

void ProtocolPush::closePush()
{
    PluginUtils::callJavaFunctionWithName(this, "closePush");
}

void ProtocolPush::setAlias(const char *alias)
{
    jstring str = PluginUtils::getEnv()->NewStringUTF(alias);
    PluginUtils::callJavaFunctionWithName_oneParam(this, "setAlias", "(Ljava/lang/String;)V", str);
    PluginUtils::getEnv()->DeleteLocalRef(str);
}

void ProtocolPush::delAlias(const char *alias)
{
    jstring str = PluginUtils::getEnv()->NewStringUTF(alias);
    PluginUtils::callJavaFunctionWithName_oneParam(this, "delAlias", "(Ljava/lang/String;)V", str);
    PluginUtils::getEnv()->DeleteLocalRef(str);
}

void ProtocolPush::setTags(const char *tags)
{
    jstring str = PluginUtils::getEnv()->NewStringUTF(tags);
    PluginUtils::callJavaFunctionWithName_oneParam(this, "setTags", "(Ljava/lang/String;)V", str);
    PluginUtils::getEnv()->DeleteLocalRef(str);
}

void ProtocolPush::delTags(const char *tags)
{
    jstring str = PluginUtils::getEnv()->NewStringUTF(tags);
    PluginUtils::callJavaFunctionWithName_oneParam(this, "delTags", "(Ljava/lang/String;)V", str);
    PluginUtils::getEnv()->DeleteLocalRef(str);
}

}} // namespace aonesdk { namespace plugin {
