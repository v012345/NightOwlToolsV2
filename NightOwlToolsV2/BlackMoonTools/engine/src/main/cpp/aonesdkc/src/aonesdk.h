#ifndef __AONESDK_HH__
#define __AONESDK_HH__

#include "xcore_define.h"
#include "uuSdkResultListener.h"
#include "uuSdkWrapper.h"
#include "msg_xx_global.h"
#include "msg_xx_apiserver.h"
#include "msg_xx_apiserver_ex.h"
#include "msg_xx_entryserver.h"
#include "msg_xx_idcard.h"
#include "AoneClient.h"

#include <chrono>
#include <fstream>
#include<iostream>

#include "zf_log.h"


namespace aonesdk {

typedef void(*AONESDK_CB)(int result, map<string, string> dataMap);
typedef void (uuObject::*NET_CB)(int result, byte *responseBytes, int len);
typedef void(uuObject::*PAYCOMMIT_CB)(int result, map<string, string> dataMap);
typedef void (uuObject::*HTTP_CB)(int result, map<string, string> dataMap);

class AoneSDK;
class PayStorage {
public:
	struct PayRecord {
		uint32  user_id;  // 用户ID
		uint32  role_id;  // 角色ID
		uint32  purchase_num; //购买商品的数量
        
        /// An array will record the following field:
        /// 0 -> last_pay_no
        /// 1 -> product_id
        /// 2 -> payChannel
        /// 3 -> cpExt
        /// 4 -> default is an empty string, if finished purchase, store the receipt.
		string  record[5];
        
        /// An Array will record the following field:
        /// 0 -> cp_role_id
        /// 1 -> cp_role_group_id
        string cpID[2];
        
		size_t getLength() {
			int len = 5 * 2 + 2 * 2 + 3 * 4;  // 5个数据长度 + 2个数据长度 + 3个int值
			for (int i = 0; i < 5; ++i) {
				len += record[i].size();
			}
            for(int j = 0; j < 2; ++j){
                len += cpID[j].size();
            }
			return len;
		}
		int getNumber() { return 5; }
        int getCpNum() {return 2;}
	};
	PayStorage() {
        m_store_payno = "";
	}
	boolean save(PayRecord data, boolean toSave = true);
	boolean load();
	vector<PayRecord>& getRecords() { return m_vecData; }
	void checkRecords(AoneSDK *sdk);
	boolean saveToFile();

public:
	vector<PayRecord> m_vecData;
    
public:
    string m_store_payno; // 正在提交的订单
    vector<string> m_store_payno_fail; // 运行过程中提交失败的订单

};

class AoneSDK : public uuObject
{
public:
	struct AoneUserInfo{
		string account;
		string password;
		string type;
		string userId;
		string thirdAccount;
		string thirdUserId;
		string oauthType;
	};
	AoneSDK();
	static AoneSDK* getInstance();

	//void init(AONESDK_CB cb);
	void initAsync(AONESDK_CB cb);
	void initAsyncCallback(int ret, byte *responseBytes, int len);
	/**
	 * 删除角色
	 * @param type 0: 拉取注销时间; 1: 取消注销; 2: 30天后注销; 3: 立刻注销
	 * */
	void deleteAccountAsync(int type, AONESDK_CB cb);
	void deleteAccountAsyncCallback(int ret, byte *responseBytes, int len);

	void login(AONESDK_CB cb);
	void login(const string& sdkName, AONESDK_CB cb);
	/*
	*	third party login
	*/
	void thirdPartyLogin(AONESDK_CB cb);
	void thirdPartyLogin(const string& sdkName, AONESDK_CB cb);

	/*
	*	third party submitRoleData
	*/
	void thirdPartySubmitRoleData(const std::string& roleID, const std::string& name, int level, const std::string& groupID, const std::string& groupName, int roleVip, bool isLogin, std::string action, unsigned long long roleCTime);

	void logout(AONESDK_CB cb);
	void logout(const string& sdkName, AONESDK_CB cb);

	void exit();
	bool hasExitUI();

	void share(map<string, string> params, AONESDK_CB cb);
	void share(const string& sdkName, map<string, string> params, AONESDK_CB cb);
	void setShareSdk(const char* sdkName);
    
    /**
     系统分享接口
     调用系统级别分享接口。
     可由`share`接口来调用该接口，params参数配置如下key-value:
     "useSystemShare": "true"
     */
    void systemShare(map<string, string> params, AONESDK_CB cb);
    

	void pay(const string& product_id, unsigned int purchaseNum, const string& cp_ext, AONESDK_CB cb);
	void pay(const string& sdkName, const string& product_id, unsigned int purchaseNum, const string& cp_ext, const bool thirdPartyCallback, AONESDK_CB cb);

	void shareCallback(uuESdkResultType type, map<string, string> params);
	void loginCallback(uuESdkResultType type, map<string, string> params);
	/*
	*	third party login callBack
	*/
	void thirdPartyLoginCallback(uuESdkResultType type, map<string, string> params);


	void logoutCallback(uuESdkResultType type, map<string, string> params);
	void payCallback(uuESdkResultType type, map<string, string> params);
	/*
	*	third party pay callBack
	*/
	void thirdPartyWorkCallback(uuESdkResultType type, map<string, string> params);

	void initCallback(uuESdkResultType type, map<string, string> params);
    void initPayCallback(uuESdkResultType type, map<string, string> params);
	bool isProductOKForCoin(string productId, int coin);
	void adsCallback(uuESdkResultType type, map<string, string> params);
	void pushCallback(uuESdkResultType type, map<string, string> params);

	/*
	*	third party submitRoleData callBack
	*/
	void thirdPartySubmitRoleDataCallback(int ret, byte *responseBytes, int len);

	// 获取客户端支持的所有语言
	// key为语言代号，value为语言名称
	map<string, string> getAllAppLangs();

	// 获取客户端语言
	string getAppLang();

	// 设置客户端语言
	// 必须是所支持的语言中的一种
	bool setAppLang(const string& lang);

	// 检测客户端版本
	// 返回0成功时调用getGameVersion()接口获取返回的游戏版本信息
	int checkVersion(uint8 majorVersion, uint8 minorVersion, uint8 revisionVersion);
	void checkVersionAsync(uint8 majorVersion, uint8 minorVersion, uint8 revisionVersion, AONESDK_CB cb);
	void checkVersionCallback(int ret, byte *responseBytes, int len);

    void setAppVersionInfo(int major_version, int minor_version ,int revision_version );
    
	bool isNeedPatch(string packageResDir, string patchResDir);
	bool isNeedForceUpdate(string packageResDir, string patchResDir);

    
	// 快速注册
	int quickRegister();
	void quickRegisterAsync(AONESDK_CB cb);
	void quickRegisterCallback(int ret, byte *responseBytes, int len);
	//保存快速注册成功后 用户名和密码图片
	void saveQuickRegisterPic();
    

	// 快速登陆
	int quickLogin();
	void quickLoginAsync(AONESDK_CB cb);
	void quickLoginCallback(int ret, byte *responseBytes, int len);

	// aone账号注册
	int aoneRegister(const string& account, const string& password);
	void aoneRegisterAsync(const string& account, const string& password, AONESDK_CB cb);
	void aoneRegisterCallback(int ret, byte *responseBytes, int len);

	// aone账号登陆
	int aoneLogin(const string& account, const string& password);
	void aoneLoginAsync(const string& account, const string& password, AONESDK_CB cb);
	void aoneLoginCallback(int ret, byte *responseBytes, int len);

	// 第三方sdk登陆
	//int sdkLogin(const string& userId, const string& token);
	void sdkLoginAsync(const string& userId, const string& token, AONESDK_CB cb);
	void sdkLoginCallback(int ret, byte *responseBytes, int len);
	/*
	*	third party sdk login 
	*/
	void thirdPartySdkLoginAsync(const string& userId, const string& token, AONESDK_CB cb);
	void thirdPartySdkLoginCallback(int ret, byte *responseBytes, int len);
	/*
	*	third party sdk login 
	*/

	//int OAuthLogin(const string& OAuthType, const string& OAuthToken);
	void OAuthLoginAsync(AONESDK_CB cb);
	void OAuthLoginCallback(int ret, byte *responseBytes, int len);

	void OAuthInfoAsync(AONESDK_CB cb);
	void OAuthInfoCallback(int ret, byte *responseBytes, int len);

	// 创建角色
	// 返回0成功时调用getSelectedRole()接口获取返回的新角色信息
	int createRole(const std::string& name, unsigned int type, const std::map<std::string, std::string>& features, unsigned int owner_groupid);
	
	/*
	 * CP创建角色，透传角色信息参数
	 */
	int createRoleCP(const std::string& roleID, const std::string& name, unsigned int level, const std::map<std::string, std::string>& features, const std::string& owner_groupid, std::string groupName, int roleVIP);

	void createRoleAsync(const std::string& name, unsigned int type, const std::map<std::string, std::string>& features, unsigned int owner_groupid, AONESDK_CB cb);
	void createRoleCallback(int ret, byte *responseBytes, int len);


	// 选择角色和分区
	int selectRoleAndGroup(unsigned int role_id, unsigned int group_id);
	void selectRoleAndGroupAsync(unsigned int role_id, unsigned int group_id, AONESDK_CB cb);
	void selectRoleAndGroupCallback(int ret, byte *responseBytes, int len);
	void pullGameGroupAsync(AONESDK_CB cb);
	void pullGameGroupCallback(int ret, byte *responseBytes, int len);

	// 支付提交请求
	//int payCommit(const string& pay_no, const string& product_id, const string& pay_channel, const string& cp_ext, const string& receipt);
	void payCommitAsync(const string& pay_no, const string& product_id,unsigned int purchaseNum, const string& pay_channel, const string& cp_ext, const string& receipt, PAYCOMMIT_CB handler);
	void payCommitAsync(PayStorage::PayRecord &record, PAYCOMMIT_CB handler);
	void payCommitCallback(int ret, byte *responseBytes, int len);
	void payCommitResult(int result, map<string, string> params);
	void payReCommitResult(int result, map<string, string> params);

	// 限制代充值 支付提交请求
    void receiptCommitAsync(PayStorage::PayRecord &record, PAYCOMMIT_CB handler);
	void receiptCommitAsync(const string& pay_no, const string& receipt, const string& pay_channel, PAYCOMMIT_CB handler);
	void receiptCommitCallback(int ret, byte *responseBytes, int len);

	// 支付订单请求
	void payRequestAsync(const string& pay_no, const string& product_id,unsigned int purchaseNum, const string& pay_channel, const string& cp_ext, AONESDK_CB cb);
	void payRequestCallback(int ret, byte *responseBytes, int len);
    
    // 查询订单状态
    void queryOrderStatus(AONESDK_CB cb);
    void queryOrderStatusCallback(int ret, byte *responseBytes, int len);
    
    //查询角色是否达到切支付条件
    void queryPayMode(AONESDK_CB cb);
    void queryPayModeCallback(int ret, byte *responseBytes, int len);

    /**
     查询具体订单号的支付状态

     @param appId 应用ID
     @param payNO 需要查询的订单号
     @param cb 回调
     */
    void queryPayNoStatus(uint32 appId, const string& payNO, AONESDK_CB cb);
    void queryPayNoStatusCallback(int ret, byte *responseBytes, int len);
    
	// 修改aone账号密码
	int AoneChangePassword(const string oldpass, const string newpass);
	void AoneChangePasswordAsync(const string oldpass, const string newpass, AONESDK_CB cb);
	void AoneChangePasswordCallback(int ret, byte *responseBytes, int len);
	//引继码(日本版本)
	void AoneCreateInheritance(const string password, AONESDK_CB cb);
	// aone绑定
	int AoneBind(const string account, const string password);
	void AoneBindAsync(const string account, const string password, AONESDK_CB cb);
	void AoneBindCallback(int ret, byte *responseBytes, int len);

	// 第三方账号绑定
	void OAuthBindAsync(AONESDK_CB cb);
	void OAuthBindCallback(int ret, byte *responseBytes, int len);

	// aone账号绑定到其他账号上
	void aoneBindOtherAccount(string newAccount,AONESDK_CB cb);
	void aoneBindOtherAccountCallback(int ret, byte *responseBytes, int len);

	// aone账号从其他账号上解绑
	void aoneUnbindOtherAccount(AONESDK_CB cb);
	void aoneUnbindOtherAccountCallback(int ret, byte *responseBytes, int len);
	// 查询aone账号绑定的其他账号
	void aoneQueryBindOtherAccount(AONESDK_CB cb);
	int aoneQueryBindOtherAccount();
	void aoneQueryBindOtherAccountCallback(int ret, byte *responseBytes, int len);

	void recordGameDownloadException(const string url, const string code, const string detail, AONESDK_CB cb);
	void recordGameDownloadExceptionCallback(int ret, byte *responseBytes, int len);

	// 判定是否显示当地支付支付商页面请求, 用于海外发行避开谷歌审查
	// 当retCode=0时，params["enable"] = "0" 或者 params["enable"] = "1";
	void checkIsEnableLocalPay(AONESDK_CB cb);
	void checkIsEnableLocalPayCallback(int ret, byte *responseBytes, int len);

    void setDefaultUserSdkName(const char* sdkName);
	bool getChannelIsTest();
	bool getPayChannelIsTest();
	bool getAuthTypeIsTest();

public:
	uint32 getAppID();
	string getAppKey();
	string getChannelID();
    
    map<string, string> getM_channel_params();
    string getSdkConfig();
    string getPluginConfig();
    void setUserId(int userId){m_user_id = userId;}//慎用,为了解绑第三方添加
	int getUserId() { return m_user_id; }
	string getUserAccount() { return m_user_account; }
	uint32 getUserToken() { return m_user_token; }
	string getToken(){ return m_token;}
    
	GameVersion getGameVersion();
	map<string, GameProduct> getAllProducts();
	vector<GameGroup> getAllGroups();
	vector<GameRole> getAllRoles() { return m_roles; }
	string getAoneAccount() { return m_aone_account; }
	string getAonePassword() { return m_aone_password; }
	GameRole getSelectedRole() { return m_select_role; }
	GameGroup getSelectedGroup();
	GameRoleCP getSelectedRoleCP() { return m_select_role_cp; }
	GameGroupCP getSelectedGroupCP() { return m_select_group_cp; }
	DeviceInfo& getDeviceInfo() { return m_device_info; }
	AppVersionInfo getAppVersionInfo() { return m_version_info; }
    
    string getLastPayNo() { return m_last_payno; };

public:
	void callFunc(const char* funcName);
	void callFuncWithStringParam(const char* funcName, const char* param);
    void callFuncWithParam(const char* funcName, void* param);
    void callFuncWithParam(const char* funcName, PluginParam* param);
	int callIntFunc(const char* funcName, void* param);
	string callStringFunc(const char* funcName);
    string callStringFunc(const char* funcName, const char* sdkName);
	string callStringFuncWithParam(const char* funcName, void* param);
	int loadAllSdks();

public:
	void checkVersionPlatform();
	void checkVersionPlatformCallback(int ret, byte *responseBytes, int len);
	void checkUpdatePlatform();
	void checkAnnouncePlatform();
	bool ShouldAnnounceAppear();
	void afterCheckVersion();

public:
	int send_recv_async(const string& ip, int port, const string& key, byte* request, uint32 requestLen, byte** response, uint32& responseLen);
	int http_send_recv(const string& ip, int port, const string& key, byte* request, uint32 requestLen, map<string, string>& params);

	void setOAuthType(const string oauth_type) { m_oauth_type = oauth_type;  uuLOG("hhh m_oauth_type is: %s", m_oauth_type.c_str()); }
	string getOAuthType() { return m_oauth_type; }
	void setPayChannel(const string payChannel) { m_pay_channel = payChannel; }
	string getPayChannel() { return m_pay_channel; }

	PAYCOMMIT_CB m_handler;

public:
	// 广告相关接口
	void showAds(map<string, string> info, int pos, AONESDK_CB cb);
	void hideAds(map<string, string> info, AONESDK_CB cb);
	void queryPoints(AONESDK_CB cb);
	void spendPoints(int points, AONESDK_CB cb);

	void setAdsSdk(const char* sdkName);


	// 统计相关接口
	void LogPageStart(const char* pageName);
	void LogPageEnd(const char* pageName);
	void LogEvent(const char* eventId, bool useInternal = false);
	void LogEvent(const char* eventId, map<string, string>  info, bool useInternal = false);
	void LogEventBegin(const char* eventId);
	void LogEventEnd(const char* eventId);
	void LogKVEventBegin(const char* eventId, map<string, string> info);
	void LogKVEventEnd(const char* eventId);
	void LogError(const char* errorId, const char* message);
	vector<std::string> GetOnlineParams(const char* key);
	// [可选接口] 设置统计类sdk名称，如果只有一个统计类sdk，可以不显示调用此接口，而在uuSdkConfig.json内设置："defaultAnalyticsSdk":"analytics sdk name"(eg. AnalyticsUmeng)
	void setAnalyticsSdk(const char* sdkName);


	// 推送相关接口
	void startPush(AONESDK_CB cb);
	void closePush(AONESDK_CB cb);
	void setAlias(const char* alias, AONESDK_CB cb);
	void delAlias(const char* alias, AONESDK_CB cb);
	void setTags(const char* tags, AONESDK_CB cb);
	void delTags(const char* tags, AONESDK_CB cb);
	void setPushSdk(const char* sdkName);
	// 设置本地推送内容
	// 需设置参数：info["content"] info["title"] info["tickText"] info["interval"](单位：秒）
	// 可选参数：info["notifyId"] clearNotification()需用到
	void addNotify(std::map<std::string, std::string> info);
	// 取消特定id的本地推送
	void clearNotify(int notifyId);
	// 取消所有本地推送
	void clearAllNotify();

    // 异常信息统计
    void initBuglySDK(const char*appID, bool isDebug);
    void logException(uint32 code, const char* msg);
    
public:
	void addFileSearchPath(const string path);
	void setConfigData(const char* data);
	void setUseUserLogin(bool useUserLogin);
	void setDeviceId(const string deviceId);
	bool isUse01game(uuESdkType inWhere = uuESdkType::eSDK_USER);
	bool isUse01gameAndNotBackEnd(uuESdkType inWhere = uuESdkType::eSDK_USER);
    string getChannelParamFromKey(string key);

public:
	void openUrl(const char* url);
	//复制到剪切板
	void copyToClipboard(const char* text);
	//一键加群
	void joinQGroup();
    //app内调起苹果评星、评论
    void starReviewInApp(const char* appid);
    //应用内评星
    void starInApp();
    //应用内调起苹果商店评论
    void reviewInApp(const char* appId);
	//根据包名区分是否安装某个App(android:packageName,ios:bundle id)
	//传"",android:googleplay,ios:app store
	int isInstallApp(const string& packageName="");

private:

	// 用户登陆
	int userLogin();
	void userLoginAsync(AONESDK_CB cb);
	void userLoginCallback(int ret, byte *responseBytes, int len);

	void httpRequest(string host, AONESDK_CB cb);
	void httpRequestCallback(int ret, map<string, string> params);

	void recordSDKPayException(int code, string& msg);
	void recordSDKPayExceptionCallback(int ret, byte *responseBytes, int len);

	void recordSDKDownloadException(map<string, string> params);
	void recordSDKDownloadExceptionCallback(int ret, byte *responseBytes, int len);
    
	void setAppVersionFromCallFunc(uint8 majorVersion, uint8 minorVersion, uint8 revisionVersion);

private:
	static void to_upper(char* pszSrc);
	static string make_authkeyA(const string& strData);

	// 生成订单号
	static string generate_payno(uint32 appid, uint32 groupid, uint32 roleid);

	// 生成订单号(芒果玩要求订单号不能包含特殊字符，只能为数字,字母)
	static string generate_payno_num(uint32 appid, uint32 groupid, uint32 roleid);

	// 生成订单号(联通要求订单号必须为24位)
	static string generate_payno_unicom(uint32 appid, uint32 groupid, uint32 roleid);

	// 生成订单号(CP)
	static string generate_payno_cp(uint32 appid, const string& groupid, const string& roleid);

	// 生成订单号(芒果玩要求订单号不能包含特殊字符，只能为数字,字母)
	static string generate_payno_num_cp(uint32 appid, const string& groupid, const string& roleid);

	// 生成订单号(联通要求订单号必须为24位)(CP)
	static string generate_payno_unicom_cp(uint32 appid, const string& groupid, const string& roleid);

	// sync method
	int apiserver_send_recv(Aone2BaseMessage* request, Aone2BaseMessage** response);
	int entryserver_send_recv(Aone2BaseMessage* request, Aone2BaseMessage** response);
	// async method
	void apiserver_send_recv_async(Aone2BaseMessage* request, NET_CB cb);
	void entryserver_send_recv_async(Aone2BaseMessage* request, NET_CB cb);
	
	// 从多语言json串中按默认语言解析出相应的字符串
	string parse_for_lang(const string& content);

	// 上传角色信息给第三方sdk,如有需要，请在UserSdk内实现此同名方法
	void submitRoleData();
	// some channel require submit user info, chang wan e.g.
	void submitUserInfo();

	

	void onAoneLoginEndAsync();
	int  onAoneLoginEnd();
	bool isRoleAndGroupValid();
	bool is01gameBackEnd();
	bool shouldLog(bool useInternal);

	void copyProduct(GameProduct& fromProduct, map<string, GameProduct>& toMap);
public:
	// 清空角色相关信息
	void cleanRoleData();
private:
	AONESDK_CB m_cb;
	AONESDK_CB m_LogIncb;
	AONESDK_CB m_exceptionCb;
    AONESDK_CB m_queryPayNoCb;
    AONESDK_CB m_queryPayModeCb; 
	uuESdkResultType m_loginResult; //用来表示正常登录还是切换账号登录
	string m_sdkName;
	map<string, string> m_params;
	map<string, string> m_productIdMap;
	string m_httpUrl;

	uuESdkResultType m_initResultType;
	map<string, string> m_initResultParams;

private:
	static string m_apiserver_host; // api服域名
	static string m_apiserver_ip; // 域名解析失败时的备用IP
	static uint16 m_apiserver_port; // api服监听端口
	static string m_apiserver_key; // api服加密key

	bool m_init_ok; // 初始化完成标记
	uint32 m_appid;  // 应用ID
	string m_appkey; // 应用密钥
	DeviceInfo m_device_info; // 设备信息
	AppVersionInfo m_version_info; // 版本信息
	string m_entryserver_ip; // 入口IP
	uint16 m_entryserver_port; // 入口端口
	string m_bbs_url; // 游戏论坛(以及用户中心)URL地址
	map<string, string> m_channel_params; // 渠道参数
	map<string, string> m_app_langs; // 应用支持的语言列表
	map<string, GameProduct> m_channel_products; // 渠道商品
	GameVersion m_game_version; // 游戏版本信息
	uint32 m_user_id; // 用户ID
	string m_thirdparty_user_id;//第三方账号登录成功后返回的uid，如：Twitter等，华为等(主要用于第三方的绑定操作)
	string m_thirdparty_token;//第三方账号登录成功后返回的session，如：Twitter等，华为等(主要用于第三方的绑定操作)
	string m_thirdparty_account;//第三方账号登录成功后返回的sessio或者uid，如：Twitter等，华为等(主要用于第三方的绑定操作)
	string m_channel_user_id; //user id in specific thirdparty channel system, huawei for example
	string m_user_account; // 账号
	string m_aone_account; // 设备绑定的账号
	string m_aone_password; // 快速注册绑定的aone密码
	string m_aone_account_third;//
	string m_aone_password_third;//
	
	
	//bool m_is_quickregister;//标记是否是快速注册登录(日本版本)，为true就用来替换本地保存的密码
	bool m_is_quickregister;//标记是否是使用引继码进行登录(报密码错误时:1.如果使用引继码进行登录，则报密码错误2.如果是使用本地账号进行登录，说明该设备生成过引继码，并且在B设备上登录过)
	bool m_is_jp_third_login;//标记是否是第三方登录(日本版本)，为true时在调用完quickRegister的时候执行aoneQueryBindOtherAccount->aoneBindOtherAccount(登录报密码错误的时候使用)
	bool m_is_jp_first_third_login;//当首次在手机上进行绑定过的第三方账号进行登录时,为了保存第三方账号到本地使用
	bool m_is_create_inheritancecode;//标记是否是生成引继码使用(生成引继码使用的是修改密码接口)
	//bool m_is_inheritance_login;//标记是否是使用引继码登录(目前使用到的场景是:密码错误的时候)
	string m_new_password;
	string m_token; // api服务返回的验证token
	uint32 m_user_token; // 入口返回的验证token, 使用userlogin之后返回的token
	vector<GameGroup> m_groups; // 分区列表
	vector<GameRole> m_roles; // 角色列表
	GameRole m_select_role; // 登陆游戏时选中的角色
	GameGroup m_select_group; // 登陆游戏时选中的分区

	uint32 m_actual_discount; // 支付打折，解决在游戏中不能及时拉取到折扣信息

	std::chrono::high_resolution_clock::time_point m_ineffective_time;
	std::chrono::high_resolution_clock::time_point m_begin_login_time;
	std::chrono::high_resolution_clock::time_point m_begin_pay_time;

	/**
	 *	CP分区信息与角色信息
	 */
	vector<GameGroupCP> m_groups_cp; // 分区列表
	vector<GameRoleCP> m_roles_cp; // 角色列表
	GameRoleCP m_select_role_cp; // 登陆游戏时选中的角色
	GameGroupCP m_select_group_cp; // 登陆游戏时选中的分区

	string m_select_product_id; // 选中的商品ID
	string m_cp_product_id; // CP商品id
	unsigned int m_purchase_num;//CP支付时传入的购买了几笔
	string m_cp_ext; // CP支付时传入的扩展字段
	string m_last_payno; // 最后生成的本平台订单号
	string m_recipt;

	string m_oauth_type; // 使用oauth登陆时的类型,空字符串表示使用sdk登陆方式
	string m_oauth_token; // 使用oauth登陆方式返回的token。
	string m_pay_channel; // 使用非渠道支付方式时的支付类型，空字符串表示使用渠道支付方式

	vector<string> m_analyticsSdks;
	string m_shareSdk;
	string m_adsSdk;
	string m_pushSdk;
	string m_device_id;

	PayStorage m_storage;

private:
	int m_select_group_id; // 登陆游戏时选中的分区
	string m_eventLoginType;
	bool m_commitAfterPay;
	string mLoginSdkName;
	bool mUseUserLogin;
	message::ChannelVersionInfo m_newVersionInfo;
	message::ChannelVersionInfo m_currentVersionInfo;
	string m_versionCheckServerTime;
	int m_pos;//ads pos
	map<string, string> m_ads_info; // 广告SDK infos(CP传入)
	vector<string> m_platform_adsSdks;//数据中心拉取的所有广告SDK
	int m_ad_num = 0;//标记从数据中心取下来的第几个广告SDK
	vector<string> m_config_adsSdks;//uusdkConfig.json中获取的所有广告SDK
	string m_default_adsSdk;
private:
    void getAdTrackLinkCallback(int ret, byte *responseBytes, int len);
    void getAdSourceCallback(int ret, byte *responseBytes, int len);
	void recordDevicePointCallback(int ret, byte *responseBytes, int len);
	void recordRolePointCallback(int ret, byte *responseBytes, int len);
public:
    // 分享系统
    
    /**
     获取广告跟踪链接（分享的URL）

     @param userData 绑定自定义数据字符串，传入与这个跟踪链接绑定的自定义参数字符串
     */
    void GetAdTrackLink(string userData, AONESDK_CB cb);
    
    // 获取应用内跳转URL
    string GetAppLink();
    
    /**
     获取当前设备来自的广告源

     @param userData GetAdTrackLink设置的用户参数
     @param cb 回调
     'token' 表示广告源标识
     'userData' 表示GetAdTrackLink设置的用户参数
     */
    void GetAdSource(string& userData , AONESDK_CB cb);
    
    /**
     嵌入链接二维码图片

     @param srcImgPath 待嵌入二维码的源图片路径
     @param info 用于生成二维码的信息
     @param x 二维码图片在源图片上的位置x
     @param y 二维码图片在源图片上的位置y
     @param w 二维码图片在源图片上的位置w
     @param h 二维码图片在源图片上的位置h
     @return 生成的嵌入二维码的图片的路径
     */
//    string EmbedQR(string srcImgPath, string info, int x, int y, int w, int h);

	/**
	设备埋点
	@param sdkPoint 埋点信息
	*/
	void recordDevicePoint(uint32 sdkPoint);
	/**
	角色埋点
	@param gamePoint 埋点信息
	*/
	void recordRolePoint(uint32 gamePoint);
	/**
		账号实名认证信息查询请求
	*/
	void realnameInfoAsync(AONESDK_CB cb);
	void realnameInfoCallback(int ret, byte *responseBytes, int len);

	/**
	账号实名认证请求
	*/
	void realnameCertificateAsync(const char* identity_number, const char* real_name, AONESDK_CB cb);
	void realnameCertificateCallback(int ret, byte *responseBytes, int len);
	
	/**
	为了防止合作渠道进行其他渠道的分发
	*/
	void setVerifyKey(const char* key);
	void setPackageName(const char* pkgName);//android packageName
	void setClassName(const char* activity);//android launcher activity
	void setAppName(const char* appName);//android app_name
	
	void IdcardLogin();
	void IdcardLoginCallback(int ret, byte *responseBytes, int len);
	void IdcardLogout();
	void IdcardLogoutCallback(int ret, byte *responseBytes, int len);
	
	void IdcardHeart();
	void IdcardHeartCallback(int ret, byte *responseBytes, int len);
	/**
	实名认证
	@param identity_number 有效证件号(身份证号等)
	@param real_name 真实姓名
	*/
	void IdcardCertificate(const char* identity_number, const char* real_name, AONESDK_CB cb);
	void IdcardCertificateCallback(int ret, byte *responseBytes, int len);
	
	void requestPlayerExtraInfo(AONESDK_CB cb);
	void requestPlayerExtraInfoCallback(uuESdkResultType type, map<string, string> params);

	private:
	
		bool dealAppMsg(string key,const char* name);
		bool isAppOK(const string& code);
		bool mChceckFirstRecord = false;
		string m_encrypt_number;
		string m_verify_key;
		string m_app_name;
		string m_package_name;
		string m_launcher_activity;
		int m_idcardlogin_token;
};

}//namespace aonesdk

#endif // __AONESDK_HH__
