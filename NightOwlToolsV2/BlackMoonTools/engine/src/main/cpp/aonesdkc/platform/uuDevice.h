#ifndef __UUDEVICE_H__
#define __UUDEVICE_H__

#include <string>
#include <vector>
#include <map>


using namespace std;

enum UPDATE_TYPE{
	UPDATE_TYPE_NONE = 0,
	UPDATE_TYPE_OPTION,
	UPDATE_TYPE_FORCE
};

typedef void(*UpdateCallBack)();
typedef void(*AnnounceCallBack)();
typedef void(*ASS_CB)(int result, map<string, string> dataMap);
typedef void(*RPEI_CB)(int result, map<string, string> dataMap);


class uuDevice {
public:
    static std::string getDeviceId();
    static std::string getCountry();
    static std::string getLanguage();
    static std::string getModel();
    static std::string getOS();
    static std::string getIMEI();
    static std::string getIp();
    static std::string getMac();
	// wifi or 3g,4g, or null(no network)
	static std::string getNetworkType();
	// opertor code: eg. cmcc: 46000
	static std::string getOpertorCode();
	static std::string getIdfa();

	// 保存图片
    static void saveQuickRegisterPic(const std::string& aoneAccount, const std::string& aonePassword, const std::string& currentLang);

	static void getAppVersion(unsigned char& majorVersion, unsigned char& minorVersion, unsigned char& revisionVersion);

	static void showUpdateDialog(const std::string& updateURL, const std::string& updateText, UPDATE_TYPE updateType, UpdateCallBack cb);

	static void showAnnounceDialog(const std::string& announceURL, AnnounceCallBack cb);
	static void aoneUpdateCallback();
	static void aoneAnnounceCallback();

	//打开网页
	static void openUrl(const std::string& url);
	
    static void systemShare(map<string, string> params, ASS_CB cb);
	//复制内容到剪切板
    static void copyToClipboard(const std::string& text);
	//跳转到群界面(可以一键添加群)
    static void joinQGroup(const std::string& uin,const std::string& key);

    //应用内评星
    static void starInApp();
    //应用内调起苹果商店评论
    static void reviewInApp(const std::string& appId);
    //应用内评星、评论(appId:应用的 APPID)
    static void starReviewInApp(const std::string& appId);
	//根据包名区分是否安装某个App(android:packageName,ios:bundle id)
	//传"",android:googleplay,ios:app store
	static int isInstallApp(const std::string& packageName="");
	//显示实名认证页面
	static void showIdcardUI();
	//查询玩家信息：是否未成年，在线时长等
	static void requestPlayerExtraInfo(RPEI_CB cb);
	
	static void requestPlayerExtraInfoCallback(int code, std::map < std::string, std::string> msg);
};

#endif // __UUDEVICE_H__
