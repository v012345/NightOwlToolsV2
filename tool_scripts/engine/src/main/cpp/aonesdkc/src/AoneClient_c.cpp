#include "AoneClient_c.h"
#include "AoneClient.h"
#include "aonesdk.h"
#include "AonesdkConfig.h"
#include "xcore_json.h"
#include "cpfile.h"

using namespace aonesdk;


static char* c_strdup(const string& str)
{
	char* result = (char*)malloc(str.size() + 1);
	if (!str.empty()) memcpy(result, str.c_str(), str.size());
	*(result + str.size()) = '\0';
	return result;
}

void free_kvpairs(C_KVPair** args)
{
	int i = 0;
	for (C_KVPair* it = args[0]; it != NULL; it = args[++i])
	{
		free(it->Key);
		free(it->Value);
		free(it);
	}
	free(args);
}

void free_gameversions(C_GameVersion** args)
{
	int i = 0;
	for (C_GameVersion* it = args[0]; it != NULL; it = args[++i])
	{
		free(it->Notice);
		free(it->DownloadURL);
		free(it->UpdateURL);
		free(it);
	}
	free(args);
}

void free_gamegroup(C_GameGroup* arg)
{
	free(arg->Name);
	free(arg->ServerIP);
	free(arg);
}

void free_gamegroups(C_GameGroup** args)
{
	int i = 0;
	for (C_GameGroup* it = args[0]; it != NULL; it = args[++i])
	{
		free(it->Name);
		free(it->ServerIP);
		free(it);
	}
	free(args);
}

void free_gamegroupcp(C_GameGroupCP* arg)
{
	free(arg->ID);
	free(arg->Name);
	free(arg->ServerIP);
	free(arg);
}

void free_gamegroupcps(C_GameGroupCP** args)
{
	int i = 0;
	for (C_GameGroupCP* it = args[0]; it != NULL; it = args[++i])
	{
		free_gamegroupcp(it);
	}
	free(args);
}

void free_gamerole(C_GameRole* arg)
{
	free(arg->Name);
	free(arg->CreateTime);
	free(arg->LastLoginTime);
	free_kvpairs(arg->Features);
	free(arg);
}

void free_gameroles(C_GameRole** args)
{
	int i = 0;
	for (C_GameRole* it = args[0]; it != NULL; it = args[++i])
	{
		free_gamerole(it);
	}
	free(args);
}

void free_gamerolecp(C_GameRoleCP* arg)
{
	free(arg->ID);
	free(arg->Name);
	free(arg->CreateTime);
	free(arg->LastLoginTime);
	free(arg->OwnerGroupID);
	free(arg->LastGroupID);
	free_kvpairs(arg->Features);
	free(arg);
}

void  free_gamerolecps(C_GameRoleCP** args)
{
	int i = 0;
	for (C_GameRoleCP* it = args[0]; it != NULL; it = args[++i])
	{
		free_gamerolecp(it);
	}
	free(args);
}

void free_gameproducts(C_GameProduct** args)
{
	int i = 0;
	for (C_GameProduct* it = args[0]; it != NULL; it = args[++i])
	{
		free(it->ID);
		free(it->Name);
		free(it->Describe);
		free(it->Currency);
		free(it->Type);
		free(it);
	}
	free(args);
}

void free_aonedeviceinfo(C_AoneDeviceInfo** args)
{
	int i = 0;
	for (C_AoneDeviceInfo* it = args[0]; it != NULL; it = args[++i])
	{
		free(it->DeviceId);
		free(it->Locale);
		free(it->Language);
		free(it->Model);
		free(it->Os);
		free(it->Imei);
		free(it->Ip);
		free(it->Mac);
		free(it->Idfa);
		free(it->NetworkType);
	}
	free(args);
}

void free_aoneappinfo(C_AoneAppInfo** args)
{
	int i = 0; 
	for (C_AoneAppInfo* it = args[0]; it != NULL; it = args[++i])
	{
		free(it->AppLang);
		free(it->Channel);
	}
	free(args);
}

static C_AONECLIENT_CB cb_init_sdk = NULL;
static void init_sdk_callback(int retCode, map<string, string> params)
{
	int i = 0;
	struct C_KVPair** args = (struct C_KVPair**)malloc(sizeof(struct C_KVPair*) * (params.size() + 1));
	for (map<string, string>::iterator it = params.begin(); it != params.end(); ++it, ++i)
	{
		args[i] = (struct C_KVPair*)malloc(sizeof(struct C_KVPair));
		args[i]->Key = c_strdup(it->first);
		args[i]->Value = c_strdup(it->second);
	}
	args[i] = NULL;

	cb_init_sdk(retCode, (const struct C_KVPair**)args);
	free_kvpairs(args);
}

void init_sdk(C_AONECLIENT_CB cb)
{
	cb_init_sdk = cb;
	AoneSDK::getInstance()->initAsync(init_sdk_callback);
}

static string aoneConfigData = "";
const char* get_aoneconfig_data(const char* key)
{
	aoneConfigData = AonesdkConfig::getInstance()->getAoneConfigData(key);
	return aoneConfigData.c_str();
}

struct C_GameProduct** get_all_products()
{
	map<string, GameProduct> allproducts = AoneSDK::getInstance()->getAllProducts();

	int i = 0;
	struct C_GameProduct** products = (struct C_GameProduct**)malloc(sizeof(struct C_GameProduct*) * (allproducts.size() + 1));
	for (map<string, GameProduct>::iterator it = allproducts.begin(); it != allproducts.end(); ++it, ++i)
	{
		products[i] = (struct C_GameProduct*)malloc(sizeof(struct C_GameProduct));
		products[i]->ID = c_strdup(it->second.ID);
		products[i]->Name = c_strdup(it->second.Name);
		products[i]->Describe = c_strdup(it->second.Describe);
		products[i]->Currency = c_strdup(it->second.Currency);
		products[i]->Amount = it->second.Amount;
		products[i]->Amount_usd = it->second.Amount_usd;
		products[i]->Coin = it->second.Coin;
		products[i]->Type = c_strdup(it->second.Type);
	}
	products[i] = NULL;
	return products;
}

struct C_KVPair** get_all_langs()
{
	map<string, string> alllangs = AoneSDK::getInstance()->getAllAppLangs();

	int i = 0;
	struct C_KVPair** langs = (struct C_KVPair**)malloc(sizeof(struct C_KVPair*) * (alllangs.size() + 1));
	for (map<string, string>::iterator it = alllangs.begin(); it != alllangs.end(); ++it, ++i)
	{
		langs[i] = (struct C_KVPair*)malloc(sizeof(struct C_KVPair));
		langs[i]->Key = c_strdup(it->first);
		langs[i]->Value = c_strdup(it->second);
	}
	langs[i] = NULL;
	return langs;
}

int select_lang(const char* lang)
{
	if (!AoneSDK::getInstance()->setAppLang(lang))
	{
		return -10;
	}
	return 0;
}

static C_AONECLIENT_CB cb_check_version = NULL;
static void check_version_callback(int retCode, map<string, string> params)
{
	int i = 0;
	struct C_KVPair** args = (struct C_KVPair**)malloc(sizeof(struct C_KVPair*) * (params.size() + 1));
	for (map<string, string>::iterator it = params.begin(); it != params.end(); ++it, ++i)
	{
		args[i] = (struct C_KVPair*)malloc(sizeof(struct C_KVPair));
		args[i]->Key = c_strdup(it->first);
		args[i]->Value = c_strdup(it->second);
	}
	args[i] = NULL;

	cb_check_version(retCode, (const struct C_KVPair**)args);
	free_kvpairs(args);
}

void check_version(unsigned char majorVersion, unsigned char minorVersion, unsigned char revisionVersion, C_AONECLIENT_CB cb)
{
	cb_check_version = cb;
	AoneSDK::getInstance()->checkVersionAsync(majorVersion, minorVersion, revisionVersion, check_version_callback);
}

C_GameVersion* get_game_version()
{
	GameVersion v = AoneSDK::getInstance()->getGameVersion();

	C_GameVersion* version = (C_GameVersion*)malloc(sizeof(C_GameVersion));
	version->MajorVersion = v.MajorVersion;
	version->MinorVersion = v.MinorVersion;
	version->RevisionVersion = v.RevisionVersion;
	version->Notice = c_strdup(v.Notice);
	version->DownloadURL = c_strdup(v.DownloadURL);
	version->UpdateURL = c_strdup(v.UpdateURL);
	version->Status = (GameVersionStatus)v.Status;
    version->BigPatcherURL = c_strdup(v.BigPatcherURL);
	return version;
}

void set_game_version(int major_version, int minor_version ,int revision_version){
	AoneSDK::getInstance()->setAppVersionInfo(major_version,minor_version,revision_version);
}

int is_need_patch(const char* packageResDir, const char* patchResDir)
{
	return AoneSDK::getInstance()->isNeedPatch(packageResDir, patchResDir) ? 1 : 0;
}

int is_need_force_update(const char* packageResDir, const char* patchResDir)
{
	return AoneSDK::getInstance()->isNeedForceUpdate(packageResDir, patchResDir) ? 1 : 0;
}

int is_use_01gamesdk()
{
	return AoneSDK::getInstance()->isUse01gameAndNotBackEnd() ? 1 : 0;
}

void set_use_user_login(int useUserLogin)
{
	//bool isUse = useUserLogin ? true : false;
	AoneSDK::getInstance()->setUseUserLogin(useUserLogin);
}

static C_AONECLIENT_CB cb_login = NULL;
static void login_callback(int retCode, map<string, string> params)
{
	int i = 0;
	struct C_KVPair** args = (struct C_KVPair**)malloc(sizeof(struct C_KVPair*) * (params.size() + 1));
	for (map<string, string>::iterator it = params.begin(); it != params.end(); ++it, ++i)
	{
		args[i] = (struct C_KVPair*)malloc(sizeof(struct C_KVPair));
		args[i]->Key = c_strdup(it->first);
		args[i]->Value = c_strdup(it->second);
	}
	args[i] = NULL;

	cb_login(retCode, (const struct C_KVPair**)args);
	free_kvpairs(args);
}

void login(C_AONECLIENT_CB cb)
{
	cb_login = cb;
	AoneSDK::getInstance()->login(login_callback);
}

unsigned int user_id()
{
	return AoneSDK::getInstance()->getUserId();
}

unsigned int user_token()
{
	return AoneSDK::getInstance()->getUserToken();
}

static string aoneToken = "";
const char* get_token()
{
	aoneToken = AoneSDK::getInstance()->getToken();
	return aoneToken.c_str();
}

static string userAccount = "";
const char* get_user_account()
{
	userAccount = AoneSDK::getInstance()->getUserAccount();
	return userAccount.c_str();
}

struct C_GameGroup** get_all_groups()
{
	vector<GameGroup> allgroups = AoneSDK::getInstance()->getAllGroups();

	int i = 0;
	struct C_GameGroup** groups = (struct C_GameGroup**)malloc(sizeof(struct C_GameGroup*) * (allgroups.size() + 1));
	for (vector<GameGroup>::iterator it = allgroups.begin(); it != allgroups.end(); ++it, ++i)
	{
		groups[i] = (struct C_GameGroup*)malloc(sizeof(struct C_GameGroup));
		groups[i]->ID = it->ID;
		groups[i]->Name = c_strdup(it->Name);
		groups[i]->Status = (GameGroupStatus)it->Status;
		groups[i]->ServerIP = c_strdup(it->ServerIP);
		groups[i]->ServerPort = it->ServerPort;
		groups[i]->IsRecommend = it->IsRecommend ? 1 : 0;
		groups[i]->IsInitialed = it->IsInitialed ? 1 : 0;
	}
	groups[i] = NULL;
	return groups;
}

struct C_GameRole** get_all_roles()
{
	vector<GameRole> allroles = AoneSDK::getInstance()->getAllRoles();

	int i = 0;
	struct C_GameRole** roles = (struct C_GameRole**)malloc(sizeof(struct C_GameGroup*) * (allroles.size() + 1));
	for (vector<GameRole>::iterator it = allroles.begin(); it != allroles.end(); ++it, ++i)
	{
		roles[i] = (struct C_GameRole*)malloc(sizeof(struct C_GameRole));
		roles[i]->ID = it->ID;
		roles[i]->Name = c_strdup(it->Name);
		roles[i]->Type = it->Type;
		roles[i]->Level = it->Level;
		roles[i]->VIP = it->VIP;
		roles[i]->CreateTime = c_strdup(it->CreateTime);
		roles[i]->LastLoginTime = c_strdup(it->LastLoginTime);
		roles[i]->LastGroupID = it->LastGroupID;
		roles[i]->OwnerGroupID = it->OwnerGroupID;

		map<string, string>& features = it->Features;
		int j = 0;
		roles[i]->Features = (struct C_KVPair**)malloc(sizeof(struct C_KVPair*) * (it->Features.size() + 1));
		for (map<string, string>::iterator it2 = features.begin(); it2 != features.end(); ++it2, ++j)
		{
			roles[i]->Features[j] = (struct C_KVPair*)malloc(sizeof(struct C_KVPair));
			roles[i]->Features[j]->Key = c_strdup(it2->first);
			roles[i]->Features[j]->Value = c_strdup(it2->second);
		}
		roles[i]->Features[j] = NULL;
	}
	roles[i] = NULL;
	return roles;
}

static C_AONECLIENT_CB cb_crate_role= NULL;
static void create_role_callback(int retCode, map<string, string> params)
{
	int i = 0;
	struct C_KVPair** args = (struct C_KVPair**)malloc(sizeof(struct C_KVPair*) * (params.size() + 1));
	for (map<string, string>::iterator it = params.begin(); it != params.end(); ++it, ++i)
	{
		args[i] = (struct C_KVPair*)malloc(sizeof(struct C_KVPair));
		args[i]->Key = c_strdup(it->first);
		args[i]->Value = c_strdup(it->second);
	}
	args[i] = NULL;

	cb_crate_role(retCode, (const struct C_KVPair**)args);
	free_kvpairs(args);
}

static map<string, string> json_to_map(const char* jsonData)
{
	XJsonValue root;
	if (!XJsonReader().parse(jsonData, root, false) || root.type() != XJsonValue::objectValue)
	{
		return map<string, string>();
	}

	map<string, string> mapData;
	XJsonValue::ObjectValues& objs = root.as_objects();
	for (XJsonValue::ObjectIterator it = objs.begin(); it != objs.end(); ++it)
	{
		if (it->second.type() != XJsonValue::stringValue) {
			return map<string, string>();
		}
		mapData[it->first] = it->second.as_string();
	}
	return mapData;
}

void create_role(const char* name, unsigned int type, const char* features, unsigned int owner_groupid, C_AONECLIENT_CB cb)
{
	cb_crate_role = cb;

	map<string, string> f = json_to_map(features);

	AoneSDK::getInstance()->createRoleAsync(name, type, f, owner_groupid, create_role_callback);
}

void submit_role_data(const char* roleID, const char* name, unsigned int level, const char* groupId, const char* groupName, int roleVip, int isLogin, const char* action,unsigned long long roleCTime)
{
	bool login = isLogin ? true : false;
	
	AoneSDK::getInstance()->thirdPartySubmitRoleData(roleID, name, level, groupId, groupName, roleVip, login, action,roleCTime);
}

static C_AONECLIENT_CB cb_select_role_and_group = NULL;
static void select_role_and_group_callback(int retCode, map<string, string> params)
{
	int i = 0;
	struct C_KVPair** args = (struct C_KVPair**)malloc(sizeof(struct C_KVPair*) * (params.size() + 1));
	for (map<string, string>::iterator it = params.begin(); it != params.end(); ++it, ++i)
	{
		args[i] = (struct C_KVPair*)malloc(sizeof(struct C_KVPair));
		args[i]->Key = c_strdup(it->first);
		args[i]->Value = c_strdup(it->second);
	}
	args[i] = NULL;

	cb_select_role_and_group(retCode, (const struct C_KVPair**)args);
	free_kvpairs(args);
}

void select_role_and_group(unsigned int role_id, unsigned int group_id, C_AONECLIENT_CB cb)
{
	cb_select_role_and_group = cb;
	AoneSDK::getInstance()->selectRoleAndGroupAsync(role_id, group_id, select_role_and_group_callback);
}

static C_AONECLIENT_CB cb_push = NULL;
static void push_callback(int retCode, map<string, string> params)
{
	int i = 0;
	struct C_KVPair** args = (struct C_KVPair**)malloc(sizeof(struct C_KVPair*) * (params.size() + 1));
	for (map<string, string>::iterator it = params.begin(); it != params.end(); ++it, ++i)
	{
		args[i] = (struct C_KVPair*)malloc(sizeof(struct C_KVPair));
		args[i]->Key = c_strdup(it->first);
		args[i]->Value = c_strdup(it->second);
	}
	args[i] = NULL;

	cb_push(retCode, (const struct C_KVPair**)args);
	free_kvpairs(args);
}

void pull_game_group(C_AONECLIENT_CB cb)
{
	cb_push = cb;
	AoneSDK::getInstance()->pullGameGroupAsync(push_callback);
}

C_GameRole* get_selected_role()
{
	GameRole r = AoneSDK::getInstance()->getSelectedRole();

	C_GameRole* role = (C_GameRole*)malloc(sizeof(C_GameRole));
	role->ID = r.ID;
	role->Name = c_strdup(r.Name);
	role->Type = r.Type;
	role->Level = r.Level;
	role->VIP = r.VIP;
	role->CreateTime = c_strdup(r.CreateTime);
	role->LastLoginTime = c_strdup(r.LastLoginTime);
	role->LastGroupID = r.LastGroupID;
	role->OwnerGroupID = r.OwnerGroupID;

	map<string, string>& features = r.Features;
	int j = 0;
	role->Features = (struct C_KVPair**)malloc(sizeof(struct C_KVPair*) * (r.Features.size() + 1));
	for (map<string, string>::iterator it2 = features.begin(); it2 != features.end(); ++it2, ++j)
	{
		role->Features[j] = (struct C_KVPair*)malloc(sizeof(struct C_KVPair));
		role->Features[j]->Key = c_strdup(it2->first);
		role->Features[j]->Value = c_strdup(it2->second);
	}
	role->Features[j] = NULL;
	return role;
}

C_GameGroup* get_selected_group()
{
	GameGroup g = AoneSDK::getInstance()->getSelectedGroup();

	C_GameGroup* group = (C_GameGroup*)malloc(sizeof(C_GameGroup));
	group->ID = g.ID;
	group->Name = c_strdup(g.Name);
	group->Status = (GameGroupStatus)g.Status;
	group->ServerIP = c_strdup(g.ServerIP);
	group->ServerPort = g.ServerPort;
	group->IsRecommend = g.IsRecommend ? 1 : 0;
	group->IsInitialed = g.IsInitialed ? 1 : 0;
	return group;
}

C_AoneDeviceInfo* get_device_info()
{
	AoneDeviceInfo d = AoneClient::getDeviceInfo();

	C_AoneDeviceInfo* deviceInfo = (C_AoneDeviceInfo*)malloc(sizeof(C_AoneDeviceInfo));
	deviceInfo->DeviceId = c_strdup(d.DeviceId);
	deviceInfo->Idfa = c_strdup(d.Idfa);
	deviceInfo->Imei = c_strdup(d.Imei);
	deviceInfo->Ip = c_strdup(d.Ip);
	deviceInfo->Language = c_strdup(d.Language);
	deviceInfo->Locale = c_strdup(d.Locale);
	deviceInfo->Mac = c_strdup(d.Mac);
	deviceInfo->Model = c_strdup(d.Model);
	deviceInfo->NetworkType = c_strdup(d.NetworkType);
	deviceInfo->Os = c_strdup(d.Os);
	return deviceInfo;
}

static C_AONECLIENT_CB cb_pay = NULL;
static void pay_callback(int retCode, map<string, string> params)
{
	int i = 0;
	struct C_KVPair** args = (struct C_KVPair**)malloc(sizeof(struct C_KVPair*) * (params.size() + 1));
	for (map<string, string>::iterator it = params.begin(); it != params.end(); ++it, ++i)
	{
		args[i] = (struct C_KVPair*)malloc(sizeof(struct C_KVPair));
		args[i]->Key = c_strdup(it->first);
		args[i]->Value = c_strdup(it->second);
	}
	args[i] = NULL;

	cb_pay(retCode, (const struct C_KVPair**)args);
	free_kvpairs(args);
}

void pay(const char* product_id, unsigned int purchaseNum, const char* cp_ext, C_AONECLIENT_CB cb)
{
	cb_pay = cb;
	AoneSDK::getInstance()->pay(product_id, purchaseNum, cp_ext, pay_callback);
}

static C_AONECLIENT_CB cb_logout = NULL;
static void logout_callback(int retCode, map<string, string> params)
{
	int i = 0;
	struct C_KVPair** args = (struct C_KVPair**)malloc(sizeof(struct C_KVPair*) * (params.size() + 1));
	for (map<string, string>::iterator it = params.begin(); it != params.end(); ++it, ++i)
	{
		args[i] = (struct C_KVPair*)malloc(sizeof(struct C_KVPair));
		args[i]->Key = c_strdup(it->first);
		args[i]->Value = c_strdup(it->second);
	}
	args[i] = NULL;

	cb_logout(retCode, (const struct C_KVPair**)args);
	free_kvpairs(args);
}

void logout(C_AONECLIENT_CB cb)
{
	cb_logout = cb;

	AoneSDK::getInstance()->logout(logout_callback);
}

void exit_game()
{
	AoneSDK::getInstance()->exit();
}

int has_exit_ui()
{
	return AoneSDK::getInstance()->hasExitUI() ? 1 : 0;
}

static C_AONECLIENT_CB cb_common = NULL;
static void common_callback(int retCode, map<string, string> params)
{
	int i = 0;
	struct C_KVPair** args = (struct C_KVPair**)malloc(sizeof(struct C_KVPair*) * (params.size() + 1));
	for (map<string, string>::iterator it = params.begin(); it != params.end(); ++it, ++i)
	{
		args[i] = (struct C_KVPair*)malloc(sizeof(struct C_KVPair));
		args[i]->Key = c_strdup(it->first);
		args[i]->Value = c_strdup(it->second);
	}
	args[i] = NULL;
	if (cb_common) {
		cb_common(retCode, (const struct C_KVPair**)args);
	}
	free_kvpairs(args);
}

int  quick_reister()
{
	return AoneSDK::getInstance()->quickRegister();
}

void quick_register_async(C_AONECLIENT_CB cb)
{
	cb_common = cb;
	AoneSDK::getInstance()->quickRegisterAsync(common_callback);
}

static string aoneAccount = "";
const char* get_aone_account()
{
	aoneAccount = AoneSDK::getInstance()->getAoneAccount();
	return aoneAccount.c_str();
}

static string aonePassword = "";
const char* get_aone_password()
{
	aonePassword = AoneSDK::getInstance()->getAonePassword();
	return aonePassword.c_str();
}

int quick_login()
{
	return AoneSDK::getInstance()->quickLogin();
}

void quick_login_async(C_AONECLIENT_CB cb)
{
	cb_common = cb;
	AoneSDK::getInstance()->quickLoginAsync(common_callback);
}
int aone_register(const char* account, const char* password)
{
	return AoneSDK::getInstance()->aoneRegister(account, password);
}
void aone_register_async(const char* account, const char* password, C_AONECLIENT_CB cb)
{
	cb_common = cb;
	AoneSDK::getInstance()->aoneRegisterAsync(account, password, common_callback);
}

int aone_login(const char* account, const char* password)
{
	return AoneSDK::getInstance()->aoneLogin(account, password);
}

void aone_login_async(const char* account, const char* password, C_AONECLIENT_CB cb)
{
	cb_common = cb;
	AoneSDK::getInstance()->aoneLoginAsync(account, password, common_callback);
}

int aone_change_password(const char* oldpass, const char* newpass)
{
	return AoneSDK::getInstance()->AoneChangePassword(oldpass, newpass);
}

void aone_change_password_async(const char* oldpass, const char* newpass, C_AONECLIENT_CB cb)
{
	cb_common = cb;
	AoneSDK::getInstance()->AoneChangePasswordAsync(oldpass, newpass, common_callback);
}

void aone_create_inheritance(const char* password, C_AONECLIENT_CB cb)
{
	cb_common = cb;
	AoneSDK::getInstance()->AoneCreateInheritance(password, common_callback);
}

int aone_bind(const char* account, const char* password)
{
	return AoneSDK::getInstance()->AoneBind(account, password);
}

void aone_bind_async(const char* account, const char* password, C_AONECLIENT_CB cb)
{
	cb_common = cb;
	AoneSDK::getInstance()->AoneBindAsync(account, password, common_callback);
}

void callFunc(const char* funcName)
{
	AoneSDK::getInstance()->callFunc(funcName);
}

void callFuncWithStringParam( const char* funcName, const char* param )
{
	AoneSDK::getInstance()->callFuncWithStringParam(funcName, param);
}

void callFuncWithParam( const char* funcName, void* param )
{
    AoneSDK::getInstance()->callFuncWithParam(funcName, param);
}

int callIntFunc(const char* funcName, void* param)
{
	return AoneSDK::getInstance()->callIntFunc(funcName, param);
}

static string funcRetuenVlaue1 = "";
const char* callStringFunc(const char* funcName)
{
	funcRetuenVlaue1 = AoneSDK::getInstance()->callStringFunc(funcName);
	return funcRetuenVlaue1.c_str();
}

static string funcRetuenVlaue2 = "";
const char* callStringFuncFromSDK(const char* funcName, const char* sdkName)
{
	funcRetuenVlaue2 = AoneSDK::getInstance()->callStringFunc(funcName, sdkName);
	return funcRetuenVlaue2.c_str();
}

static string funcRetuenVlaue3 = "";
const char*  callStringFuncWithParam(const char* funcName, void* param)
{
	funcRetuenVlaue3 = AoneSDK::getInstance()->callStringFuncWithParam(funcName, param);
	return funcRetuenVlaue3.c_str();
}

void add_file_search_path( const char* path )
{
	AoneSDK::getInstance()->addFileSearchPath(path);
}

void set_config_data( const char* data )
{
	AoneSDK::getInstance()->setConfigData(data);
}

void set_oAuth_type( const char* oauth_type )
{
	AoneSDK::getInstance()->setOAuthType(oauth_type);
}

static string oAuthType = "";
const char* get_oAuth_type()
{
	oAuthType = AoneSDK::getInstance()->getOAuthType();
	return oAuthType.c_str();
}

void set_pay_channel( const char* payChannel )
{
	AoneSDK::getInstance()->setPayChannel(payChannel);
}

static string payChannel = "";
const char* get_pay_channel()
{
	payChannel = AoneSDK::getInstance()->getPayChannel();
	return payChannel.c_str();
}

void set_share_sdk( const char* sdkName )
{
	AoneSDK::getInstance()->setShareSdk(sdkName);
}

static C_AONECLIENT_CB cb_share = NULL;
static void share_callback(int retCode, map<string, string> params)
{
	int i = 0;
	struct C_KVPair** args = (struct C_KVPair**)malloc(sizeof(struct C_KVPair*) * (params.size() + 1));
	for (map<string, string>::iterator it = params.begin(); it != params.end(); ++it, ++i)
	{
		args[i] = (struct C_KVPair*)malloc(sizeof(struct C_KVPair));
		args[i]->Key = c_strdup(it->first);
		args[i]->Value = c_strdup(it->second);
	}
	args[i] = NULL;

	cb_share(retCode, (const struct C_KVPair**)args);
	free_kvpairs(args);
}

void share( const char* info, C_AONECLIENT_CB cb )
{
	cb_share = cb;

	XJsonValue root;
	if (!XJsonReader().parse(info, root, false) || root.type() != XJsonValue::objectValue)
	{
		return;
	}

	map<string, string> f;
	XJsonValue::ObjectValues& objs = root.as_objects();
	for (XJsonValue::ObjectIterator it = objs.begin(); it != objs.end(); ++it)
	{
		if (it->second.type() != XJsonValue::stringValue) return;
		f[it->first] = it->second.as_string();
	}
	AoneSDK::getInstance()->share(f, share_callback);
}

void log_page_start( const char* pageName )
{
	AoneSDK::getInstance()->LogPageStart(pageName);
}

void log_page_end( const char* pageName )
{
	AoneSDK::getInstance()->LogPageEnd(pageName);
}

void log_event( const char* eventId )
{
	AoneSDK::getInstance()->LogEvent(eventId);
}

void log_event_with_params( const char* eventId, const char* info )
{
	XJsonValue root;
	if (!XJsonReader().parse(info, root, false) || root.type() != XJsonValue::objectValue)
	{
		return;
	}

	map<string, string> f;
	XJsonValue::ObjectValues& objs = root.as_objects();
	for (XJsonValue::ObjectIterator it = objs.begin(); it != objs.end(); ++it)
	{
		if (it->second.type() != XJsonValue::stringValue) return;
		f[it->first] = it->second.as_string();
	}

	AoneSDK::getInstance()->LogEvent(eventId, f);
}

void set_analytics_sdk( const char* sdkName )
{
	AoneSDK::getInstance()->setAnalyticsSdk(sdkName);
}


static C_AONECLIENT_CB cb_ads = NULL;
static void ads_callback(int retCode, map<string, string> params)
{
	int i = 0;
	struct C_KVPair** args = (struct C_KVPair**)malloc(sizeof(struct C_KVPair*) * (params.size() + 1));
	for (map<string, string>::iterator it = params.begin(); it != params.end(); ++it, ++i)
	{
		args[i] = (struct C_KVPair*)malloc(sizeof(struct C_KVPair));
		args[i]->Key = c_strdup(it->first);
		args[i]->Value = c_strdup(it->second);
	}
	args[i] = NULL;

	cb_ads(retCode, (const struct C_KVPair**)args);
	free_kvpairs(args);
}
void show_ads( const char* info, int pos, C_AONECLIENT_CB cb )
{
	cb_ads = cb;

	map<string, string> f = json_to_map(info);
	AoneSDK::getInstance()->showAds(f, pos, ads_callback);
}

void hide_ads( const char* info, C_AONECLIENT_CB cb )
{
	cb_ads = cb;
	
	map<string, string> f = json_to_map(info);
	AoneSDK::getInstance()->hideAds(f, ads_callback);
}

void query_points( C_AONECLIENT_CB cb )
{
	cb_ads = cb;

	AoneSDK::getInstance()->queryPoints(ads_callback);
}

void spend_points( int points, C_AONECLIENT_CB cb )
{
	cb_ads = cb;
	AoneSDK::getInstance()->spendPoints(points, ads_callback);
}

void set_ads_sdk( const char* sdkName )
{
	AoneSDK::getInstance()->setAdsSdk(sdkName);
}

void start_push( C_AONECLIENT_CB cb )
{
	cb_push = cb;
	AoneSDK::getInstance()->startPush(push_callback);
}

void close_push( C_AONECLIENT_CB cb )
{
	cb_push = cb;
	AoneSDK::getInstance()->closePush(push_callback);
}

void set_alias( const char* alias, C_AONECLIENT_CB cb )
{
	cb_push = cb;
	AoneSDK::getInstance()->setAlias(alias, push_callback);
}

void del_alias( const char* alias, C_AONECLIENT_CB cb )
{
	cb_push = cb;
	AoneSDK::getInstance()->delAlias(alias, push_callback);
}

void set_tags( const char* tags, C_AONECLIENT_CB cb )
{
	cb_push = cb;
	AoneSDK::getInstance()->setTags(tags, push_callback);
}

void del_tags( const char* tags, C_AONECLIENT_CB cb )
{
	cb_push = cb;
	AoneSDK::getInstance()->delTags(tags, push_callback);
}

void set_push_sdk( const char* sdkName )
{
	AoneSDK::getInstance()->setPushSdk(sdkName);
}


void add_notify( const char* info )
{
	map<string, string> mapData = json_to_map(info);
	AoneSDK::getInstance()->addNotify(mapData);
}

void clear_notify( int notifyId )
{
	AoneSDK::getInstance()->clearNotify(notifyId);
}

void clear_all_notify()
{
	AoneSDK::getInstance()->clearAllNotify();
}

void oauth_bind_async(C_AONECLIENT_CB cb)
{
	cb_common = cb;
	AoneSDK::getInstance()->OAuthBindAsync(common_callback);
}

void check_isEnable_localPay( C_AONECLIENT_CB cb )
{
	cb_common = cb;
	AoneSDK::getInstance()->checkIsEnableLocalPay(common_callback);
}

void oauth_login_async(C_AONECLIENT_CB cb)
{
	cb_common = cb;
	AoneSDK::getInstance()->OAuthLoginAsync(common_callback);
}

void aone_bind_otheraccount_async(const char* newaccount, C_AONECLIENT_CB cb)
{
	cb_common = cb;
	AoneSDK::getInstance()->aoneBindOtherAccount(newaccount,common_callback);
}

void aone_unbind_otheraccount_async(C_AONECLIENT_CB cb)
{
	cb_common = cb;
	AoneSDK::getInstance()->aoneUnbindOtherAccount(common_callback);
}

void aone_querybind_otheraccount_async(C_AONECLIENT_CB cb)
{
	cb_common = cb;
	AoneSDK::getInstance()->aoneQueryBindOtherAccount(common_callback);
}

void load_all_sdks()
{
	AoneSDK::getInstance()->loadAllSdks();
}

void set_device_id(const char* deviceId)
{
	AoneSDK::getInstance()->setDeviceId(deviceId);
}

void copy_to_clipboard(const char* text){
	AoneSDK::getInstance()->copyToClipboard(text);
}
void joinQGroup(){
	AoneSDK::getInstance()->joinQGroup();
}

//app内调起苹果评星、评论
void starReviewInApp(const char* appid){
    AoneSDK::getInstance() -> starReviewInApp(appid);
}

//应用内评星
void starInApp(){
    AoneSDK::getInstance() -> starInApp();
}
//应用内调起苹果商店评论
void reviewInApp(const char* appId){
    AoneSDK::getInstance() -> reviewInApp(appId);
}

int isInstallApp(const char* packageName){
	return AoneSDK::getInstance() ->isInstallApp(packageName);
}

//获取aonesdk.json文件
std::string AoneClient::getSdkConfig()
{
    return AoneSDK::getInstance() -> getSdkConfig();
}
//获取uusdkConfig.json
std::string AoneClient::getPluginConfig()
{
    return AoneSDK::getInstance() -> getPluginConfig();
}

void recordDevicePoint(unsigned int sdkPoint){
	AoneSDK::getInstance()->recordDevicePoint(sdkPoint);
}

void recordRolePoint(unsigned int gamePoint){
	AoneSDK::getInstance()->recordRolePoint(gamePoint);
}


static C_AONECLIENT_CB cb_realname_info = NULL;
static void realname_info_callback(int retCode, map<string, string> params)
{
	int i = 0;
	struct C_KVPair** args = (struct C_KVPair**)malloc(sizeof(struct C_KVPair*) * (params.size() + 1));
	for (map<string, string>::iterator it = params.begin(); it != params.end(); ++it, ++i)
	{
		args[i] = (struct C_KVPair*)malloc(sizeof(struct C_KVPair));
		args[i]->Key = c_strdup(it->first);
		args[i]->Value = c_strdup(it->second);
	}
	args[i] = NULL;

	cb_realname_info(retCode, (const struct C_KVPair**)args);
	free_kvpairs(args);
}

void realname_info(C_AONECLIENT_CB cb){
	AoneSDK::getInstance()->realnameInfoAsync(realname_info_callback);
}



static C_AONECLIENT_CB cb_realname_certificate = NULL;
static void realname_certificate_callback(int retCode, map<string, string> params)
{
	int i = 0;
	struct C_KVPair** args = (struct C_KVPair**)malloc(sizeof(struct C_KVPair*) * (params.size() + 1));
	for (map<string, string>::iterator it = params.begin(); it != params.end(); ++it, ++i)
	{
		args[i] = (struct C_KVPair*)malloc(sizeof(struct C_KVPair));
		args[i]->Key = c_strdup(it->first);
		args[i]->Value = c_strdup(it->second);
	}
	args[i] = NULL;

	cb_realname_certificate(retCode, (const struct C_KVPair**)args);
	free_kvpairs(args);
}

static C_AONECLIENT_CB cb_request_player_extra_info = NULL;
static void request_player_extra_info_callback(int retCode, map<string, string> params)
{
	int i = 0;
	struct C_KVPair** args = (struct C_KVPair**)malloc(sizeof(struct C_KVPair*) * (params.size() + 1));
	for (map<string, string>::iterator it = params.begin(); it != params.end(); ++it, ++i)
	{
		args[i] = (struct C_KVPair*)malloc(sizeof(struct C_KVPair));
		args[i]->Key = c_strdup(it->first);
		args[i]->Value = c_strdup(it->second);
	}
	args[i] = NULL;

	cb_request_player_extra_info(retCode, (const struct C_KVPair**)args);
	free_kvpairs(args);
}

void realname_certificate(const char* identity_number, const char* real_name, C_AONECLIENT_CB cb){
	AoneSDK::getInstance()->realnameCertificateAsync(identity_number, real_name,realname_certificate_callback);
}

void request_player_extra_info(C_AONECLIENT_CB cb){
	AoneSDK::getInstance()->requestPlayerExtraInfo(request_player_extra_info_callback);
}

unsigned char* load_file_to_bytes(const char* fileName, unsigned long* read_size){
	return cpfile_read(fileName,read_size);
}

bool check_file_exists(const char* path){
	return cpfile_exist(path);
}
