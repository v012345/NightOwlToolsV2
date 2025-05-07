#include "ProtocolVoice.h"
#include "PluginJniHelper.h"
#include <android/log.h>
#include "PluginUtils.h"
#include "PluginJavaData.h"

namespace aonesdk {namespace plugin {
extern "C"{
	JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_VoiceWrapper_nativeOnVoiceEvent(JNIEnv* env, jobject thiz, jstring className, jint type, jint code)
	{
		std::string strClassName = PluginJniHelper::jstring2string(className);
		PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
		PluginUtils::outputLog("aonesdk info ProtocolVoice", "nativeOnVoiceEvent(), get plugin ptr:, %p", pPlugin);

		if(NULL != pPlugin)
		{
			PluginUtils::outputLog("aonesdk info ProtocolVoice", "nativeOnVoiceEvent(), get plugin name: %s", pPlugin->getPluginName());
			ProtocolVoice* pVoice = dynamic_cast<ProtocolVoice*>(pPlugin);
			if(NULL != pVoice)
			{
				VoiceActionListener* listener = pVoice->getActionListener();
				if(NULL != listener)
					listener->onVoiceEvent(pVoice, VocieEventType(type), VoiceErrorCode(code));
				else
					PluginUtils::outputLog("aonesdk info ProtocolVoice", "Listener of plugin %s not set correctly", pPlugin->getPluginName());
			}
		}
	}
}


ProtocolVoice::ProtocolVoice() : _listener(NULL)
{
}

ProtocolVoice::~ProtocolVoice()
{
}

void ProtocolVoice::configDeveloperInfo(TVoiceDeveloperInfo devInfo)
{
	if (devInfo.empty())
    {
        PluginUtils::outputLog("aonesdk info ProtocolVoice", "The developer info is empty!");
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

void ProtocolVoice::setVoiceParams(TVoiceDeveloperInfo devInfo)
{
	if (devInfo.empty())
    {
        PluginUtils::outputLog("aonesdk info ProtocolVoice", "The developer info is empty!");
        // return;
    }
    // else
    {
        PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
    	PluginJniMethodInfo t;
        if (PluginJniHelper::getMethodInfo(t
    		, pData->jclassName.c_str()
    		, "setVoiceParams"
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

void ProtocolVoice::start(const char* roomId)
{
	jstring jstr = PluginUtils::getEnv()->NewStringUTF(roomId);
	PluginUtils::callJavaFunctionWithName_oneParam(this, "start", "(Ljava/lang/String;)V", jstr);
	PluginUtils::getEnv()->DeleteLocalRef(jstr);
}

void ProtocolVoice::stop()
{
	PluginUtils::callJavaFunctionWithName(this, "stop");
}

bool ProtocolVoice::isMicMute()
{
	return PluginUtils::callJavaBoolFuncWithName(this, "isMicMute");
}

void ProtocolVoice::setMicMute(bool mute)
{
	PluginUtils::callJavaFunctionWithName_oneParam(this, "setMicMute", "(Z)V", mute);
}

int ProtocolVoice::getVolume()
{
	return PluginUtils::callJavaIntFuncWithName(this, "getVolume");
}

void ProtocolVoice::setVolume(int volume)
{
	PluginUtils::callJavaFunctionWithName_oneParam(this, "setMicMute", "(I)V", volume);
}

}}