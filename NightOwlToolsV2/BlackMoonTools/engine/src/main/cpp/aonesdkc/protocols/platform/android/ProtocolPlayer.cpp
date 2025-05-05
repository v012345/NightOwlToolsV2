/****************************************************************************
Copyright (c) 2013 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "ProtocolPlayer.h"
#include "PluginJniHelper.h"
#include <android/log.h>
#include "PluginUtils.h"
#include "PluginJavaData.h"

namespace aonesdk { namespace plugin {

extern "C"{
	JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_PlayerWrapper_nativeOnPlayerStart(JNIEnv* env, jobject thiz, jstring className)
	{
		std::string strClassName = PluginJniHelper::jstring2string(className);
		PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
		PluginUtils::outputLog("aonesdk info ProtocolPlayer", "nativeOnPlayerStart(), get plugin ptr: , %p", pPlugin);

		if(NULL != pPlugin)
		{
			PluginUtils::outputLog("aonesdk info ProtocolPlayer", "nativeOnPlayerStart(), get plugin name: %s", pPlugin->getPluginName());
			ProtocolPlayer* pPlayer = dynamic_cast<ProtocolPlayer*>(pPlugin);
			if(NULL != pPlayer)
			{
				PlayerActionListener* listener = pPlayer->getActionListener();
				if(NULL != listener)
					listener->onPlayerStart(pPlayer);
				else
					PluginUtils::outputLog("aonesdk info ProtocolPlayer", "Listener of plugin %s not set correctly", pPlugin->getPluginName());
			}
		}
	}

	JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_PlayerWrapper_nativeOnPlayerStop(JNIEnv* env, jobject thiz, jstring className, jint ret)
	{
		std::string strClassName = PluginJniHelper::jstring2string(className);
		PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
		PluginUtils::outputLog("aonesdk info ProtocolPlayer", "nativeOnPlayerStop(), get plugin ptr: , %p", pPlugin);

		if(NULL != pPlugin)
		{
			PluginUtils::outputLog("aonesdk info ProtocolPlayer", "nativeOnPlayerStop(), get plugin name: %s", pPlugin->getPluginName());
			ProtocolPlayer* pPlayer = dynamic_cast<ProtocolPlayer*>(pPlugin);
			if(NULL != pPlayer)
			{
				PlayerActionListener* listener = pPlayer->getActionListener();
				if(NULL != listener)
					listener->onPlayerStop(pPlayer, (PlayerErrorCode)ret);
				else
					PluginUtils::outputLog("aonesdk info ProtocolPlayer", "Listener of plugin %s not set correctly", pPlugin->getPluginName());
			}
		}
	}
}

ProtocolPlayer::ProtocolPlayer()
: _listener(NULL)
{
}

ProtocolPlayer::~ProtocolPlayer()
{
}

void ProtocolPlayer::configDeveloperInfo(TPlayerDeveloperInfo devInfo)
{
	if (devInfo.empty())
    {
        PluginUtils::outputLog("aonesdk info ProtocolPlayer", "The developer info is empty!");
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

void ProtocolPlayer::setStreamParams(TPlayerDeveloperInfo devInfo)
{
	if (devInfo.empty())
    {
        PluginUtils::outputLog("aonesdk info ProtocolPlayer", "The developer info is empty!");
        // return;
    }
    // else
    {
        PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
    	PluginJniMethodInfo t;
        if (PluginJniHelper::getMethodInfo(t
    		, pData->jclassName.c_str()
    		, "setStreamParams"
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

void ProtocolPlayer::start()
{
	PluginUtils::callJavaFunctionWithName(this, "start");
}

void ProtocolPlayer::stop()
{
	PluginUtils::callJavaFunctionWithName(this, "stop");
}

void ProtocolPlayer::close()
{
	PluginUtils::callJavaFunctionWithName(this, "close");
}

void ProtocolPlayer::playSoundEffect(const char* pcmData, int len)
{
	if(len <= 0 || pcmData == NULL)
 	{
        PluginUtils::outputLog("aonesdk info ProtocolPlayer", "playSoundEffect data is invalid");
        return;
    }

 	JNIEnv* env = PluginUtils::getEnv();

 	jbyteArray jbArray = env->NewByteArray(len);

    jbyte *jby = env->GetByteArrayElements(jbArray, 0);

    memcpy(jby, pcmData, len);

 	env->SetByteArrayRegion(jbArray, 0, len, jby);
 	
    PluginUtils::callJavaFunctionWithName_oneParam(this, "playSoundEffect", "([B)V", jbArray);
    	
    PluginUtils::getEnv()->DeleteLocalRef(jbArray);
}

}} // namespace aonesdk { namespace plugin {
