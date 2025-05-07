/****************************************************************************
Copyright (c) 2012-2013 cocos2d-x.org

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
#include "ProtocolIAP.h"
#include "PluginJniHelper.h"
#include <android/log.h>
#include "PluginUtils.h"
#include "PluginJavaData.h"

namespace aonesdk { namespace plugin {

extern "C" {
	JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_IAPWrapper_nativeOnPayResult(JNIEnv*  env, jobject thiz, jstring className, jint ret, jstring msg)
	{
		std::string strMsg = PluginJniHelper::jstring2string(msg);
		std::string strClassName = PluginJniHelper::jstring2string(className);
		PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
		PluginUtils::outputLog("aonesdk info ProtocolIAP", "nativeOnPayResult(), Get plugin ptr : %p", pPlugin);
		if (pPlugin != NULL)
		{
			PluginUtils::outputLog("aonesdk info ProtocolIAP", "nativeOnPayResult(), Get plugin name : %s", pPlugin->getPluginName());
			ProtocolIAP* pIAP = dynamic_cast<ProtocolIAP*>(pPlugin);
			if (pIAP != NULL)
			{
				pIAP->onPayResult((PayResultCode) ret, strMsg.c_str());
			}
		}
	}

	JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_IAPWrapper_nativeOnRequestProduct(JNIEnv*  env, jobject thiz, jstring className, jint ret, jobjectArray productinfos)
	{
		jclass class_Hashtable = env->FindClass("java/util/Hashtable");
		jmethodID construct_method = env->GetMethodID( class_Hashtable, "<init>","()V");
		//jobject obj_Map = env->NewObject( class_Hashtable, construct_method, "");
		int size = env->GetArrayLength(productinfos);
		TProductList pdlist;
		for(int i=0;i<size;i++)
		{
			jobject obj_Map = env->GetObjectArrayElement(productinfos,i);
			jmethodID get_method= env->GetMethodID( class_Hashtable,"get","(Ljava/lang/Object;)Ljava/lang/Object;");
			jstring id = (jstring)env->CallObjectMethod(obj_Map, get_method, env->NewStringUTF("productId"));
			jstring name = (jstring)env->CallObjectMethod(obj_Map, get_method, env->NewStringUTF("productName"));
			jstring price = (jstring)env->CallObjectMethod(obj_Map, get_method, env->NewStringUTF("productPrice"));
			jstring desc = (jstring)env->CallObjectMethod(obj_Map, get_method, env->NewStringUTF("productDesc"));
			jstring localprice = (jstring)env->CallObjectMethod(obj_Map, get_method, env->NewStringUTF("productLocalPrice"));
			TProductInfo productinfo;
			productinfo.insert(std::make_pair("productId",PluginJniHelper::jstring2string(id)));
			productinfo.insert(std::make_pair("productName",PluginJniHelper::jstring2string(name)));
			productinfo.insert(std::make_pair("productPrice",PluginJniHelper::jstring2string(price)));
			productinfo.insert(std::make_pair("productDesc",PluginJniHelper::jstring2string(desc)));
			productinfo.insert(std::make_pair("productLocalPrice",PluginJniHelper::jstring2string(localprice)));
			pdlist.push_back(productinfo);
		}

		std::string strClassName = PluginJniHelper::jstring2string(className);
		PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
		PluginUtils::outputLog("aonesdk info ProtocolIAP", "nativeOnRequestProduct(), Get plugin ptr : %p", pPlugin);
		ProtocolIAP* iapPlugin = dynamic_cast<ProtocolIAP*>(pPlugin);
		PayResultListener* listener = iapPlugin->getResultListener();
		if (iapPlugin != NULL)
		{
			if(listener != NULL)
			{
				listener->onRequestProductsResult((IAPProductRequest)ret,pdlist);
			}
			else
			{
				PluginUtils::outputLog("aonesdk info ProtocolIAP", "%s can't find the onRequestProductsResult listener!",pPlugin->getPluginName());
			}

		}
//		else
//		{
//			PluginUtils::outputLog("aonesdk info ProtocolIAP", "can't find the C++ object of the IAP plugin!");
//		}
	}

}



bool ProtocolIAP::_paying = false;

ProtocolIAP::ProtocolIAP()
: _listener(NULL)
{
}

ProtocolIAP::~ProtocolIAP()
{
}

void ProtocolIAP::configDeveloperInfo(TIAPDeveloperInfo devInfo)
{
    if (devInfo.empty())
    {
        PluginUtils::outputLog("aonesdk info ProtocolIAP", "The developer info is empty!");
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

void ProtocolIAP::payForProduct(TProductInfo info)
{
    if (_paying)
    {
        PluginUtils::outputLog("aonesdk info ProtocolIAP", "Now is paying");
        // return;
    }

    if (info.empty())
    {
        if (NULL != _listener)
        {
            onPayResult(kPayFail, "Product info error");
        }
        PluginUtils::outputLog("aonesdk info ProtocolIAP", "The product info is empty!");
        return;
    }
    else
    {
        _paying = true;
        _curInfo = info;

        PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
		PluginJniMethodInfo t;
		if (PluginJniHelper::getMethodInfo(t
			, pData->jclassName.c_str()
			, "payForProduct"
			, "(Ljava/util/Hashtable;)V"))
		{
			// generate the hashtable from map
			jobject obj_Map = PluginUtils::createJavaMapObject(&info);

			// invoke java method
			t.env->CallVoidMethod(pData->jobj, t.methodID, obj_Map);
			t.env->DeleteLocalRef(obj_Map);
			t.env->DeleteLocalRef(t.classID);
		}
    }
}

void ProtocolIAP::setResultListener(PayResultListener* pListener)
{
	_listener = pListener;
}

void ProtocolIAP::onPayResult(PayResultCode ret, const char* msg)
{
    _paying = false;
    if (_listener)
    {
    	_listener->onPayResult(ret, msg, _curInfo);
    }
    else
    {
        PluginUtils::outputLog("aonesdk info ProtocolIAP", "Result listener is null!");
    }
    _curInfo.clear();
    PluginUtils::outputLog("aonesdk info ProtocolIAP", "Pay result is : %d(%s)", (int) ret, msg);
}

bool ProtocolIAP::canRequestProducts()
{
    return PluginUtils::callJavaBoolFuncWithName(this, "canRequestProducts");
}

}} // namespace aonesdk { namespace plugin {
