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
#include "ProtocolStream.h"
#include "PluginJniHelper.h"
#include <android/log.h>
#include "PluginUtils.h"
#include "PluginJavaData.h"

namespace aonesdk { namespace plugin {

extern "C"{
	JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_StreamWrapper_nativeOnInitCameraFail(JNIEnv* env, jobject thiz, jstring className)
	{
		std::string strClassName = PluginJniHelper::jstring2string(className);
		PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
		PluginUtils::outputLog("aonesdk info ProtocolStream", "nativeOnInitCameraFail(), get plugin ptr: , %p", pPlugin);

		if(NULL != pPlugin)
		{
			PluginUtils::outputLog("aonesdk info ProtocolStream", "nativeOnInitCameraFail(), get plugin name: %s", pPlugin->getPluginName());
			ProtocolStream* pStream = dynamic_cast<ProtocolStream*>(pPlugin);
			if(NULL != pStream)
			{
				StreamActionListener* listener = pStream->getActionListener();
				if(NULL != listener)
					listener->onInitCameraFail(pStream);
				else
					PluginUtils::outputLog("aonesdk info ProtocolStream", "Listener of plugin %s not set correctly", pPlugin->getPluginName());
			}
		}
	}

	JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_StreamWrapper_nativeOnInitMicrophoneFail(JNIEnv* env, jobject thiz, jstring className)
	{
		std::string strClassName = PluginJniHelper::jstring2string(className);
		PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
		PluginUtils::outputLog("aonesdk info ProtocolStream", "nativeOnInitMicrophoneFail(), get plugin ptr: , %p", pPlugin);

		if(NULL != pPlugin)
		{
			PluginUtils::outputLog("aonesdk info ProtocolStream", "nativeOnInitMicrophoneFail(), get plugin name: %s", pPlugin->getPluginName());
			ProtocolStream* pStream = dynamic_cast<ProtocolStream*>(pPlugin);
			if(NULL != pStream)
			{
				StreamActionListener* listener = pStream->getActionListener();
				if(NULL != listener)
					listener->onInitMicrophoneFail(pStream);
				else
					PluginUtils::outputLog("aonesdk info ProtocolStream", "Listener of plugin %s not set correctly", pPlugin->getPluginName());
			}
		}
	}

	JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_StreamWrapper_nativeOnCaptureStart(JNIEnv* env, jobject thiz, jstring className)
	{
		std::string strClassName = PluginJniHelper::jstring2string(className);
		PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
		PluginUtils::outputLog("aonesdk info ProtocolStream", "nativeOnCaptureStart(), get plugin ptr: , %p", pPlugin);

		if(NULL != pPlugin)
		{
			PluginUtils::outputLog("aonesdk info ProtocolStream", "nativeOnCaptureStart(), get plugin name: %s", pPlugin->getPluginName());
			ProtocolStream* pStream = dynamic_cast<ProtocolStream*>(pPlugin);
			if(NULL != pStream)
			{
				StreamActionListener* listener = pStream->getActionListener();
				if(NULL != listener)
					listener->onCaptureStart(pStream);
				else
					PluginUtils::outputLog("aonesdk info ProtocolStream", "Listener of plugin %s not set correctly", pPlugin->getPluginName());
			}
		}
	}

	JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_StreamWrapper_nativeOnCaptureStop(JNIEnv* env, jobject thiz, jstring className)
	{
		std::string strClassName = PluginJniHelper::jstring2string(className);
		PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
		PluginUtils::outputLog("aonesdk info ProtocolStream", "nativeOnCaptureStop(), get plugin ptr: , %p", pPlugin);

		if(NULL != pPlugin)
		{
			PluginUtils::outputLog("aonesdk info ProtocolStream", "nativeOnCaptureStop(), get plugin name: %s", pPlugin->getPluginName());
			ProtocolStream* pStream = dynamic_cast<ProtocolStream*>(pPlugin);
			if(NULL != pStream)
			{
				StreamActionListener* listener = pStream->getActionListener();
				if(NULL != listener)
					listener->onCaptureStop(pStream);
				else
					PluginUtils::outputLog("aonesdk info ProtocolStream", "Listener of plugin %s not set correctly", pPlugin->getPluginName());
			}
		}
	}

	JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_StreamWrapper_nativeOnPublishStart(JNIEnv* env, jobject thiz, jstring className, jstring uid)
	{
		std::string strClassName = PluginJniHelper::jstring2string(className);
		std::string strUid = PluginJniHelper::jstring2string(uid);
		PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
		PluginUtils::outputLog("aonesdk info ProtocolStream", "nativeOnPublishStart(), get plugin ptr: , %p", pPlugin);

		if(NULL != pPlugin)
		{
			PluginUtils::outputLog("aonesdk info ProtocolStream", "nativeOnPublishStart(), get plugin name: %s", pPlugin->getPluginName());
			ProtocolStream* pStream = dynamic_cast<ProtocolStream*>(pPlugin);
			if(NULL != pStream)
			{
				StreamActionListener* listener = pStream->getActionListener();
				if(NULL != listener)
					listener->onPublishStart(pStream, (void*)strUid.c_str());
				else
					PluginUtils::outputLog("aonesdk info ProtocolStream", "Listener of plugin %s not set correctly", pPlugin->getPluginName());
			}
		}
	}

	JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_StreamWrapper_nativeOnPublishStop(JNIEnv* env, jobject thiz, jstring className, jint ret)
	{
		std::string strClassName = PluginJniHelper::jstring2string(className);
		PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
		PluginUtils::outputLog("aonesdk info ProtocolStream", "nativeOnPublishStop(), get plugin ptr: , %p", pPlugin);

		if(NULL != pPlugin)
		{
			PluginUtils::outputLog("aonesdk info ProtocolStream", "nativeOnPublishStop(), get plugin name: %s", pPlugin->getPluginName());
			ProtocolStream* pStream = dynamic_cast<ProtocolStream*>(pPlugin);
			if(NULL != pStream)
			{
				StreamActionListener* listener = pStream->getActionListener();
				if(NULL != listener)
					listener->onPublishStop(pStream, (StreamErrorCode)ret);
				else
					PluginUtils::outputLog("aonesdk info ProtocolStream", "Listener of plugin %s not set correctly", pPlugin->getPluginName());
			}
		}
	}

	JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_StreamWrapper_nativeOnPublishDidOffLine(JNIEnv* env, jobject thiz, jstring className, jint ret, jstring uid)
	{
		std::string strClassName = PluginJniHelper::jstring2string(className);
		std::string strUid = PluginJniHelper::jstring2string(uid);
		PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
		PluginUtils::outputLog("aonesdk info ProtocolStream", "nativeOnPublishDidOffLine(), get plugin ptr: , %p", pPlugin);

		if (NULL != pPlugin)
		{
			PluginUtils::outputLog("aonesdk info ProtocolStream", "nativeOnPublishDidOffLine(), get plugin name: %s", pPlugin->getPluginName());
			ProtocolStream* pStream = dynamic_cast<ProtocolStream*>(pPlugin);
			if (NULL != pStream)
			{
				StreamActionListener* listener = pStream->getActionListener();
				if (NULL != listener)
					listener->onPublishDidOffLine(pStream, (StreamErrorCode)ret, (void*)strUid.c_str());
				else
					PluginUtils::outputLog("aonesdk info ProtocolStream", "Listener of plugin %s not set correctly", pPlugin->getPluginName());
			}
		}
	}

	JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_StreamWrapper_nativeOnPublishDidLine(JNIEnv* env, jobject thiz, jstring className, jint ret, jstring uid)
	{
		std::string strClassName = PluginJniHelper::jstring2string(className);
		std::string strUid = PluginJniHelper::jstring2string(uid);
		PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
		PluginUtils::outputLog("aonesdk info ProtocolStream", "nativeOnPublishDidLine(), get plugin ptr: , %p", pPlugin);

		if (NULL != pPlugin)
		{
			PluginUtils::outputLog("aonesdk info ProtocolStream", "nativeOnPublishDidLine(), get plugin name: %s", pPlugin->getPluginName());
			ProtocolStream* pStream = dynamic_cast<ProtocolStream*>(pPlugin);
			if (NULL != pStream)
			{
				StreamActionListener* listener = pStream->getActionListener();
				if (NULL != listener)
					listener->onPublishDidLine(pStream, (StreamErrorCode)ret, (void*)strUid.c_str());
				else
					PluginUtils::outputLog("aonesdk info ProtocolStream", "Listener of plugin %s not set correctly", pPlugin->getPluginName());
			}
		}
	}
	
	JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_StreamWrapper_nativeOnVolumeIndication(JNIEnv* env, jobject thiz, jstring className, jstring uid)
	{
		std::string strClassName = PluginJniHelper::jstring2string(className);
		std::string strUid = PluginJniHelper::jstring2string(uid);
		PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
		PluginUtils::outputLog("aonesdk info ProtocolStream", "nativeOnVolumeIndication(), get plugin ptr: , %p", pPlugin);

		if(NULL != pPlugin)
		{
			PluginUtils::outputLog("aonesdk info ProtocolStream", "nativeOnVolumeIndication(), get plugin name: %s", pPlugin->getPluginName());
			ProtocolStream* pStream = dynamic_cast<ProtocolStream*>(pPlugin);
			if(NULL != pStream)
			{
				StreamActionListener* listener = pStream->getActionListener();
				if(NULL != listener)
					listener->onVolumeIndication(pStream, (void*)strUid.c_str());
				else
					PluginUtils::outputLog("aonesdk info ProtocolStream", "Listener of plugin %s not set correctly", pPlugin->getPluginName());
			}
		}
	}
	
	
	JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_StreamWrapper_nativeOnVideoStreamStatus(JNIEnv* env, jobject thiz, jstring className, jint ret, jstring uid)
	{
		std::string strClassName = PluginJniHelper::jstring2string(className);
		std::string strUid = PluginJniHelper::jstring2string(uid);
		PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
		PluginUtils::outputLog("aonesdk info ProtocolStream", "nativeOnVideoStreamStatus(), get plugin ptr: , %p", pPlugin);

		if (NULL != pPlugin)
		{
			PluginUtils::outputLog("aonesdk info ProtocolStream", "nativeOnVideoStreamStatus(), get plugin name: %s", pPlugin->getPluginName());
			ProtocolStream* pStream = dynamic_cast<ProtocolStream*>(pPlugin);
			if (NULL != pStream)
			{
				StreamActionListener* listener = pStream->getActionListener();
				if (NULL != listener)
					listener->onVideoStreamStatus(pStream, (StreamErrorCode)ret, (void*)strUid.c_str());
				else
					PluginUtils::outputLog("aonesdk info ProtocolStream", "Listener of plugin %s not set correctly", pPlugin->getPluginName());
			}
		}
	}
}

ProtocolStream::ProtocolStream()
: _listener(NULL)
{
}

ProtocolStream::~ProtocolStream()
{
}

void ProtocolStream::configDeveloperInfo(TStreamDeveloperInfo devInfo)
{
	if (devInfo.empty())
    {
        PluginUtils::outputLog("aonesdk info ProtocolStream", "The developer info is empty!");
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

void ProtocolStream::setStreamParams(TStreamDeveloperInfo devInfo)
{
	if(devInfo.empty())
	{
		 PluginUtils::outputLog("aonesdk info ProtocolStream", "The achievement info is empty!");
        return;
	}
	else
	{
		PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
		PluginJniMethodInfo t;
		if(PluginJniHelper::getMethodInfo(t
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

void ProtocolStream::startCapture()
{
	PluginUtils::callJavaFunctionWithName(this, "startCapture");
}

void ProtocolStream::stopCapture()
{
	PluginUtils::callJavaFunctionWithName(this, "stopCapture");
}

void ProtocolStream::startPublish()
{
	PluginUtils::callJavaFunctionWithName(this, "startPublish");
}

void ProtocolStream::stopPublish()
{
	PluginUtils::callJavaFunctionWithName(this, "stopPublish");
}

void ProtocolStream::close()
{
	PluginUtils::callJavaFunctionWithName(this, "close");
}

int ProtocolStream::getCameraCount()
{
	return PluginUtils::callJavaIntFuncWithName(this, "getCameraCount");
}

StreamCameraType ProtocolStream::getCurCameraType()
{
	return (StreamCameraType)PluginUtils::callJavaIntFuncWithName(this, "getCurCameraType");
}

bool ProtocolStream::isMicMute()
{
	return PluginUtils::callJavaBoolFuncWithName(this, "isMicMute");
}

void ProtocolStream::setMicMute(bool isMute)
{
	PluginUtils::callJavaFunctionWithName_oneParam(this, "setMicMute", "(Z)V", isMute);
}

 void ProtocolStream::playSoundEffect(const char* pcmData, int len)
 {
 	if(len <= 0 || pcmData == NULL)
 	{
 		PluginUtils::outputLog("aonesdk info ProtocolStream", "playSoundEffect data is invalid");
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
