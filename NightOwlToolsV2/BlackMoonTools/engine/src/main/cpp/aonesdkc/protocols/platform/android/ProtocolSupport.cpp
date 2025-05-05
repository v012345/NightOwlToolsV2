#include "ProtocolSupport.h"
#include "PluginJniHelper.h"
#include <android/log.h>
#include "PluginUtils.h"
#include "PluginJavaData.h"

namespace aonesdk{
	namespace plugin{
		ProtocolSupport::ProtocolSupport()
		{
		}
		
		ProtocolSupport::~ProtocolSupport()
		{
		}

		void ProtocolSupport::configDeveloperInfo(TSupportDeveloperInfo devInfo)
		{
			if (devInfo.empty()){
				PluginUtils::outputLog("aonesdk info ProtocolSupport", "The developer info is empty!");
			}

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

		void ProtocolSupport::showConversation()
		{
			PluginUtils::callJavaFunctionWithName(this, "showConversation");
		}

		void ProtocolSupport::showFAQS()
		{
			PluginUtils::callJavaFunctionWithName(this, "showFAQS");
		}

		void ProtocolSupport::setUserName(const char * userName)
		{
			jstring jstr = PluginUtils::getEnv()->NewStringUTF(userName);
			PluginUtils::callJavaFunctionWithName_oneParam(this, "setUserName", "(Ljava/lang/String;)V", jstr);
			PluginUtils::getEnv()->DeleteLocalRef(jstr);
		}

		void ProtocolSupport::setUserIdentifier(const char * userId)
		{
			jstring jstr = PluginUtils::getEnv()->NewStringUTF(userId);
			PluginUtils::callJavaFunctionWithName_oneParam(this, "setUserIdentifier", "(Ljava/lang/String;)V", jstr);
			PluginUtils::getEnv()->DeleteLocalRef(jstr);
		}
	}
}