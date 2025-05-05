#ifndef _AONE_CLIENT_C_H_
#define _AONE_CLIENT_C_H_

#if defined(_MSC_VER) || defined(WIN32) || defined(_WIN32)
	#if defined(AONESDK_DLL)
		#if defined(IS_DLL)
			#define AONE_DLL     __declspec(dllexport)
		#else         /* use a DLL library */
			#define AONE_DLL     __declspec(dllimport)
		#endif
	#else
		#define AONE_DLL
	#endif
#else
	#define AONE_DLL
#endif

#ifdef __cplusplus  
extern "C" {
#endif

// 游戏版本状态
enum AONE_DLL GameVersionStatus
{
	VERSION_UNKNOWN = 0,  // 未知
	VERSION_NORAML = 1,   // 正常，可登陆
	VERSION_LIMITED = 2,  // 版本过低，功能受限
	VERSION_OVERDUE = 3,  // 版本过低，不能进行游戏
};

// 游戏分区状态
enum AONE_DLL GameGroupStatus
{
	GROUP_UNKNOWN = 0, // 未知
	GROUP_CLOSE = 1,   // 维护
	GROUP_NORMAL = 2,  // 普通
	GROUP_BUSY = 3,    // 繁忙
	GROUP_FULL = 4,    // 爆满
};

// 键值对结构
struct AONE_DLL C_KVPair
{
	char*  Key; // 键
	char*  Value; // 值
};
// 释放申请的C_KVPair列表
void AONE_DLL free_kvpairs(C_KVPair** args);

// 游戏版本信息
struct AONE_DLL C_GameVersion
{
	unsigned char       MajorVersion; // 最新版本主版本号
	unsigned char       MinorVersion;  // 最新版本子版本号
	unsigned char       RevisionVersion;  // 最新版本修订版本号
	GameVersionStatus   Status;  // 游戏版本状态
	char*               Notice;  // 公告
	char*               DownloadURL;  // (强制更新时)APP包的下载URL地址
	char*               UpdateURL;  // (资源更新时)APP资源更新列表URL地址
    char*               BigPatcherURL; // (资源更新时)APP大包资源更新地址
};
// 释放申请的C_GameVersion列表
void AONE_DLL free_gameversions(C_GameVersion** args);

// 游戏分区信息
struct AONE_DLL C_GameGroup
{
	unsigned int     ID;  // 分区ID
	char*            Name;  // 分区名字
	GameGroupStatus  Status;  // 游戏分区状态
	char*            ServerIP;  // 对外服务IP地址(维护和爆满状态时置空)
	unsigned short   ServerPort;  // 对外服务端口(维护和爆满状态时置0)
	unsigned char    IsRecommend;  // 是否为推荐区(1为true,0为false)
	unsigned char    IsInitialed;  // 是否为新区(1为true,0为false)
};
// 释放申请的C_GameGroup
void AONE_DLL free_gamegroup(C_GameGroup* arg);
// 释放申请的C_GameGroup列表
void AONE_DLL free_gamegroups(C_GameGroup** args);

struct AONE_DLL C_GameGroupCP
{
	char*            ID;  // 分区ID
	char*            Name;  // 分区名字
	GameGroupStatus  Status;  // 游戏分区状态
	char*           ServerIP;  // 对外服务IP地址(维护和爆满状态时置空)
	unsigned short   ServerPort;  // 对外服务端口(维护和爆满状态时置0)
	unsigned char    IsRecommend;  // 是否为推荐区(1为true,0为false)
	unsigned char    IsInitialed;  // 是否为新区(1为true,0为false)
};
// 释放申请的C_GameGroupCP
void AONE_DLL free_gamegroupcp(C_GameGroupCP* arg);
// 释放申请的C_GameGroupCP列表
void AONE_DLL free_gamegroupcps(C_GameGroupCP** args);

// 游戏角色信息
struct AONE_DLL C_GameRole
{
	unsigned int       ID;  // 角色ID
	char*              Name;  // 角色名
	unsigned int       Type;  // 角色类型
	unsigned int       Level;  // 角色等级
	unsigned int       VIP;  // 角色VIP等级
	char*              CreateTime;  // 角色创建时间
	char*              LastLoginTime;  // 角色最后登陆时间
	unsigned int       OwnerGroupID;  // 所属分区ID(不属于任何分区置0)
	unsigned int       LastGroupID;  // 最后登陆分区ID(创建完角色未登陆游戏服时为0)
	struct C_KVPair**  Features; // 角色扩展特征信息列表(遍历到值为NULL为止)
};
// 释放申请的C_GameRole
void AONE_DLL free_gamerole(C_GameRole* arg);
// 释放申请的C_GameRole列表
void AONE_DLL free_gameroles(C_GameRole** args);

// 游戏角色信息(CP信息)
struct AONE_DLL C_GameRoleCP
{
	char*                        ID;  // 角色ID
	char*                        Name;  // 角色名
	unsigned int                  Type;  // 角色类型
	unsigned int                  Level;  // 角色等级
	unsigned int                  VIP;  // 角色VIP等级
	char*                         CreateTime;  // 角色创建时间
	char*                         LastLoginTime;  // 角色最后登陆时间
	char*                         OwnerGroupID;  // 所属分区ID(不属于任何分区置0)
	char*                         LastGroupID;  // 最后登陆分区ID(创建完角色未登陆游戏服时为0)
	struct C_KVPair**              Features;  // 角色扩展特征信息列表
};
// 释放申请的C_GameRoleCP
void AONE_DLL free_gamerolecp(C_GameRoleCP* arg);
// 释放申请的C_GameRoleCP列表
void AONE_DLL free_gamerolecps(C_GameRoleCP** args);

// 渠道支付商品信息
struct AONE_DLL C_GameProduct
{
	char*         ID;  // 商口ID(如：com.aonesoft.sanguo.item1)
	char*         Name;  // 商品名称(如：小袋金币)
	char*         Describe;  // 商品描述信息
	char*         Currency;  // 币种(如人民币:CNY,港币:HKY,美元:USD)
	double        Amount;  // 币值(人民币单位为元,美元单位为美元)
	double        Amount_usd;  // 转换为美元后的币值
	unsigned int  Coin;  // 游戏代币数(不同游戏叫法不同,如金币,元宝等)
	char*		  Type; // 商品类型('' or 'normal': 空字符串表示定额普通商品; 特殊类型，例如：'mol_pinpay': MOL pinPay(MOL钱包按normal处理); 'fortumo': fortumo支付)
};
// 释放申请的C_GameProduct列表
void AONE_DLL free_gameproducts(C_GameProduct** args);

struct AONE_DLL C_AoneDeviceInfo
{
	char* DeviceId; // 设备ID(唯一标识该设备)
	char* Locale; // (手机上设置的)国家和地区
	char* Language; // (手机上设置的)语言
	char* Model; // 设备型号(HTC渴望v5、Iphone5)
	char* Os; // 操作系统(IOS5、Android4.2)
	char* Imei; // IMEI码(不能获取则置空)
	char* Ip; // IP地址(客户端可能获取的为内网地址,服务端需要对该字段更新)
	char* Mac; // MAC地址(不能获取则置空)
	char* Idfa;  // IOS填IDFA，安卓填安卓ID
	char* NetworkType; //获取网络类型 wifi/4g
};
void AONE_DLL free_aonedeviceinfo(C_AoneDeviceInfo** args);

struct AONE_DLL C_AoneAppInfo
{
	unsigned int AppId; // 应用ID(向平台申请得到)
	char*       AppLang;  // 应用语言
	char*       Channel;  // 联运(发行)渠道商(见联运渠道编码表)
	unsigned char MajorVersion;  // 客户端主版本号
	unsigned char MinorVersion;  // 客户端子版本号
	unsigned char RevisionVersion;  // 客户端修正版本号
};
void AONE_DLL free_aoneappinfo(C_AoneAppInfo** args);

/*
函数同步调用和回调返回码定义：
> 0: 服务端返回的错误码
= 0: 成功
< 0: 客户端本地错误码
本地错误码定义：
-1: 连接服务端错误
-2: 与服务端通信错误或超时
-3: 未正确初始化或正确调用
-4: 无效的参数
-5: 无效的角色选择
-6: 无效的分区选择
-7: 选择的角色和分区不匹配
-8: 没有匹配的SDK
-9: 无效的商品选择
-10: 无效的语言选择
-11: 无效的token或者channel
*/

// params以NULL结尾
typedef AONE_DLL void(*C_AONECLIENT_CB)(int retCode, const struct C_KVPair** params);


// [必选接口] 初始化SDK
// 在调用其他接口前调用，需要在平台配置渠道及参数
void AONE_DLL init_sdk(C_AONECLIENT_CB cb);

// [可选接口] 获取aonesdk.json中的数据
AONE_DLL const char* get_aoneconfig_data(const char* key);

// [可选接口] 获取游戏(使用真实货币支付购买的)商品列表
// 在调用init_sdk成功后可调用，需要在平台配置商品列表
AONE_DLL struct C_GameProduct** get_all_products();
// [可选接口] 获取游戏支持的语言种类列表
// 在调用init_sdk成功后可调用，需要在平台配置语言列表
AONE_DLL struct C_KVPair** get_all_langs(); // 语言代号索引，语言名称为值



// [可选接口] 设置SDK使用的语言代号
// 游戏需要支持多语言时使用，默认使用语言代号为zh-cn或en
// lang = 语系代号[-地区码]
// 语系代号参见ISO 639-1语系编码标准
// 地区码参见ISO 3166-1地区码标准
// 参考 http://blog.sina.com.cn/s/blog_985bedb40101na3x.html
// 匹配规则：主要按语系匹配，若语系后带有地区码的优先匹配。
// 返回：0表示成功; <0 本地错误
int AONE_DLL select_lang(const char* lang);



// [必选接口] 检测客户端版本
// 回调成功后调用get_game_version()获取版本信息, 用于指导升级版本
// 调用参数:
//     majorVersion: 主版本号
//     minorVersion: 子版本号
//     revisionVersion: 修订版本号
// 返回码：0表示成功; <0 本地错误; >0服务端错误
void AONE_DLL check_version(unsigned char majorVersion, unsigned char minorVersion, unsigned char revisionVersion, C_AONECLIENT_CB cb);

// [必选接口] 获取客户端版本信息
// 必须在调用check_version成功后调用才有效
AONE_DLL C_GameVersion* get_game_version();

//
void AONE_DLL set_game_version(int major_version, int minor_version ,int revision_version);

int AONE_DLL is_need_patch(const char* packageResDir, const char* patchResDir);
int AONE_DLL is_need_force_update(const char* packageResDir, const char* patchResDir);

//  判断是否用的是01GameSDK
int AONE_DLL is_use_01gamesdk();


// 设置是否使用用户登录
//useUserLogin 1:true  0:false
void AONE_DLL set_use_user_login(int useUserLogin);

// [必选接口] 登陆验证用户账号
// 登陆需要的参数定义在aonesdk.json
// 分为渠道登陆及第三方登陆两种方式。如果使用第三方登陆，在回调函数的params内，会有两个key："registerDevice"（设备ID是否已注册），"registerOauth"（第三方账号是否已注册），值为0时表示未注册，值为1时表示已注册。
void AONE_DLL login(C_AONECLIENT_CB cb);

// [必选接口] 获取用户ID
// 在调用login成功后可调用
unsigned int AONE_DLL user_id();
// [必选接口] 获取用户验证token
// 在调用login成功后可调用
unsigned int AONE_DLL user_token();
//[必选接口] 获取用户验证token
//[调用规则] 在调用Login成功后可调用
AONE_DLL const char* get_token();
//[必选接口] 获取user_account,创建角色或者上传角色内部协议信息使用
//[调用规则] 在调用Login成功后可调用
AONE_DLL const char* get_user_account();
// [必选接口] 获取游戏分区列表
// 在调用login成功后可调用
AONE_DLL struct C_GameGroup** get_all_groups(); // 分区ID索引
// [可选接口] 获取游戏角色列表
// 在调用login成功后可调用
AONE_DLL struct C_GameRole** get_all_roles(); // 角色ID索引


// [可选接口] 创建角色
// 公司内部游戏使用，合作方CP可选用，回调成功后调用get_selected_role()函数获取新角色信息
// 调用参数：
//   name: 创建的角色名
//   type: 创建的角色类型，不分类型置0
//   features: 创建的角色扩展属性字段,json格式kv结构
//   owner_groupid: 角色所属的分区ID；在不分区的游戏中该字段置0
// 返回码：0表示成功; <0 本地错误; >0服务端错误
void AONE_DLL create_role(const char* name, unsigned int type, const char* features, unsigned int owner_groupid, C_AONECLIENT_CB cb);


// [必选接口] 提交角色信息
// [调用规则] 同步调用，登录后调用
// [调用参数] roleID: 创建成功时返回的角色ID，不可为空
//            name: 创建的角色名
//            level: 角色等级
//            groupId: 角色所属的分区, 在不分区的游戏中该字段置0，不可为空
//            groupName；所在分区名
//            roleVip： 角色VIP等级
//            isLogin: 是否是登录时提交角色信息 0表示非登录状态 1表示登录状态
//            action:有四种状态 1."login"登录即进入游戏 2."createRole"创建角色 3."roleUp"角色升级 4."logout"退出游戏
void AONE_DLL submit_role_data(const char* roleID, const char* name, unsigned int level, const char* groupId, const char* groupName, int roleVip, int isLogin, const char* action,unsigned long long roleCTime);



// [必选接口] 选择角色和分区
// 角色登陆成功后，必须调用该接口，否则后续支付将失败
// 调用参数：
//     role_id: 登陆游戏服时选择的角色ID
//     group_id: 登陆游戏服务时选择的游戏分区ID
// 返回码：0表示成功; <0 本地错误; >0服务端错误
void AONE_DLL select_role_and_group(unsigned int role_id, unsigned int group_id, C_AONECLIENT_CB cb);

// [可选接口] 拉取服务器列表，可用于刷新服务器列表
void AONE_DLL pull_game_group(C_AONECLIENT_CB cb);

// [可选接口] 获取当前选中的角色
// 在调用create_role()或者select_role_and_group()成功后可调用
AONE_DLL C_GameRole* get_selected_role();

// [可选接口] 获取当前选中的分区
// 在调用select_role_and_group()成功后可调用
AONE_DLL C_GameGroup* get_selected_group();


// [可选接口] 获取设备信息
AONE_DLL C_AoneDeviceInfo* get_device_info();

// [必选接口] 支付购买
// 商品列表可以在平台配置后SDK拉取过来，也可以由游戏服提供，二者数据需保持一致
// 支付完成后，等待支付服回调游戏服给玩家代币
// 调用参数：
//     product_id: 要购买的虚拟商品ID
//     purchaseNum:购买某个商品的数量
//     cp_ext: 游戏开发商扩展字段(一般用来存开发商的订单号，不能超过255字节，可以为空)
//     cb: 支付结果回调函数
void AONE_DLL pay(const char* product_id, unsigned int purchaseNum, const char* cp_ext, C_AONECLIENT_CB cb);

// [可选接口] 注销当前账号 或者 切换账号
// 游戏内注销账号或者切换时必须使用
void AONE_DLL logout(C_AONECLIENT_CB cb);

// [必选接口] 退出游戏
void AONE_DLL exit_game();

// [必选接口] 判断退出游戏时当前sdk是否有自己的界面
// return 1:true, 0:false
int AONE_DLL has_exit_ui();


///////////////////////////////////////////////////////
// 以下为测试接口，请不要使用!!!

// 快速注册
// 同步调用
int AONE_DLL quick_reister();
//异步调用
void AONE_DLL quick_register_async(C_AONECLIENT_CB cb);

// [可选接口] 获取绑定的账号
// 在调用QuickLogin、QuickRegister成功后可调用
AONE_DLL const char* get_aone_account();
//[可选接口] 获取快速注册成功后返回的aone密码
// 在调用QuickRegister成功后可以调用
AONE_DLL const char* get_aone_password();

// 设备ID快速登陆
// 同步调用
int AONE_DLL quick_login();
// 异步调用，建议使用
void AONE_DLL quick_login_async(C_AONECLIENT_CB cb);

// Aone账号注册
// 同步调用
int AONE_DLL aone_register(const char* account, const char* password);
// 异步调用，建议使用
void AONE_DLL aone_register_async(const char* account, const char* password, C_AONECLIENT_CB cb);

// Aone账号登陆
// 同步调用
int AONE_DLL aone_login(const char* account, const char* password);
// 异步调用，建议使用
void AONE_DLL aone_login_async(const char* account, const char* password, C_AONECLIENT_CB cb);

// Aone账号修改密码
// 同步调用
int AONE_DLL aone_change_password(const char* oldpass, const char* newpass);
// 异步调用，建议使用
void AONE_DLL aone_change_password_async(const char* oldpass, const char* newpass, C_AONECLIENT_CB cb);
//继承码[引继码](日本版本)
void AONE_DLL aone_create_inheritance(const char* password, C_AONECLIENT_CB cb);
// Aone账号注册同时绑定到设备
// 同步调用
int AONE_DLL aone_bind(const char* account, const char* password);
// 异步调用，建议使用
void AONE_DLL aone_bind_async(const char* account, const char* password, C_AONECLIENT_CB cb);

// 第三方账号绑定
void AONE_DLL oauth_bind_async(C_AONECLIENT_CB cb);
// 第三方账号登录，注意，调用过Login()，成功获取第三方token之后，才能调用该接口
void AONE_DLL oauth_login_async(C_AONECLIENT_CB cb);

// aone账号绑定到其他账号上
void AONE_DLL aone_bind_otheraccount_async(const char* newaccount,C_AONECLIENT_CB cb);

// aone账号从其他账号上解绑
void AONE_DLL aone_unbind_otheraccount_async(C_AONECLIENT_CB cb);
// 查询aone账号绑定的其他账号
void AONE_DLL aone_querybind_otheraccount_async(C_AONECLIENT_CB cb);



// [可选接口] sdk相关接口
void AONE_DLL callFunc(const char* funcName);
void AONE_DLL callFuncWithStringParam(const char* funcName, const char* param);
void AONE_DLL callFuncWithParam(const char* funcName, void* param);
int AONE_DLL callIntFunc(const char* funcName, void* param);
AONE_DLL const char* callStringFunc(const char* funcName);
AONE_DLL const char* callStringFuncFromSDK(const char* funcName, const char* sdkName);
AONE_DLL const char* callStringFuncWithParam(const char* funcName, void* param);

// [可选接口] 设置第三方登陆sdk，eg:setOAuthType("facebook");
// 如果只有一个第三方登陆类sdk，可以不显示调用此接口，而在uuSdkConfig.json内设置："defaultUserSdk":"user sdk name" (eg. UserFacebook)
void AONE_DLL set_oAuth_type(const char* oauth_type);
AONE_DLL const char* get_oAuth_type();

// [可选接口] 设置支付类sdk，eg:setPayChannel("appstore");
// 如果只有一个支付类sdk，可以不显示调用此接口，而在uuSdkConfig.json内设置："defaultIapSdk":"iap sdk name" (eg. appstore)
void AONE_DLL set_pay_channel(const char* payChannel);
AONE_DLL const char* get_pay_channel();


// [可选接口] 分享类sdk相关接口
// 支持参数：imagePath, text, warning！可能添加完善其他参数！！！
void AONE_DLL share(const char* info, C_AONECLIENT_CB cb);
// [可选接口] 设置分享类sdk名称. eg: SetShareSdk("ShareWeibo")
// 如果只有一个分享类sdk，可以不显示调用此接口，而在uuSdkConfig.json内设置："defaultShareSdk":"share sdk name"(eg. ShareWeibo)
void AONE_DLL set_share_sdk(const char* sdkName);


// [可选接口] 统计类sdk相关接口
// 统计某一事件，举例：eventId = "purchase"
void AONE_DLL log_event(const char* eventId);
// 统计带参数的某一事件，举例：eventId = "purchase"，info["金额"] = price, info["类型"] = type
// 注意：还可统计数值分布，需要添加一个特殊的key：__ct__ 举例：统计播放音乐的时长：info["__ct__"] = play_music_duration
void AONE_DLL log_event_with_params(const char* eventId, const char* info);
// 注意！以下3个接口已不再维护，新项目内请不要使用！
// 统计在某一个界面显示时长（慎用，目前只支持umeng，可使用logEvent变通实现此功能）
void AONE_DLL log_page_start(const char* pageName);
void AONE_DLL log_page_end(const char* pageName);
// [可选接口] 设置统计类sdk名称，如果只有一个统计类sdk，可以不显示调用此接口，而在uuSdkConfig.json内设置："defaultAnalyticsSdk":"analytics sdk name"(eg. AnalyticsUmeng)
void AONE_DLL set_analytics_sdk(const char* sdkName);


// [可选接口] 广告类sdk相关接口
void AONE_DLL show_ads( const char* info, int pos, C_AONECLIENT_CB cb );
void AONE_DLL hide_ads( const char* info, C_AONECLIENT_CB cb );
void AONE_DLL query_points(C_AONECLIENT_CB cb);
void AONE_DLL spend_points(int points, C_AONECLIENT_CB cb);
// [可选接口] 设置广告类sdk名称，如果只有一个广告类sdk，可以不显示调用此接口，而在uuSdkConfig.json内设置："defaultAdsSdk":"ads sdk name"(eg. AdsAdmob)
void AONE_DLL set_ads_sdk(const char* sdkName);



// [可选接口] 推送类sdk相关接口
// 注意：目前发现有些推送sdk的联网速度很慢，会很久才会调用回调函数。所以建议将传入的回调函数设置成NULL。
// 开启推送，默认情况下推送接口都是开启的，所以大多数情况下不需要显示调用此接口
void AONE_DLL start_push(C_AONECLIENT_CB cb);
// 关闭推送，关闭后会不会再收到推送通知（如果不想接收点对点推送或者群组推送通知，建议使用下面的DelAlias和DelTags接口）
void AONE_DLL close_push(C_AONECLIENT_CB cb);
// 设置别名，用于点对点推送。别名可设置为能唯一标识的id。建议使用roleId，可以在每次登陆后调用一次：SetAlias(roleId)。
void AONE_DLL set_alias(const char* alias, C_AONECLIENT_CB cb);
// 删除别名，删除之后，不再接收给原别名推送的消息（注意：腾讯信鸽并不支持取消别名）
void AONE_DLL del_alias(const char* alias, C_AONECLIENT_CB cb);
// 设置标签，用于给一组设备推送。举例：标签设置成公会id，可给在该公会的所有设备推送。
// 如果想给设置一组标签，请使用分号";"，连接。 
// 注意：极光推送是覆盖设置（每次设置标签需要设置设备关联的所有标签）。而腾讯信鸽是追加设置（建议每次仅设置一个标签）
void AONE_DLL set_tags(const char* tags, C_AONECLIENT_CB cb);
// 删除标签，删除之后，向原标签推送时不会再推送到此设备
// 注意：极光推送会删除所有标签（如果想删除特定标签，请使用setTags从新设置标签组）。腾讯信鸽会删除指定的标签（）
void AONE_DLL del_tags(const char* tags, C_AONECLIENT_CB cb);
// 设置本地推送内容
// 需设置参数：info["content"] info["title"] info["tickText"](小tick提示，点开后或者下拉后会显示title和content内容) info["interval"](单位：秒）
// 可选参数：info["notifyId"] clearNotification()需用到
void AONE_DLL add_notify(const char* info);
// 取消特定id的本地推送
void AONE_DLL clear_notify(int notifyId);
// 取消所有本地推送
void AONE_DLL clear_all_notify();
// [可选接口] 设置推送类sdk名称，如果只有一个推送类sdk，可以不显示调用此接口，而在uuSdkConfig.json内设置："defaultPushSdk":"ads sdk name"(eg. PushJpush)
void AONE_DLL set_push_sdk(const char* sdkName);

// [可选接口] 加载所有sdk plugin，在iOS平台需手动调用
void AONE_DLL load_all_sdks();

// [测试接口] 设置自定义deviceId
void AONE_DLL set_device_id(const char* deviceId);

//[可选接口] 复制到剪切板
void AONE_DLL copy_to_clipboard(const char* text);
//[可选接口] 一键加群
void AONE_DLL joinQGroup();
//[可选接口] app内调起苹果评星、评论
void AONE_DLL starReviewInApp(const char* appid);
//[可选接口] 应用内评星
void AONE_DLL starInApp();
//[可选接口] 应用内调起苹果商店评论
void AONE_DLL reviewInApp(const char* appId);

//[可选接口] 根据包名区分是否安装某个App(android:packageName,ios:bundle id)
//传"",android:googleplay,ios:app store
int AONE_DLL isInstallApp(const char* packageName = "");

// [可选接口] 设置配置所在路径（用于unity项目）
void AONE_DLL add_file_search_path(const char* path);
void AONE_DLL set_config_data(const char* data);

// [可选接口] 判定是否显示当地支付支付商页面请求, 用于海外发行避开谷歌审查
// 当retCode=0时，params["enable"] = "0" 或者 params["enable"] = "1";
void AONE_DLL check_isEnable_localPay(C_AONECLIENT_CB cb);

// [可选接口] 设备埋点
void AONE_DLL recordDevicePoint(unsigned int sdkPoint);

// [可选接口] 角色埋点
void AONE_DLL recordRolePoint(unsigned int gamePoint);

/**
账号实名认证信息查询请求
*/
void AONE_DLL realname_info(C_AONECLIENT_CB cb);

/**
账号实名认证请求
*/
void AONE_DLL realname_certificate(const char* identity_number, const char* real_name, C_AONECLIENT_CB cb);

void AONE_DLL request_player_extra_info(C_AONECLIENT_CB cb);

AONE_DLL unsigned char* load_file_to_bytes(const char* fileName, unsigned long* read_size);

AONE_DLL bool check_file_exists(const char* path);

#ifdef __cplusplus  
}//extern "C"
#endif 

#endif//_AONE_CLIENT_C_H_
