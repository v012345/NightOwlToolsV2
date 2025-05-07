
#ifndef __AONE_CLIENT_TOLUA_HH__
#define __AONE_CLIENT_TOLUA_HH__

#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <Aonesdk/AoneClient.h>
#else
#include "AoneClient.h"
#endif
using namespace aonesdk;
using namespace std;

class AoneClientToLua
{
public:
	static void init();
    static int InitSDK(lua_State* s);
	static int UserID(lua_State* s);
	static int UserToken(lua_State* s);
	static int AllGroups(lua_State *s);
	static int AllRoles(lua_State *s);
	static int AllProducts(lua_State *s);
    static int AoneAccount(lua_State *s);
	static int AonePassword(lua_State *s);
	static int Login(lua_State *s);
	static int ThirdParthLogin(lua_State *s);
	static int Logout(lua_State *s);
	static int CheckVersion(lua_State *s);
	static int GetGameVersion(lua_State *s);
	static int CreateRole(lua_State *s);
	static int Pay(lua_State *s);
	static int QuickLogin(lua_State* s);
	static int QuickRegister(lua_State* s);
	static int AoneRegister(lua_State* s);
	static int AoneLogin(lua_State* s);
	static int AoneChangePassword(lua_State* s);
	static int AoneBind(lua_State* s);
	static int SelectRoleAndGroup(lua_State* s);
    static int pullGameGroup(lua_State* s);
	static int GetSelectedRole(lua_State* s);
	static int GetSelectedGroup(lua_State* s);
	static int SelectLang(lua_State* s);
	static int AllLangs(lua_State* s);
	static int Exit(lua_State* s);
	static int hasExitUI(lua_State* s);

	static int callFunc(lua_State* s);
	static int callFuncWithStringParam(lua_State* s);
	static int callFuncWithParam(lua_State* s);
	static int callIntFunc(lua_State* s);

	static int SetOAuthType(lua_State* s);
	static int SetPayChannel(lua_State* s);
	static int GetOAuthType(lua_State* s);
	static int GetPayChannel(lua_State* s);
	static int Share(lua_State* s);
	static int SetShareSdk(lua_State* s);
	static int LogPageStart(lua_State* s);
	static int LogPageEnd(lua_State* s);
	static int LogEvent(lua_State* s);
	static int LogEventWithInfo(lua_State* s);
	static int SetAnalyticsSdk(lua_State* s);
	static int ShowAds(lua_State* s);
	static int HideAds(lua_State* s);
	static int QueryPoints(lua_State* s);
	static int SpendPoints(lua_State* s);
	static int SetAdsSdk(lua_State* s);
	static int StartPush(lua_State* s);
	static int ClosePush(lua_State* s);
	static int SetAlias(lua_State* s);
	static int DelAlias(lua_State* s);
	static int SetTags(lua_State* s);
	static int DelTags(lua_State* s);
	// 需设置参数：info["content"] info["title"] info["tickText"](小tick提示，点开后或者下拉后会显示title和content内容) info["interval"](单位：秒）
	// 可选参数：info["notifyId"] clearNotification()需用到
	static int AddNotify(lua_State* s);
	// 取消特定id的本地推送
	static int ClearNotify(lua_State* s);
	// 取消所有本地推送
	static int ClearAllNotify(lua_State* s);
	// [可选接口] 设置推送类sdk名称，如果只有一个推送类sdk，可以不显示调用此接口，而在uuSdkConfig.json内设置："defaultPushSdk":"ads sdk name"(eg. PushJpush)
	static int setPushSdk(lua_State* s);
    static int loadAllSdks(lua_State* s);
    // [可选接口] 获取设备信息
    static int getDeviceInfo(lua_State* s);
    // [可选接口] 获取app版本信息
    static int getAppVersionInfo(lua_State* s);
	// 第三方账号绑定
	static int OAuthBind(lua_State* s);
	// 第三方账号登录，注意，掉用过Login()，成功获取第三方token之后，才能调用该接口
	static int OAuthLogin(lua_State* s);
	// [可选接口] 设置配置所在路径（用于unity项目）
	static int AddFileSearchPath(lua_State* s);
	// [可选接口] 判定是否显示当地支付支付商页面请求, 用于海外发行避开谷歌审查
	// 当retCode=0时，params["enable"] = "0" 或者 params["enable"] = "1";
	static int CheckIsEnableLocalPay(lua_State* s);
	// [可选接口] 删除账号 需要设置参数type和回调
	static int DeleteAccount(lua_State* s);
	// [必选接口] 提交角色信息
	static int SimpSubmitRoleData(lua_State* s);
    // [测试接口] 设置自定义deviceId
    static int SetDeviceId(lua_State* s);
	// 判断是否用的是01GameSDK
	static int isUse01gameSdk(lua_State* s);
	// [可选接口] sdk相关接口
	static int callStringFunc(lua_State* s);
	
	static int OpenUrl(lua_State * s);
	static int recordDevicePoint(lua_State * s);
	static int recordRolePoint(lua_State * s);

    static void MapTable(map<string, string>& params);
    static void MapTable2(map<unsigned int, string>& params);
    static map<std::string, string> GameVersionToMap(GameVersion &version);
    static map<std::string, string> GameGroupToMap(GameGroup &group);
    static map<std::string, string> GameRoleToMap(GameRole &role);
    static map<std::string, string> GameProductToMap(GameProduct &product);
    static map<std::string, string> AoneDeviceInfoToMap(AoneDeviceInfo &device);
    static map<std::string, string> AoneAppInfoToMap(AoneAppInfo &version);
};

#endif