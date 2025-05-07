#ifndef ___channel_dispatch_lua_h___
#define ___channel_dispatch_lua_h___

static const char* __FUNC_NAME = "FromChannel";

#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
inline void call_channel(const char* arg)
{

}

void call_luafunc(const char* arg)
{
	ScriptEngineProtocol* protocol = ScriptEngineManager::getInstance()->getScriptEngine();
	LuaEngine *engine = dynamic_cast<LuaEngine *>(protocol);
	if (nullptr == engine) {
		return;
	}

	lua_State *L = engine->getLuaStack()->getLuaState();
	int ret = -1;
	int top = lua_gettop(L);

	lua_getglobal(L, __FUNC_NAME);
	if (lua_isfunction(L, -1))
	{
		lua_pushstring(L, arg);
		int ok = lua_pcall(L, 1, 1, 0);
		if (ok == 0)
		{
			ret = lua_tonumber(L, -1);
		}
		else
		{
			ret = -ok;
		}
	}

	lua_settop(L, top);
}
#endif

#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)

//#include "SdkWarpperIOS.h"

inline void call_channel(const char* arg)
{
	//SdkWarpperIOS::cFromGame(arg);
}

void call_luafunc(const char* arg)
{
	ScriptEngineProtocol* protocol = ScriptEngineManager::getInstance()->getScriptEngine();
	LuaEngine *engine = dynamic_cast<LuaEngine *>(protocol);
	if (nullptr == engine) {
		return;
	}

	lua_State *L = engine->getLuaStack()->getLuaState();
	int ret = -1;
	int top = lua_gettop(L);

	lua_getglobal(L, __FUNC_NAME);
	if (lua_isfunction(L, -1))
	{
		lua_pushstring(L, arg);
		int ok = lua_pcall(L, 1, 1, 0);
		if (ok == 0)
		{
			ret = lua_tonumber(L, -1);
		}
		else
		{
			ret = -ok;
		}
	}

	lua_settop(L, top);
}
#endif


#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)

#include <jni.h>
#include <platform/android/jni/JniHelper.h>
#define  LOG_TAG    "game"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

void call_channel(const char* arg)
{
	JniMethodInfo jmi;
	if (!JniHelper::getStaticMethodInfo(jmi,
		"channel.adapter/GamePipe",
		"FromGame",
		"(Ljava/lang/String;)V")
		)
	{
		LOGD("----- err: can not find channel.adapter.GamePipe::FromGame");
		return;
	}

	jstring j_arg = jmi.env->NewStringUTF(arg);
	jmi.env->CallStaticVoidMethod(jmi.classID, jmi.methodID, j_arg);
}

#include "platform/android/CCLuaJavaBridge.h"

void call_luafunc(const char* arg)
{
	LuaJavaBridge::callLuaGlobalFunction(__FUNC_NAME, arg);
}


#endif

#if (CC_TARGET_PLATFORM==CC_PLATFORM_WP8)

#include "channel\channel.h"
#include "json_inc.h"

inline void call_luafunc(const char* arg){}
inline void call_channel(const char* arg)
{
	string s = arg;
	Json::Value j;
	Json::Reader r;
	if (false == r.parse(s, j))
		return;
	const char* id = j["id"].asCString();

	PhoneDirect3DXamlAppComponent::game2chnl(id, 0, arg);
}
#endif

#endif
