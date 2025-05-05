#ifndef _AONE_CLIENT_H_
#define _AONE_CLIENT_H_

#include <string>
#include <map>
#include <vector>


#define AONESDK_VERSION "1.3.11"

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

namespace aonesdk {

// 游戏版本信息
struct AONE_DLL GameVersion
{
	// 游戏版本状态
	enum GameVersionStatus
	{
		UNKNOWN = 0,  // 未知
		NORAML = 1,   // 正常，可登陆
		LIMITED = 2,  // 版本过低，功能受限
		OVERDUE = 3,  // 版本过低，不能进行游戏
	};

	unsigned char       MajorVersion; // 最新版本主版本号
	unsigned char       MinorVersion;  // 最新版本子版本号
	unsigned char       RevisionVersion;  // 最新版本修订版本号
	GameVersionStatus   Status;  // 游戏版本状态
	std::string         Notice;  // 公告
	std::string         DownloadURL;  // (强制更新时)APP包的下载URL地址
	std::string         UpdateURL;  // (资源更新时)APP资源更新列表URL地址
    std::string         BigPatcherURL; //(资源大包更新时zip文件) APP资源更新地址
    
	GameVersion() : MajorVersion(0), MinorVersion(0), RevisionVersion(0), Status(GameVersionStatus::UNKNOWN) {}
};

// 游戏分区信息
struct AONE_DLL GameGroup
{
	// 游戏分区状态
	enum GameGroupStatus
	{
		UNKNOWN = 0, // 未知
		CLOSE = 1,   // 维护
		NORMAL = 2,  // 普通
		BUSY = 3,    // 繁忙
		FULL = 4,    // 爆满
	};

	unsigned int     ID;  // 分区ID
	std::string      Name;  // 分区名字
	GameGroupStatus  Status;  // 游戏分区状态
	std::string      ServerIP;  // 对外服务IP地址(维护和爆满状态时置空)
	unsigned short   ServerPort;  // 对外服务端口(维护和爆满状态时置0)
	bool             IsRecommend;  // 是否为推荐区
	bool             IsInitialed;  // 是否为新区

	GameGroup() : ID(0), Status(GameGroupStatus::UNKNOWN), ServerPort(0), IsRecommend(false), IsInitialed(false) {}
};

// 游戏分区信息(CP)
struct AONE_DLL GameGroupCP
{
	// 游戏分区状态
	enum GameGroupStatus
	{
		UNKNOWN = 0, // 未知
		CLOSE = 1,   // 维护
		NORMAL = 2,  // 普通
		BUSY = 3,    // 繁忙
		FULL = 4,    // 爆满
	};

	std::string      ID;  // 分区ID
	std::string      Name;  // 分区名字
	GameGroupStatus  Status;  // 游戏分区状态
	std::string      ServerIP;  // 对外服务IP地址(维护和爆满状态时置空)
	unsigned short   ServerPort;  // 对外服务端口(维护和爆满状态时置0)
	bool             IsRecommend;  // 是否为推荐区
	bool             IsInitialed;  // 是否为新区

	GameGroupCP() : Status(GameGroupStatus::UNKNOWN), ServerPort(0), IsRecommend(false), IsInitialed(false) {}
};

// 游戏角色信息
struct AONE_DLL GameRole
{
	unsigned int                        ID;  // 角色ID
	std::string                         Name;  // 角色名
	unsigned int                        Type;  // 角色类型
	unsigned int                        Level;  // 角色等级
	unsigned int                        VIP;  // 角色VIP等级
	std::string                         CreateTime;  // 角色创建时间
	std::string                         LastLoginTime;  // 角色最后登陆时间
	unsigned int                        OwnerGroupID;  // 所属分区ID(不属于任何分区置0)
	unsigned int                        LastGroupID;  // 最后登陆分区ID(创建完角色未登陆游戏服时为0)
	std::map<std::string, std::string>  Features;  // 角色扩展特征信息列表

	GameRole() : ID(0), Type(0), Level(0), VIP(0), OwnerGroupID(0), LastGroupID(0) {}
};

// 游戏角色信息(CP信息)
struct AONE_DLL GameRoleCP
{
	std::string                         ID;  // 角色ID
	std::string                         Name;  // 角色名
	unsigned int                        Type;  // 角色类型
	unsigned int                        Level;  // 角色等级
	unsigned int                        VIP;  // 角色VIP等级
	std::string                         CreateTime;  // 角色创建时间
	std::string                         LastLoginTime;  // 角色最后登陆时间
	std::string                         OwnerGroupID;  // 所属分区ID(不属于任何分区置0)
	std::string                         LastGroupID;  // 最后登陆分区ID(创建完角色未登陆游戏服时为0)
	std::map<std::string, std::string>  Features;  // 角色扩展特征信息列表

	GameRoleCP() : Type(0), Level(0), VIP(0){}
};

// 渠道支付商品信息
struct AONE_DLL GameProduct
{
	std::string   ID;  // 商口ID(如：com.aonesoft.sanguo.item1)
	std::string   Name;  // 商品名称(如：小袋金币)
	std::string   Describe;  // 商品描述信息
	std::string   Currency;  // 币种(如人民币:CNY,港币:HKY,美元:USD)
	double        Amount;  // 币值(人民币单位为元,美元单位为美元)
	double        Amount_usd;  // 转换为美元后的币值
	unsigned int  Coin;  // 游戏代币数(不同游戏叫法不同,如金币,元宝等)
	std::string   Type; // 商品类型('' or 'normal': 空字符串表示定额普通商品; 特殊类型，例如：'mol_pinpay': MOL pinPay(MOL钱包按normal处理); 'fortumo': fortumo支付)
	std::string	  Discount; //商品折扣
};

struct AONE_DLL AoneDeviceInfo
{
	std::string DeviceId; // 设备ID(唯一标识该设备)
	std::string Locale; // (手机上设置的)国家和地区
	std::string Language; // (手机上设置的)语言
	std::string Model; // 设备型号(HTC渴望v5、Iphone5)
	std::string Os; // 操作系统(IOS5、Android4.2)
	std::string Imei; // IMEI码(不能获取则置空)
	std::string Ip; // IP地址(客户端可能获取的为内网地址,服务端需要对该字段更新)
	std::string Mac; // MAC地址(不能获取则置空)
	std::string Idfa;  // IOS填IDFA，安卓填安卓ID
	std::string NetworkType; //获取网络类型 wifi/4g
};

struct AONE_DLL AoneAppInfo
{
	unsigned int AppId; // 应用ID(向平台申请得到)
	std::string AppLang;  // 应用语言
	std::string Channel;  // 联运(发行)渠道商(见联运渠道编码表)
	unsigned char MajorVersion;  // 客户端主版本号
	unsigned char MinorVersion;  // 客户端子版本号
	unsigned char RevisionVersion;  // 客户端修正版本号
	std::string cpExt;  // 扩展参数
};

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

typedef void(*AONECLIENT_CB)(int retCode, std::map<std::string, std::string> params);

class AONE_DLL AoneClient
{
public:
    /**
     *  简化版接口
     *  简化版接口只实现了初始化登录支付等基本功能
     *  如果需要实现更多功能请接入完整版接口
     *  所有简化版接口均以Simp开头
     */

	/*
	 * [必选接口] 初始化SDK
	 * [调用规则] 异步调用，在调用其他接口前调用，需要在平台配置渠道及参数
	 */
	static void SimpInitSDK(AONECLIENT_CB cb);

	/*
	 * [必选接口] 退出游戏
	 * [调用规则] 退出时调用，hasExitUI方法返回true，调用SDK自带退出UI，
	 */
	static void SimpExit();

	/*
	 * [必选接口] 判断退出游戏时当前sdk是否有自己的界面
	 * [调用规则] 调用Exit前调用
	 * [返回值]  true：SDK自带退出UI，false：SDK未自带退出UI
	 */
	static bool SimpHasExitUI();

	/*
	 * [可选接口] 获取游戏(使用真实货币支付购买的)商品列表
	 * [调用规则] 初始化成功后调用(需要在平台配置商品列表)
	 */
	static std::map<std::string, GameProduct> SimpAllProducts();  // 商品ID索引

	/*
	 * [可选接口] 获取游戏支持的语言种类列表
	 * [调用规则] 初始化成功后调用(需要在平台配置语言列表)
	 */
	static std::map<std::string, std::string> SimpAllLangs(); // 语言代号索引，语言名称为值

	/*
	 * [可选接口] 设置SDK使用的语言代号
	 * [调用规则] 同步调用，游戏需要支持多语言时使用，默认使用语言代号为zh-cn或en
	 * [方法说明] lang = 语系代号[-地区码]
	 * 		           语系代号参见ISO 639-1语系编码标准
	 * 		           地区码参见ISO 3166-1地区码标准
	 * 		           参考 http://blog.sina.com.cn/s/blog_985bedb40101na3x.html
	 * [匹配规则] 主要按语系匹配，若语系后带有地区码的优先匹配。
	 * [返回值]  0：成功，-10： 本地错误
	 */
	static int SimpSelectLang(const std::string& lang);

	/*
	 * [必选接口] 客户端登录
	 * [调用规则] 异步调用，登陆需要的参数定义在aonesdk.json
	 * [方法说明] 分为渠道登陆及第三方登陆两种方式。如果使用第三方登陆，
	 * 		      在回调函数的params内，会有两个key："registerDevice"（设备ID是否已注册），
	 * 		      "registerOauth"（第三方账号是否已注册），值为0时表示未注册，值为1时表示已注册。
	 */
	static void SimpLogin(AONECLIENT_CB cb);

	/*
	 * [必选接口] 获取用户ID
	 * [调用规则] 在调用Login成功后可调用
	 */
	static unsigned int SimpUserID();

	/*
	 * [必选接口] 获取用户验证token
	 * [调用规则] 在调用Login成功后可调用
	 */
	static const std::string SimpUserToken();

	/*
	 * [必选接口] 创建角色后调用接口
	 * [调用规则] 同步调用，创建角色后调用
	 * [调用参数] roleID: 创建成功时返回的角色ID
	 *         name: 创建的角色名
	 *         level: 角色等级
	 *         features: 创建的角色扩展属性字段
	 *         groupId: 角色所属的分区, 在不分区的游戏中该字段置0
	 *         groupName；所在分区名
	 *         roleVip： 角色VIP等级
	 * [返回值]  0表示成功; <0 本地错误; >0服务端错误
	 */
	static void SimpCreateRole(const std::string& roleID, const std::string& name, unsigned int level, const std::map<std::string, std::string>& features, const std::string& owner_groupid, std::string groupName, int roleVIP);

	/*
	 * [必选接口] 提交角色信息
	 * [调用规则] 同步调用，登录后调用
	 * [调用参数] roleID: 创建成功时返回的角色ID，不可为空
	 *         name: 创建的角色名
	 *         level: 角色等级
	 *         groupId: 角色所属的分区, 在不分区的游戏中该字段置0，不可为空
	 *         groupName；所在分区名
	 *         roleVip： 角色VIP等级
	 *         isLogin: 是否是登录时提交角色信息
	 *         action:有四种状态 1."login"登录即进入游戏 2."createRole"创建角色 3."roleUp"角色升级 4."logout"退出游戏
	 */
	static void SimpSubmitRoleData(const std::string& roleID, const std::string& name, unsigned int level, const std::string& groupId, const std::string& groupName, int roleVip, bool isLogin = true, std::string action = "login",unsigned long long roleCTime = 0);

	/*
	 * [可选接口] 设置允许使用的支付渠道
	 * [调用规则] 同步调用，SimpPay()之前调用
	 * [调用参数] payChannels: 支付渠道，多个渠道之前以分号分割开
	 */
	static void SimpEnablePayChannels(const std::string& payChannels);

	/*
	 * [必选接口] 支付购买
	 * [调用规则] 异步调用，商品列表可以在平台配置后SDK拉取过来，也可以由游戏服提供，二者数据需保持一致
	 * 		           支付完成后，等待支付服回调成功后游戏服给玩家下发代币
	 * [调用参数] product_id: 要购买的虚拟商品ID(需要在后台配置)
	 *			purchaseNum:购买某个商品的笔数
	 *         cp_ext: 游戏开发商扩展字段(一般用来存开发商的订单号，不能超过255字节，可以为空)
	 */
	static void SimpPay(const std::string& product_id, unsigned int purchaseNum, const std::string& cp_ext, AONECLIENT_CB cb);

	/*
	 * [必选接口] 注销当前账号 或者 切换账号
	 * [调用规则] 游戏内注销账号或者切换时必须使用
	 */
	static void SimpLogout(AONECLIENT_CB cb);

public:
    // [必选接口] 加载所有sdk plugin，在iOS平台需手动调用,推荐在`didFinishLaunchingWithOptions` 里面调用
    static void loadAllSdks();
    
	// [必选接口] 初始化SDK
	// 异步调用，在调用其他接口前调用，需要在平台配置渠道及参数
	static void InitSDK(AONECLIENT_CB cb);

	// [可选接口] 获取aonesdk.json中的数据
	static std::string GetAoneConfigData(std::string key);

	// [可选接口] 获取游戏(使用真实货币支付购买的)商品列表
	// 在调用InitSDK成功后可调用，需要在平台配置商品列表
	static std::map<std::string, GameProduct> AllProducts();  // 商品ID索引
	// [可选接口] 获取游戏支持的语言种类列表
	// 在调用InitSDK成功后可调用，需要在平台配置语言列表
	static std::map<std::string, std::string> AllLangs(); // 语言代号索引，语言名称为值



	// [可选接口] 设置SDK使用的语言代号
	// 同步调用，游戏需要支持多语言时使用，默认使用语言代号为zh-cn或en
	// lang = 语系代号[-地区码]
	// 语系代号参见ISO 639-1语系编码标准
	// 地区码参见ISO 3166-1地区码标准
	// 参考 http://blog.sina.com.cn/s/blog_985bedb40101na3x.html
	// 匹配规则：主要按语系匹配，若语系后带有地区码的优先匹配。
	// 返回：0表示成功; <0 本地错误
	static int SelectLang(const std::string& lang);



	// [必选接口] 检测客户端版本
	// 同步调用，获取到的版本信息用于指导升级版本
	// 调用参数:
	//     version: 调用成功时返回版本信息
	//     majorVersion: 主版本号
	//     minorVersion: 子版本号
	//     revisionVersion: 修订版本号
	// 返回码：0表示成功; <0 本地错误; >0服务端错误
	static int CheckVersion(/*out*/GameVersion& version, unsigned char majorVersion, unsigned char minorVersion, unsigned char revisionVersion);

	// 异步调用,回调成功后调用GetGameVersion()获取信息
	static void CheckVersion(unsigned char majorVersion, unsigned char minorVersion, unsigned char revisionVersion, AONECLIENT_CB cb);
	static GameVersion GetGameVersion();
	static bool IsNeedPatch(std::string packageResDir, std::string patchResDir);
	static bool IsNeedForceUpdate(std::string packageResDir, std::string patchResDir);

    //  判断是否用的是01GameSDK
    static bool isUse01gameSdk();
    
	static void setGameVersion(int major_version, int minor_version ,int revision_version);
    
	// 设置是否使用用户登录
	static void SetUseUserLogin(bool useUserLogin);

	// [必选接口] 登陆验证用户账号
	// 异步调用，登陆需要的参数定义在aonesdk.json
	// 分为渠道登陆及第三方登陆两种方式。如果使用第三方登陆，在回调函数的params内，会有两个key："registerDevice"（设备ID是否已注册），"registerOauth"（第三方账号是否已注册），值为0时表示未注册，值为1时表示已注册。
	static void Login(AONECLIENT_CB cb);

    // [必选接口] 第三方登陆验证用户账号
    static void ThirdParthLogin(AONECLIENT_CB cb);
    
	// [必选接口] 获取用户ID
	// 在调用Login成功后可调用
	static unsigned int UserID();
	// [必选接口] 获取用户验证token
	// 在调用Login成功后可调用
	static unsigned int UserToken();
	// [必选接口] 获取游戏分区列表
	// 在调用Login成功后可调用
	static std::vector<GameGroup> AllGroups();
	// [可选接口] 获取游戏角色列表
	// 在调用Login成功后可调用
	static std::vector<GameRole>  AllRoles();

	// [可选接口] 创建角色, 请优先使用异步调用方式
	// 同步调用，公司内部游戏使用，合作方CP可选用
	// 调用参数：
	//   role: 创建成功时返回的角色信息
	//   name: 创建的角色名
	//   type: 创建的角色类型，不分类型置0
	//   features: 创建的角色扩展属性字段
	//   owner_groupid: 角色所属的分区ID；在不分区的游戏中该字段置0
	// 返回码：0表示成功; <0 本地错误; >0服务端错误
	static int CreateRole(/*out*/GameRole& role, const std::string& name, unsigned int type, const std::map<std::string, std::string>& features, unsigned int owner_groupid);

	// 异步调用, 回调成功后调用getSelectedRole获取role信息
	static void CreateRole(const std::string& name, unsigned int type, const std::map<std::string, std::string>& features, unsigned int owner_groupid, AONECLIENT_CB cb);

	// [必选接口] 选择角色和分区, 请优先使用异步调用方式
	// 同步调用，角色登陆成功后，必须调用该接口，否则后续支付将失败
	// 调用参数：
	//     role_id: 登陆游戏服时选择的角色ID
	//     group_id: 登陆游戏服务时选择的游戏分区ID
	// 返回码：0表示成功; <0 本地错误; >0服务端错误
	static int SelectRoleAndGroup(unsigned int role_id, unsigned int group_id);

	// 异步调用
	static void SelectRoleAndGroup(unsigned int role_id, unsigned int group_id, AONECLIENT_CB cb);

	// [可选接口] 拉取服务器列表，可用于刷新服务器列表
	static void pullGameGroup(AONECLIENT_CB cb);

	// [可选接口] 获取选择的角色信息，在调用SelectRoleAndGroup()之后才可调用
	static GameRole GetSelectedRole();

	// [可选接口] 获取选择的分区信息，在调用SelectRoleAndGroup()之后才可调用
	static GameGroup GetSelectedGroup();

    
    // [必选接口] 支付购买
    // 异步调用，商品列表可以在平台配置后SDK拉取过来，也可以由游戏服提供，二者数据需保持一致
    // 支付完成后，等待支付服回调游戏服给玩家代币
    // 方法默认一个商品购买一次， 如需一个商品购买多次，请调用`Pay(product_id, purchaseNum, cp_ext, cb)方法
    // 调用参数：
    //     product_id: 要购买的虚拟商品ID, 当设为“”时，表示特殊支付方式：充值卡（如mol）或者不定额（如fortumo）支付
    //     cp_ext: 游戏开发商扩展字段(一般用来存开发商的订单号，不能超过255字节，可以为空)
    //     cb: 支付结果回调函数
    static void Pay(const std::string& product_id, const std::string& cp_ext, AONECLIENT_CB cb);
    
	// [必选接口] 支付购买
	// 异步调用，商品列表可以在平台配置后SDK拉取过来，也可以由游戏服提供，二者数据需保持一致
	// 支付完成后，等待支付服回调游戏服给玩家代币
	// 调用参数：
	//     product_id: 要购买的虚拟商品ID, 当设为“”时，表示特殊支付方式：充值卡（如mol）或者不定额（如fortumo）支付
	//     purchaseNum:购买某个商品的笔数
	//     cp_ext: 游戏开发商扩展字段(一般用来存开发商的订单号，不能超过255字节，可以为空)
	//     cb: 支付结果回调函数
	static void Pay(const std::string& product_id, unsigned int purchaseNum, const std::string& cp_ext, AONECLIENT_CB cb);
    

	// [可选接口] 注销当前账号 或者 切换账号
	// 异步调用，游戏内注销账号或者切换时必须使用
	static void Logout(AONECLIENT_CB cb);



	// [必选接口] 退出游戏
	static void Exit();
	// [必选接口] 判断退出游戏时当前sdk是否有自己的界面
	static bool hasExitUI();


	// [可选接口] 设置配置所在路径（用于unity项目）
	static void AddFileSearchPath(std::string path);


	// [可选接口] 判定是否显示当地支付支付商页面请求, 用于海外发行避开谷歌审查
	// 当retCode=0时，params["enable"] = "0" 或者 params["enable"] = "1";
	static void CheckIsEnableLocalPay(AONECLIENT_CB cb);

	/**
	 * [可选接口] 角色删除功能
	 * @param type 0: 拉取注销时间; 1: 取消注销; 2: 30天后注销; 3: 立刻注销
	 * */
	static void DeleteAccount(int type, AONECLIENT_CB cb);


	///////////////////////////////////////////////////////
	// 以下为测试接口，请不要使用!!!

	// 快速注册（此过程在快速注册时生成aone账号和密码，并使玩家登录游戏）
	// 同步调用
	static int QuickRegister();
	// 异步调用
	static void QuickRegister(AONECLIENT_CB cb);

	// [可选接口] 获取绑定的账号
	// 在调用QuickLogin、QuickRegister成功后可调用
	static std::string AoneAccount();
	//[可选接口] 获取快速注册成功后返回的aone密码
	// 在调用QuickRegister成功后可以调用
	static std::string AonePassword();

	// 设备ID快速登陆
	// 此接口没有参数，唯一标识里面已在内部封装。如果想在ui上显示快速登录后的账号，可以使用AoneAccount()获取。
	// 同步调用
	static int QuickLogin();
	// 异步调用
	static void QuickLogin(AONECLIENT_CB cb);
	// Aone账号注册
	// 同步调用
	static int AoneRegister(const std::string& account, const std::string& password);
	// 异步调用
	static void AoneRegister(const std::string& account, const std::string& password, AONECLIENT_CB cb);

	// Aone账号登陆
	// 同步调用
	static int AoneLogin(const std::string& account, const std::string& password);
	// 异步调用
	static void AoneLogin(const std::string& account, const std::string& password, AONECLIENT_CB cb);

	// Aone账号修改密码
	// 同步调用
	static int AoneChangePassword(const std::string& oldpass, const std::string& newpass);
	// 异步调用
	static void AoneChangePassword(const std::string& oldpass, const std::string& newpass, AONECLIENT_CB cb);
	//继承码[引继码](日本版本)
	static void AoneCreateInheritance(const std::string& password, AONECLIENT_CB cb);
	// Aone账号注册同时绑定到设备
	// 同步调用
	static int AoneBind(const std::string& account, const std::string& password);
	// 异步调用
	static void AoneBind(const std::string& account, const std::string& password, AONECLIENT_CB cb);

	// 第三方账号绑定
	static void OAuthBind(AONECLIENT_CB cb);
	// 第三方账号登录，注意，掉用过Login()，成功获取第三方token之后，才能调用该接口
	static void OAuthLogin(AONECLIENT_CB cb);
	// aone账号绑定到其他账号上
	static void AoneBindOtherAccount(std::string newAccount,AONECLIENT_CB cb);

	// aone账号从其他账号上解绑
	static void AoneUnbindOtherAccount(AONECLIENT_CB cb);
	// 查询aone账号绑定的其他账号
	static int AoneQueryBindOtherAccount();
	static void AoneQueryBindOtherAccount(AONECLIENT_CB cb);

public:
	// [可选接口] sdk相关接口
	static void callFunc(const char* funcName);
	static void callFuncWithStringParam(const char* funcName, const char* param);
	static void callFuncWithParam(const char* funcName, void* param);
	static int callIntFunc(const char* funcName, void* param);
	static std::string callStringFunc(const char* funcName);
    static std::string callStringFuncFromSDK(const char* funcName, const char* sdkName);
	static std::string callStringFuncWithParam(const char* funcName, void* param);
	


	// [可选接口] 设置第三方登陆sdk，eg:setOAuthType("facebook");
	// 如果只有一个第三方登陆类sdk，可以不显示调用此接口，而在uuSdkConfig.json内设置："defaultUserSdk":"user sdk name" (eg. UserFacebook)
	static void SetOAuthType(const std::string oauth_type);
	static std::string GetOAuthType();

	// [可选接口] 设置\支付类sdk，eg:setPayChannel("appstore");
	// 如果只有一个第三方支付类sdk，可以不显示调用此接口，而在uuSdkConfig.json内设置："defaultIapSdk":"iap sdk name" (eg. IAPAppstore)
	static void SetPayChannel(const std::string payChannel);
	static std::string GetPayChannel();



	// [可选接口] 分享类sdk相关接口
	// 支持参数：imagePath, text, warning！可能添加完善其他参数！！！
	static void Share(std::map<std::string, std::string> info, AONECLIENT_CB cb);
	// [可选接口] 设置分享类sdk名称. eg: SetShareSdk("ShareWeibo")
	// 如果只有一个分享类sdk，可以不显示调用此接口，而在uuSdkConfig.json内设置："defaultShareSdk":"share sdk name"(eg. ShareWeibo)
	static void SetShareSdk(const char* sdkName);



	// [可选接口] 统计类sdk相关接口
	// 统计某一事件，举例：eventId = "purchase"
	static void LogEvent(const char* eventId);
	// 由于某些sdk的特殊性，在这里需要统一进行规范：确保info中的第一个键为eventValue，即info["eventValue"] = ""
	// 统计带参数的某一事件，举例：eventId = "purchase"，info["金额"] = price, info["类型"] = type
	// 注意：还可统计数值分布，需要添加一个特殊的key：__ct__ 举例：统计播放音乐的时长：info["__ct__"] = play_music_duration
	static void LogEvent(const char* eventId, std::map<std::string, std::string>  info);
	// 统计在某一个界面显示时长（慎用，目前只支持umeng，可使用logEvent变通实现此功能）
	static void LogPageStart(const char* pageName);
	static void LogPageEnd(const char* pageName);
	// [可选接口] 设置统计类sdk名称，如果只有一个统计类sdk，可以不显示调用此接口，而在uuSdkConfig.json内设置："defaultAnalyticsSdk":"analytics sdk name"(eg. AnalyticsUmeng)
	static void SetAnalyticsSdk(const char* sdkName);


	// [可选接口] 广告类sdk相关接口
	static void ShowAds(std::map<std::string, std::string> info, int pos, AONECLIENT_CB cb);
	static void HideAds(std::map<std::string, std::string> info, AONECLIENT_CB cb);
	static void QueryPoints(AONECLIENT_CB cb);
	static void SpendPoints(int points, AONECLIENT_CB cb);
	// [可选接口] 设置广告类sdk名称，如果只有一个广告类sdk，可以不显示调用此接口，而在uuSdkConfig.json内设置："defaultAdsSdk":"ads sdk name"(eg. AdsAdmob)
	static void SetAdsSdk(const char* sdkName);


	// [可选接口] 推送类sdk相关接口
	// 注意：目前发现有些推送sdk的联网速度很慢，会很久才会调用回调函数。所以建议使用无回调函数的版本。
	// 开启推送，默认情况下推送接口都是开启的，所以大多数情况下不需要显示调用此接口
	static void StartPush();
	static void StartPush(AONECLIENT_CB cb);
	// 关闭推送，关闭后会不会再收到推送通知（如果不想接收点对点推送或者群组推送通知，建议使用下面的DelAlias和DelTags接口）
	static void ClosePush();
	static void ClosePush(AONECLIENT_CB cb);
	// 设置别名，用于点对点推送。别名可设置为能唯一标识的id。建议使用roleId，可以在每次登陆后调用一次：SetAlias(roleId)。
	static void SetAlias(const char* alias);
	static void SetAlias(const char* alias, AONECLIENT_CB cb);
	// 删除别名，删除之后，不再接收给原别名推送的消息（注意：腾讯信鸽并不支持取消别名）
	static void DelAlias(const char* alias);
	static void DelAlias(const char* alias, AONECLIENT_CB cb);
	// 设置标签，用于给一组设备推送。举例：标签设置成公会id，可给在该公会的所有设备推送。
	// 如果想给设置一组标签，请使用分号";"，连接。 
	// 注意：极光推送是覆盖设置（每次设置标签需要设置设备关联的所有标签）。而腾讯信鸽是追加设置（建议每次仅设置一个标签）
	static void SetTags(const char* tags);
	static void SetTags(const char* tags, AONECLIENT_CB cb);
	// 删除标签，删除之后，向原标签推送时不会再推送到此设备
	// 注意：极光推送会删除所有标签（如果想删除特定标签，请使用setTags从新设置标签组）。腾讯信鸽会删除指定的标签（）
	static void DelTags(const char* tags);
	static void DelTags(const char* tags, AONECLIENT_CB cb);
	// 设置本地推送内容
	// 需设置参数：info["content"] info["title"] info["tickText"](小tick提示，点开后或者下拉后会显示title和content内容) info["interval"]：距离当前多长时间后提送(单位：秒）
	// 可选参数：info["notifyId"] clearNotification()需用到; info["repeatInterval"] 每隔一段时间发送一次，单位为秒，只有大于0时才有效
	static void AddNotify(std::map<std::string, std::string> info);
	// 取消特定id的本地推送
	static void ClearNotify(int notifyId);
	// 取消所有本地推送
	static void ClearAllNotify();
	// [可选接口] 设置推送类sdk名称，如果只有一个推送类sdk，可以不显示调用此接口，而在uuSdkConfig.json内设置："defaultPushSdk":"ads sdk name"(eg. PushJpush)
	static void setPushSdk(const char* sdkName);


	// [可选接口] 获取设备信息
	static AoneDeviceInfo getDeviceInfo();
	// [可选接口] 获取app版本信息
	static AoneAppInfo getAppVersionInfo();

	// [测试接口] 设置自定义deviceId
	static void SetDeviceId(const char* deviceId);
    
    static std::string GetChannelParam(const char* paramName);
	
public:
    // 分享系统
    
    /**
     获取广告跟踪链接
     
     @param userData 绑定自定义数据字符串，传入与这个跟踪链接绑定的自定义参数字符串
     */
    static void GetAdTrackLink(std::string userData, AONECLIENT_CB cb);
    
    // 获取应用内跳转URL
    static std::string GetAppLink();
    
    /**
     获取当前设备来自的广告源
     
     @param userData GetAdTrackLink设置的用户参数
     @param cb 回调
     'token' 表示广告源标识
     'userData' 表示GetAdTrackLink设置的用户参数
     */
    static void GetAdSource(std::string& userData, AONECLIENT_CB cb);
    
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
//    static std::string EmbedQR(std::string srcImgPath, std::string info, int x, int y, int w, int h);

	/**
	设备埋点
	@param sdkPoint 埋点信息
	*/
	static void recordDevicePoint(unsigned int sdkPoint);

	/**
	角色埋点
	@param gamePoint 埋点信息
	*/
	static void recordRolePoint(unsigned int gamePoint);

public:
	static void OpenUrl(std::string url);
	
	static void copyToClipboard(std::string text);
	//一键加群
	static void joinQGroup();
    //app内调起苹果评星、评论
    static void starReviewInApp(const char* appid);
    //应用内评星
    static void starInApp();
    //应用内调起苹果商店评论
    static void reviewInApp(const char* appId);
	//根据包名区分是否安装某个App(android:packageName,ios:bundle id)
	//传"",android:googleplay,ios:app store
	static int isInstallApp(const std::string& packageName="");
    
    //获取aonesdk.json文件
    static std::string getSdkConfig();
    //获取uusdkConfig.json
    static std::string getPluginConfig();
	//获取user_account
	static std::string getUserAccount();

	/**
	账号实名认证信息查询请求
	*/
	static void realnameInfo(AONECLIENT_CB cb);

	/**
	账号实名认证请求
	*/
	static void realnameCertificate(const char* identity_number, const char* real_name, AONECLIENT_CB cb);
	
	static void requestPlayerExtraInfo(AONECLIENT_CB cb);

	static unsigned char* loadFileToBytes(const char* fileName, unsigned long* read_size);
	
	static bool check_file_exists(const char* path);

private:
	AoneClient();
	~AoneClient();
	AoneClient(const AoneClient& other);
	AoneClient operator=(const AoneClient& other);
};

}//namespace aonesdk

#endif//_AONE_CLIENT_H_
