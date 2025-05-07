#include "AoneClient.h"
#include "aonesdk.h"
#include "AonesdkConfig.h"
#include "uuDevice.h"
#include "cpfile.h"

namespace aonesdk {

AoneClient::AoneClient()
{
	// empty
}

AoneClient::~AoneClient()
{
	// empty
}
    
/**
 *  简化版接口
 *  简化版接口只实现了初始化登录支付等基本功能
 *  如果需要实现更多功能请接入完整版接口
 *  所有简化版接口均以Simp开头
 */
void AoneClient::SimpInitSDK(AONECLIENT_CB cb)
{
	//AoneSDK::getInstance()->init(cb);
	AoneSDK::getInstance()->initAsync(cb);
}

void AoneClient::SimpExit()
{
	AoneSDK::getInstance()->exit();
}

bool AoneClient::SimpHasExitUI()
{
	return AoneSDK::getInstance()->hasExitUI();
}

std::map<std::string, GameProduct> AoneClient::SimpAllProducts()
{
	return AoneSDK::getInstance()->getAllProducts();
}

std::map<std::string, std::string> AoneClient::SimpAllLangs()
{
	return AoneSDK::getInstance()->getAllAppLangs();
}

int AoneClient::SimpSelectLang(const std::string& lang)
{
	if (!AoneSDK::getInstance()->setAppLang(lang))
	{
		return -10;
	}
	return 0;
}

void AoneClient::SimpLogin(AONECLIENT_CB cb)
{
    AoneSDK::getInstance() -> setUseUserLogin(false);
    AoneSDK::getInstance() -> login(cb);
//	AoneSDK::getInstance()->thirdPartyLogin(cb);
}

unsigned int AoneClient::SimpUserID()
{
	return AoneSDK::getInstance()->getUserId();
}

const std::string AoneClient::SimpUserToken()
{
	return AoneSDK::getInstance()->getToken();
}

void AoneClient::SimpCreateRole(const std::string& roleID, const std::string& name, unsigned int level, const std::map<std::string, std::string>& features, const std::string& owner_groupid, std::string groupName, int roleVIP)
{
	AoneSDK::getInstance()->createRoleCP(roleID, name, level, features, owner_groupid, groupName, roleVIP);
}

void AoneClient::SimpSubmitRoleData(const std::string& roleID, const std::string& name, unsigned int level, const std::string& groupId, const std::string& groupName, int roleVip, bool isLogin /*= true*/, std::string action /*= "login"*/, unsigned long long roleCTime /*= 0*/)
{
	AoneSDK::getInstance()->thirdPartySubmitRoleData(roleID, name, level, groupId, groupName, roleVip, isLogin, action, roleCTime);
}

void AoneClient::SimpEnablePayChannels(const std::string& payChannels)
{
	callFuncWithStringParam("enablePayChannels", payChannels.c_str());
}

void AoneClient::SimpPay(const std::string& product_id, unsigned int purchaseNum, const std::string& cp_ext, AONECLIENT_CB cb)
{
	AoneSDK::getInstance()->pay(product_id, purchaseNum, cp_ext, cb);
}

void AoneClient::SimpLogout(AONECLIENT_CB cb)
{
	aonesdk::AoneSDK::getInstance()->logout(cb);
}
    
/******************************************/

unsigned int AoneClient::UserID()
{
	return AoneSDK::getInstance()->getUserId();
}

unsigned int AoneClient::UserToken()
{
	return AoneSDK::getInstance()->getUserToken();
}

string AoneClient::getUserAccount(){
	return AoneSDK::getInstance()->getUserAccount();
}

vector<GameGroup> AoneClient::AllGroups()
{
	return AoneSDK::getInstance()->getAllGroups();
}

vector<GameRole> AoneClient::AllRoles()
{
	return AoneSDK::getInstance()->getAllRoles();
}

std::string AoneClient::AoneAccount()
{
	return AoneSDK::getInstance()->getAoneAccount();
}

std::string AoneClient::AonePassword()
{
	return AoneSDK::getInstance()->getAonePassword();
}

map<string, GameProduct> AoneClient::AllProducts()
{
	return AoneSDK::getInstance()->getAllProducts();
}

map<string, string> AoneClient::AllLangs()
{
	return AoneSDK::getInstance()->getAllAppLangs();
}

void AoneClient::InitSDK(AONECLIENT_CB cb)
{
	//AoneSDK::getInstance()->init(cb);
	AoneSDK::getInstance()->initAsync(cb);
}

std::string AoneClient::GetAoneConfigData(std::string key)
{
	return AonesdkConfig::getInstance()->getAoneConfigData(key);
}

int AoneClient::SelectLang(const string& lang)
{
	if (!AoneSDK::getInstance()->setAppLang(lang))
	{
		return -10;
	}
	return 0;
}

int AoneClient::CheckVersion(GameVersion& version, unsigned char majorVersion, unsigned char minorVersion, unsigned char revisionVersion)
{
	int ret = AoneSDK::getInstance()->checkVersion(majorVersion, minorVersion, revisionVersion);
	if (ret != 0) return ret;
	version = AoneSDK::getInstance()->getGameVersion();
	return 0;
}

void AoneClient::setGameVersion(int major_version, int minor_version ,int revision_version){
	AoneSDK::getInstance()->setAppVersionInfo(major_version,minor_version,revision_version);
}

void AoneClient::CheckVersion(unsigned char majorVersion, unsigned char minorVersion, unsigned char revisionVersion, AONECLIENT_CB cb)
{
	AoneSDK::getInstance()->checkVersionAsync(majorVersion, minorVersion, revisionVersion, cb);
}

GameVersion AoneClient::GetGameVersion()
{
	return AoneSDK::getInstance()->getGameVersion();
}


bool AoneClient::IsNeedPatch(string packageResDir, string patchResDir)
{
	return AoneSDK::getInstance()->isNeedPatch(packageResDir, patchResDir);
}

bool AoneClient::IsNeedForceUpdate(string packageResDir, string patchResDir)
{
	return AoneSDK::getInstance()->isNeedForceUpdate(packageResDir, patchResDir);
}

int AoneClient::CreateRole(GameRole& role, const std::string& name, unsigned int type, const std::map<std::string, std::string>& features, unsigned int owner_groupid)
{
	int ret = AoneSDK::getInstance()->createRole(name, type, features, owner_groupid);
	if (ret != 0) return ret;
	role = AoneSDK::getInstance()->getSelectedRole();
	return ret;
}

void AoneClient::CreateRole(const std::string& name, unsigned int type, const std::map<std::string, std::string>& features, unsigned int owner_groupid, AONECLIENT_CB cb)
{
	AoneSDK::getInstance()->createRoleAsync(name, type, features, owner_groupid, cb);
}

int AoneClient::SelectRoleAndGroup(unsigned int role_id, unsigned int group_id)
{
	return AoneSDK::getInstance()->selectRoleAndGroup(role_id, group_id);
}

void AoneClient::SelectRoleAndGroup(unsigned int role_id, unsigned int group_id, AONECLIENT_CB cb)
{
	AoneSDK::getInstance()->selectRoleAndGroupAsync(role_id, group_id, cb);
}

void AoneClient::pullGameGroup(AONECLIENT_CB cb)
{
	AoneSDK::getInstance()->pullGameGroupAsync(cb);
}

GameRole AoneClient::GetSelectedRole()
{
	return AoneSDK::getInstance()->getSelectedRole();
}

GameGroup AoneClient::GetSelectedGroup()
{
	return AoneSDK::getInstance()->getSelectedGroup();
}

void AoneClient::SetUseUserLogin(bool useUserLogin)
{
	AoneSDK::getInstance()->setUseUserLogin(useUserLogin);
}

void AoneClient::Login(AONECLIENT_CB cb)
{
	AoneSDK::getInstance()->login(cb);
}
    
void AoneClient::ThirdParthLogin(AONECLIENT_CB cb)
{
    AoneSDK::getInstance() -> thirdPartyLogin(cb);
}

void AoneClient::Logout(AONECLIENT_CB cb)
{
	AoneSDK::getInstance()->logout(cb);
}

void AoneClient::Exit()
{
	AoneSDK::getInstance()->exit();
}

bool AoneClient::hasExitUI()
{
	return AoneSDK::getInstance()->hasExitUI();
}
    
void AoneClient::Pay(const std::string& product_id, const std::string& cp_ext, AONECLIENT_CB cb)
{
    AoneSDK::getInstance()->pay(product_id, 1, cp_ext, cb);
}
    
void AoneClient::Pay(const std::string& product_id, unsigned int purchaseNum, const std::string& cp_ext, AONECLIENT_CB cb)
{
	AoneSDK::getInstance()->pay(product_id, purchaseNum, cp_ext, cb);
}

// 同步调用
int AoneClient::QuickRegister()
{
	return AoneSDK::getInstance()->quickRegister();
}
// 异步调用
void AoneClient::QuickRegister(AONECLIENT_CB cb)
{
	AoneSDK::getInstance()->quickRegisterAsync(cb);
}

int AoneClient::QuickLogin()
{
	return AoneSDK::getInstance()->quickLogin();
}

void AoneClient::QuickLogin(AONECLIENT_CB cb)
{
	AoneSDK::getInstance()->quickLoginAsync(cb);	
}

int AoneClient::AoneRegister(const std::string& account, const std::string& password)
{
	return AoneSDK::getInstance()->aoneRegister(account, password);
}

void AoneClient::AoneRegister(const std::string& account, const std::string& password, AONECLIENT_CB cb)
{
	AoneSDK::getInstance()->aoneRegisterAsync(account, password, cb);
}

int AoneClient::AoneLogin(const std::string& account, const std::string& password)
{
	return AoneSDK::getInstance()->aoneLogin(account, password);
}

void AoneClient::AoneLogin(const std::string& account, const std::string& password, AONECLIENT_CB cb)
{
	AoneSDK::getInstance()->aoneLoginAsync(account, password, cb);
}

int AoneClient::AoneChangePassword(const std::string& oldpass, const std::string& newpass)
{
	return AoneSDK::getInstance()->AoneChangePassword(oldpass, newpass);
}

void AoneClient::AoneChangePassword(const std::string& oldpass, const std::string& newpass, AONECLIENT_CB cb)
{
	AoneSDK::getInstance()->AoneChangePasswordAsync(oldpass, newpass, cb);
}

void AoneClient::AoneCreateInheritance(const std::string& password, AONECLIENT_CB cb){
	AoneSDK::getInstance()->AoneCreateInheritance(password, cb);
}

int AoneClient::AoneBind(const std::string& account, const std::string& password)
{
	return AoneSDK::getInstance()->AoneBind(account, password);
}

void AoneClient::AoneBind(const std::string& account, const std::string& password, AONECLIENT_CB cb)
{
	AoneSDK::getInstance()->AoneBindAsync(account, password, cb);
}

void AoneClient::callFunc(const char* funcName)
{
	AoneSDK::getInstance()->callFunc(funcName);
}

void AoneClient::callFuncWithStringParam( const char* funcName, const char* param )
{
	AoneSDK::getInstance()->callFuncWithStringParam(funcName, param);
}

void AoneClient::callFuncWithParam( const char* funcName, void* param )
{
    AoneSDK::getInstance()->callFuncWithParam(funcName, param);
}

int AoneClient::callIntFunc(const char* funcName, void* param)
{
	return AoneSDK::getInstance()->callIntFunc(funcName, param);
}

std::string AoneClient::callStringFunc(const char* funcName)
{
	return AoneSDK::getInstance()->callStringFunc(funcName);
}

std::string AoneClient::callStringFuncFromSDK(const char *funcName, const char *sdkName)
{
    return AoneSDK::getInstance() -> callStringFunc(funcName, sdkName);
}
    
std::string AoneClient::callStringFuncWithParam(const char* funcName, void* param)
{
	return AoneSDK::getInstance()->callStringFuncWithParam(funcName, param);
}

void AoneClient::LogPageStart( const char* pageName )
{
	AoneSDK::getInstance()->LogPageStart(pageName);
}

void AoneClient::LogPageEnd( const char* pageName )
{
	AoneSDK::getInstance()->LogPageEnd(pageName);
}

void AoneClient::LogEvent( const char* eventId )
{
	AoneSDK::getInstance()->LogEvent(eventId);
}

void AoneClient::LogEvent( const char* eventId, map<string, string> info )
{
	AoneSDK::getInstance()->LogEvent(eventId, info);
}

//void AoneClient::LogEventBegin( const char* eventId )
//{
//	AoneSDK::getInstance()->LogEventBegin(eventId);
//}
//
//void AoneClient::LogEventEnd( const char* eventId )
//{
//	AoneSDK::getInstance()->LogEventEnd(eventId);
//}
//
//void AoneClient::LogKVEventBegin( const char* eventId, map<string, string> info )
//{
//	AoneSDK::getInstance()->LogKVEventBegin(eventId, info);
//}
//
//void AoneClient::LogKVEventEnd( const char* eventId )
//{
//	AoneSDK::getInstance()->LogKVEventEnd(eventId);
//}
//
//void AoneClient::LogError( const char* errorId, const char* message )
//{
//	AoneSDK::getInstance()->LogError(errorId, message);
//}
//
//
//std::string AoneClient::GetOnlineParams( const char* key )
//{
//	return AoneSDK::getInstance()->GetOnlineParams(key);
//}

void AoneClient::SetAnalyticsSdk( const char* sdkName )
{
	AoneSDK::getInstance()->setAnalyticsSdk(sdkName);
}

void AoneClient::Share(std::map<std::string, std::string> info, AONECLIENT_CB cb)
{
	AoneSDK::getInstance()->share(info, cb);
}

void AoneClient::SetShareSdk( const char* sdkName )
{
	AoneSDK::getInstance()->setShareSdk(sdkName);
}

void AoneClient::SetOAuthType( const std::string oauth_type )
{
	AoneSDK::getInstance()->setOAuthType(oauth_type);
}

std::string AoneClient::GetOAuthType()
{
	return AoneSDK::getInstance()->getOAuthType();
}

void AoneClient::SetPayChannel( const std::string payChannel )
{
	AoneSDK::getInstance()->setPayChannel(payChannel);
}

std::string AoneClient::GetPayChannel()
{
	return AoneSDK::getInstance()->getPayChannel();
}

void AoneClient::AddFileSearchPath(std::string path)
{
	AoneSDK::getInstance()->addFileSearchPath(path);
}

void AoneClient::SetAdsSdk( const char* sdkName )
{
	AoneSDK::getInstance()->setAdsSdk(sdkName);
}

void AoneClient::ShowAds( std::map<std::string, std::string> info, int pos, AONECLIENT_CB cb )
{
	AoneSDK::getInstance()->showAds(info, pos, cb);
}

void AoneClient::HideAds( std::map<std::string, std::string> info, AONECLIENT_CB cb )
{
	AoneSDK::getInstance()->hideAds(info, cb);
}

void AoneClient::QueryPoints( AONECLIENT_CB cb )
{
	AoneSDK::getInstance()->queryPoints(cb);
}

void AoneClient::SpendPoints( int points, AONECLIENT_CB cb )
{
	AoneSDK::getInstance()->spendPoints(points, cb);
}

void AoneClient::StartPush()
{
	AoneSDK::getInstance()->startPush(NULL);
}

void AoneClient::StartPush( AONECLIENT_CB cb )
{
	AoneSDK::getInstance()->startPush(cb);
}

void AoneClient::ClosePush()
{
	AoneSDK::getInstance()->closePush(NULL);
}

void AoneClient::ClosePush( AONECLIENT_CB cb )
{
	AoneSDK::getInstance()->closePush(cb);
}

void AoneClient::SetAlias( const char* alias)
{
	AoneSDK::getInstance()->setAlias(alias, NULL);
}

void AoneClient::SetAlias( const char* alias, AONECLIENT_CB cb )
{
	AoneSDK::getInstance()->setAlias(alias, cb);
}

void AoneClient::DelAlias( const char* alias)
{
	AoneSDK::getInstance()->delAlias(alias, NULL);
}

void AoneClient::DelAlias( const char* alias, AONECLIENT_CB cb )
{
	AoneSDK::getInstance()->delAlias(alias, cb);
}

void AoneClient::SetTags( const char* tags)
{
	AoneSDK::getInstance()->setTags(tags, NULL);
}

void AoneClient::SetTags( const char* tags, AONECLIENT_CB cb )
{
	AoneSDK::getInstance()->setTags(tags, cb);
}

void AoneClient::DelTags( const char* tags)
{
	AoneSDK::getInstance()->delTags(tags, NULL);
}

void AoneClient::DelTags( const char* tags, AONECLIENT_CB cb )
{
	AoneSDK::getInstance()->delTags(tags, cb);
}

void AoneClient::loadAllSdks()
{
	AoneSDK::getInstance()->loadAllSdks();
}

void AoneClient::AddNotify( std::map<std::string, std::string> info )
{
	AoneSDK::getInstance()->addNotify(info);
}

void AoneClient::ClearNotify( int notifyId )
{
	AoneSDK::getInstance()->clearNotify(notifyId);
}

void AoneClient::ClearAllNotify()
{
	AoneSDK::getInstance()->clearAllNotify();
}

void AoneClient::setPushSdk( const char* sdkName )
{
	AoneSDK::getInstance()->setPushSdk(sdkName);
}

void AoneClient::OAuthBind( AONECLIENT_CB cb )
{
	AoneSDK::getInstance()->OAuthBindAsync(cb);
}

void AoneClient::CheckIsEnableLocalPay( AONECLIENT_CB cb )
{
	AoneSDK::getInstance()->checkIsEnableLocalPay(cb);
}
void AoneClient::DeleteAccount(int type, AONECLIENT_CB cb){
	AoneSDK::getInstance()->deleteAccountAsync(type, cb);
}

void AoneClient::OAuthLogin( AONECLIENT_CB cb )
{
	AoneSDK::getInstance()->OAuthLoginAsync(cb);
}

void AoneClient::AoneBindOtherAccount(string newAccount,AONECLIENT_CB cb){
	AoneSDK::getInstance()->aoneBindOtherAccount(newAccount,cb);
}

void AoneClient::AoneUnbindOtherAccount(AONECLIENT_CB cb){
	AoneSDK::getInstance()->aoneUnbindOtherAccount(cb);
}

int AoneClient::AoneQueryBindOtherAccount(){
	return AoneSDK::getInstance()->aoneQueryBindOtherAccount();
}

void AoneClient::AoneQueryBindOtherAccount(AONECLIENT_CB cb){
	AoneSDK::getInstance()->aoneQueryBindOtherAccount(cb);
}

AoneDeviceInfo AoneClient::getDeviceInfo()
{
	DeviceInfo info = AoneSDK::getInstance()->getDeviceInfo();
	AoneDeviceInfo deviceInfo;
	deviceInfo.DeviceId = info.m_device_id;
	deviceInfo.Locale = info.m_locale;
	deviceInfo.Language = info.m_language;
	deviceInfo.Model = info.m_model;
	deviceInfo.Os = info.m_os;
	deviceInfo.Imei = info.m_imei;
	deviceInfo.Ip = info.m_ip;
	deviceInfo.Mac = info.m_mac;
	deviceInfo.Idfa = info.m_idfa;
	deviceInfo.NetworkType = uuDevice::getNetworkType();

	return deviceInfo;
}

AoneAppInfo AoneClient::getAppVersionInfo()
{
	AppVersionInfo info = AoneSDK::getInstance()->getAppVersionInfo();
	AoneAppInfo appInfo;
	appInfo.AppId = info.m_app_id;
	appInfo.AppLang = info.m_app_lang;
	appInfo.Channel = info.m_channel;
	appInfo.MajorVersion = info.m_major_version;
	appInfo.MinorVersion = info.m_minor_version;
	appInfo.RevisionVersion = info.m_revision_version;

	return appInfo;
}

void AoneClient::SetDeviceId(const char* deviceId)
{
	AoneSDK::getInstance()->setDeviceId(deviceId);
}
    
bool AoneClient::isUse01gameSdk()
{
    return AoneSDK::getInstance()->isUse01gameAndNotBackEnd();
}
   
void AoneClient::GetAdTrackLink(std::string userData, AONECLIENT_CB cb)
{
    return AoneSDK::getInstance() -> GetAdTrackLink(userData, cb);
}
  
std::string AoneClient::GetAppLink()
{
    return AoneSDK::getInstance() -> GetAppLink();
}

void AoneClient::GetAdSource(std::string &userData, AONECLIENT_CB cb)
{
    return AoneSDK::getInstance() -> GetAdSource(userData, cb);
}


//std::string AoneClient::EmbedQR(std::string srcImgPath, std::string info, int x, int y, int w, int h)
//{
//    return AoneSDK::getInstance() -> EmbedQR(srcImgPath, info, x, y, w, h);
//}
    
std::string AoneClient::GetChannelParam(const char *paramName)
{
    return AoneSDK::getInstance()->getChannelParamFromKey(paramName);
}

void AoneClient::OpenUrl(std::string url){
	AoneSDK::getInstance()->openUrl(url.c_str());
}

void AoneClient::copyToClipboard(std::string text){
	AoneSDK::getInstance()->copyToClipboard(text.c_str());
}
void AoneClient::joinQGroup(){
	AoneSDK::getInstance()->joinQGroup();
}

//app内调起苹果评星、评论
void AoneClient::starReviewInApp(const char* appid){
    AoneSDK::getInstance() -> starReviewInApp(appid);
}

//应用内评星
void AoneClient::starInApp(){
    AoneSDK::getInstance() -> starInApp();
}
//应用内调起苹果商店评论
void AoneClient::reviewInApp(const char* appId){
    AoneSDK::getInstance() -> reviewInApp(appId);
}

int AoneClient::isInstallApp(const std::string&  packageName){
	return AoneSDK::getInstance() ->isInstallApp(packageName);
}

void AoneClient::recordDevicePoint(unsigned int sdkPoint){
	AoneSDK::getInstance()->recordDevicePoint(sdkPoint);
}

void AoneClient::recordRolePoint(unsigned int gamePoint){
	AoneSDK::getInstance()->recordRolePoint(gamePoint);
}

void AoneClient::realnameInfo(AONECLIENT_CB cb){
	AoneSDK::getInstance()->realnameInfoAsync(cb);
}

void AoneClient::realnameCertificate(const char* identity_number, const char* real_name, AONECLIENT_CB cb){
	AoneSDK::getInstance()->realnameCertificateAsync(identity_number,real_name,cb);
}

void AoneClient::requestPlayerExtraInfo(AONECLIENT_CB cb){
	AoneSDK::getInstance()->requestPlayerExtraInfo(cb);
}
unsigned char* AoneClient::loadFileToBytes(const char* fileName, unsigned long* read_size){
	return cpfile_read(fileName,read_size);
}


bool AoneClient::check_file_exists(const char* path){
	return cpfile_exist(path);
}
    
} // namespace aonesdk

