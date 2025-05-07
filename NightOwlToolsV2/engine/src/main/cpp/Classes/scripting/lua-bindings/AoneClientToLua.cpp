
#include "scripting/lua-bindings/AoneClientToLua.h"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "aone/version/version.h"
#include "json_inc.h"
#include <sstream>

using namespace cocos2d;

LUA_FUNCTION funcID = 0;

LUA_FUNCTION funcInitID = 0;

LUA_FUNCTION funcCheckVersionID = 0;

LUA_FUNCTION funcLogInID = 0;

LUA_FUNCTION funcCreateRoleID = 0;

LUA_FUNCTION funcShareID = 0;

LUA_FUNCTION funcAdsID = 0;

LUA_FUNCTION funcPushID = 0;

LUA_FUNCTION funcCheckIsEnableLocalPayID = 0;

LUA_FUNCTION funcDeleteAccount = 0;

LUA_FUNCTION funcOAuthBindID = 0;

LUA_FUNCTION funcOAuthLoginID = 0;

LUA_FUNCTION funcSelectID = 0;

LUA_FUNCTION funcPullGameGroupID = 0;

aonesdk::GameVersion version;
aonesdk::GameRole roleInfo;
aonesdk::GameGroup groupInfo;
aonesdk::AoneDeviceInfo deviceInfo;
aonesdk::AoneAppInfo appInfo;

void AoneClientToLua::init()
{
	const luaL_Reg functions[] = {
		{ "UserID", UserID },
        { "InitSDK", InitSDK },
		{ "UserToken", UserToken },
		{ "AllGroups", AllGroups },
		{ "AllRoles", AllRoles },
		{ "AllProducts", AllProducts },
        { "AoneAccount", AoneAccount },
		{ "AonePassword", AonePassword },
		{ "AllLangs", AllLangs },
		{ "Login", Login },
		{ "ThirdParthLogin", ThirdParthLogin },
		{ "Logout", Logout },
		{ "CheckVersion", CheckVersion },
		{ "GetGameVersion", GetGameVersion },
		{ "CreateRole", CreateRole },
		{ "Pay", Pay },
		{ "QuickLogin", QuickLogin },
		{ "QuickRegister", QuickRegister },
		{ "AoneRegister", AoneRegister },
		{ "AoneLogin", AoneLogin },
		{ "AoneChangePassword", AoneChangePassword },
		{ "AoneBind", AoneBind },
		{ "SelectRoleAndGroup", SelectRoleAndGroup },
        { "pullGameGroup", pullGameGroup },
		{ "GetSelectedRole", GetSelectedRole },
		{ "GetSelectedGroup", GetSelectedGroup },
		{ "SelectLang", SelectLang },
		{ "Exit", Exit },
		{ "hasExitUI", hasExitUI },
		{ "callFunc", callFunc },
		{ "callFuncWithStringParam", callFuncWithStringParam },
		{ "callFuncWithParam", callFuncWithParam },
		{ "callIntFunc", callIntFunc },
		{ "SetOAuthType", SetOAuthType },
		{ "SetPayChannel", SetPayChannel },
		{ "GetOAuthType", GetOAuthType },
		{ "GetPayChannel", GetPayChannel },
		{ "Share", Share },
		{ "SetShareSdk", SetShareSdk },
		{ "LogPageStart", LogPageStart },
		{ "LogPageEnd", LogPageEnd },
		{ "LogEvent", LogEvent },
		{ "LogEventWithInfo", LogEventWithInfo },
		{ "SetAnalyticsSdk", SetAnalyticsSdk },
		{ "ShowAds", ShowAds },
		{ "HideAds", HideAds },
		{ "QueryPoints", QueryPoints },
		{ "SpendPoints", SpendPoints },
		{ "SetAdsSdk", SetAdsSdk },
		{ "StartPush", StartPush },
		{ "ClosePush", ClosePush },
		{ "SetAlias", SetAlias },
		{ "DelAlias", DelAlias },
		{ "SetTags", SetTags },
		{ "DelTags", DelTags },
		{ "AddNotify", AddNotify },
		{ "ClearNotify", ClearNotify },
		{ "ClearAllNotify", ClearAllNotify },
		{ "setPushSdk", setPushSdk },
        { "loadAllSdks", loadAllSdks },
        { "getDeviceInfo", getDeviceInfo },
        { "getAppVersionInfo", getAppVersionInfo },
		{ "OAuthBind", OAuthBind },
		{ "OAuthLogin", OAuthLogin },
		{ "AddFileSearchPath", AddFileSearchPath },
		{ "CheckIsEnableLocalPay", CheckIsEnableLocalPay },
		{ "DeleteAccount", DeleteAccount },
		{ "SimpSubmitRoleData", SimpSubmitRoleData },
        { "SetDeviceId", SetDeviceId },
		{ "isUse01gameSdk", isUse01gameSdk },
		{ "callStringFunc", callStringFunc },
		{ "OpenUrl", OpenUrl },
		{ "recordDevicePoint", recordDevicePoint },
		{ "recordRolePoint", recordRolePoint },
		{ NULL, NULL }
	};
	luaL_register(LuaEngine::getInstance()->getLuaStack()->getLuaState(), "AoneClient", functions);
}

string int2Str(unsigned int d)
{
	std::string str;
	std::stringstream stream;
	stream << d;
	str = stream.str();
	return str;
}

string double2Str(double d)
{
	std::string str;
	std::stringstream stream;
	stream << d;
	str = stream.str();
	return str;
}

void AoneClientToLua::MapTable(std::map<std::string, std::string>& params)
{
	LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
	lua_State* l = luaEngine->getLuaStack()->getLuaState();
	lua_newtable(l);
	std::map<std::string, std::string>::iterator iter = params.begin();
	for (; iter != params.end(); iter++) {
		lua_pushstring(l, iter->first.c_str());
		lua_pushstring(l, iter->second.c_str());
		std::cout << iter->first.c_str() << iter->second.c_str() << endl;
		lua_settable(l, -3);
	}
}

void AoneClientToLua::MapTable2(std::map<unsigned int, std::string>& params)
{
	LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
	lua_State* l = luaEngine->getLuaStack()->getLuaState();
	lua_newtable(l);
	std::map<unsigned int, std::string>::iterator iter = params.begin();
	for (; iter != params.end(); iter++) {
		lua_pushnumber(l, iter->first);
		lua_pushstring(l, iter->second.c_str());
		lua_settable(l, -3);
	}
}

Json::Value MapToJson(std::map<std::string, std::string> mapV)
{
    Json::Value jsonValue;
    std::map<std::string, std::string >::iterator iter = mapV.begin();
    while (iter != mapV.end())
    {
        jsonValue[iter->first] = iter->second;
        iter++;
    }
    return jsonValue;
}

string MapToJsonStr(std::map<std::string, std::string> mapV)
{
    Json::Value jsonValue;
	std::map<std::string, std::string >::iterator iter = mapV.begin();
	while (iter != mapV.end())
	{
        jsonValue[iter->first] = iter->second;
		iter++;
	}
	return jsonValue.toStyledString();
}

map<string, string> AoneClientToLua::GameVersionToMap(GameVersion &version)
{
	map<string, string> params;
    params["MajorVersion"] = int2Str(version.MajorVersion);
    params["MinorVersion"] = int2Str(version.MinorVersion);
    params["RevisionVersion"] = int2Str(version.RevisionVersion);
	params["Status"] = version.Status;
	switch (version.Status)
	{
	case GameVersion::GameVersionStatus::UNKNOWN:
		params["Status"] = "0";
		break;
	case GameVersion::GameVersionStatus::NORAML:
		params["Status"] = "1";
		break;
	case GameVersion::GameVersionStatus::LIMITED:
		params["Status"] = "2";
		break;
	case GameVersion::GameVersionStatus::OVERDUE:
		params["Status"] = "3";
		break;
	default:
		params["Status"] = "";
		break;
	}
	params["Notice"] = version.Notice;
	params["DownloadURL"] = version.DownloadURL;
	params["UpdateURL"] = version.UpdateURL;
	params["BigPatcherURL"] = version.BigPatcherURL;
	return params;
}

map<string, string> AoneClientToLua::GameGroupToMap(GameGroup &group)
{
	map<string, string> params;
	params["ID"] = int2Str(group.ID);
	switch (group.Status)
	{
	case GameGroup::GameGroupStatus::UNKNOWN:
		params["Status"] = "0";
		break;
	case GameGroup::GameGroupStatus::CLOSE:
		params["Status"] = "1";
		break;
	case GameGroup::GameGroupStatus::NORMAL:
		params["Status"] = "2";
		break;
	case GameGroup::GameGroupStatus::BUSY:
		params["Status"] = "3";
		break;
	case GameGroup::GameGroupStatus::FULL:
		params["Status"] = "4";
		break;
	default:
		params["Status"] = "";
		break;
	}
	params["ServerIP"] = group.ServerIP;
	params["ServerPort"] = int2Str(group.ServerPort);
	if (group.IsRecommend)
	{
		params["IsRecommend"] = "1";
	}
	else
	{
		params["IsRecommend"] = "0";
	}
	if (group.IsInitialed)
	{
		params["IsInitialed"] = "1";
	}
	else
	{
		params["IsInitialed"] = "0";
	}
	return params;
}

string GameRoleToJsonStr(GameRole &role)
{
    Json::Value jsonValue;
    jsonValue["ID"] = int2Str(role.ID);
    jsonValue["Name"] = role.Name;
    jsonValue["Type"] = int2Str(role.Type);
    jsonValue["Level"] = int2Str(role.Level);
    jsonValue["VIP"] = int2Str(role.VIP);
    jsonValue["CreateTime"] = role.CreateTime;
    jsonValue["LastLoginTime"] = role.LastLoginTime;
    jsonValue["OwnerGroupID"] = int2Str(role.OwnerGroupID);
    jsonValue["LastGroupID"] = int2Str(role.LastGroupID);
    jsonValue["Features"] = MapToJson(role.Features);
    return jsonValue.toStyledString();
}

map<string, string> AoneClientToLua::GameRoleToMap(GameRole &role)
{
	map<string, string> params;
	params["ID"] = int2Str(role.ID);
	params["Name"] = role.Name;
	params["Type"] = int2Str(role.Type);
	params["Level"] = int2Str(role.Level);
	params["VIP"] = int2Str(role.VIP);
	params["CreateTime"] = role.CreateTime;
	params["LastLoginTime"] = role.LastLoginTime;
	params["OwnerGroupID"] = int2Str(role.OwnerGroupID);
	params["LastGroupID"] = int2Str(role.LastGroupID);
	params["Features"] = MapToJsonStr(role.Features);
	return params;
}

map<string, string> AoneClientToLua::GameProductToMap(GameProduct &product)
{
	map<string, string> params;
	params["ID"] = product.ID;
	params["Name"] = product.Name;
	params["Describe"] = product.Describe;
	params["Currency"] = product.Currency;
	params["Amount"] = double2Str(product.Amount);
	params["Amount_usd"] = double2Str(product.Amount_usd);
	params["Coin"] = int2Str(product.Coin);
	return params;
}

map<std::string, string> AoneClientToLua::AoneDeviceInfoToMap(AoneDeviceInfo &device)
{
    map<string, string> params;
    params["DeviceId"] = device.DeviceId;
    params["Locale"] = device.Locale;
    params["Language"] = device.Language;
    params["Model"] = device.Model;
    params["Os"] = device.Os;
    params["Imei"] = device.Imei;
    params["Ip"] = device.Ip;
    params["Mac"] = device.Mac;
    params["Idfa"] = device.Idfa;
    params["NetworkType"] = device.NetworkType;
    return params;
}

map<std::string, string> AoneClientToLua::AoneAppInfoToMap(AoneAppInfo &version)
{
    map<string, string> params;
    params["AppId"] = int2Str(version.AppId);
    params["AppLang"] = version.AppLang;
    params["Channel"] = version.Channel;
    params["MajorVersion"] = int2Str(version.MajorVersion);
    params["MinorVersion"] = int2Str(version.MinorVersion);
    params["RevisionVersion"] = int2Str(version.RevisionVersion);
    return params;
}

void AoneInitCallBack(int retCode, map<string, string> dataMap)
{
    CCLOG("AoneInitCallBack funcInitID ============ %d", (int)funcInitID);
    if (funcInitID != 0)
    {
        LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
        luaEngine->getLuaStack()->pushInt(retCode);
        AoneClientToLua::MapTable(dataMap);
        luaEngine->getLuaStack()->executeFunctionByHandler(funcInitID, 2);
    }
}

void AoneCallBack(int retCode, map<string, string> dataMap)
{
	CCLOG("AoneCallBack funcLogInID ============ %d", (int)funcLogInID);
	if (funcLogInID != 0)
	{
		LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
		luaEngine->getLuaStack()->pushInt(retCode);
		AoneClientToLua::MapTable(dataMap);
		luaEngine->getLuaStack()->executeFunctionByHandler(funcLogInID, 2);
	}
}

void AoneCreateRoleCallBack(int retCode, map<string, string> dataMap)
{
	CCLOG("AoneCreateRoleCallBack funcCreateRoleID ============ %d", (int)funcCreateRoleID);
	if (funcCreateRoleID != 0)
	{
		LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
		luaEngine->getLuaStack()->pushInt(retCode);
		if (retCode == 0)
		{
			roleInfo = AoneClient::GetSelectedRole();
			std::map<std::string, std::string> roleMap = AoneClientToLua::GameRoleToMap(roleInfo);
			AoneClientToLua::MapTable(roleMap);
			luaEngine->getLuaStack()->executeFunctionByHandler(funcCreateRoleID, 2);
		}
		else
		{
			luaEngine->getLuaStack()->executeFunctionByHandler(funcCreateRoleID, 1);
		}
	}
}

void AonePayCallBack(int retCode, map<string, string> dataMap)
{
	CCLOG("AonePayCallBack funcID ============ %d", (int)funcID);
	if (funcID != 0)
	{
		LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
		luaEngine->getLuaStack()->pushInt(retCode);
		AoneClientToLua::MapTable(dataMap);
		luaEngine->getLuaStack()->executeFunctionByHandler(funcID, 2);
	}
}

void AoneSelectCallBack(int retCode, map<string, string> dataMap)
{
	CCLOG("AoneSelectCallBack funcSelectID ============ %d", (int)funcSelectID);
	if (funcSelectID != 0)
	{
		LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
		luaEngine->getLuaStack()->pushInt(retCode);
		AoneClientToLua::MapTable(dataMap);
		luaEngine->getLuaStack()->executeFunctionByHandler(funcSelectID, 2);
	}
}

void AonePullGameGroup(int retCode, map<string, string> dataMap)
{
    CCLOG("AonePullGameGroup funcPullGameGroupID ============ %d", (int)funcPullGameGroupID);
    if (funcPullGameGroupID != 0)
    {
        LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
        luaEngine->getLuaStack()->pushInt(retCode);
        AoneClientToLua::MapTable(dataMap);
        luaEngine->getLuaStack()->executeFunctionByHandler(funcPullGameGroupID, 2);
    }
}

void AoneShareCallBack(int retCode, map<string, string> dataMap)
{
	CCLOG("AoneShareCallBack funcShareID ============ %d", (int)funcShareID);
	if (funcShareID != 0)
	{
		LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
		luaEngine->getLuaStack()->pushInt(retCode);
		AoneClientToLua::MapTable(dataMap);
		luaEngine->getLuaStack()->executeFunctionByHandler(funcShareID, 2);
	}
}

void AoneAdsCallBack(int retCode, map<string, string> dataMap)
{
	CCLOG("AoneAdsCallBack funcAdsID ============ %d", (int)funcAdsID);
	if (funcAdsID != 0)
	{
		LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
		luaEngine->getLuaStack()->pushInt(retCode);
		AoneClientToLua::MapTable(dataMap);
		luaEngine->getLuaStack()->executeFunctionByHandler(funcAdsID, 2);
	}
}

void AonePushCallBack(int retCode, map<string, string> dataMap)
{
	CCLOG("AonePushCallBack funcPushID ============ %d", (int)funcPushID);
	if (funcPushID != 0)
	{
		LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
		luaEngine->getLuaStack()->pushInt(retCode);
		AoneClientToLua::MapTable(dataMap);
		luaEngine->getLuaStack()->executeFunctionByHandler(funcPushID, 2);
	}
}

void AoneCheckIsEnableLocalPayCallBack(int retCode, map<string, string> dataMap)
{
	CCLOG("AoneCheckIsEnableLocalPayCallBack funcCheckIsEnableLocalPayID ============ %d", (int)funcCheckIsEnableLocalPayID);
	if (funcCheckIsEnableLocalPayID != 0)
	{
		LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
		luaEngine->getLuaStack()->pushInt(retCode);
		AoneClientToLua::MapTable(dataMap);
		luaEngine->getLuaStack()->executeFunctionByHandler(funcCheckIsEnableLocalPayID, 2);
	}
}

void AoneDeleteAccountCallBack(int retCode, map<string, string> dataMap)
{
	CCLOG("AoneDeleteAccountCallBack funcDeleteAccount ============ %d, %", (int)funcDeleteAccount, retCode);

	if (funcDeleteAccount != 0)
	{
		LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
		luaEngine->getLuaStack()->pushInt(retCode);
		if (retCode == 0)
		{
			AoneClientToLua::MapTable(dataMap);
			luaEngine->getLuaStack()->executeFunctionByHandler(funcDeleteAccount, 2);
		}
		else
		{	
			luaEngine->getLuaStack()->executeFunctionByHandler(funcDeleteAccount, 1);
		}
	}
}



void AoneOAuthBindCallBack(int retCode, map<string, string> dataMap)
{
	CCLOG("AoneOAuthBindCallBack funcOAuthBindID ============ %d", (int)funcOAuthBindID);
	if (funcOAuthBindID != 0)
	{
		LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
		luaEngine->getLuaStack()->pushInt(retCode);
		AoneClientToLua::MapTable(dataMap);
		luaEngine->getLuaStack()->executeFunctionByHandler(funcOAuthBindID, 2);
	}
}

void AoneOAuthLoginCallBack(int retCode, map<string, string> dataMap)
{
	CCLOG("AoneOAuthLoginCallBack funcOAuthLoginID ============ %d", (int)funcOAuthLoginID);
	if (funcOAuthLoginID != 0)
	{
		LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
		luaEngine->getLuaStack()->pushInt(retCode);
		AoneClientToLua::MapTable(dataMap);
		luaEngine->getLuaStack()->executeFunctionByHandler(funcOAuthLoginID, 2);
	}
}

void AoneCheckVersionCallBack(int retCode, map<string, string> dataMap)
{
	CCLOG("AoneCheckVersionCallBack funcOAuthLoginID ============ %d, %", (int)funcCheckVersionID, retCode);

	if (funcCheckVersionID != 0)
	{
		LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
		luaEngine->getLuaStack()->pushInt(retCode);
		if (retCode == 0)
		{
			version = AoneClient::GetGameVersion();
			map<string, string> versionMap = AoneClientToLua::GameVersionToMap(version);
			string versionStr = MapToJsonStr(versionMap);
			CCLOG("CheckVersion, version ========= %s", versionStr.c_str());
			AoneClientToLua::MapTable(versionMap);
			luaEngine->getLuaStack()->executeFunctionByHandler(funcCheckVersionID, 2);
		}
		else
		{	
			luaEngine->getLuaStack()->executeFunctionByHandler(funcCheckVersionID, 1);
		}
	}
}

int AoneClientToLua::InitSDK(lua_State* s)
{
    if (!lua_isfunction(s, 1)) {
        CCLOG("function InitSDK request, param 1 must be callback function");
        return 0;
    }
    funcInitID = toluafix_ref_function(s, 1, 0);
    AoneClient::InitSDK(AoneInitCallBack);
    return 0;
}

int AoneClientToLua::UserID(lua_State* s)
{
	unsigned int userId = AoneClient::UserID();
	tolua_pushnumber(s, (lua_Number)userId);
	return 1;
}

int AoneClientToLua::UserToken(lua_State* s)
{
	unsigned int userToken = AoneClient::UserToken();
	tolua_pushnumber(s, (lua_Number)userToken);
	return 1;
}

int AoneClientToLua::AllGroups(lua_State *s)
{
	if (!lua_isfunction(s, 1)) {
		CCLOG("function product request, param 1 must be callback function");
		return 0;
	}
	funcID = toluafix_ref_function(s, 1, 0);
	std::vector<GameGroup> groupMap = AoneClient::AllGroups();
	std::map<unsigned int, std::string> params;
	std::map<unsigned int, std::string> params2;
	std::vector<GameGroup>::iterator iter = groupMap.begin();
	unsigned int id = 1;
	while (iter != groupMap.end())
	{
		params[id] = MapToJsonStr(GameGroupToMap(*iter));
		params2[id] = iter->Name;
		iter ++;	
		id++;
	}
	LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
	MapTable2(params);
	MapTable2(params2);
	luaEngine->getLuaStack()->executeFunctionByHandler(funcID, 2);
	return 0;
}

int AoneClientToLua::AllRoles(lua_State *s)
{
	if (!lua_isfunction(s, 1)) {
		CCLOG("function product request, param 1 must be callback function");
		return 0;
	}
	funcID = toluafix_ref_function(s, 1, 0);
	std::vector<GameRole> roleMap = AoneClient::AllRoles();
	map<unsigned int, string> params;
	map<unsigned int, string> params2;
	std::vector<GameRole>::iterator iter = roleMap.begin();
	unsigned int id = 1;
	while (iter != roleMap.end())
	{
		params[id] = GameRoleToJsonStr((*iter));
		iter++;
		id++;
	}
	LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
	MapTable2(params);
	luaEngine->getLuaStack()->executeFunctionByHandler(funcID, 1);
	return 0;
}

int AoneClientToLua::AllProducts(lua_State *s)
{
	if (!lua_isfunction(s, 1)) {
		CCLOG("function product request, param 1 must be callback function");
		return 0;
	}
	funcID = toluafix_ref_function(s, 1, 0);
	map<string, GameProduct> productMap = AoneClient::AllProducts();
	map<unsigned int, string> params;
	map<string, GameProduct>::iterator iter = productMap.begin();
	unsigned int id = 1;
	while (iter != productMap.end())
	{
		params[id] = MapToJsonStr(GameProductToMap(iter->second));
		iter++;
		id++;
	}
	LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
	MapTable2(params);
	luaEngine->getLuaStack()->executeFunctionByHandler(funcID, 1);
	return 0;
}

int AoneClientToLua::AoneAccount(lua_State *s)
{
    string aoneAccount = AoneClient::AoneAccount();
    tolua_pushstring(s, aoneAccount.c_str());
    return 1;
}

int AoneClientToLua::AonePassword(lua_State *s)
{
	string aonePassword = AoneClient::AonePassword();
	tolua_pushstring(s, aonePassword.c_str());
	return 1;
}

int AoneClientToLua::Login(lua_State *s)
{
	if (!lua_isfunction(s, 1)) {
		CCLOG("function product request, param 1 must be callback function");
		return 0;
	}
	funcLogInID = toluafix_ref_function(s, 1, 0);
	AoneClient::Login(AoneCallBack);
	return 0;
}
int AoneClientToLua::ThirdParthLogin(lua_State *s)
{
	if (!lua_isfunction(s, 1)) {
		CCLOG("ThirdParthLogin, param 1 must be callback function");
		return 0;
	}
	funcLogInID = toluafix_ref_function(s, 1, 0);
	AoneClient::ThirdParthLogin(AoneCallBack);
	return 0;
}

int AoneClientToLua::Logout(lua_State *s)
{
	if (!lua_isfunction(s, 1)) {
		CCLOG("function product request, param 1 must be callback function");
		return 0;
	}
	funcLogInID = toluafix_ref_function(s, 1, 0);
	AoneClient::Logout(AoneCallBack);
    return 0;
}

int AoneClientToLua::CheckVersion(lua_State *s)
{
	if (lua_isnoneornil(s, 1) || !lua_isfunction(s, 1)){
		CCLOG("function CheckVersion, param 1 must be callback function");
		return 0;
	}
	
    string filename = "version";
	class Version v(filename);
	log(" -- msg : version = %d.%d.%d", v.mainv(), v.minor(), v.revise());
	unsigned char majorVersion = (unsigned char)v.mainv();  // 客户端主版本号
	unsigned char minorVersion = (unsigned char)v.minor();  // 客户端子版本号
	unsigned char revisionVersion  = (unsigned char)v.revise();  // 版本扩展信息(产品名、发布时间、描述等)
	
	funcCheckVersionID = toluafix_ref_function(s, 1, 0);
	AoneClient::CheckVersion( majorVersion, minorVersion, revisionVersion, AoneCheckVersionCallBack);
	
	//int ret = AoneClient::CheckVersion(version, majorVersion, minorVersion, revisionVersion);
	/*
	LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
	luaEngine->getLuaStack()->pushInt(ret);
	if (ret == 0)
	{
        map<string, string> versionMap = GameVersionToMap(version);
		string versionStr = MapToJsonStr(versionMap);
		CCLOG("CheckVersion, version ========= %s",versionStr.c_str());
		MapTable(versionMap);
		luaEngine->getLuaStack()->executeFunctionByHandler(funcCheckVersionID, 2);
	}
	else
	{
		luaEngine->getLuaStack()->executeFunctionByHandler(funcCheckVersionID, 1);
	}
	*/
	return 0;
}

int AoneClientToLua::GetGameVersion(lua_State *s)
{
	if (lua_isnoneornil(s, 1) || !lua_isfunction(s, 1)){
		CCLOG("function CheckVersion, param 1 must be callback function");
		return 0;
	}
	funcID = toluafix_ref_function(s, 1, 0);
	version = AoneClient::GetGameVersion();
	LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
	map<string, string> versionMap = GameVersionToMap(version);
	string versionStr = MapToJsonStr(versionMap);
	CCLOG("GetGameVersion, version ========= %s", versionStr.c_str());
	MapTable(versionMap);
	luaEngine->getLuaStack()->executeFunctionByHandler(funcID, 1);
	return 0;
}

int AoneClientToLua::CreateRole(lua_State *s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function CreateRole, param 1 must be name string");
		return 0;
	}
	if (!lua_isnumber(s, 2)) {
		CCLOG("function CreateRole, param 2 must be type number");
		return 0;
	}
	if (!lua_istable(s, 3)) {
		CCLOG("function CreateRole, param 3 must be feature param table");
		return 0;
	}
	if (!lua_isnumber(s, 4)) {
		CCLOG("function CreateRole, param 4 must be groupId number");
		return 0;
	}
	if (!lua_isfunction(s, 5)) {
		CCLOG("function CreateRole, param 5 must be callback function");
		return 0;
	}
	const char* name = lua_tostring(s, 1);
	unsigned int nType = (unsigned int)tolua_tonumber(s, 2, 0);
	map<string, string> params;
	lua_pushnil(s);
	while (lua_next(s, 3) != 0) {
		const char* key = lua_tostring(s, -2);
		const char* value = lua_tostring(s, -1);
		if (key == NULL || value == NULL) {
			CCLOG("key and value must be able convert to string");
			return 0;
		}
		CCLOG("pay param , key is %s, value is %s", key, value);
		params[key] = value;
		lua_pop(s, 1);
	}
	unsigned int groupId = (unsigned int)tolua_tonumber(s, 4, 0);
	funcCreateRoleID = toluafix_ref_function(s, 5, 0);
	AoneClient::CreateRole(name, nType, params, groupId, AoneCreateRoleCallBack);
	/*int ret = AoneClient::CreateRole(roleInfo, name, nType, params, groupId);
	LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
	luaEngine->getLuaStack()->pushInt(ret);
	if (ret == 0)
	{
		std::map<std::string, std::string> roleMap = GameRoleToMap(roleInfo);
		MapTable(roleMap);
		luaEngine->getLuaStack()->executeFunctionByHandler(funcCreateRoleID, 2);
	}
	else
	{
		luaEngine->getLuaStack()->executeFunctionByHandler(funcCreateRoleID, 1);
	}*/
	return 0;
}

int AoneClientToLua::Pay(lua_State *s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function pay, param 1 must be param table");
		return 0;
	}
	if (!lua_isstring(s, 2)) {
		CCLOG("function pay, param 1 must be param table");
		return 0;
	}
	if (!lua_isfunction(s, 3)) {
		CCLOG("function pay, param 2 must be callback function");
		return 0;
	}
	const char* produceId = lua_tostring(s, 1);
	const char* cp_ext = lua_tostring(s, 2);
	funcID = toluafix_ref_function(s, 3, 0);
	AoneClient::Pay(produceId, cp_ext, AonePayCallBack);
	return 0;
}

int AoneClientToLua::QuickLogin(lua_State* s)
{
//#if defined(WIN32) || defined(_WIN32) || defined(IOS_OFFICAL)
	int ret = AoneClient::QuickLogin();
	tolua_pushnumber(s, (lua_Number)ret);
	return 1;
//#else
   // return 0;
//#endif
}

int AoneClientToLua::QuickRegister(lua_State* s)
{
	//#if defined(WIN32) || defined(_WIN32) || defined(IOS_OFFICAL)
	int ret = AoneClient::QuickRegister();
	tolua_pushnumber(s, (lua_Number)ret);
	return 1;
	//#else
	// return 0;
	//#endif
}

int AoneClientToLua::AoneRegister(lua_State* s)
{
//#if defined(WIN32) || defined(_WIN32) || defined(IOS_OFFICAL)
	if (!lua_isstring(s, 1)) {
		CCLOG("function AoneRegister, param 1 must be name string");
		return 0;
	}
	if (!lua_isstring(s, 2)) {
		CCLOG("function AoneRegister, param 1 must be password string");
		return 0;
	}
	const char* name = lua_tostring(s, 1);
	const char* password = lua_tostring(s, 2);
	int ret = AoneClient::AoneRegister(name, password);
	tolua_pushnumber(s, (lua_Number)ret);
	return 1;
//#else
//	return 0;
//#endif
}

int AoneClientToLua::AoneLogin(lua_State* s)
{
//#if defined(WIN32) || defined(_WIN32) || defined(IOS_OFFICAL)
	if (!lua_isstring(s, 1)) {
		CCLOG("function AoneLogin, param 1 must be name string");
		return 0;
	}
	if (!lua_isstring(s, 2)) {
		CCLOG("function AoneLogin, param 2 must be password string");
		return 0;
	}
	const char* name = lua_tostring(s, 1);
	const char* password = lua_tostring(s, 2);
	int ret = AoneClient::AoneLogin(name, password);
	tolua_pushnumber(s, (lua_Number)ret);
	return 1;
//#else
//	return 0;
//#endif
}

int AoneClientToLua::AoneChangePassword(lua_State* s)
{
//#if defined(WIN32) || defined(_WIN32) || defined(IOS_OFFICAL)
	if (!lua_isstring(s, 1)) {
		CCLOG("function AoneLogin, param 1 must be name string");
		return 0;
	}
	if (!lua_isstring(s, 2)) {
		CCLOG("function AoneLogin, param 2 must be password string");
		return 0;
	}
	const char* oldpass = lua_tostring(s, 1);
	const char* newpass = lua_tostring(s, 2);
	int ret = AoneClient::AoneChangePassword(oldpass, newpass);
	tolua_pushnumber(s, (lua_Number)ret);
	return 1;
//#else
//	return 0;
//#endif
}

int AoneClientToLua::AoneBind(lua_State* s)
{
//#if defined(WIN32) || defined(_WIN32) || defined(IOS_OFFICAL)
	if (!lua_isstring(s, 1)) {
		CCLOG("function AoneLogin, param 1 must be name string");
		return 0;
	}
	if (!lua_isstring(s, 2)) {
		CCLOG("function AoneLogin, param 2 must be password string");
		return 0;
	}
	const char* name = lua_tostring(s, 1);
	const char* password = lua_tostring(s, 2);
	int ret = AoneClient::AoneBind(name, password);
	tolua_pushnumber(s, (lua_Number)ret);
	return 1;
//#else
//	return 0;
//#endif
}

int AoneClientToLua::SelectRoleAndGroup(lua_State* s)
{
	if (!lua_isnumber(s, 1)) {
		CCLOG("function SelectRoleAndGroup, param 1 must be role_id");
		return 0;
	}
	if (!lua_isnumber(s, 2)) {
		CCLOG("function SelectRoleAndGroup, param 2 must be group_id");
		return 0;
	}
	unsigned int role_id = (unsigned int)tolua_tonumber(s, 1, 0);
	unsigned int group_id = (unsigned int)tolua_tonumber(s, 2, 0);
	if (lua_isnoneornil(s, 3) || !lua_isfunction(s, 3)){
		int ret = AoneClient::SelectRoleAndGroup(role_id, group_id);
		tolua_pushnumber(s, (lua_Number)ret);
		return 1;
	}
	funcSelectID = toluafix_ref_function(s, 3, 0);
	AoneClient::SelectRoleAndGroup(role_id, group_id, AoneSelectCallBack);
	return 0;
}

int AoneClientToLua::pullGameGroup(lua_State* s)
{
    if (!lua_isfunction(s, 1)) {
        CCLOG("function product request, param 1 must be callback function");
        return 0;
    }
    funcPullGameGroupID = toluafix_ref_function(s, 1, 0);
    AoneClient::pullGameGroup(AonePullGameGroup);
    return 0;
}

int AoneClientToLua::GetSelectedRole(lua_State* s)
{
	if (lua_isnoneornil(s, 1) || !lua_isfunction(s, 1)){
		CCLOG("function GetSelectedRole, param 1 must be callback function");
		return 0;
	}
	funcID = toluafix_ref_function(s, 1, 0);
	roleInfo = AoneClient::GetSelectedRole();
	LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
	map<string, string> roleMap = GameRoleToMap(roleInfo);
	string roleStr = MapToJsonStr(roleMap);
	CCLOG("GetSelectedRole, roleInfo ========= %s", roleStr.c_str());
	MapTable(roleMap);
	MapTable(roleInfo.Features);
	luaEngine->getLuaStack()->executeFunctionByHandler(funcID, 2);
	return 0;
}

int AoneClientToLua::GetSelectedGroup(lua_State* s)
{
	if (lua_isnoneornil(s, 1) || !lua_isfunction(s, 1)){
		CCLOG("function GetSelectedGroup, param 1 must be callback function");
		return 0;
	}
	funcID = toluafix_ref_function(s, 1, 0);
	groupInfo = AoneClient::GetSelectedGroup();
	LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
	map<string, string> groupMap = GameGroupToMap(groupInfo);
	string groupStr = MapToJsonStr(groupMap);
	CCLOG("GetSelectedGroup, groupInfo ========= %s", groupStr.c_str());
	MapTable(groupMap);
	luaEngine->getLuaStack()->pushString(groupInfo.Name.c_str());
	luaEngine->getLuaStack()->executeFunctionByHandler(funcID, 2);
	return 0;
}

int AoneClientToLua::SelectLang(lua_State* s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function SelectLang, param 1 must be name string");
		return 0;
	}
	const char* lang = lua_tostring(s, 1);
//	int ret = AoneClient::SelectLang(lang);
//	tolua_pushnumber(s, (lua_Number)ret);
	return 1;
}

int AoneClientToLua::AllLangs(lua_State* s)
{
	if (!lua_isfunction(s, 1)) {
		CCLOG("function AllLangs, param 1 must be callback function");
		return 0;
	}
	funcID = toluafix_ref_function(s, 1, 0);
	LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
	std::map<std::string, std::string> langMap = AoneClient::AllLangs();
	MapTable(langMap);
	luaEngine->getLuaStack()->executeFunctionByHandler(funcID, 1);
	return 0;
}

int AoneClientToLua::Exit(lua_State* s)
{
	AoneClient::Exit();
	return 0;
}

int AoneClientToLua::hasExitUI(lua_State* s)
{
	bool ret = AoneClient::hasExitUI();
	tolua_pushboolean(s, (int)ret);
	return 1;
}

int AoneClientToLua::callFunc(lua_State* s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function callFunc, param 1 must be name string");
		return 0;
	}
	const char* name = lua_tostring(s, 1);
	AoneClient::callFunc(name);
	return 0;
}

int AoneClientToLua::callFuncWithStringParam(lua_State* s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function callFuncWithStringParam, param 1 must be name string");
		return 0;
	}
	if (!lua_isstring(s, 2)) {
		CCLOG("function callFuncWithStringParam, param 2 must be name string");
		return 0;
	}
	const char* name = lua_tostring(s, 1);
	const char* param = lua_tostring(s, 2);
	AoneClient::callFuncWithStringParam(name, param);
	return 0;
}

int AoneClientToLua::callFuncWithParam(lua_State* s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function callFuncWithParam, param 1 must be name string");
		return 0;
	}
	const char* sdkname = lua_tostring(s, 1);
	if (lua_isnoneornil(s, 2))
	{
		AoneClient::callFuncWithParam(sdkname, NULL);
	}
	else if (lua_isstring(s, 2))
	{
		const char* param = lua_tostring(s, 2);
		AoneClient::callFuncWithParam(sdkname, (void*)param);
	}
	else
	{
		CCLOG("function callFuncWithParam, param 2 must be name string");
		return 0;
	}
	return 0;
}

int AoneClientToLua::callIntFunc(lua_State* s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function callIntFunc, param 1 must be name string");
		return 0;
	}
	const char* sdkname = lua_tostring(s, 1);
	int ret = 0;
	if (lua_isnoneornil(s, 2))
	{
		ret = AoneClient::callIntFunc(sdkname, NULL);
	}
	else if (lua_isstring(s, 2))
	{
		const char* param = lua_tostring(s, 2);
		ret = AoneClient::callIntFunc(sdkname, (void*)param);
	}
	else
	{
		CCLOG("function callIntFunc, param 2 must be name string");
		return 0;
	}
	tolua_pushnumber(s, (lua_Number)ret);
	return 1;
}

int AoneClientToLua::SetOAuthType(lua_State* s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function SetOAuthType, param 1 must be name string");
		return 0;
	}
	const char* oauth_type = lua_tostring(s, 1);
	AoneClient::SetOAuthType(oauth_type);
	return 0;
}

int AoneClientToLua::SetPayChannel(lua_State* s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function SetPayChannel, param 1 must be name string");
		return 0;
	}
	const char* pay_channel = lua_tostring(s, 1);
	AoneClient::SetPayChannel(pay_channel);
	return 0;
}

int AoneClientToLua::GetOAuthType(lua_State* s)
{
	const char* oauthType = AoneClient::GetOAuthType().c_str();
	tolua_pushstring(s, oauthType);
	return 1;
}

int AoneClientToLua::GetPayChannel(lua_State* s)
{
	const char* payType = AoneClient::GetPayChannel().c_str();
	tolua_pushstring(s, payType);
	return 1;
}

int AoneClientToLua::Share(lua_State* s)
{
	if (!lua_istable(s, 1)) {
		CCLOG("function Share, param 1 must be feature param table");
		return 0;
	}
	if (!lua_isfunction(s, 2)) {
		CCLOG("function Share, param 2 must be callback function");
		return 0;
	}
	funcShareID = toluafix_ref_function(s, 2, 0);
	map<string, string> params;
	lua_pushnil(s);
	while (lua_next(s, 1) != 0) {
		const char* key = lua_tostring(s, -2);
		const char* value = lua_tostring(s, -1);
		if (key == NULL || value == NULL) {
			CCLOG("key and value must be able convert to string");
			return 0;
		}
		CCLOG("pay param , key is %s, value is %s", key, value);
		params[key] = value;
		lua_pop(s, 1);
	}
	AoneClient::Share(params, AoneShareCallBack);
	return 0;
}

int AoneClientToLua::SetShareSdk(lua_State* s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function SetShareSdk, param 1 must be name string");
		return 0;
	}
	const char* name = lua_tostring(s, 1);
	AoneClient::SetShareSdk(name);
	return 0;
}

int AoneClientToLua::LogPageStart(lua_State* s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function LogPageStart, param 1 must be name string");
		return 0;
	}
	const char* pagename = lua_tostring(s, 1);
	AoneClient::LogPageStart(pagename);
	return 0;
}

int AoneClientToLua::LogPageEnd(lua_State* s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function LogPageEnd, param 1 must be name string");
		return 0;
	}
	const char* pagename = lua_tostring(s, 1);
	AoneClient::LogPageEnd(pagename);
	return 0;
}

int AoneClientToLua::LogEvent(lua_State* s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function LogEvent, param 1 must be name string");
		return 0;
	}
	const char* eventId = lua_tostring(s, 1);
	AoneClient::LogEvent(eventId);
	return 0;
}

int AoneClientToLua::LogEventWithInfo(lua_State* s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function LogEventWithInfo, param 1 must be name string");
		return 0;
	}
	if (!lua_istable(s, 2)) {
		CCLOG("function LogEventWithInfo, param 2 must be feature param table");
		return 0;
	}
	const char* eventId = lua_tostring(s, 1);
	map<string, string> params;
	lua_pushnil(s);
	while (lua_next(s, 2) != 0) {
		const char* key = lua_tostring(s, -2);
		const char* value = lua_tostring(s, -1);
		if (key == NULL || value == NULL) {
			CCLOG("key and value must be able convert to string");
			return 0;
		}
		CCLOG("pay param , key is %s, value is %s", key, value);
		params[key] = value;
		lua_pop(s, 1);
	}
	AoneClient::LogEvent(eventId, params);
	return 0;
}

int AoneClientToLua::SetAnalyticsSdk(lua_State* s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function SetAnalyticsSdk, param 1 must be name string");
		return 0;
	}
	const char* name = lua_tostring(s, 1);
	AoneClient::SetAnalyticsSdk(name);
	return 0;
}

int AoneClientToLua::ShowAds(lua_State* s)
{
	if (!lua_istable(s, 1)) {
		CCLOG("function ShowAds, param 1 must be feature param table");
		return 0;
	}
	if (!lua_isnumber(s, 2)) {
		CCLOG("function CreateRole, param 2 must be groupId number");
		return 0;
	}	
	if (!lua_isfunction(s, 3)) {
		CCLOG("function ShowAds, param 3 must be callback function");
		return 0;
	}
	int pos = (int)tolua_tonumber(s, 2, 0);
	funcAdsID = toluafix_ref_function(s, 3, 0);
	map<string, string> params;
	lua_pushnil(s);
	while (lua_next(s, 1) != 0) {
		const char* key = lua_tostring(s, -2);
		const char* value = lua_tostring(s, -1);
		if (key == NULL || value == NULL) {
			CCLOG("key and value must be able convert to string");
			return 0;
		}
		CCLOG("pay param , key is %s, value is %s", key, value);
		params[key] = value;
		lua_pop(s, 1);
	}
	AoneClient::ShowAds(params, pos, AoneAdsCallBack);
	return 0;
}

int AoneClientToLua::HideAds(lua_State* s)
{
	if (!lua_istable(s, 1)) {
		CCLOG("function HideAds, param 1 must be feature param table");
		return 0;
	}
	if (!lua_isfunction(s, 2)) {
		CCLOG("function HideAds, param 2 must be callback function");
		return 0;
	}
	funcAdsID = toluafix_ref_function(s, 2, 0);
	map<string, string> params;
	lua_pushnil(s);
	while (lua_next(s, 1) != 0) {
		const char* key = lua_tostring(s, -2);
		const char* value = lua_tostring(s, -1);
		if (key == NULL || value == NULL) {
			CCLOG("key and value must be able convert to string");
			return 0;
		}
		CCLOG("pay param , key is %s, value is %s", key, value);
		params[key] = value;
		lua_pop(s, 1);
	}
	AoneClient::HideAds(params, AoneAdsCallBack);
	return 0;
}

int AoneClientToLua::QueryPoints(lua_State* s)
{
	if (!lua_isfunction(s, 1)) {
		CCLOG("function QueryPoints, param 1 must be callback function");
		return 0;
	}
	funcAdsID = toluafix_ref_function(s, 1, 0);
	AoneClient::QueryPoints(AoneAdsCallBack);
	return 0;
}

int AoneClientToLua::SpendPoints(lua_State* s)
{
	if (!lua_isnumber(s, 1)) {
		CCLOG("function CreateRole, param 1 must be groupId number");
		return 0;
	}
	if (!lua_isfunction(s, 2)) {
		CCLOG("function SpendPoints, param 2 must be callback function");
		return 0;
	}
	int points = (int)tolua_tonumber(s, 1, 0);
	funcAdsID = toluafix_ref_function(s, 2, 0);
	AoneClient::SpendPoints(points, AoneAdsCallBack);
	return 0;
}

int AoneClientToLua::SetAdsSdk(lua_State* s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function SetAdsSdk, param 1 must be name string");
		return 0;
	}
	const char* name = lua_tostring(s, 1);
	AoneClient::SetAdsSdk(name);
	return 0;
}

int AoneClientToLua::StartPush(lua_State* s)
{
	if (lua_isnoneornil(s, 1))
	{
		AoneClient::StartPush();
		funcPushID = 0;
		return 0;
	}
	if (!lua_isfunction(s, 1)) {
		CCLOG("function StartPush, param 1 must be callback function");
		return 0;
	}
	funcPushID = toluafix_ref_function(s, 1, 0);
	AoneClient::StartPush(AonePushCallBack);
	return 0;
}

int AoneClientToLua::ClosePush(lua_State* s)
{
	if (lua_isnoneornil(s, 1))
	{
		AoneClient::ClosePush();
		funcPushID = 0;
		return 0;
	}
	if (!lua_isfunction(s, 1)) {
		CCLOG("function ClosePush, param 1 must be callback function");
		return 0;
	}
	funcPushID = toluafix_ref_function(s, 1, 0);
	AoneClient::ClosePush(AonePushCallBack);
	return 0;
}

int AoneClientToLua::SetAlias(lua_State* s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function SetAlias, param 1 must be name string");
		return 0;
	}
	const char* alias = lua_tostring(s, 1);
	if (lua_isnoneornil(s, 2))
	{
		AoneClient::SetAlias(alias);
		funcPushID = 0;
		return 0;
	}
	if (!lua_isfunction(s, 2)) {
		CCLOG("function SetAlias, param 2 must be callback function");
		return 0;
	}
	funcPushID = toluafix_ref_function(s, 2, 0);
	AoneClient::SetAlias(alias, AonePushCallBack);
	return 0;
}

int AoneClientToLua::DelAlias(lua_State* s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function DelAlias, param 1 must be name string");
		return 0;
	}
	const char* alias = lua_tostring(s, 1);
	if (lua_isnoneornil(s, 2))
	{
		AoneClient::DelAlias(alias);
		funcPushID = 0;
		return 0;
	}
	if (!lua_isfunction(s, 2)) {
		CCLOG("function DelAlias, param 2 must be callback function");
		return 0;
	}
	funcPushID = toluafix_ref_function(s, 2, 0);
	AoneClient::DelAlias(alias, AonePushCallBack);
	return 0;
}

int AoneClientToLua::SetTags(lua_State* s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function SetTags, param 1 must be name string");
		return 0;
	}
	const char* tags = lua_tostring(s, 1);
	if (lua_isnoneornil(s, 2))
	{
		AoneClient::SetTags(tags);
		funcPushID = 0;
		return 0;
	}
	if (!lua_isfunction(s, 2)) {
		CCLOG("function SetTags, param 2 must be callback function");
		return 0;
	}
	funcPushID = toluafix_ref_function(s, 2, 0);
	AoneClient::SetTags(tags, AonePushCallBack);
	return 0;
}

int AoneClientToLua::DelTags(lua_State* s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function DelTags, param 1 must be name string");
		return 0;
	}
	const char* tags = lua_tostring(s, 1);
	if (lua_isnoneornil(s, 2))
	{
		AoneClient::DelTags(tags);
		funcPushID = 0;
		return 0;
	}
	if (!lua_isfunction(s, 2)) {
		CCLOG("function DelTags, param 2 must be callback function");
		return 0;
	}
	funcPushID = toluafix_ref_function(s, 2, 0);
	AoneClient::DelTags(tags, AonePushCallBack);
	return 0;
}

int AoneClientToLua::AddNotify(lua_State* s)
{
	if (!lua_istable(s, 1)) {
		CCLOG("function ShowAds, param 1 must be feature param table");
		return 0;
	}
	map<string, string> params;
	lua_pushnil(s);
	while (lua_next(s, 1) != 0) {
		const char* key = lua_tostring(s, -2);
		const char* value = lua_tostring(s, -1);
		if (key == NULL || value == NULL) {
			CCLOG("key and value must be able convert to string");
			return 0;
		}
		CCLOG("pay param , key is %s, value is %s", key, value);
		params[key] = value;
		lua_pop(s, 1);
	}
	AoneClient::AddNotify(params);
	return 0;
}

int AoneClientToLua::ClearNotify(lua_State* s)
{
	if (!lua_isnumber(s, 1)) {
		CCLOG("function CreateRole, param 1 must be groupId number");
		return 0;
	}
	int notifyId = (int)tolua_tonumber(s, 1, 0);
	AoneClient::ClearNotify(notifyId);
	return 0;
}

int AoneClientToLua::ClearAllNotify(lua_State* s)
{
	AoneClient::ClearAllNotify();
	return 0;
}

int AoneClientToLua::setPushSdk(lua_State* s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function DelTags, param 1 must be name string");
		return 0;
	}
	const char* sdkName = lua_tostring(s, 1);
	AoneClient::setPushSdk(sdkName);
	return 0;
}

int AoneClientToLua::loadAllSdks(lua_State* s)
{
	AoneClient::loadAllSdks();
	return 0;
}

int AoneClientToLua::getDeviceInfo(lua_State* s)
{
    if (lua_isnoneornil(s, 1) || !lua_isfunction(s, 1)){
        CCLOG("function CheckVersion, param 1 must be callback function");
        return 0;
    }
    funcID = toluafix_ref_function(s, 1, 0);
    deviceInfo = AoneClient::getDeviceInfo();
    LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
    map<string, string> deviceMap = AoneDeviceInfoToMap(deviceInfo);
    string deviceInfoStr = MapToJsonStr(deviceMap);
    CCLOG("getDeviceInfo, deviceInfo ========= %s", deviceInfoStr.c_str());
    MapTable(deviceMap);
    luaEngine->getLuaStack()->executeFunctionByHandler(funcID, 1);
    return 0;
}

int AoneClientToLua::getAppVersionInfo(lua_State* s)
{
    if (lua_isnoneornil(s, 1) || !lua_isfunction(s, 1)){
        CCLOG("function CheckVersion, param 1 must be callback function");
        return 0;
    }
    funcID = toluafix_ref_function(s, 1, 0);
    appInfo = AoneClient::getAppVersionInfo();
    LuaEngine* luaEngine = (LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine();
    map<string, string> appInfoMap = AoneAppInfoToMap(appInfo);
    string appInfoStr = MapToJsonStr(appInfoMap);
    CCLOG("getAppVersionInfo, appInfo ========= %s", appInfoStr.c_str());
    MapTable(appInfoMap);
    luaEngine->getLuaStack()->executeFunctionByHandler(funcID, 1);
    return 0;
}

int AoneClientToLua::OAuthBind(lua_State* s)
{
	if (!lua_isfunction(s, 1)) {
		CCLOG("function OAuthBind, param 1 must be callback function");
		return 0;
	}
	funcOAuthBindID = toluafix_ref_function(s, 1, 0);
	AoneClient::OAuthBind(AoneOAuthBindCallBack);
	return 0;
}

int AoneClientToLua::OAuthLogin(lua_State* s)
{
	if (!lua_isfunction(s, 1)) {
		CCLOG("function OAuthLogin, param 1 must be callback function");
		return 0;
	}
	funcOAuthLoginID = toluafix_ref_function(s, 1, 0);
	AoneClient::OAuthLogin(AoneOAuthLoginCallBack);
	return 1;
}

int AoneClientToLua::AddFileSearchPath(lua_State* s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function AddFileSearchPath, param 1 must be name string");
		return 0;
	}
	const char* path = lua_tostring(s, 1);
	AoneClient::AddFileSearchPath(path);
	return 0;
}

int AoneClientToLua::CheckIsEnableLocalPay(lua_State* s)
{
	if (!lua_isfunction(s, 1)) {
		CCLOG("function CheckIsEnableLocalPay, param 1 must be callback function");
		return 0;
	}
	funcCheckIsEnableLocalPayID = toluafix_ref_function(s, 1, 0);
	AoneClient::CheckIsEnableLocalPay(AoneCheckIsEnableLocalPayCallBack);
	return 0;
}
int AoneClientToLua::DeleteAccount(lua_State* s)
{
	if (!lua_isnumber(s, 1)) {
		CCLOG("function DeleteAccount, param 1 must be type number");
		return 0;
	}
	if (!lua_isfunction(s, 2)) {
		CCLOG("function DeleteAccount, param 2 must be callback function");
		return 0;
	}
	int type = tolua_tonumber(s, 1, 0);
	funcDeleteAccount = toluafix_ref_function(s, 2, 0);

	AoneClient::DeleteAccount(type, AoneDeleteAccountCallBack);
	return 0;
}


int AoneClientToLua::SimpSubmitRoleData(lua_State* s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function SimpSubmitRoleData, param 1 must be roleID string");
		return 0;
	}
	if (!lua_isstring(s, 2)) {
		CCLOG("function SimpSubmitRoleData, param 2 must be name string");
		return 0;
	}
	if (!lua_isnumber(s, 3)) {
		CCLOG("function SimpSubmitRoleData, param 3 must be level number");
		return 0;
	}
	if (!lua_isstring(s, 4)) {
		CCLOG("function SimpSubmitRoleData, param 4 must be groupId string");
		return 0;
	}
	if (!lua_isstring(s, 5)) {
		CCLOG("function SimpSubmitRoleData, param 5 must be groupName string");
		return 0;
	}
	if (!lua_isnumber(s, 6)) {
		CCLOG("function SimpSubmitRoleData, param 6 must be roleVip number");
		return 0;
	}
	if(!lua_isboolean(s, 7)) {
		CCLOG("function SimpSubmitRoleData, param 7 must be isLogin boolean");
		return 0;
	}
	if (!lua_isstring(s, 8)) {
		CCLOG("function SimpSubmitRoleData, param 5 must be action string");
		return 0;
	}
	if (!lua_isnumber(s, 9)) {
		CCLOG("function SimpSubmitRoleData, param 5 must be roleCTime number");
		return 0;
	}

	const char* roleID = lua_tostring(s, 1);
	const char* name = lua_tostring(s, 2);
	unsigned int level = (unsigned int)tolua_tonumber(s, 3, 0);
	const char* groupId = lua_tostring(s, 4);
	const char* groupName = lua_tostring(s, 5);
	int roleVip = (int)tolua_tonumber(s, 6, 0);
	bool isLogin = lua_toboolean(s, 7);
	const char* action = lua_tostring(s, 8);
	unsigned long long roleCTime = (unsigned long long)tolua_tonumber(s, 9, 0);

	AoneClient::SimpSubmitRoleData(roleID, name, level, groupId, groupName, roleVip, isLogin, action, roleCTime);
	return 1;
}


int AoneClientToLua::SetDeviceId(lua_State* s)
{
    if (!lua_isstring(s, 1))
    {
        CCLOG("function SetDeviceId, param 1 must be deviceId string");
        return 0;
    }
    const char* deviceId = lua_tostring(s, 1);
    AoneClient::SetDeviceId(deviceId);
    return 1;
}

int AoneClientToLua::isUse01gameSdk(lua_State* s)
{
	bool isUse01gmeSdk = AoneClient::isUse01gameSdk();
	tolua_pushboolean(s, (bool)isUse01gmeSdk);
	return 1;
}

int AoneClientToLua::callStringFunc(lua_State* s)
{
	if (!lua_isstring(s, 1)) {
		CCLOG("function callStringFunc, param 1 must be name string");
		return 0;
	}
	const char* name = lua_tostring(s, 1);
	string info = AoneClient::callStringFunc(name);
	tolua_pushstring(s, info.c_str());
	return 1;
}

int AoneClientToLua::OpenUrl(lua_State * s)
{
	if (!lua_isstring(s, 1)){
		CCLOG("function OpenUrl, param 1 must be url string");
		return 0;
	}

	const char* url = lua_tostring(s, 1);
	AoneClient::OpenUrl(url);
	return 1;
}

int AoneClientToLua::recordDevicePoint(lua_State * s)
{
	if (!lua_isnumber(s, 1)){
		CCLOG("function recordDevicePoint, param 1 must be point number");
		return 0;
	}
	unsigned int point = (unsigned int)tolua_tonumber(s, 1, 0);
	AoneClient::recordDevicePoint(point);
	return 1;
}

int AoneClientToLua::recordRolePoint(lua_State * s)
{
	if (!lua_isnumber(s, 1)){
		CCLOG("function recordRolePoint, param 1 must be point number");
		return 0;
	}
	unsigned int point = (unsigned int)tolua_tonumber(s, 1, 0);
	AoneClient::recordRolePoint(point);
	return 1;
}
