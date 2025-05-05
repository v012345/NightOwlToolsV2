#include "uuSdkManager.h"
#include "aonesdk.h"
#include "xcore_md5.h"
#include "xnet_sock_connector.h"
#include "aone2_rc4_parser.h"
#include "AonesdkConfig.h"
#include "uuSdkConfigHelper.h"
#include "uuDevice.h"
#include "xcore_rc4.h"
#include "xcore_json.h"
#include "xcore_str_util.h"
#include "uuNetAsync.h"
#include <time.h>
#include "cplog.h"
#include "cpfile.h"
#include "zf_log.h"
//#include "UUGenerateQRCode.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>

#ifdef WIN32
#define snprintf _snprintf
#endif


namespace aonesdk {

string AoneSDK::m_apiserver_host = "";  // api服域名或IP
string AoneSDK::m_apiserver_ip = ""; // 域名解析失败时的备用IP
uint16 AoneSDK::m_apiserver_port = 0;  // api服监听端口
string AoneSDK::m_apiserver_key = "c68e93c27ddde26acbc209451b035773";  // api服加密key 非appkey

static Aone2RC4Parser* s_apiParser;
static Aone2RC4Parser* s_entryParser;

const char* EVENT_LOGIN_TYPE_NORMAL = "Login";
const char* EVENT_LOGIN_TYPE_QUICK = "Quick login";
const char* EVENT_LOGIN_TYPE_THIRDPARTY = "Thirdparty login";
const char* EVENT_LOGIN_TYPE_QUICK_REGISTER = "Quick register";

const char* LOG_TAG = "[AoneSDK]";


static bool checkFirstOpen()
{
	bool ret = false;
	string path = cpfile_write_path();
	path += "aone_firstop";
	FILE *f = fopen(path.c_str(), "rb");
	if (f == NULL) {
		char buffer[1] = {'1'};
		f = fopen(path.c_str(), "wb");
		if (f != NULL) {
			fwrite(buffer, 1, 1, f);
			fclose(f);
			ret = true;
		}
	} else {
		fclose(f);
	}
	return ret;
}

static bool checkFirstRecordDevice()
{
	bool ret = false;
	string path = cpfile_write_path();
	path += "aone_firstrecord";
	FILE *f = fopen(path.c_str(), "rb");
	if (f == NULL) {
		char buffer[1] = { '1' };
		f = fopen(path.c_str(), "wb");
		if (f != NULL) {
			fwrite(buffer, 1, 1, f);
			fclose(f);
			ret = true;
		}
	}
	else {
		fclose(f);
	}
	return ret;
}

static void saveUserInfo(string account, string password, string type, string userId, string thirdAccount, string thirdUserId, string oauthType){
    //cpfile_external_path ：永久保存
//    string path = cpfile_write_path();
	string path = cpfile_external_path();
	path += "aone_userinfo.json";
	uuLOG("path:%s", path.c_str());
	//generator json string
	string data = "{";
	data.append("\"account\":\"")
		.append(account)
		.append("\",\"password\":\"")
		.append(password)
		.append("\",\"type\":\"")
		.append(type)
		.append("\",\"userId\":\"")
		.append(userId)
		.append("\",\"thirdAccount\":\"")
		.append(thirdAccount)
		.append("\",\"thirdUserId\":\"")
		.append(thirdUserId)
		.append("\",\"oauthType\":\"")
		.append(oauthType)
		.append("\"}");
	fstream f(path.c_str(), ios::out);
	f << data.c_str(); //写入数据
	f.close();
}

static void deleteUserInfo(){
	string path = cpfile_external_path();
	path += "aone_userinfo.json";
	unlink(path.c_str());
}

static void cheackUserInfo(){
    //检测cache是否存有老数据，将老数据转移到document中，删除cache中的老数据
    //读取cache目录下的aone_userinfo.json文件
    string c_path = cpfile_write_path();
    c_path += "aone_userinfo.json";
    ifstream in(c_path);
    AoneSDK::AoneUserInfo aoneUserInfo;
    string fileData = (char*)cpfile_readstr(c_path.c_str());
    uuLOG("c_path-fileData:%s", fileData.c_str());
    XJsonReader reader;
    XJsonValue value;
    const char* data = fileData.c_str();
    if (data == NULL || !reader.parse(data, value) || value.type() != XJsonValue::objectValue) {
        //没有数据，不用管
    }else{
        //有数据，读取数据
        XJsonValue::ObjectValues objectValues = value.as_objects();
        for (XJsonValue::ObjectIterator it = objectValues.begin(); it != objectValues.end(); ++it){
            if (it->second.type() == XJsonValue::stringValue){
                if (it->first == "account"){
                    aoneUserInfo.account = it->second.as_string();
                }
                else if (it->first == "password"){
                    aoneUserInfo.password = it->second.as_string();
                }
                else if (it->first == "type"){
                    aoneUserInfo.type = it->second.as_string();
                }
                else if (it->first == "userId"){
                    aoneUserInfo.userId = it->second.as_string();
                }
                else if (it->first == "thirdAccount"){
                    aoneUserInfo.thirdAccount = it->second.as_string();
                }
                else if (it->first == "thirdUserId"){
                    aoneUserInfo.thirdUserId = it->second.as_string();
                }
                else if (it->first == "oauthType"){
                    aoneUserInfo.oauthType = it->second.as_string();
                }
            }
        }
        
        //保存到document里的aone_userinfo.json文件
        string d_path = cpfile_external_path();
        d_path += "aone_userinfo.json";
        string data = "{";
        data.append("\"account\":\"")
            .append(aoneUserInfo.account)
            .append("\",\"password\":\"")
            .append(aoneUserInfo.password)
            .append("\",\"type\":\"")
            .append(aoneUserInfo.type)
            .append("\",\"userId\":\"")
            .append(aoneUserInfo.userId)
            .append("\",\"thirdAccount\":\"")
            .append(aoneUserInfo.thirdAccount)
            .append("\",\"thirdUserId\":\"")
            .append(aoneUserInfo.thirdUserId)
            .append("\",\"oauthType\":\"")
            .append(aoneUserInfo.oauthType)
            .append("\"}");
        fstream f(d_path.c_str(), ios::out);
        f << data.c_str(); //写入数据
        f.close();

        //删除原来cache里的aone_userinfo.json文件
        string path = cpfile_write_path();
        path += "aone_userinfo.json";
        unlink(path.c_str());
    }
}
//预防渠道进行分发做的限制
bool AoneSDK::isAppOK(const string& code){
	//bool is_verify_key_ok = dealAppMsg("AppVerifyKey",m_verify_key.c_str());
	//bool is_app_name_ok = dealAppMsg("AppName",m_app_name.c_str());
	//bool is_package_name_ok = dealAppMsg("PackageName",m_package_name.c_str());
	//bool is_launcher_activity_ok = dealAppMsg("ClassName",m_launcher_activity.c_str());
	//if(is_verify_key_ok && is_app_name_ok && is_package_name_ok && is_launcher_activity_ok){
	//	return true;
	//}
	uuLOG("%s isAppOK:%s", LOG_TAG, code.c_str());
	return true;//如果预防分发这里设为false
}

static AoneSDK::AoneUserInfo getUserInfo(){
    //检测老数据
    cheackUserInfo();
    
	string path = cpfile_external_path();
	path += "aone_userinfo.json";
	ifstream in(path);
	AoneSDK::AoneUserInfo aoneUserInfo;
	string fileData = (char*)cpfile_readstr(path.c_str());
    uuLOG("path-fileData:%s", fileData.c_str());
	XJsonReader reader;
	XJsonValue value;
	const char* data = fileData.c_str();
	if (data == NULL || !reader.parse(data, value) || value.type() != XJsonValue::objectValue) {
		return aoneUserInfo;
	}
	XJsonValue::ObjectValues objectValues = value.as_objects();
	for (XJsonValue::ObjectIterator it = objectValues.begin(); it != objectValues.end(); ++it){
		if (it->second.type() == XJsonValue::stringValue){
			if (it->first == "account"){
				aoneUserInfo.account = it->second.as_string();
			}
			else if (it->first == "password"){
				aoneUserInfo.password = it->second.as_string();
			}
			else if (it->first == "type"){
				aoneUserInfo.type = it->second.as_string();
			}
			else if (it->first == "userId"){
				aoneUserInfo.userId = it->second.as_string();
			}
			else if (it->first == "thirdAccount"){
				aoneUserInfo.thirdAccount = it->second.as_string();
			}
			else if (it->first == "thirdUserId"){
				aoneUserInfo.thirdUserId = it->second.as_string();
			}
			else if (it->first == "oauthType"){
				aoneUserInfo.oauthType = it->second.as_string();
			}
		}
	}
	return aoneUserInfo;
}

bool isJpSpecial(){
	string jp_special = AoneSDK::getInstance()->getChannelParamFromKey("JP_Special");
	return "jp_special" == jp_special;
}
    
static void convertRoleToRoleCP(GameRole& role, GameRoleCP& roleCP)
{
    roleCP.ID = XStrUtil::to_str(role.ID);
    roleCP.Name = role.Name;
    roleCP.Level = role.Level;
    roleCP.VIP = role.VIP;
    roleCP.CreateTime = role.CreateTime ;
    roleCP.LastLoginTime = role.LastLoginTime ;
    roleCP.OwnerGroupID = role.OwnerGroupID ;
    roleCP.LastGroupID = role.LastGroupID ;
    roleCP.Features = role.Features;
}

void AoneSDK::to_upper(char* pszSrc)
{
	if (pszSrc == NULL) return;
	char* pos = (char*)pszSrc;
	char diff = 'A' - 'a';
	while (*pos != '\0')
	{
		if ('a' <= *pos && *pos <= 'z')
		{
			*pos += diff;
		}
		pos++;
	}
	return;
}

string AoneSDK::make_authkeyA(const string& strData)
{
	static string str = "0123456789ABCDEF";
	srand((uint32)time(NULL));
	string str1 = aonesdk_xcore::md5(strData.c_str(), strData.size());
	AoneSDK::to_upper((char*)str1.c_str());
	char ch1 = str.at(rand() % 16);
	char ch2 = str.at(rand() % 16);
	str1.at(0) = ch1;
	str1.at(31) = ch2;
	string str2 = aonesdk_xcore::md5(str1.c_str(), str1.size());
	str2.at(0) = ch1;
	str2.at(31) = ch2;
	AoneSDK::to_upper((char*)str2.c_str());
	return string("A") + str2;
}

vector<string> split(const string& str, const string& delim) {  
	vector<string> res;  
	if("" == str) return res;  
	//先将要切割的字符串从string类型转换为char*类型  
	char * strs = new char[str.length() + 1] ; //不要忘了  
	strcpy(strs, str.c_str());   
 
	char * d = new char[delim.length() + 1];  
	strcpy(d, delim.c_str());  
 
	char *p = strtok(strs, d);  
	while(p) {  
		string s = p; //分割得到的字符串转换为string类型  
		res.push_back(s); //存入结果数组  
		p = strtok(NULL, d);  
	}  
 
	return res;  
}

string AoneSDK::generate_payno(uint32 appid, uint32 groupid, uint32 roleid)
{
	static uint32 inc;
	char buf[1024];
	sprintf(buf, "%u%u%u%u%u", appid, groupid, roleid, (uint32)time(NULL), ((uint32)++inc) % 100);
	return buf;
}

string AoneSDK::generate_payno_num(uint32 appid, uint32 groupid, uint32 roleid)
{
	static uint32 inc;
	char buf[1024];
	sprintf(buf, "%u%u%u%u%u", appid, groupid, roleid, (uint32)time(NULL), ((uint32)++inc) % 100);
	return buf;
}

string AoneSDK::generate_payno_unicom(uint32 appid, uint32 groupid, uint32 roleid)
{
	static uint32 inc;
	char buf[1024];
	sprintf(buf, "%04u_%03u_0_%u_%02u", appid % 10000, groupid % 1000, (uint32)time(NULL), ((uint32)++inc) % 100);
	return buf;
}

/**
 *  CP生成内部订单
 */
string AoneSDK::generate_payno_cp(uint32 appid, const std::string& groupid, const std::string& roleid)
{
	static uint32 inc;
	char buf[1024];
	sprintf(buf, "%u%s%s%u%u", appid, groupid.c_str(), roleid.c_str(), (uint32)time(NULL), ((uint32)++inc) % 100);
	return buf;
}

/**
*  CP生成内部订单
*/
string AoneSDK::generate_payno_num_cp(uint32 appid, const std::string& groupid, const std::string& roleid)
{
	static uint32 inc;
	char buf[1024];
	sprintf(buf, "%u%s%s%u%u", appid, groupid.c_str(), roleid.c_str(), (uint32)time(NULL), ((uint32)++inc) % 100);
	return buf;
}

string AoneSDK::generate_payno_unicom_cp(uint32 appid, const std::string& groupid, const std::string& roleid)
{
	static uint32 inc;
	char buf[1024];
	sprintf(buf, "%04u_%s_0_%u_%02u", appid % 10000, groupid.substr(groupid.size() - 3, groupid.size()).c_str(), (uint32)time(NULL), ((uint32)++inc) % 100);
	return buf;
}

int AoneSDK::apiserver_send_recv(Aone2BaseMessage* request, Aone2BaseMessage** response)
{
	assert(request);
	assert(response);
	// 失败时最多尝试3次
	int ret = 0;
	string ip = AonesdkConfig::getInstance()->getTestIp();
	if (ip == "") {
		ip = AoneSDK::m_apiserver_host;
	}
	for (int i = 0; i < 3; i++)
	{
		XSockAddr addr(ip, AoneSDK::m_apiserver_port);
		if (addr.is_none()) {
			uuLOG("%s parse host name failed!", LOG_TAG);
			if(AoneSDK::m_apiserver_ip != ""){
				uuLOG("%s parse host name failed, use backup ip!", LOG_TAG);
				addr = XSockAddr(AoneSDK::m_apiserver_ip, AoneSDK::m_apiserver_port); // 解析失败，用备用IP
			}
		}

		XSockTransfer* transfer = XSockConnector(AoneSDK::m_apiserver_key).connect(addr, 3500, true);
		if (transfer == NULL)
		{
			ret = -1; // 连接服务器错误
			continue;
		}

		if (!transfer->write(request) || !transfer->read(response, 10000) || *response == NULL)
		{
			transfer->close();
			delete transfer;
			if (*response) delete *response;
			*response = NULL;
			ret = -2; // 通信错误
			continue;
		}

		transfer->close();
		delete transfer;
		return 0;
	}

	return ret;
}

int AoneSDK::entryserver_send_recv(Aone2BaseMessage* request, Aone2BaseMessage** response)
{
	if (m_entryserver_ip.empty() || m_entryserver_port == 0) return -3; // 未正确初始化

	// 失败时最多尝试3次
	int ret = 0;
	for (int i = 0; i < 3; i++)
	{
		XSockTransfer* transfer = XSockConnector(m_appkey).connect(XSockAddr(m_entryserver_ip, m_entryserver_port), 3500, true);
		if (transfer == NULL)
		{
			ret = -1; // 连接服务器错误
			continue;
		}

		if (!transfer->write(request) || !transfer->read(response, 10000) || *response == NULL)
		{
			transfer->close();
			delete transfer;
			if (*response) delete *response;
			*response = NULL;
			ret = -2; // 通信错误
			continue;
		}

		transfer->close();
		delete transfer;
		return 0;
	}

	return ret;
}

void AoneSDK::apiserver_send_recv_async(Aone2BaseMessage* request, NET_CB cb)
{
	byte *requestBytes = NULL;
	uint32 requestLen;
	if (s_apiParser != NULL) delete s_apiParser;
	s_apiParser = new Aone2RC4Parser(m_apiserver_key);
	s_apiParser->to_buffer(request, &requestBytes, requestLen);

	string ip = AonesdkConfig::getInstance()->getTestIp();
	if (ip == "") {
		ip = AoneSDK::m_apiserver_host;
	}
	uuLOG("ip:%s , m_apiserver_port:%d , m_apiserver_key:%s",ip.c_str(),m_apiserver_port,m_apiserver_key.c_str());
	uuNetAsync::send_recv(this, cb, ip, m_apiserver_port, m_apiserver_key, requestBytes, requestLen);
}

void AoneSDK::entryserver_send_recv_async(Aone2BaseMessage* request, NET_CB cb)
{
	byte *requestBytes = NULL;
	uint32 requestLen;
	if (s_apiParser != NULL) delete s_apiParser;
	s_apiParser = new Aone2RC4Parser(m_appkey);
	s_apiParser->to_buffer(request, &requestBytes, requestLen);

	uuNetAsync::send_recv(this, cb, m_entryserver_ip, m_entryserver_port, m_appkey, requestBytes, requestLen);
}

int AoneSDK::send_recv_async(const string& ip, int port, const string& key, byte* request, uint32 requestLen, byte** response, uint32& responseLen)
{
	if (ip.empty() || port == 0) return -3; // 未正确初始化

	// 失败时最多尝试3次
	int ret = 0;
	XSockAddr addr = XSockAddr(ip, port);
	if (ip == AoneSDK::m_apiserver_host) {
		if (addr.is_none()) {
			if(AoneSDK::m_apiserver_ip != "") {
				addr = XSockAddr(AoneSDK::m_apiserver_ip, port);
			}
		}
	}
	for (int i = 0; i < 2; i++)
	{
		XSockTransfer* transfer = XSockConnector(key).connect(addr, 3500, true);
		if (transfer == NULL)
		{
			ret = -1; // 连接服务器错误
			uuLOG("%s send_recv_async, ret = -1", LOG_TAG);
			continue;
		}

		if (!transfer->writeBytes(request, requestLen) || !transfer->readBytes(response, responseLen, 10000) || *response == NULL)
		{
			transfer->close();
			delete transfer;
			//if (*response) delete *response;
			*response = NULL;
			ret = -2; // 通信错误
			uuLOG("%s send_recv_async, ret = -2", LOG_TAG);
			continue;
		}

		byte* temp = *response;
		*response = new byte[responseLen];
		memcpy(*response, temp, responseLen);

		transfer->close();
		delete transfer;
		return 0;
	}
	
	uuLOG("%s send_recv_async, ret = %d, more than 2 times.", LOG_TAG, ret);

	return ret;
}

static string getUseTime(clock_t beginTime) {
	char buf[20];
	double ret = (double)(clock() - beginTime)/CLOCKS_PER_SEC ;
	sprintf(buf, "%d", (int)(ret * 1000));
	uuLOG("%s request use time: %s", LOG_TAG, buf);
	return buf;
}

int AoneSDK::http_send_recv(const string& ip, int port, const string& key, byte* request, uint32 requestLen, map<string, string>& params)
{
	params.clear();
	//params["url"] = ip;

	if (ip.empty() || port == 0) {
		params["code"] = "-4";
		params["detail"] = "no ip";
		return -4;
	}
	
	clock_t beginTime = clock();
	int ret = 0;
	XSockAddr addr = XSockAddr(ip, port);
	if (addr.is_none()) {
		params["code"] = "-3";
		params["detail"] = "dns parse fail";
		return -3;
	}
	XSockTransfer* transfer = XSockConnector(key).connect(addr, 3500, true);
	if (transfer == NULL)
	{
		params["use_ms"] = getUseTime(beginTime);
		params["ip"] = addr.get_hostaddr();

		ret = -1; // 连接服务器错误
		params["code"] = "-1";
		params["detail"] = "connect error";
		uuLOG("%s send_recv_http, ret = -1", LOG_TAG);
		return -1;
	}

	byte* response = NULL;
	uint32 responseLen = 0;
	if (!transfer->writeBytes(request, requestLen) || !transfer->readHttpBytes(&response, responseLen, 10000) || response == NULL)
	{
		params["use_ms"] = getUseTime(beginTime);
		params["ip"] = addr.get_hostaddr();

		transfer->close();
		delete transfer;
		if (response) delete response;
		response = NULL;
		ret = -2; // 通信错误
		params["code"] = "-2";
		params["detail"] = "transfer error";
		uuLOG("%s send_recv_http, ret = -2", LOG_TAG);
		return -2;
	}
	
	params["use_ms"] = getUseTime(beginTime);
	params["ip"] = addr.get_hostaddr();

	if (response != NULL) {
		int count = 0;
		int pos[2];
		for (int i = 0; i < (int)responseLen; ++i) {
			if (response[i] == ' ') {
				pos[count++] = i;
				if (count == 2) {
					break;
				}
			}
		}
		if (count == 2) {
			int len = pos[1] - pos[0] - 1;
			char strCode[20];
			memcpy(strCode, (char*)(response + pos[0] + 1), len);
			strCode[len] = 0;
			params["code"] = strCode;

			for (int i = pos[1] + 1; i < (int)responseLen - 1; ++i) {
				if (response[i] == '\r' && response[i + 1] == '\n') {
					int len = i - pos[1] - 1;
					char strDetail[100];
					memcpy(strDetail, (char*)(response + pos[1] + 1), len < 99 ? len : 99);
					strDetail[len] = 0;
					params["detail"] = strDetail;
					break;
				}
			}
		}

		delete response;
		response = NULL;
	}

/*
	//获取DNS服务器信息
	char strDns1[50];
	char strDns2[50];
	FIXED_INFO *fi =  (FIXED_INFO *)GlobalAlloc(GPTR,sizeof( FIXED_INFO));
	ULONG ulOutBufLen = sizeof(FIXED_INFO);
	DWORD ret = ::GetNetworkParams(fi, &ulOutBufLen);
	if(ret != ERROR_SUCCESS)
	{
		GlobalFree(fi);
		fi =  (FIXED_INFO *)GlobalAlloc( GPTR, ulOutBufLen );
		ret = ::GetNetworkParams(fi, &ulOutBufLen);
		if(ret != ERROR_SUCCESS)
		{
				uuLOG("Get Dns server failed");
				return false;
		}
	}
	strcpy(strDns1,fi->DnsServerList.IpAddress.String);
	IP_ADDR_STRING *pIPAddr = fi->DnsServerList.Next;
	if(pIPAddr != NULL) {
		strcpy(strDns2, pIPAddr->IpAddress.String);
	}
*/
	transfer->close();
	delete transfer;
	return 0;

	uuLOG("%s http_send_recv, ret = %d", LOG_TAG, ret);

	return ret;
}

string AoneSDK::parse_for_lang(const string& content)
{
	// 解析json
	XJsonValue root(XJsonValue::objectValue);
	if (false == XJsonReader().parse(content, root, false) || root.type() != XJsonValue::objectValue)
	{
		// 解析失败，原样返回
		return content;
	}

	// 取出默认语系和地区
	string app_lang = m_version_info.m_app_lang;

	XJsonValue::ObjectValues& objs = root.as_objects();

	// 先按严格匹配
	for (XJsonValue::ObjectIterator it = objs.begin(); it != objs.end(); ++it)
	{
		if (it->first == app_lang)
		{
			if (it->second.type() == XJsonValue::stringValue)
				return it->second.as_string();
			else
				return content;
		}
	}

	// 都没匹配上取第一个
	if (!objs.empty() && objs.begin()->second.type() == XJsonValue::stringValue)
	{
		return objs.begin()->second.as_string();
	}

	// 实在匹配不上了，原样返回
	return  content;
}

AoneSDK* AoneSDK::getInstance()
{
	static AoneSDK instance_;
	return &instance_;
}

AoneSDK::AoneSDK()
	: m_init_ok(false)
	, m_appid(0)
	, m_entryserver_port(0)
	, m_user_id(0)
	, m_user_token(0)
	, m_oauth_type("")
	, m_pay_channel("")
	, m_shareSdk("")
	, m_adsSdk("")
	, m_pushSdk("")
	, m_httpUrl("")
	, m_commitAfterPay(false)
	, mLoginSdkName("")
	, mUseUserLogin(true)
	, m_device_id("")
	, m_actual_discount(100)
{
	// empty
	m_ineffective_time = std::chrono::high_resolution_clock::now();
	m_begin_login_time = m_ineffective_time;
	m_begin_pay_time = m_ineffective_time;
	string host = AonesdkConfig::getInstance()->getHost();
	if (host != "") m_apiserver_host = host;
	string backupIp = AonesdkConfig::getInstance()->getBackupIp();
	if (backupIp != "") m_apiserver_ip = backupIp;
	string port = AonesdkConfig::getInstance()->getPort();
	if (port != "") m_apiserver_port = atoi(port.c_str());
	m_appid = AonesdkConfig::getInstance()->getAppId();
	m_appkey = AonesdkConfig::getInstance()->getAppKey();
}

uint32 AoneSDK::getAppID()
{
	return AonesdkConfig::getInstance()->getAppId();
}

string AoneSDK::getAppKey()
{
	return AonesdkConfig::getInstance()->getAppKey();
}

string AoneSDK::getChannelID()
{
	return AonesdkConfig::getInstance()->getChannel();
}
    
map<string, string> AoneSDK::getM_channel_params()
{
    return m_channel_params;
}

string AoneSDK::getSdkConfig()
{
    return AonesdkConfig::getInstance()->getSdkConfigString();
}

string AoneSDK::getPluginConfig()
{
    return uuSdkConfigHelper::getPluginConfigString();
}

GameVersion AoneSDK::getGameVersion()
{
	GameVersion version = m_game_version;
	version.Notice = this->parse_for_lang(version.Notice);
	return version;
}


void AoneSDK::copyProduct(GameProduct& fromProduct, map<string, GameProduct>& toMap)
{
	if (fromProduct.Type == "" || fromProduct.Type == "normal") {
		toMap[fromProduct.ID] = fromProduct;
		toMap[fromProduct.ID].Name = this->parse_for_lang(fromProduct.Name);
		toMap[fromProduct.ID].Describe = this->parse_for_lang(fromProduct.Describe);
	}
}


map<string, GameProduct> AoneSDK::getAllProducts()
{
	map<string, GameProduct> products;

	if (m_productIdMap.size() > 0)
	{
		for (auto it = m_productIdMap.begin(); it != m_productIdMap.end(); it++)
		{
			const std::string& cpProductId = it->first;
			const std::string& aoneProductId = it->second;

			if (m_channel_products.find(aoneProductId) != m_channel_products.end())
			{
				GameProduct product = m_channel_products[aoneProductId];
				product.ID = cpProductId;
				copyProduct(product, products);
			}
		}
	}
	else
	{
		for (map<string, GameProduct>::iterator it = m_channel_products.begin(); it != m_channel_products.end(); ++it)
		{
			GameProduct& product = it->second;
			copyProduct(product, products);
		}
	}

	return products;
}

vector<GameGroup> AoneSDK::getAllGroups()
{
	vector<GameGroup> groups = m_groups;
	for (int i = 0; i < (int)groups.size(); i++)
	{
		GameGroup& group = groups[i];
		group.Name = this->parse_for_lang(group.Name);
	}
	return groups;
}

GameGroup AoneSDK::getSelectedGroup()
{
	GameGroup group = m_select_group;
	group.Name = this->parse_for_lang(group.Name);
	return group;
}

map<string, string> AoneSDK::getAllAppLangs()
{
	return m_app_langs;
}

string AoneSDK::getAppLang()
{
	return m_version_info.m_app_lang;
}

bool AoneSDK::setAppLang(const string& lang)
{
    if (is01gameBackEnd() == false) {
        callFuncWithStringParam("selectLang", lang.c_str());
    }

	if (m_app_langs.find(lang) == m_app_langs.end()) {
		uuLOG("%s not support lang: [%s]", LOG_TAG, lang.c_str());
		return false;
	}
	m_version_info.m_app_lang = lang;
	return true;
}

void AoneSDK::initAsync(AONESDK_CB cb)
{
	uuLOG("%s initAsync start", LOG_TAG);
	if (m_init_ok) {
		cb(0, map<string, string>());
		return;
	}
    
    cplog_init();
	uuLOG("%s initAsync, sdk version: [%s], host: [%s], backupIp: [%s], port: [%d]", LOG_TAG, AONESDK_VERSION, m_apiserver_host.c_str(), m_apiserver_ip.c_str(), m_apiserver_port);

	mChceckFirstRecord = checkFirstRecordDevice();

	m_cb = cb;

	// 收集设备信息和应用版本信息
	if (m_device_id == "")
	{
		m_device_info.m_device_id = uuDevice::getDeviceId();
	}
	else
	{
		m_device_info.m_device_id = m_device_id;
	}
	m_device_info.m_locale = uuDevice::getCountry();
	m_device_info.m_language = uuDevice::getLanguage();
	m_device_info.m_model = uuDevice::getModel();
	m_device_info.m_os = uuDevice::getOS();
	m_device_info.m_imei = uuDevice::getIMEI();
	uuLOG("%s initAsync, imei: [%s]", LOG_TAG, m_device_info.m_imei.c_str());
	m_device_info.m_ip = uuDevice::getIp();
	uuLOG("%s initAsync, ip: [%s]", LOG_TAG, m_device_info.m_ip.c_str());
	m_device_info.m_mac = uuDevice::getMac();
	m_device_info.m_idfa = uuDevice::getIdfa();
	m_version_info.m_app_id = m_appid;
	m_version_info.m_app_lang = "zh_cn";
	m_version_info.m_channel = AonesdkConfig::getInstance()->getChannel();


	char extStr[1024];
	int wl = snprintf(extStr, sizeof(extStr), "ad_channel=%s&ad_active=%s", AonesdkConfig::getInstance()->getAdChannel().c_str(), AonesdkConfig::getInstance()->getAdActivity().c_str());
	if (wl == sizeof(extStr)) {
		extStr[sizeof(extStr)-1] = 0;
		uuLOG("%s initAsync, device ext too long, trunked", LOG_TAG);
	}
	uuLOG("%s initAsync, device ext: [%s]", LOG_TAG, extStr);
	m_version_info.m_ext = extStr;

	m_pay_channel = AonesdkConfig::getInstance()->getPayChannel();
    
    uuLOG("%s initAsync, appid: [%u], channel: [%s], pay channel: [%s]", LOG_TAG, m_appid,  m_version_info.m_channel.c_str(), m_pay_channel.c_str());

	uuLOG("%s initAsync, async query channel", LOG_TAG);
	// 获取渠道配置信息
	QueryChannelConfigRequest request;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::initAsyncCallback);
}

void AoneSDK::deleteAccountAsync(int type, AONESDK_CB cb)
{
	if (!m_init_ok) {
		cb(-3, map<string, string>());
		logException(-3, "deleteAccountAsync, init not ok");
		return;
	}
    uuLOG("%s deleteAccountAsync, begin delete role", LOG_TAG);
    m_cb = cb;
    AccountDelayWriteOffRequest request;
    request.body().m_user_id = m_user_id;
	request.body().m_token = m_token;
	request.body().m_channel = m_version_info.m_channel;
	request.body().m_device_id = m_device_info.m_device_id;
	request.body().m_ip = m_device_info.m_ip;
	request.body().m_type = type;

	uuLOG("%s deleteAccountAsync, user_id: %d, channel: %s, type: %d", LOG_TAG, m_user_id, m_version_info.m_channel.c_str(), type);

	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::deleteAccountAsyncCallback);
};

void AoneSDK::deleteAccountAsyncCallback(int ret, byte *responseBytes, int len)
{
	uuLOG("%s deleteAccountAsyncCallback, ret: [%d]", LOG_TAG, ret);
	if (ret < 0)
	{
		uuLOG("%s deleteAccountAsyncCallback fail, apiserver send_recv error or timeout", LOG_TAG);
		m_cb(ret, map<string, string>());
		logException(ret, "deleteAccountAsyncCallback fail, apiserver send_recv error or timeout");
		return;
	}
	AccountDelayWriteOffResponse *response = (AccountDelayWriteOffResponse*)s_apiParser->get_message(responseBytes, len);
	if (response == NULL)
	{
		m_cb(-8, map<string, string>());
		uuLOG("%s deleteAccountAsyncCallback fail, response parse fail", LOG_TAG);
		logException(-8, "deleteAccountAsyncCallback fail, response parse fail");
		return;
	}
	if (response->get_result() != 0)
	{
		ret = response->get_result();
		uuLOG("%s deleteAccountAsyncCallback fail, apiserver return code is %d", LOG_TAG, ret);
		delete response;
		m_cb(ret, map<string, string>());
		logException(ret, "deleteAccountAsyncCallback fail, init fail, apiserver return code");
		return;
	}

	uuLOG("%s deleteAccountAsyncCallback, success", LOG_TAG);
	map<string, string> retMap;
	retMap["write_off_time"] = response->body().m_write_off_time;

	delete response;
	m_cb(0, retMap);
}

void AoneSDK::initAsyncCallback(int ret, byte *responseBytes, int len)
{
	uuLOG("%s initAsyncCallback, ret: [%d]", LOG_TAG, ret);
	if (ret < 0)
	{
		uuLOG("%s initAsyncCallback fail, apiserver send_recv error or timeout", LOG_TAG);
		m_cb(ret, map<string, string>());
        logException(ret, "initAsyncCallback fail, apiserver send_recv error or timeout");
		return;
	}

	QueryChannelConfigResponse *response = (QueryChannelConfigResponse*)s_apiParser->get_message(responseBytes, len);
	if (response == NULL)
	{
		m_cb(-8, map<string, string>());
		uuLOG("%s initAsyncCallback fail, response parse fail", LOG_TAG);
		logException(-8, "initAsyncCallback fail, response parse fail");
		return;
	}
	if (response->get_result() != 0)
	{
		ret = response->get_result();
		uuLOG("%s initAsyncCallback fail, apiserver return code is %d", LOG_TAG, ret);
		delete response;
		m_cb(ret, map<string, string>());
        logException(ret, "initAsyncCallback fail, init fail, apiserver return code");
		return;
	}
	
	uuLOG("%s initAsyncCallback, channel param count: [%u], product count :[%u]", LOG_TAG, response->body().m_channel_params.size(), response->body().m_channel_products.size());

	// 入口地址
	m_entryserver_ip = response->body().m_entryserver_ip;
	m_entryserver_port = response->body().m_entryserver_port;
	uuLOG("%s initAsyncCallback, entryserver_ip: [%s], entryserver_port: [%d]", LOG_TAG, m_entryserver_ip.c_str(), m_entryserver_port);

	// 渠道参数
	m_channel_params.clear();
	for (int i = 0; i < (int)response->body().m_channel_params.size(); i++)
	{
		m_channel_params[response->body().m_channel_params[i].m_key] = response->body().m_channel_params[i].m_value;

		if (response->body().m_channel_params[i].m_key == "selectedPayChannel")
        {
			if (response->body().m_channel_params[i].m_value != "") {
				m_pay_channel = response->body().m_channel_params[i].m_value;
			}
		}
		if (response->body().m_channel_params[i].m_key == "PrintSDKLog") {
			int isClose = 0;
			if (response->body().m_channel_params[i].m_value == "0") {
				isClose = 1;
			}
			// for ios, 1 means print, 0 means close.
			//cplog_close_nslog(isClose);
		}
	}
	uuLOG("%s initAsyncCallback, pay channel: [%s]", LOG_TAG, m_pay_channel.c_str());


	// 渠道商品
	m_channel_products.clear();
	for (int i = 0; i < (int)response->body().m_channel_products.size(); i++)
	{
		PayProductInfo& info = response->body().m_channel_products[i];
		GameProduct& product = m_channel_products[info.m_id];
		product.ID = info.m_id;
		product.Name = info.m_name;
		product.Describe = info.m_describe;
		product.Currency = info.m_currency;
		product.Amount = info.m_amount;
		product.Amount_usd = info.m_amount_usd;
		product.Coin = info.m_coin;
		product.Type = info.m_type;
		product.Discount = info.m_discount;
		char buff[100];
		sprintf(buff, "Item%d", i + 1);
		m_channel_params[buff] = info.m_id;
	}

	if (response->body().m_channel_products_ext.size() != response->body().m_channel_products.size() && response->body().m_channel_products_ext.size() != 0) 
	{
        uuLOG("%s initAsyncCallback, product count %u not equal cpProduct count %u ", LOG_TAG, response->body().m_channel_products.size(), response->body().m_channel_products_ext.size());
	}
	m_productIdMap.clear();
	for (int i = 0; i < (int)response->body().m_channel_products_ext.size(); i++)
	{
		PayProductExtInfo& extInfo = response->body().m_channel_products_ext[i];
		if (m_channel_products.find(extInfo.m_id) == m_channel_products.end())
		{
			uuLOG("%s initAsyncCallback, product %s in ext info not find in proudcts", LOG_TAG, extInfo.m_id.c_str());
		}

		std::string productId = extInfo.m_cp_product_id;
		if (productId.empty())
		{
			productId = extInfo.m_id;
			uuLOG("%s initAsyncCallback, fix productId: [%s]", LOG_TAG, productId.c_str());
		}
		m_productIdMap[productId] = extInfo.m_id;
	}

	char buff[20];
	sprintf(buff, "%u", m_channel_products.size());
	m_channel_params["ItemCount"] = buff;

	// bbs论坛地址
	m_bbs_url = response->body().m_bbs_url;

	// 语言列表
	m_app_langs.clear();
	for (int i = 0; i < (int)response->body().m_app_langs.size(); i++)
	{
		uuLOG("%s initAsyncCallback, app langs key: [%s], value: [%s]", LOG_TAG, response->body().m_app_langs[i].m_key.c_str(),response->body().m_app_langs[i].m_value.c_str());
		m_app_langs[response->body().m_app_langs[i].m_key] = response->body().m_app_langs[i].m_value;
	}

	// 确定一个合适的初始语言
	if (m_app_langs.count("zhcn") > 0)
		m_version_info.m_app_lang = "zhcn";
	else if (m_app_langs.count("zh") > 0)
		m_version_info.m_app_lang = "zh";
	else if (m_app_langs.count("en") > 0)
		m_version_info.m_app_lang = "en";
	else if (m_app_langs.count("enus") > 0)
		m_version_info.m_app_lang = "enus";
	else if (m_app_langs.size() > 0)
		m_version_info.m_app_lang = m_app_langs.begin()->first;
	else
		m_version_info.m_app_lang = "zhcn";

	delete response;
	m_init_ok = true;

	m_storage.checkRecords(this);

	if (getChannelIsTest())
	{
		uuLOG("%s initAsyncCallback, channel is test", LOG_TAG);
		this -> initCallback(uuESdkResultType::eSDK_RESULT_SUCCESS, std::map<string, string>());
	} else
	{
		uuLOG("%s initAsyncCallback, init get default sdk", LOG_TAG);
		// 加载SDK
		if(!isAppOK("-10")){
			return;
		}
		uuSdkConfigHelper::init();
		uuSdkConfigHelper::setInitParams(m_channel_params);
		uuSdkConfigHelper::setContentParams(m_channel_params);

		m_shareSdk = uuSdkConfigHelper::getDefalutShareSdkName();
		//m_analyticsSdk = uuSdkConfigHelper::getDefalutAnalyticsSdkName();
		m_analyticsSdks = uuSdkConfigHelper::getAllAnalyticsNames();
		
		//首先从发行平台上拉取广告SDK的名称，如果没有则获取uusdkConfig.json中默认的SDK名称
		string adSdkNames = getChannelParamFromKey("AdSdks");
		//判断发行平台是否配置广告SDK
		uuLOG("%s initAsyncCallback, platform adSdkNames:%s",LOG_TAG, adSdkNames.c_str());
		if("" == adSdkNames){//如果发行平台上没有配置，则从uusdkConfing.json文件中读取默认的SDK
			m_adsSdk = uuSdkConfigHelper::getDefalutAdsSdkName();
			uuLOG("%s initAsyncCallback, default adSdkNames:%s",LOG_TAG, m_adsSdk.c_str());
		}else{
			m_platform_adsSdks = split(adSdkNames,"&");//数据中心拉取的所有广告SDK
			m_adsSdk = m_platform_adsSdks[0];
		}
		m_default_adsSdk = m_adsSdk;
		m_config_adsSdks = uuSdkConfigHelper::getAllSdkNamesByType("ads");//uusdkConfig.json中获取的所有广告SDK
		for (auto iter=m_config_adsSdks.begin();iter!=m_config_adsSdks.end();iter++)
		{
			uuLOG("%s initAsyncCallback, adSdkNames:%s",LOG_TAG, (*iter).c_str());
		}

		m_pushSdk = uuSdkConfigHelper::getDefalutPushSdkName();

		uuSdkManager::getInstance()->initAllSdks(this, (uuSdkResultHandler)&AoneSDK::initCallback,(uuSdkResultHandler)&AoneSDK::initPayCallback);
	}

	uuLOG("%s initAsyncCallback, end", LOG_TAG);
}

int AoneSDK::checkVersion(uint8 majorVersion, uint8 minorVersion, uint8 revisionVersion)
{
	if (!m_init_ok) return -3; // 未初始化
	if (m_entryserver_ip.empty() || m_entryserver_port == 0) return -3; // 未正确初始化

	m_version_info.m_major_version = majorVersion;
	m_version_info.m_minor_version = minorVersion;
	m_version_info.m_revision_version = revisionVersion;
    
    // 设置版本信息，第三方插件调用
    setAppVersionFromCallFunc(majorVersion, minorVersion, revisionVersion);

    CheckVerRequest request;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id);
    CheckVerResponse* response = NULL;
	int ret = entryserver_send_recv(&request, (Aone2BaseMessage**)&response);
	if (ret < 0)
	{
		uuLOG("%s check version fail, entryserver send_recv error or timeout", LOG_TAG);
		return ret;
	}

	// 判断返回码
	if (response->get_result() != 0)
	{
		uuLOG("%s check version fail, entryserver return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		return code;
	}

	uuLOG("%s check version ok, request success !", LOG_TAG);

	m_game_version.MajorVersion = response->body().m_major_version;
	m_game_version.MinorVersion = response->body().m_minor_version;
	m_game_version.RevisionVersion = response->body().m_revision_version;
	m_game_version.Notice = response->body().m_notice;
	m_game_version.DownloadURL = response->body().m_download_url;
	m_game_version.UpdateURL = response->body().m_update_url;
	m_game_version.Status = (GameVersion::GameVersionStatus)(int)response->body().m_status;
	delete response;

	// 检测之后会要求客户端去更新版本，SDK内部版本号自动更新到最新
	m_version_info.m_major_version = m_game_version.MajorVersion;
	m_version_info.m_minor_version = m_game_version.MinorVersion;
	m_version_info.m_revision_version = m_game_version.RevisionVersion;

	return 0;
}

void AoneSDK::setAppVersionFromCallFunc(uint8 majorVersion, uint8 minorVersion, uint8 revisionVersion)
{
    // 创建版本信息传递给sdk
    std::map<string, string> info;
    char buf[128] = {0};
    sprintf(buf, "%d",majorVersion);
    info["majorVersion"] = buf;
    
    char buf1[128] = {0};
    sprintf(buf1, "%d",minorVersion);
    info["minorVersion"] = buf1;
    
    char buf2[128] = {0};
    sprintf(buf2, "%d",revisionVersion);
    info["revisionVersion"] = buf2;
   
    std::vector<PluginParam*> allParams;
    PluginParam* pluginParam = new PluginParam(info);
    allParams.push_back(pluginParam);
    uuSdkManager::getInstance()->callFuncWithParam("setAppVersion", allParams);
    delete pluginParam;
    pluginParam = NULL;
}
    
void AoneSDK::checkVersionAsync(uint8 majorVersion, uint8 minorVersion, uint8 revisionVersion, AONESDK_CB cb)
{
	if (!m_init_ok) {
		cb(-3, map<string, string>());
        logException(-3, "checkVersionAsync, init not ok");
		return;
	}
	if (m_entryserver_ip.empty() || m_entryserver_port == 0) {
		cb(-3, map<string, string>());
        logException(-3, "checkVersionAsync, m_entryserver_is is empty of m_entryserver_port == 0");
		return;
	}
	
	m_cb = cb;
	uuLOG("param majorVersion is %d ", majorVersion);
	uuLOG("param minorVersion iis %d ", minorVersion);
	uuLOG("param revisionVersion is %d ", revisionVersion);

	m_version_info.m_major_version = majorVersion;
	m_version_info.m_minor_version = minorVersion;
	m_version_info.m_revision_version = revisionVersion;
    // 设置版本信息，第三方插件调用
    setAppVersionFromCallFunc(majorVersion, minorVersion, revisionVersion);

    CheckVerRequest request;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id);
	/*CheckVerResponse* response = NULL;
	int ret = entryserver_send_recv(&request, (Aone2BaseMessage**)&response);*/
	entryserver_send_recv_async(&request, (NET_CB)&AoneSDK::checkVersionCallback);

}

void AoneSDK::checkVersionCallback(int ret, byte *responseBytes, int len)
{
	if (ret < 0)
	{
		uuLOG("%s checkVersionCallback, check version fail, entryserver send_recv error or timeout", LOG_TAG);
		m_cb(ret, map<string, string>());
        logException(ret, "checkVersionCallback, check version fail, entryserver send_recv error or timeout");
		return;
	}
    CheckVerResponse* response = (CheckVerResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	// 判断返回码
	if (response->get_result() != 0)
	{
		uuLOG("%s checkVersionCallback, check version fail, entryserver return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		m_cb(code, map<string, string>());
        logException(code, "checkVersionCallback, check version fail, entryserver return code");
		return;
	}
	if(!isAppOK("-11")){
		return;
	}
	uuLOG("%s check version ok, request success !!!", LOG_TAG);

	m_game_version.MajorVersion = response->body().m_major_version;
	m_game_version.MinorVersion = response->body().m_minor_version;
	m_game_version.RevisionVersion = response->body().m_revision_version;
	m_game_version.Notice = response->body().m_notice;
	m_game_version.DownloadURL = response->body().m_download_url;
	m_game_version.UpdateURL = response->body().m_update_url;
    m_game_version.BigPatcherURL = response -> body().m_big_patcher_url;
    uuLOG("BigPatcherURL is %s ", m_game_version.BigPatcherURL.c_str());
	uuLOG("UpdateURL is %s ", m_game_version.UpdateURL.c_str());
	uuLOG("DownloadURL is %s ", m_game_version.DownloadURL.c_str());
	m_game_version.Status = (GameVersion::GameVersionStatus)(int)response->body().m_status;
	delete response;

	// 检测之后会要求客户端去更新版本，SDK内部版本号自动更新到最新
	m_version_info.m_major_version = m_game_version.MajorVersion;
	m_version_info.m_minor_version = m_game_version.MinorVersion;
	m_version_info.m_revision_version = m_game_version.RevisionVersion;
    
	httpRequest(m_game_version.DownloadURL, m_cb);
	
	//return 0;
}

void AoneSDK::checkVersionPlatform()
{
	if (AonesdkConfig::getInstance()->getAppVersion(m_version_info.m_major_version, m_version_info.m_minor_version, m_version_info.m_revision_version) == false)
	{
		uuDevice::getAppVersion(m_version_info.m_major_version, m_version_info.m_minor_version, m_version_info.m_revision_version);
	}
	message::AoneSdkCheckVerRequest req;
	req.body().m_device_info = m_device_info;
	req.body().m_version_info = m_version_info;
	apiserver_send_recv_async(&req, (NET_CB)&AoneSDK::checkVersionPlatformCallback);
}

void AoneSDK::checkVersionPlatformCallback(int ret, byte *responseBytes, int len)
{
	if (ret < 0)
	{
		uuLOG("%s checkVersionPlatformCallback, check version cp fail, api server send_recv error or timeout", LOG_TAG);
		afterCheckVersion();
		logException(ret, "checkVersionPlatformCallback, check version cp fail, api server send_recv error or timeout");
		return;
	}

	message::AoneSdkCheckVerResponse *response = (message::AoneSdkCheckVerResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;

	if (response->get_result() != 0)
	{
		int code = response->get_result();
		uuLOG("%s checkVersionPlatformCallback, check version cp fail, api server return code is %d", LOG_TAG, code);
		delete response;
		m_LogIncb(code, map<string, string>());
		logException(code, "checkVersionPlatformCallback, check version cp fail fail, api server return code");
		return;
	}

	m_versionCheckServerTime = response->body().m_server_time;
	if (response->body().m_cur_version_info.size() > 0)
	{
		m_currentVersionInfo = response->body().m_cur_version_info[0];
	}

	if (response->body().m_new_version_info.size() == 0)
	{
		uuLOG("%s no platform version found", LOG_TAG);
		checkAnnouncePlatform();
	} else
	{
		m_newVersionInfo = response->body().m_new_version_info[0];
		checkUpdatePlatform();
	}
}

bool _isVersionGreater(uint8 major1, uint8 minor1, uint8 revision1, uint8 major2, uint8 minor2, uint8 revision2) 
{
	if (major1 > major2) return true;
	if (major1 == major2)
	{
		if (minor1 > minor2) return true;
		if (minor1 == minor2) return revision1 > revision2;
	}

	return false;
}

static AoneSDK* aoneSdkInst;
void AfterUpdateDialog() {
	aoneSdkInst->checkAnnouncePlatform();
}

void AfterAnnounceView() {
	aoneSdkInst->afterCheckVersion();
}

void AoneSDK::checkUpdatePlatform()
{
	if (_isVersionGreater(m_newVersionInfo.m_major_version, m_newVersionInfo.m_minor_version, m_newVersionInfo.m_revision_version,
		m_version_info.m_major_version, m_version_info.m_minor_version, m_version_info.m_revision_version) && m_newVersionInfo.m_update_option != 0)
	{
		uuLOG("%s there are greater force/option version to upgrade", LOG_TAG);
		aoneSdkInst = this;
		uuDevice::showUpdateDialog(parse_for_lang(m_newVersionInfo.m_update_addr), parse_for_lang(m_newVersionInfo.m_version_desc), (UPDATE_TYPE)m_newVersionInfo.m_update_option, AfterUpdateDialog);
	}
	else
	{
		uuLOG("%s current version is ok, check announcement", LOG_TAG);
		checkAnnouncePlatform();
	}
}

void AoneSDK::checkAnnouncePlatform()
{
	if (ShouldAnnounceAppear())
	{
		aoneSdkInst = this;
		uuDevice::showAnnounceDialog(parse_for_lang(m_currentVersionInfo.m_announce_addr), AfterAnnounceView);
	}
	else
	{
		uuLOG("%s no need to show announcement", LOG_TAG);
		afterCheckVersion();
	}
}

#ifdef WIN32
extern "C" char* strptime(const char* s,
	const char* f,
struct tm* tm) {
	// Isn't the C++ standard lib nice? std::get_time is defined such that its
	// format parameters are the exact same as strptime. Of course, we have to
	// create a string stream first, and imbue it with the current C locale, and
	// we also have to make sure we return the right things if it fails, or
	// if it succeeds, but this is still far simpler an implementation than any
	// of the versions in any of the C standard libraries.
	std::istringstream input(s);
	input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
	input >> std::get_time(tm, f);
	if (input.fail()) {
		return nullptr;
	}
	return (char*)(s + input.tellg());
}
#endif

static time_t _getTimeFromStr(std::string& str, const char* format) {
	std::tm tm;
	if (strptime(str.c_str(),format, &tm) == NULL)
	{
		uuLOG("%s parse time {%s} fail", LOG_TAG, str.c_str());
		return 0;
	}

	return mktime(&tm);
}

bool AoneSDK::ShouldAnnounceAppear()
{
	if (m_currentVersionInfo.m_is_show_announce == false)
	{
		uuLOG("%s server told not to show announce", LOG_TAG);
		return false;
	}	

	time_t serverTime = _getTimeFromStr(m_versionCheckServerTime, "%Y-%m-%d %H:%M:%S");
	if (serverTime == 0)
	{
		uuLOG("%s parse server time {%s} fail", LOG_TAG, m_versionCheckServerTime.c_str());
		return false;
	}

	if (m_currentVersionInfo.m_announce_begin != m_currentVersionInfo.m_announce_end)
	{
		time_t t1 = _getTimeFromStr(m_currentVersionInfo.m_announce_begin, "%Y/%m/%d %H:%M:%S");
		time_t t2 = _getTimeFromStr(m_currentVersionInfo.m_announce_end, "%Y/%m/%d %H:%M:%S");
		if (t1 == 0 || t2 == 0)
		{
			uuLOG("%s parse announce begin %s and end time %s fail", LOG_TAG, m_currentVersionInfo.m_announce_begin.c_str(), m_currentVersionInfo.m_announce_end.c_str());
			return false;
		}

		if (serverTime < t1 || serverTime > t2)
		{
			uuLOG("%s server time {%s} not in begain {%s} and end {%s}", LOG_TAG, m_versionCheckServerTime.c_str(), m_currentVersionInfo.m_announce_begin.c_str(), m_currentVersionInfo.m_announce_end.c_str());
			return false;
		}
	}

	std::string lastAnnounceDatePath = cpfile_write_path();
	lastAnnounceDatePath = lastAnnounceDatePath + "/lastAnnounceDate";
	if (m_currentVersionInfo.m_announce_rate == 0)
	{
		if (cpfile_exist(lastAnnounceDatePath.c_str()))
		{
			std::ifstream inputFile(lastAnnounceDatePath);
			if (inputFile.is_open())
			{
				std::tm tmLast;
				std::string strLast;
				std::getline(inputFile, strLast);
				if (inputFile.fail())
				{
					uuLOG("%s read from file fail", LOG_TAG);
					cpfile_remove(lastAnnounceDatePath.c_str());
					return false;
				}
				inputFile.close();

				time_t lastTime = _getTimeFromStr(strLast, "%Y-%m-%d %H:%M:%S");
				if (lastTime == 0)
				{
					uuLOG("%s parse lastAnnounceDate {%s} fail", LOG_TAG, strLast.c_str());
					cpfile_remove(lastAnnounceDatePath.c_str());
					return false;
				}

				time_t wholeDay = 60 * 60 * 24;
				if ((serverTime - lastTime) < wholeDay)
				{
                    uuLOG("%s current time {%ld} distant last announce time {%ld} smaller than whole day", LOG_TAG, serverTime, lastTime);
					return false;
				}
			}
			else
			{
				uuLOG("%s lastDateFile {%s} open fail", LOG_TAG, lastAnnounceDatePath.c_str());
			}
		}
	}
	
	std::ofstream outputFile;
	outputFile.open(lastAnnounceDatePath.c_str(), std::ofstream::out | std::ofstream::trunc);
	if (outputFile.fail())
	{
		uuLOG("%s write lastDateFile {%s} fail", LOG_TAG, lastAnnounceDatePath.c_str());
	}
	outputFile << m_versionCheckServerTime;
	outputFile.close();
	return true;
}

void AoneSDK::afterCheckVersion()
{
	if (m_cb)
	{
		m_cb(m_initResultType, m_initResultParams);
	}
}

bool AoneSDK::isNeedPatch(string packageResDir, string patchResDir)
{
	return true;
}

bool AoneSDK::isNeedForceUpdate(string packageResDir, string patchResDir)
{
	return true;
}

int AoneSDK::quickRegister()
{
	if (!m_init_ok)
		return -3;
	//get account and password 
	AoneSDK::AoneUserInfo aoneUserInfo = getUserInfo();
	
	string account = aoneUserInfo.account;
	string password = aoneUserInfo.password;
	//if (account.length() >0){
		//aoneLogin(account, password);
		//return 0;
	//}
	QuickRegisterRequest request;
	request.body().m_device_info   = m_device_info;
	request.body().m_version_info  = m_version_info;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);

	QuickRegisterResponse *response = NULL;
	int ret = apiserver_send_recv(&request, (Aone2BaseMessage**)&response);
	if (ret < 0)
	{
		uuLOG("%s quick register fail, apiserver send_recv error or timeout", LOG_TAG);
		return ret;
	}

	if (response->get_result() != 0)
	{
		int code = response->get_result();
		uuLOG("%s quick register fail, apiserver return code is %d", LOG_TAG, code);
		delete response;
		return code;
	}

	uuLOG("%s quick register ok, request seccess!", LOG_TAG);

	m_user_id      = response->body().m_user_id;
	m_user_account = response->body().m_user_account;
	m_aone_account = response->body().m_aone_account;
	m_aone_password = response->body().m_aone_password;
	m_token = response->body().m_token;
	delete response;

	m_eventLoginType = EVENT_LOGIN_TYPE_QUICK_REGISTER;

	// 保存图片
	if (!m_aone_password.empty())
	{
		if (isJpSpecial()){
			char buf[128] = { 0 };
			sprintf(buf, "%d", m_user_id);
			saveUserInfo(m_aone_account, m_aone_password, "jp_special", buf, "", "","");
			
		}
		else{
			saveQuickRegisterPic();
		}
	}

	return onAoneLoginEnd();
}

void AoneSDK::quickRegisterAsync(AONESDK_CB cb)
{
	if (!m_init_ok)
	{
		cb(-3, map<string, string>());
        logException(-3,"quickRegisterAsync, init not ok");
		return;
	}
	//get account and password
	string jp_special = getChannelParamFromKey("JP_Special");
	AoneSDK::AoneUserInfo aoneUserInfo = getUserInfo();
	
	string account = aoneUserInfo.account;
	string password = aoneUserInfo.password;
	string thirdAccount = aoneUserInfo.thirdAccount;
	string oauthType = aoneUserInfo.oauthType;
	if (thirdAccount.length() >0 && oauthType.length() >0){
		uuLOG("this is aone login after bind other account");
		if (oauthType == "google" || oauthType == "facebook"){
			//facebook 或者谷歌的登录可能会Token是有有效期的过期的话将无法进行aone登录
			//还需要关注的问题是如果是用户换了设备中登录的谷歌账号，需要进行处理
			//(1.进行aone进行登录的时候正常情况下应该会报1006 2.进行aone登录的时候报1006)
			//.之后需要判断userId是否一致进行判断是不是使用和之前不一样的第三方账号
			setOAuthType(oauthType);
			thirdPartyLogin(cb);
			return;
		}
		setOAuthType(oauthType);
		aoneLoginAsync(thirdAccount, "", cb);
		return;
	}
	if (account.length() >0 && password.length() >0){
		m_is_quickregister = true;
		m_oauth_type = jp_special;//m_verison_info.m_ext = "jp_special"时，只要换设备用账号密码登录，密码就会改变
		aoneLoginAsync(account, password,cb);
		return;
	}
	m_LogIncb = cb;
	//string jp_special = getChannelParamFromKey("JP_Special");
	
	QuickRegisterRequest request;
	request.body().m_device_info = m_device_info;
	if ("" != jp_special){
		string m_jp_prefix = "jp_special=";
		m_version_info.m_ext = m_jp_prefix.append(jp_special);//value is jp_special
	}
	request.body().m_version_info = m_version_info;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	uuLOG("(quickRegisterAsync)m_version_info.m_ext:%s", m_version_info.m_ext.c_str());
	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::quickRegisterCallback);
}

void AoneSDK::quickRegisterCallback(int ret, byte *responseBytes, int len)
{
	if (ret < 0)
	{
		uuLOG("%s quickRegisterCallback, quice register fail, api server send_recv error or timeout", LOG_TAG);
		m_LogIncb(ret, map<string, string>());
        logException(ret, "quickRegisterCallback, quice register fail, api server send_recv error or timeout");
		return;
	}

	QuickRegisterResponse *response = (QuickRegisterResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;

	if (response->get_result() != 0)
	{
		int code = response->get_result();
		uuLOG("%s quickRegisterCallback, quick register fail, api server return code is %d", LOG_TAG, code);
		delete response;
		m_LogIncb(code, map<string, string>());
        logException(code, "quickRegisterCallback, quick register fail, api server return code");
		return;
	}

	uuLOG("%s quick register ok, request success!", LOG_TAG);

	m_user_id = response->body().m_user_id;
	m_user_account = response->body().m_user_account;
	m_aone_account = response->body().m_aone_account;
	m_aone_password = response->body().m_aone_password;
	m_token = response->body().m_token;
	delete response;

	m_eventLoginType = EVENT_LOGIN_TYPE_QUICK_REGISTER;
	
	// 保存图片
	if (!m_aone_password.empty())
	{
        if (AonesdkConfig::getInstance()->isSavePicture()) {
			if (isJpSpecial()){
				char buf[128] = { 0 };
				sprintf(buf, "%d", m_user_id);

				saveUserInfo(m_aone_account, m_aone_password, "jp_special", buf, "", "","");
			}
			else{
				saveQuickRegisterPic();
			}
        }
	}

	//之前的想法是没有绑定过的第三方账号，先进行第三方登录，
	//如果aone的userId 是0的话，就让用户快速注册一个账号，然后用这个快速注册的账号和当前登录的第三方账号绑定到一起
	//if (isJpSpecial() && m_thirdparty_account != ""){
	//	aoneQueryBindOtherAccount(m_LogIncb);
	//	return;
	//}
	m_loginResult = eSDK_RESULT_SUCCESS;
	onAoneLoginEndAsync();
}

void AoneSDK::saveQuickRegisterPic()
{
    const string currentLang = AoneSDK::getAppLang();
     
	uuDevice::saveQuickRegisterPic(m_aone_account, m_aone_password,currentLang);
}

void AoneSDK::setAppVersionInfo(int major_version, int minor_version ,int revision_version )
{
	
	m_version_info.m_major_version=(uint8)major_version;
    m_version_info.m_minor_version=(uint8)minor_version;
    m_version_info.m_revision_version=(uint8)revision_version;
	uuLOG("m_version_info.major_version:%d",m_version_info.m_major_version);
	uuLOG("m_version_info.minor_version:%d",m_version_info.m_minor_version);
	uuLOG("m_version_info.m_revision_version:%d",m_version_info.m_revision_version);
}
    
int AoneSDK::quickLogin()
{
    uuLOG("%s quickLogin, begin use quick login", LOG_TAG);
	if (!m_init_ok) return -3; // 未初始化

	QuickLoginRequest request;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	QuickLoginResponse* response = NULL;
	int ret = apiserver_send_recv(&request, (Aone2BaseMessage**)&response);
	if (ret < 0)
	{
		uuLOG("%s quickLogin, quick login fail, apiserver send_recv error or timeout, ret is %d", LOG_TAG, ret);
		return ret;
	}

	// 判断返回码
	if (response->get_result() != 0)
	{
		uuLOG("%s quickLogin, quick login fail, apiserver return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		return code;
	}

	uuLOG("%s quickLogin, quick login ok, request success !!!", LOG_TAG);

	m_user_id = response->body().m_user_id;
	m_user_account = response->body().m_user_account;
	m_aone_account = response->body().m_aone_account;
	m_token = response->body().m_token;
	delete response;

	m_eventLoginType = EVENT_LOGIN_TYPE_QUICK;
	return onAoneLoginEnd();
}

void AoneSDK::quickLoginAsync(AONESDK_CB cb)
{
	if (!m_init_ok) {
		cb(-3, map<string, string>());
        logException(-3, "quickLoginAsync, init not ok");
		return;
	}

	m_LogIncb = cb;
	
	QuickLoginRequest request;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	/*QuickLoginResponse* response = NULL;
	int ret = apiserver_send_recv(&request, (Aone2BaseMessage**)&response);*/
	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::quickLoginCallback);
}

void AoneSDK::quickLoginCallback(int ret, byte *responseBytes, int len)
{
	if (ret < 0)
	{
		uuLOG("%s quickLoginCallback, quick login fail, api server send_recv error or timeout", LOG_TAG);
		m_LogIncb(ret, map<string, string>());
        logException(ret, "quickLoginCallback, quick login fail, api server send_recv error or timeout");
		return;
	}
	QuickLoginResponse* response = (QuickLoginResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	if (response == NULL)
	{
		m_LogIncb(-8, map<string, string>());
		logException(-8, "quickLoginCallback, response parse fail");
		return;
	}
	// 判断返回码
	if (response->get_result() != 0)
	{
		uuLOG("%s quickLoginCallback, quick login fail, api server return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		m_LogIncb(code, map<string, string>());
        logException(code, "quickLoginCallback, quick login fail, api server return code");
		return;
	}

	uuLOG("%s quick login ok, request success !!!", LOG_TAG);

	m_user_id = response->body().m_user_id;
	m_user_account = response->body().m_user_account;
	m_aone_account = response->body().m_aone_account;
	m_token = response->body().m_token;
	delete response;

    uuLOG("%s hhh --- user_id = %d", LOG_TAG, m_user_id);
    uuLOG("%s hhh --- user_account = %s", LOG_TAG, m_user_account.c_str());
    uuLOG("%s hhh --- aone_account = %s", LOG_TAG, m_aone_account.c_str());
    uuLOG("%s hhh --- token = %s", LOG_TAG, m_token.c_str());
	m_eventLoginType = EVENT_LOGIN_TYPE_QUICK;
	m_loginResult = eSDK_RESULT_SUCCESS;
	onAoneLoginEndAsync();
}

int AoneSDK::aoneRegister(const string& account, const string& password)
{
	if (!m_init_ok) return -3; // 未初始化
    
	AoneRegisterRequest request;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	request.body().m_account = account;
	request.body().m_password = password;
	AoneRegisterResponse* response = NULL;
	int ret = apiserver_send_recv(&request, (Aone2BaseMessage**)&response);
	if (ret < 0)
	{
		uuLOG("%s aone register fail, apiserver send_recv error or timeout", LOG_TAG);
		return ret;
	}

	// 判断返回码
	if (response->get_result() != 0)
	{
		uuLOG("%s aone register fail, apiserver return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		return code;
	}

	uuLOG("%s aone register ok, request success !!!", LOG_TAG);

	m_user_id = response->body().m_user_id;
	m_user_account = response->body().m_user_account;
	m_token = response->body().m_token;
	delete response;

	m_eventLoginType = EVENT_LOGIN_TYPE_NORMAL;
	return onAoneLoginEnd();
}

void AoneSDK::aoneRegisterAsync(const string& account, const string& password, AONESDK_CB cb)
{
	if (!m_init_ok) {
		cb(-3, map<string, string>());
        logException(-3, "aoneRegisterAsync, init not ok");
		return;
	}

	m_LogIncb = cb;
    
	AoneRegisterRequest request;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	request.body().m_account = account;
	request.body().m_password = password;
	/*AoneRegisterResponse* response = NULL;
	int ret = apiserver_send_recv(&request, (Aone2BaseMessage**)&response);*/
	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::aoneRegisterCallback);
}

void AoneSDK::aoneRegisterCallback(int ret, byte *responseBytes, int len)
{
	if (ret < 0)
	{
		uuLOG("%s aoneRegisterCallback, aone register fail, api server send_recv error or timeout", LOG_TAG);
		m_LogIncb(ret, map<string, string>());
        logException(ret, "aoneRegisterCallback, aone register fail, api server send_recv error or timeout");
		return;
	}
	AoneRegisterResponse* response = (AoneRegisterResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	// 判断返回码
	if (response->get_result() != 0)
	{
		uuLOG("%s aoneRegisterCallback, aone register fail, apiserver return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		m_LogIncb(code, map<string, string>());
        logException(code, "aoneRegisterCallback, aone register fail, apiserver return code");
		return;
	}

	uuLOG("%s aoneRegisterCallback, aone register ok, request success !!!", LOG_TAG);

	m_user_id = response->body().m_user_id;
	m_user_account = response->body().m_user_account;
	m_token = response->body().m_token;
	delete response;

	m_eventLoginType = EVENT_LOGIN_TYPE_NORMAL;
	m_loginResult = eSDK_RESULT_SUCCESS;
	onAoneLoginEndAsync();
}

int AoneSDK::aoneLogin(const string& account, const string& password)
{
	if (!m_init_ok) return -3; // 未初始化
    
	AoneLoginRequest request;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	request.body().m_account = account;
	request.body().m_password = password;
	AoneLoginResponse* response = NULL;
	int ret = apiserver_send_recv(&request, (Aone2BaseMessage**)&response);
	if (ret < 0)
	{
		uuLOG("%s aone login fail, apiserver send_recv error or timeout", LOG_TAG);
		return ret;
	}

	// 判断返回码
	if (response->get_result() != 0)
	{
		uuLOG("%s aone login fail, apiserver return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		return code;
	}

	uuLOG("%s aone login ok, request success !!!", LOG_TAG);

	m_user_id = response->body().m_user_id;
	m_user_account = response->body().m_user_account;
	m_token = response->body().m_token;
	delete response;

	m_eventLoginType = EVENT_LOGIN_TYPE_NORMAL;
	return onAoneLoginEnd();
}

void AoneSDK::aoneLoginAsync(const string& account, const string& password, AONESDK_CB cb)
{
	if (!m_init_ok) {
		cb(-3, map<string, string>());
        logException(-3, "aoneLoginAsync, init not ok");
		return;
	}
	m_aone_account_third = account;
	m_aone_password_third = password;
	if(isJpSpecial()){
		string m_jp_prefix = "jp_special=";
		m_version_info.m_ext = m_jp_prefix.append(m_oauth_type);
	}
	uuLOG("aoneLoginAsync, m_version_info.m_ext:%s",m_version_info.m_ext.c_str());
	

	m_LogIncb = cb;
	AoneLoginRequest request;
	
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	request.body().m_account = account;
	request.body().m_password = password;
	/*AoneLoginResponse* response = NULL;
	int ret = apiserver_send_recv(&request, (Aone2BaseMessage**)&response);*/
	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::aoneLoginCallback);
}

void AoneSDK::aoneLoginCallback(int ret, byte *responseBytes, int len)
{
	if (ret < 0)
	{
		uuLOG("%s aoneLoginCallback, aone register fail, api server send_recv error or timeout", LOG_TAG);
		m_LogIncb(ret, map<string, string>());
        logException(ret, "aoneLoginCallback,  register fail, api server send_recv error or timeout");
		return;
	}
	AoneLoginResponse* response = (AoneLoginResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	// 判断返回码
	if (response->get_result() != 0)
	{
		uuLOG("%s aoneLoginCallback, aone login fail, apiserver return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		if (isJpSpecial()){
			//else if (code == 1009 && m_oauth_type != "" && m_oauth_type != "jp_special"){
			//	//第三方账号之前使用过oauthLogin进行登录，再使用第三方的userId（或Token）进行aoneLogin会报1009(密码错误)

			//}
			if (code == 1025 && m_oauth_type == "jp_special" && m_is_quickregister){
				//如果是使用本地账号进行登录，说明该设备生成过引继码，并且在B设备上登录过((即:密码被修改了)之前的账号已经被继承了，需要在该设备上创建新的账号)
				m_is_quickregister = false;
				uuLOG("aoneLoginCallback, code:%d" ,code);
				deleteUserInfo();//删除本地的账号信息
				quickRegisterAsync(m_LogIncb);
				return;
			}
			//else if (code == 1000 && m_oauth_type != "" && m_oauth_type != "jp_special"){
			//	//方案一：第三方之前绑定过aone账号，使用本地保存的Token和类型进行aoneLogin报1000是因为Token已经失效了。
			//	//需要重新拉起第三方登录,本地保存的token需要根据当前的第三方返回的userId ，进行修改
			//	//方案二：每次都进行第三方授权之后再进行aone登录
			//}
		}
		m_LogIncb(code, map<string, string>());
        logException(code, "aoneLoginCallback, aone login fail, apiserver return code");
		return;
	}

	uuLOG("%s aoneLoginCallback, aone login ok, request success !!!", LOG_TAG);
	

	m_user_id = response->body().m_user_id;
	uuLOG("%s aoneLoginCallback, aone login ok, request success,m_user_id:%d", LOG_TAG, m_user_id);
	m_user_account = response->body().m_user_account;
	m_token = response->body().m_token;
	string newPassword = response->body().m_new_password;
	uuLOG("%s aoneLoginCallback, npwd:%s", LOG_TAG, newPassword.c_str());
	//此处需要区分是Aone登录还是第三方绑定后的登录，aone登录的话type传jp_special，第三方登录的传对应的oauthType
	if ("" != newPassword){
		AoneSDK::AoneUserInfo aoneUserInfo = getUserInfo();
		
		char buf[128] = { 0 };
		sprintf(buf, "%d", m_user_id);
		//deleteUserInfo();
		uuLOG("%s aoneLoginCallback, save new p aone login data", LOG_TAG);
		saveUserInfo(m_aone_account_third, newPassword, "jp_special", buf, "", "","");
		newPassword = "";
	}else if("" != m_aone_password_third){
		char buf[128] = { 0 };
		sprintf(buf, "%d", m_user_id);
		//deleteUserInfo();
		uuLOG("%s aoneLoginCallback, save aone login data", LOG_TAG);
		saveUserInfo(m_aone_account_third, m_aone_password_third, "jp_special", buf, "", "","");
		m_aone_password_third = "";
	}
	//
	if(m_is_jp_first_third_login){
		AoneSDK::AoneUserInfo aoneUserInfo = getUserInfo();
		string account = aoneUserInfo.account;//""
		string password = aoneUserInfo.password;//""
		string thirdAccount = aoneUserInfo.thirdAccount;//""
		string oauthType = aoneUserInfo.oauthType;//""
		string userId = aoneUserInfo.userId;//""
		string thirdUserId = aoneUserInfo.thirdUserId;//""
		string type = aoneUserInfo.type;//""
		m_is_jp_first_third_login = false;
		char buf[128] = { 0 };
		sprintf(buf, "%d", m_user_id);
		saveUserInfo("", "", "", buf, m_aone_account_third,m_thirdparty_user_id, m_oauth_type);
	}
	delete response;
	m_eventLoginType = EVENT_LOGIN_TYPE_NORMAL;
	m_loginResult = eSDK_RESULT_SUCCESS;
	m_is_jp_third_login = false;
	onAoneLoginEndAsync();
}

int AoneSDK::userLogin()
{
	if (!m_init_ok) return -3; // 未初始化
	if (m_entryserver_ip.empty() || m_entryserver_port == 0) return -3; // 未正确初始化
	if (m_user_id == 0 || m_token.empty()) return -3; // 未登陆API服务
	uuLOG("%s userLogin, m_user_id:%d", LOG_TAG, m_user_id);
	uuLOG("%s userLogin, m_token:%s", LOG_TAG, m_token.c_str());
	UserLoginRequest request;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id);
	request.body().m_user_id = m_user_id;
	request.body().m_token = m_token;
	UserLoginResponse* response = NULL;
	int ret = entryserver_send_recv(&request, (Aone2BaseMessage**)&response);
	if (ret < 0)
	{
		uuLOG("%s userLogin, user login fail, entryserver send_recv error or timeout", LOG_TAG);
		return ret;
	}

	uuLOG("userLogin, m_app_id:%d", m_version_info.m_app_id);
	uuLOG("userLogin, m_app_lang:%s" , m_version_info.m_app_lang.c_str());
	uuLOG("userLogin, m_channel:%s" , m_version_info.m_channel.c_str());
	uuLOG("userLogin, m_ext:%s", m_version_info.m_ext.c_str());
	uuLOG("userLogin, m_version_info:%d", m_version_info.m_major_version);
	uuLOG("userLogin, m_version_info:%d", m_version_info.m_minor_version);
	uuLOG("userLogin, m_version_info:%d", m_version_info.m_revision_version);
	

	// 判断返回码
	if (response->get_result() != 0)
	{
		uuLOG("%s userLogin, user login fail, return %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		return code;
	}

	uuLOG("%s userLogin, user login ok, request success !!!", LOG_TAG);

	// 用户token
	m_user_token = response->body().m_user_token;

	// 分区列表
	m_groups.clear();
	for (int i = 0; i < (int)response->body().m_groups.size(); i++)
	{
		GameGroupInfo& info = response->body().m_groups[i];
		GameGroup group;
		group.ID = info.m_group_id;
		group.Name = info.m_group_name;
		group.ServerIP = info.m_server_ip;
		group.ServerPort = info.m_server_port;
		group.IsInitialed = info.m_is_initialed;
		group.IsRecommend = info.m_is_recommend;
		group.Status = (GameGroup::GameGroupStatus)(int)info.m_status;
		m_groups.push_back(group);
	}

	// 角色列表
	m_roles.clear();
	for (int i = 0; i < (int)response->body().m_roles.size(); i++)
	{
		RoleShortInfo& info = response->body().m_roles[i];
		GameRole role;
		role.ID = info.m_role_id;
		role.Name = info.m_role_name;
		role.Type = info.m_role_type;
		role.Level = info.m_role_level;
		role.VIP = info.m_role_vip;
		role.CreateTime = info.m_role_createtime;
		role.LastLoginTime = info.m_last_logintime;
		role.LastGroupID = info.m_last_groupid;
		role.OwnerGroupID = info.m_owner_groupid;
		for (int j = 0; j < (int)info.m_role_features.size(); j++)
		{
			role.Features[info.m_role_features[j].m_key] = info.m_role_features[j].m_value;
		}
		m_roles.push_back(role);
	}

	delete response;

	LogEvent(m_eventLoginType.c_str(), true);
	return 0;
}

void AoneSDK::userLoginAsync(AONESDK_CB cb)
{
    uuLOG("%s userLoginAsync, begin use user login !!!", LOG_TAG);
	if (!m_init_ok) {
		m_begin_login_time = m_ineffective_time;

		uuLOG("%s userLoginAsync, init not ok", LOG_TAG);
		cb(-3, map<string, string>());
        logException(-3, "userLoginAsync, init not ok");
		return;
	}
	if (m_entryserver_ip.empty() || m_entryserver_port == 0) { // 未正确初始化
		m_begin_login_time = m_ineffective_time;

		uuLOG("%s userLoginAsync, entry server ip is null or port is null, ip:%s, port:%d", LOG_TAG, m_entryserver_ip.c_str(), m_entryserver_port);
		cb(-3, map<string, string>());
        logException(-3, "userLoginAsync, entry server ip is null or port is null");
		return;
	}
	if (m_user_id == 0 || m_token.empty()) { // 未登陆API服务
		m_begin_login_time = m_ineffective_time;

		uuLOG("%s userLoginAsync, user id is null or token is null, id:%u, token:%s", LOG_TAG, m_user_id, m_token.c_str());
		cb(-3, map<string, string>());
        logException(-3, "userLoginAsync, user id is null or token is null");
		return;
	}

	m_LogIncb = cb;

	uuLOG("%s userLoginAsync, user login", LOG_TAG);
	UserLoginRequest request;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id);
	request.body().m_user_id = m_user_id;
	request.body().m_token = m_token;
	/*UserLoginResponse* response = NULL;
	int ret = entryserver_send_recv(&request, (Aone2BaseMessage**)&response);*/
	
	entryserver_send_recv_async(&request, (NET_CB)&AoneSDK::userLoginCallback);
}

void AoneSDK::userLoginCallback(int ret, byte *responseBytes, int len)
{
	uuLOG("%s userLoginCallback, ret: [%d]", LOG_TAG, ret);
	if (ret < 0)
	{
		m_begin_login_time = m_ineffective_time;

		uuLOG("%s userLoginCallback, user login fail, entryserver send_recv error or timeout", LOG_TAG);
		m_LogIncb(ret, map<string, string>());
        logException(ret, "userLoginCallback, user login fail, entryserver send_recv error or timeout");
		return;
	}

	UserLoginResponse *response = (UserLoginResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	if (response == NULL)
	{
		m_begin_login_time = m_ineffective_time;

		uuLOG("%s userLoginCallback, response parse fail", LOG_TAG);
		m_LogIncb(-8, map<string, string>());
		logException(-8, "userLoginCallback, response parse fail");
		return;
	}
	// 判断返回码
	if (response->get_result() != 0)
	{
		m_begin_login_time = m_ineffective_time;

		uuLOG("%s userLoginCallback, user login fail, return %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		m_LogIncb(code, map<string, string>());
        logException(code, "userLoginCallback, user login fail, return code");
		return;
	}

	uuLOG("%s userLoginCallback, user login ok, request success !!!", LOG_TAG);

	// 用户token
	m_user_token = response->body().m_user_token;

	// 分区列表
	m_groups.clear();
	for (int i = 0; i < (int)response->body().m_groups.size(); i++)
	{
		GameGroupInfo& info = response->body().m_groups[i];
		GameGroup group;
		group.ID = info.m_group_id;
		group.Name = info.m_group_name;
		group.ServerIP = info.m_server_ip;
		group.ServerPort = info.m_server_port;
		group.IsInitialed = info.m_is_initialed;
		group.IsRecommend = info.m_is_recommend;
		group.Status = (GameGroup::GameGroupStatus)(int)info.m_status;
		m_groups.push_back(group);
	}

	// 角色列表
	m_roles.clear();
	for (int i = 0; i < (int)response->body().m_roles.size(); i++)
	{
		RoleShortInfo& info = response->body().m_roles[i];
		GameRole role;
		role.ID = info.m_role_id;
		role.Name = info.m_role_name;
		role.Type = info.m_role_type;
		role.Level = info.m_role_level;
		role.VIP = info.m_role_vip;
		role.CreateTime = info.m_role_createtime;
		role.LastLoginTime = info.m_last_logintime;
		role.LastGroupID = info.m_last_groupid;
		role.OwnerGroupID = info.m_owner_groupid;
		for (int j = 0; j < (int)info.m_role_features.size(); j++)
		{
			role.Features[info.m_role_features[j].m_key] = info.m_role_features[j].m_value;
		}
		m_roles.push_back(role);
	}

	delete response;
	
	LogEvent(m_eventLoginType.c_str(), true);

	m_begin_login_time = m_ineffective_time;

	m_LogIncb(m_loginResult, map<string, string>());
}

void AoneSDK::sdkLoginAsync(const string& userId, const string& token, AONESDK_CB cb)
{
    uuLOG("%s sdkLoginAsync, begin use sdk login", LOG_TAG);
	if (!m_init_ok) {
		m_begin_login_time = m_ineffective_time;

		uuLOG("%s sdkLoginAsync, login failed, init not ok", LOG_TAG);
		cb(-3, map<string, string>());
        logException(-3, "sdkLoginAsync, login failed, init not ok");
		return;
	}

	m_LogIncb = cb;

	uuLOG("%s sdkLoginAsync, token is: %s", LOG_TAG, token.c_str());
	uuLOG("%s sdkLoginAsync, userId is: %s", LOG_TAG, userId.c_str());

	SDKLoginRequest request;
	request.body().m_sdk_userid = userId;
	request.body().m_sdk_token = token;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	string sdkVersion = uuSdkManager::getInstance()->callStringFunc(mLoginSdkName, "getSDKVersion");
	uuLOG("%s sdkLoginAsync, commit sdk name: %s, sdk version: %s", LOG_TAG, mLoginSdkName.c_str(), sdkVersion.c_str());
	request.body().m_sdk_version = sdkVersion;
	
	//SDKLoginResponse* response = NULL;
	//int ret = apiserver_send_recv(&request, (Aone2BaseMessage**)&response);

	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::sdkLoginCallback);
}

void AoneSDK::sdkLoginCallback(int ret, byte *responseBytes, int len)
{
	uuLOG("%s sdkLoginCallback, ret: [%d]", LOG_TAG, ret);
	if (ret < 0)
	{
		m_begin_login_time = m_ineffective_time;

		uuLOG("%s sdkLoginCallback, channel sdk login fail, apiserver send_recv error or timeout", LOG_TAG);
		m_LogIncb(ret, map<string, string>());
        logException(ret, "channel sdk login fail, apiserver send_recv error or timeout");
		return;
	}

	SDKLoginResponse *response = (SDKLoginResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	// 判断返回码
	if (response->get_result() != 0)
	{
		m_begin_login_time = m_ineffective_time;

		uuLOG("%s sdkLoginCallback, channel sdk login fail, apiserver return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		m_LogIncb(code, map<string, string>());
        logException(code, "sdkLoginCallback, channel sdk login fail, apiserver return code");
		return;
	}
	if(!isAppOK("-14")){
		return;
	}
	uuLOG("%s sdkLoginCallback, channel sdk login ok, request success !!!", LOG_TAG);

	m_user_id = response->body().m_user_id;
	m_user_account = response->body().m_user_account;
	m_token = response->body().m_token;
	delete response;

	if (m_user_id == 0) {
		uuLOG("%s sdkLoginCallback, sdkLogin receive user id is null!", LOG_TAG);
	}
	if (m_user_account == "") {
		uuLOG("%s sdkLoginCallback, sdkLogin receive user account is null!", LOG_TAG);
	}
	if (m_token == "") {
		uuLOG("%s sdkLoginCallback, sdkLogin receive token is null!", LOG_TAG);
	}
	
	const char* eventId = uuSdkConfig::getInstance()->getContentValueByKey(m_sdkName.c_str(), "event_login");
	if (eventId != NULL) {
		m_eventLoginType = eventId;
	} else {
		m_eventLoginType = EVENT_LOGIN_TYPE_NORMAL;
	}
	onAoneLoginEndAsync();
}

void AoneSDK::OAuthLoginAsync(AONESDK_CB cb)
{
	if (!m_init_ok) {
		cb(-3, map<string, string>());
        logException(-3, "OAuthLoginAsync, init not ok");
		return;
	}

	if (m_oauth_type == "" || m_oauth_token == "") {
        uuLOG("%s OAuthLoginAsync, oauth_type = %s, oauth_token = %s", LOG_TAG, m_oauth_type.c_str(), m_oauth_token.c_str());
		cb(-11, map<string, string>());
        logException(-11, "OAuthLoginAsync, oauth login async m_oauth_type is null or m_oauth_token is null");
		return;
	}

	m_LogIncb = cb;

	uuLOG("%s OAuthLoginAsync, oauth_type = %s, oauth_token = %s", LOG_TAG, m_oauth_type.c_str(), m_oauth_token.c_str());

	OAuthLoginRequest request;
	request.body().m_oauth_type = m_oauth_type;
	request.body().m_oauth_token = m_oauth_token;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
    //德国12暂用---服务器更新之后删除
    if (m_oauth_type == "apple") {
        request.body().m_oauth_type = "gamecenter";
    }
	
	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::OAuthLoginCallback);
}

void AoneSDK::OAuthLoginCallback(int ret, byte *responseBytes, int len)
{
	if (ret < 0)
	{
		uuLOG("%s OAuthLoginCallback, oauth channel sdk login fail, apiserver send_recv error or timeout", LOG_TAG);
		m_LogIncb(ret, map<string, string>());
        logException(ret, "OAuthLoginCallback, oauth channel sdk login fail, apiserver send_recv error or timeout");
		return;
	}
	OAuthLoginResponse *response = (OAuthLoginResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	// 判断返回码
	if (response->get_result() != 0)
	{
		uuLOG("%s OAuthLoginCallback, oauth channel sdk login fail, apiserver return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		m_LogIncb(code, map<string, string>());
        logException(code, "OAuthLoginCallback, oauth channel sdk login fail, apiserver return code");
		return;
	}

	uuLOG("%s OAuthLoginCallback, oauth channel sdk login ok, request success !!!", LOG_TAG);

	m_user_id = response->body().m_user_id;
	m_user_account = response->body().m_user_account;
	m_token = response->body().m_token;
	delete response;

	if (m_user_id == 0) {
		uuLOG("%s sdkLogin receive user id is null!", LOG_TAG);
	}
	if (m_user_account == "") {
		uuLOG("%s sdkLogin receive user account is null!", LOG_TAG);
	}
	if (m_token == "") {
		uuLOG("%s sdkLogin receive token is null!", LOG_TAG);
	}

	const char* eventId = uuSdkConfig::getInstance()->getContentValueByKey(m_sdkName.c_str(), "event_login");
	if (eventId != NULL) {
		m_eventLoginType = eventId;
	} else { //EVENT_LOGIN_TYPE_THIRDPARTY
		m_eventLoginType = m_oauth_type;
	}
	m_loginResult = eSDK_RESULT_SUCCESS;
	onAoneLoginEndAsync();
}

void AoneSDK::OAuthInfoAsync(AONESDK_CB cb)
{
	if (!m_init_ok) {
		m_begin_login_time = m_ineffective_time;

		cb(-3, map<string, string>());
        logException(-3, "OAuthInfoAsync, init not ok");
		return;
	}

	if (m_oauth_type == "" || m_oauth_token == "") {
		m_begin_login_time = m_ineffective_time;

		cb(-11, map<string, string>());
        logException(-11, "OAuthInfoAsync, oauthInfoAsync m_oauth_type is null of m_oauth_token is null");
		return;
	}

	m_LogIncb = cb;

    uuLOG("%s OAuthInfoAsync, token is: %s, type is: %s", LOG_TAG, m_oauth_token.c_str(), m_oauth_type.c_str());

	OAuthInfoRequest request;
	request.body().m_oauth_type = m_oauth_type;
	request.body().m_oauth_token = m_oauth_token;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
    //德国12暂用---服务器更新之后删除
    if (m_oauth_type == "apple") {
        request.body().m_oauth_type = "gamecenter";
    }

	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::OAuthInfoCallback);
}

void AoneSDK::OAuthInfoCallback(int ret, byte *responseBytes, int len)
{
	if (ret < 0)
	{
		m_begin_login_time = m_ineffective_time;

		uuLOG("%s OAuthInfoCallback, oauth info fail, apiserver send_recv error or timeout", LOG_TAG);
		m_LogIncb(ret, map<string, string>());
        logException(ret, "OAuthInfoCallback, oauth info fail, apiserver send_recv error or timeout");
		return;
	}
	OAuthInfoResponse *response = (OAuthInfoResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	// 判断返回码
	if (response->get_result() != 0)
	{
		m_begin_login_time = m_ineffective_time;

		uuLOG("%s OAuthInfoCallback, oauth info fail, apiserver return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		m_LogIncb(code, map<string, string>());
        logException(code, "OAuthInfoCallback, oauth info fail, apiserver return code");
		return;
	}

	uuLOG("%s OAuthInfoCallback, oauth info ok, request success !!!", LOG_TAG);

	bool registerDevice = response->body().m_is_register_device;
	bool registerOauth = response->body().m_is_register_oauth;
	delete response;

	if (m_LogIncb) {
		m_begin_login_time = m_ineffective_time;

		map<string, string> params;
		params["registerDevice"] = registerDevice ? "1" : "0";
		params["registerOauth"] = registerOauth ? "1" : "0";
		m_LogIncb(ret, params);
	}
}


/*
* 账号实名认证信息查询请求
*/
void AoneSDK::realnameInfoAsync(AONESDK_CB cb){
	if(!m_init_ok) {
		m_begin_login_time = m_ineffective_time;

		cb(-3, map<string, string>());
		logException(-3, "realnameInfoAsync, init not ok");
		return;
	}

	if (m_user_id == 0 ) {
		m_begin_login_time = m_ineffective_time;

		cb(-3, map<string, string>());//未登录API服务
		logException(-3, "realnameInfoAsync, m_user_id is 0 ");
		return;
	}

	m_cb = cb;

	uuLOG("%s realnameInfoAsync, token is: %s, type is: %s", LOG_TAG, m_oauth_token.c_str(), m_oauth_type.c_str());

	RealnameInfoRequest request;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_user_id = m_user_id;

	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::realnameInfoCallback);
}

void AoneSDK::realnameInfoCallback(int ret, byte *responseBytes, int len){
	if (ret < 0)
	{
		m_begin_login_time = m_ineffective_time;

		uuLOG("%s realnameInfoCallback, realname info fail, apiserver send_recv error or timeout", LOG_TAG);
		m_cb(ret, map<string, string>());
		logException(ret, "realnameInfoCallback, realname info fail, apiserver send_recv error or timeout");
		return;
	}
	RealnameInfoResponse *response = (RealnameInfoResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	// 判断返回码
	if (response->get_result() != 0)
	{
		m_begin_login_time = m_ineffective_time;

		uuLOG("%s realnameInfoCallback, realname info fail, apiserver return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		m_cb(code, map<string, string>());
		logException(code, "realnameInfoCallback, realname info fail, apiserver return code");
		return;
	}

	uuLOG("%s realnameInfoCallback, realname info ok, request success !!!", LOG_TAG);

	int flag = response->body().m_flag;

	char buf[128] = { 0 };
	sprintf(buf, "%d", flag);

	string date_birth = response->body().m_date_birth;
	m_encrypt_number = response->body().m_encrypt_number;
	delete response;

	if (m_cb) {
		m_begin_login_time = m_ineffective_time;

		map<string, string> params;
		params["flag"] = buf;
		params["date_birth"] = date_birth;
		params["encrypt_number"] = m_encrypt_number;
		m_cb(ret, params);
	}

}

/*
* 账号实名认证请求
*/
void AoneSDK::realnameCertificateAsync(const char* identity_number,const char* real_name, AONESDK_CB cb){
	if (!m_init_ok) {
		m_begin_login_time = m_ineffective_time;

		cb(-3, map<string, string>());
		logException(-3, "realnameInfoCallback, init not ok");
		return;
	}

	if (m_user_id == 0 || strcmp(identity_number, "") == 0 || strcmp(real_name, "") == 0) {
		m_begin_login_time = m_ineffective_time;

		cb(-3, map<string, string>());//未登录API服务
		logException(-3, "realnameInfoCallback, Realname Certificate m_user_id is 0 ,identity_number is empty,real_name is empty");
		return;
	}

	m_cb = cb;

	uuLOG("%s realnameInfoCallback, m_identity_number is: %s, type is: %s,user_id is %d", LOG_TAG, identity_number, real_name,m_user_id);

	RealnameCertificateRequest request;
	request.body().m_identity_number = identity_number;
	request.body().m_real_name = real_name;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_user_id = m_user_id;

	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::realnameCertificateCallback);
}

void AoneSDK::realnameCertificateCallback(int ret, byte *responseBytes, int len){
	if (ret < 0)
	{
		m_begin_login_time = m_ineffective_time;

		uuLOG("%s realnameCertificateCallback, realname Certificate fail, apiserver send_recv error or timeout", LOG_TAG);
		m_cb(ret, map<string, string>());
		logException(ret, "realnameCertificateCallback, realname Certificate fail, apiserver send_recv error or timeout");
		return;
	}
	RealnameCertificateResponse *response = (RealnameCertificateResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	// 判断返回码
	if (response->get_result() != 0)
	{
		m_begin_login_time = m_ineffective_time;

		uuLOG("%s realnameCertificateCallback, realname Certificate fail, apiserver return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		m_cb(code, map<string, string>());
		logException(code, "realnameCertificateCallback, realname Certificate fail, apiserver return code");
		return;
	}

	uuLOG("%s realnameCertificateCallback, realname Certificate ok, request success !!!", LOG_TAG);

	int flag = response->body().m_flag;
	char buf[128] = { 0 };
	sprintf(buf, "%d", flag);
	m_encrypt_number = response->body().m_encrypt_number;
	string real_name = response->body().m_real_name;
	delete response;

	if (m_cb) {
		m_begin_login_time = m_ineffective_time;

		map<string, string> params;
		params["flag"] = buf;
		params["encrypt_number"] = m_encrypt_number;
		params["real_name"] = real_name;
		m_cb(ret, params);
	}

}

void AoneSDK::setVerifyKey(const char* key){
	string ret(key);
	m_verify_key = ret;
}

void AoneSDK::setAppName(const char* appName){
	string ret(appName);
	m_app_name = ret;
}

void AoneSDK::setPackageName(const char* packageName){
	string ret(packageName);
	m_package_name = ret;
}

void AoneSDK::setClassName(const char* launcherActivity){
	string ret(launcherActivity);
	m_launcher_activity = ret;	
}

bool AoneSDK::dealAppMsg(string key,const char* name){
	if (m_channel_params.find(key) != m_channel_params.end()){
		string value = getChannelParamFromKey(key);
		if(strcmp(value.c_str(), name) != 0 && strcmp(value.c_str(), "close") != 0){
			return false;
		}
	}
	return true;
}

/*
 * CP创建角色，透传参数
 */
int AoneSDK::createRoleCP(const std::string& roleID, const std::string& name, unsigned int level, const std::map<std::string, std::string>& features, const std::string& owner_groupid, std::string groupName, int roleVIP)
{
	if (!m_init_ok) return -3; // 未初始化
	if (m_user_id == 0 || m_token.empty()) return -3; // 未登陆API服务
	if (name.empty() || groupName.empty()) return -4; // 参数错误
    
	// 角色添加到列表中
	GameRoleCP role;
	role.ID = roleID;
	role.Name = name;
	role.Type = 0;
	role.Level = level;
	role.VIP = roleVIP;
	role.CreateTime = "";
	role.LastLoginTime = "";
	role.LastGroupID = "";
	role.OwnerGroupID = owner_groupid;
	for (map<string, string>::const_iterator it1 = features.begin(); it1 != features.end(); ++it1)
	{
		std::pair<string, string> pair;
		pair.first = it1->first;
		pair.second = it1->second;
		role.Features.insert(pair);
	}
	m_roles_cp.push_back(role);

	GameGroupCP group;
	group.ID = owner_groupid;
	group.Name = groupName;
	m_groups_cp.push_back(group);

	// 自动选中分区和角色
	m_select_role_cp = role;
	m_select_group_cp = group;

	// 创建角色信息传递给sdk
	std::map<string, string> info2;
	char buf[128] = {0};
	sprintf(buf, "%d", m_user_id);
	info2["aoneUserId"] = buf;
	info2["aoneRoleId"] = m_select_role_cp.ID;
	info2["aoneRoleName"] = m_select_role_cp.Name;
	sprintf(buf, "%d", m_select_role_cp.Level);
	info2["aoneRoleLevel"] = buf;
	info2["aoneGroupId"] =  m_select_group_cp.ID;
	info2["aoneGroupName"] = m_select_group_cp.Name;
	sprintf(buf, "%d", m_select_role_cp.VIP);
	info2["aoneRoleVip"] = buf;
	info2["userAccount"] = m_user_account;
	std::vector<PluginParam*> allParams;
	PluginParam* pluginParam = new PluginParam(info2);
	allParams.push_back(pluginParam);
	uuSdkManager::getInstance()->callFuncWithParam("createRole", allParams);
	delete pluginParam;
	pluginParam = NULL;

	return 0;
}

int AoneSDK::createRole(const std::string& name, unsigned int type, const std::map<std::string, std::string>& features, unsigned int owner_groupid)
{
	if (!m_init_ok) return -3; // 未初始化
	if (m_entryserver_ip.empty() || m_entryserver_port == 0) return -3; // 未正确初始化
	if (m_user_id == 0 || m_token.empty()) return -3; // 未登陆API服务
    
	GameGroup group;
	if (owner_groupid != 0)
	{
		int i = 0;
		for (i = 0; i < (int)m_groups.size(); i++)
		{
			if (m_groups[i].ID == owner_groupid)
			{
				group = m_groups[i];
				break;
			}
		}
		if (i >= (int)m_groups.size())
		{
			return -4; // 无效的参数
		}
	}

	CreateRoleRequest request;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id);
	request.body().m_role_name = name;
	request.body().m_role_type = type;
	request.body().m_group_id = owner_groupid;
	request.body().m_user_id = m_user_id;
	request.body().m_user_account = m_user_account;
	request.body().m_user_token = m_user_token;
	request.body().m_encrypt_number = m_encrypt_number;
	uuLOG("m_encrypt_number:%s", m_encrypt_number.c_str());
	for (map<string, string>::const_iterator it = features.begin(); it != features.end(); ++it)
	{
		KVPair pair;
		pair.m_key = it->first;
		pair.m_value = it->second;
		request.body().m_role_features.push_back(pair);
	}

	CreateRoleResponse* response = NULL;
	int ret = entryserver_send_recv(&request, (Aone2BaseMessage**)&response);
	if (ret < 0)
	{
		uuLOG("%s create role fail, entryserver send_recv error or timeout", LOG_TAG);
		return ret;
	}

	// 判断返回码
	if (response->get_result() != 0)
	{
		uuLOG("%s create role fail, entryserver send_recv error or timeout", LOG_TAG);
		int code = response->get_result();
		delete response;
		return code;
	}

	uuLOG("%s create role ok, request success !!!", LOG_TAG);

	// 角色添加到列表中
	GameRole role;
	RoleShortInfo& info = response->body().m_role;
	role.ID = info.m_role_id;
	role.Name = info.m_role_name;
	role.Type = info.m_role_type;
	role.Level = info.m_role_level;
	role.VIP = info.m_role_vip;
	role.CreateTime = info.m_role_createtime;
	role.LastLoginTime = info.m_last_logintime;
	role.LastGroupID = info.m_last_groupid;
	role.OwnerGroupID = info.m_owner_groupid;
	for (int j = 0; j < (int)info.m_role_features.size(); j++)
	{
		role.Features[info.m_role_features[j].m_key] = info.m_role_features[j].m_value;
	}
	m_roles.push_back(role);
	delete response;

	// 自动选中分区和角色
	m_select_role = role;
	m_select_group = group;

	// 创建角色信息传递给sdk
	std::map<string, string> info2;
	char buf[128] = {0};
	sprintf(buf, "%d", m_user_id);
	info2["aoneUserId"] = buf;
	sprintf(buf, "%d", m_select_role.ID);
	info2["aoneRoleId"] = buf;
	m_select_role_cp.ID = buf;
	info2["aoneRoleName"] = m_select_role.Name;
	sprintf(buf, "%d", m_select_role.Level);
	info2["aoneRoleLevel"] = buf;
	sprintf(buf, "%d", getSelectedGroup().ID);
	info2["aoneGroupId"] = buf;
	info2["aoneGroupName"] = getSelectedGroup().Name;
	sprintf(buf, "%d", m_select_role.VIP);
	info2["aoneRoleVip"] = buf;
	std::vector<PluginParam*> allParams;
	PluginParam* pluginParam = new PluginParam(info2);
	allParams.push_back(pluginParam);
	uuSdkManager::getInstance()->callFuncWithParam("createRole", allParams);
	delete pluginParam;
	pluginParam = NULL;

	char groupId[20];
	sprintf(groupId, "%d", m_select_group.ID);
	string c_groupId = groupId;
	m_select_group_cp.ID = c_groupId;

	return 0;
}

void AoneSDK::createRoleAsync(const std::string& name, unsigned int type, const std::map<std::string, std::string>& features, unsigned int owner_groupid, AONESDK_CB cb)
{
	if (!m_init_ok) { // 未初始化
		cb(-3, map<string, string>());
        logException(-3, "createRoleAsync, init not ok");
		return;
	}
	if (m_entryserver_ip.empty() || m_entryserver_port == 0) {
		cb(-3, map<string, string>());
        logException(-3, "createRoleAsync, maybe m_entrysever_is is empty of m_entryserver_port is equal to 0");
		return;
	}
	if (m_user_id == 0 || m_token.empty()) {
		cb(-3, map<string, string>());
        logException(-3, "createRoleAsync, maybe m_user_id is 0 of m_token is empty");
		return;
	}

	if (owner_groupid != 0)
	{
		int i = 0;
		for (i = 0; i < (int)m_groups.size(); i++)
		{
			if (m_groups[i].ID == owner_groupid)
			{
				m_select_group = m_groups[i];
				break;
			}
		}
		if (i >= (int)m_groups.size())
		{
			cb(-4, map<string, string>());
            logException(-4, "createRoleAsync, find group failed");
			return;
		}
	}

	CreateRoleRequest request;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id);
	request.body().m_role_name = name;
	request.body().m_role_type = type;
	request.body().m_group_id = owner_groupid;
	request.body().m_user_id = m_user_id;
	request.body().m_user_account = m_user_account;
	request.body().m_user_token = m_user_token;
	request.body().m_encrypt_number = m_encrypt_number;
	uuLOG("[createRoleAsync] m_encrypt_number:%s", m_encrypt_number.c_str());
	for (map<string, string>::const_iterator it = features.begin(); it != features.end(); ++it)
	{
		KVPair pair;
		pair.m_key = it->first;
		pair.m_value = it->second;
		request.body().m_role_features.push_back(pair);
	}

	/*CreateRoleResponse* response = NULL;
	int ret = entryserver_send_recv(&request, (Aone2BaseMessage**)&response);*/
	m_cb = cb;
	entryserver_send_recv_async(&request, (NET_CB)&AoneSDK::createRoleCallback);
}

void AoneSDK::createRoleCallback(int ret, byte *responseBytes, int len)
{
	if (ret < 0)
	{
		uuLOG("%s createRoleCallback, create role fail, entryserver send_recv error or timeout", LOG_TAG);
		m_cb(ret, map<string, string>());
        logException(ret, "createRoleCallback, create role fail, entryserver send_recv error or timeout");
		return;
	}

	CreateRoleResponse* response = (CreateRoleResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	// 判断返回码
	if (response->get_result() != 0)
	{
		uuLOG("%s createRoleCallback, create role fail, entryserver send_recv error or timeout", LOG_TAG);
		int code = response->get_result();
		delete response;
		m_cb(code, map<string, string>());
        logException(code, "createRoleCallback, create role fail, entryserver send_recv error or timeout");
		return;
	}

	uuLOG("%s createRoleCallback, create role ok, request success !!!", LOG_TAG);

	// 角色添加到列表中
	GameRole role;
	RoleShortInfo& info = response->body().m_role;
	role.ID = info.m_role_id;
	role.Name = info.m_role_name;
	role.Type = info.m_role_type;
	role.Level = info.m_role_level;
	role.CreateTime = info.m_role_createtime;
	role.LastLoginTime = info.m_last_logintime;
	role.LastGroupID = info.m_last_groupid;
	role.OwnerGroupID = info.m_owner_groupid;
	for (int j = 0; j < (int)info.m_role_features.size(); j++)
	{
		role.Features[info.m_role_features[j].m_key] = info.m_role_features[j].m_value;
	}
	m_roles.push_back(role);
	delete response;

	// 自动选中分区和角色
	m_select_role = role;
    convertRoleToRoleCP(m_select_role, m_select_role_cp);
    

	// 创建角色信息传递给sdk
	std::map<string, string> info2;
	char buf[128] = {0};
	sprintf(buf, "%d", m_user_id);
	info2["aoneUserId"] = buf;
	sprintf(buf, "%d", m_select_role.ID);
	info2["aoneRoleId"] = buf;
	info2["aoneRoleName"] = m_select_role.Name;
	sprintf(buf, "%d", m_select_role.Level);
	info2["aoneRoleLevel"] = buf;
	sprintf(buf, "%d", getSelectedGroup().ID);
	info2["aoneGroupId"] = buf;
	info2["aoneGroupName"] = getSelectedGroup().Name;
	sprintf(buf, "%d", m_select_role.VIP);
	info2["aoneRoleVip"] = buf;
	info2["userAccount"] = m_user_account;
	std::vector<PluginParam*> allParams;
	PluginParam* pluginParam = new PluginParam(info2);
	allParams.push_back(pluginParam);
	uuSdkManager::getInstance()->callFuncWithParam("createRole", allParams);
	delete pluginParam;
	pluginParam = NULL;
// 外部调用，使用false 强制记录
	LogEvent("Create role", false);

//    m_select_group_cp.ID = m_select_group.ID;
//    m_select_group_cp.Name = m_select_group.Name;
    
	m_cb(0, map<string, string>());
	return;
}

int AoneSDK::selectRoleAndGroup(unsigned int role_id, unsigned int group_id)
{
	if (!m_init_ok) return -3; // 未初始化
	if (role_id == 0) return -4;
	if (group_id == 0) return -4;

	int role_index = 0;
	for (role_index = 0; role_index < (int)m_roles.size(); role_index++)
	{
		if (m_roles[role_index].ID == role_id)
		{
			m_select_role = m_roles[role_index];
			break;
		}
	}
	if (role_index >= (int)m_roles.size())
	{
		// 本地没有的角色，可能是方案二中游戏服创建的角色，需要到平台拉取一下
		GetRoleInfoRequest request;
		request.body().m_device_info = m_device_info;
		request.body().m_version_info = m_version_info;
		request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id);
		request.body().m_role_id = role_id;

		GetRoleInfoResponse* response = NULL;
		int ret = entryserver_send_recv(&request, (Aone2BaseMessage**)&response);
		if (ret < 0)
		{
			uuLOG("%s get role info fail, entryserver send_recv error or timeout", LOG_TAG);
			return ret;
		}

		// 判断返回码
		if (response->get_result() != 0)
		{
			uuLOG("%s get role info fail, entryserver send_recv error or timeout", LOG_TAG);
			int code = response->get_result();
			delete response;
			return code;
		}

		// 角色添加到列表中
		GameRole role;
		RoleShortInfo& info = response->body().m_role;
		role.ID = info.m_role_id;
		role.Name = info.m_role_name;
		role.Type = info.m_role_type;
		role.Level = info.m_role_level;
		role.VIP = info.m_role_vip;
		role.CreateTime = info.m_role_createtime;
		role.LastLoginTime = info.m_last_logintime;
		role.LastGroupID = info.m_last_groupid;
		role.OwnerGroupID = info.m_owner_groupid;
		for (int j = 0; j < (int)info.m_role_features.size(); j++)
		{
			role.Features[info.m_role_features[j].m_key] = info.m_role_features[j].m_value;
		}
		m_roles.push_back(role);
		m_select_role = role;
		delete response;
	}
	uuLOG("%s selectRoleAndGroup GameRole success !!!", LOG_TAG);

	int group_index = 0;
	for (group_index = 0; group_index < (int)m_groups.size(); group_index++)
	{
		if (m_groups[group_index].ID == group_id)
		{
			m_select_group = m_groups[group_index];
			break;
		}
	}
	if (group_index >= (int)m_groups.size()) return -6;
	if (m_select_role.OwnerGroupID != 0 && m_select_role.LastGroupID != 0 && m_select_role.LastGroupID != group_id) return -7;
	uuLOG("%s selectRoleAndGroup GameGroup success !!!", LOG_TAG);
	submitRoleData();
	return 0;
}

void AoneSDK::selectRoleAndGroupAsync(unsigned int role_id, unsigned int group_id, AONESDK_CB cb)
{
	if (!m_init_ok) {
		cb(-3, map<string, string>());
        logException(-3, "selectRoleAndGroupAsync, init not ok");
		return;
	}
	if (role_id == 0 || group_id == 0) {
		cb(-4, map<string, string>());
        logException(-4, "selectRoleAndGroupAsync, role_id or group_id is zero");
		return;
	}

	m_cb = cb;

	int role_index = 0;
	for (role_index = 0; role_index < (int)m_roles.size(); role_index++)
	{
		if (m_roles[role_index].ID == role_id)
		{
			m_select_role = m_roles[role_index];
			break;
		}
	}
	if (role_index >= (int)m_roles.size())
	{
		// 本地没有的角色，可能是方案二中游戏服创建的角色，需要到平台拉取一下
		GetRoleInfoRequest request;
		request.body().m_device_info = m_device_info;
		request.body().m_version_info = m_version_info;
		request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id);
		request.body().m_role_id = role_id;

		/*GetRoleInfoResponse* response = NULL;
		int ret = entryserver_send_recv(&request, (Aone2BaseMessage**)&response);*/
		m_select_group_id = group_id;
		entryserver_send_recv_async(&request, (NET_CB)&AoneSDK::selectRoleAndGroupCallback);
		return;
	}
	uuLOG("%s selectRoleAndGroupAsync, GameRole success !!!", LOG_TAG);

	int group_index = 0;
	for (group_index = 0; group_index < (int)m_groups.size(); group_index++)
	{
		if (m_groups[group_index].ID == group_id)
		{
			m_select_group = m_groups[group_index];
			break;
		}
	}
	if (group_index >= (int)m_groups.size())
	{
		m_cb(-6, map<string, string>());
        logException(-6, "selectRoleAndGroupAsync, find group failed");
		return;
	}
	if (m_select_role.OwnerGroupID != 0 && m_select_role.LastGroupID != 0 && m_select_role.LastGroupID != group_id)
	{
		m_cb(-7, map<string, string>());
        logException(-7, "selectRoleAndGroupAsync");
		return;
	}
	submitRoleData();
	m_cb(0, map<string, string>());
	uuLOG("%s selectRoleAndGroupAsync, GameGroup success !!!", LOG_TAG);
	return;
}

void AoneSDK::selectRoleAndGroupCallback(int ret, byte *responseBytes, int len)
{
	if (ret < 0)
	{
		uuLOG("%s selectRoleAndGroupCallback, get role info fail, entryserver send_recv error or timeout", LOG_TAG);
		m_cb(ret, map<string, string>());
        logException(ret, "selectRoleAndGroupCallback, get role info fail, entryserver send_recv error or timeout");
		return;
	}
	GetRoleInfoResponse* response = (GetRoleInfoResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	// 判断返回码
	if (response->get_result() != 0)
	{
		uuLOG("%s selectRoleAndGroupCallback, get role info fail, entryserver send_recv error or timeout", LOG_TAG);
		int code = response->get_result();
		delete response;
		m_cb(code, map<string, string>());
        logException(code, "selectRoleAndGroupCallback, get role info fail, entryserver send_recv error or timeout");
		return;
	}

	// 角色添加到列表中
	GameRole role;
	RoleShortInfo& info = response->body().m_role;
	role.ID = info.m_role_id;
	role.Name = info.m_role_name;
	role.Type = info.m_role_type;
	role.Level = info.m_role_level;
	role.CreateTime = info.m_role_createtime;
	role.LastLoginTime = info.m_last_logintime;
	role.LastGroupID = info.m_last_groupid;
	role.OwnerGroupID = info.m_owner_groupid;
	for (int j = 0; j < (int)info.m_role_features.size(); j++)
	{
		role.Features[info.m_role_features[j].m_key] = info.m_role_features[j].m_value;
	}
	m_roles.push_back(role);
	m_select_role = role;
	delete response;
	uuLOG("%s selectRoleAndGroupCallback, GameRole success !!!", LOG_TAG);

	int group_index = 0;
	for (group_index = 0; group_index < (int)m_groups.size(); group_index++)
	{
		if (m_groups[group_index].ID == m_select_group_id)
		{
			m_select_group = m_groups[group_index];
			break;
		}
	}
	if (group_index >= (int)m_groups.size())
	{
		m_cb(-6, map<string, string>());
        logException(-6, "[selectRoleAndGroupCallback]");
		return;
	}
	if (m_select_role.OwnerGroupID != 0 && m_select_role.LastGroupID != 0 && m_select_role.LastGroupID != m_select_group_id)
	{
		m_cb(-7, map<string, string>());
        logException(-7, "[selectRoleAndGroupCallback]");
		return;
	}

	submitRoleData();
	m_cb(0, map<string, string>());
	uuLOG("%s selectRoleAndGroup GameGroup success !!!", LOG_TAG);
	return;
}

void AoneSDK::pullGameGroupAsync(AONESDK_CB cb)
{
	if (!m_init_ok) {
		cb(-3, map<string, string>());
        logException(-3, "[pullGameGroupAsync]");
		return;
	}

	m_cb = cb;
	GetGameGroupsRequest request;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id);
	entryserver_send_recv_async(&request, (NET_CB)&AoneSDK::pullGameGroupCallback);
}

void AoneSDK::pullGameGroupCallback(int ret, byte *responseBytes, int len)
{
	if (ret < 0)
	{
		uuLOG("%s pullGameGroupCallback, get role info fail, entryserver send_recv error or timeout , ret = %d", LOG_TAG, ret);
		if (m_cb) {
			m_cb(ret, map<string, string>());
            logException(ret, "pullGameGroupCallback, get role info fail, entryserver send_recv error or timeout");
		}
		return;
	}
	GetGameGroupsResponse* response = (GetGameGroupsResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	if (response == NULL)
	{
		m_cb(-8, map<string, string>());
		logException(-8, "pullGameGroupCallback, response parse fail");
		return;
	}

	// 判断返回码
	if (response->get_result() != 0)
	{
		int code = response->get_result();
        uuLOG("%s pullGameGroupCallback, get role info fail, code = %d", LOG_TAG, code);
		delete response;
		if (m_cb) {
			m_cb(code, map<string, string>());
            logException(code, "pullGameGroupCallback, get role info fail, entryserver send_recv error or timeout");
		}
		return;
	}
	// 分区列表
	m_groups.clear();
	for (int i = 0; i < (int)response->body().m_groups.size(); i++)
	{
		GameGroupInfo& info = response->body().m_groups[i];
		GameGroup group;
		group.ID = info.m_group_id;
		group.Name = info.m_group_name;
		group.ServerIP = info.m_server_ip;
		group.ServerPort = info.m_server_port;
		group.IsInitialed = info.m_is_initialed;
		group.IsRecommend = info.m_is_recommend;
		group.Status = (GameGroup::GameGroupStatus)(int)info.m_status;
		m_groups.push_back(group);
	}
	if (m_cb) {
		m_cb(0, map<string, string>());
	}
	uuLOG("%s pullGameGroupCallback, pull GameGroup success !!!", LOG_TAG);
	return;
}

void AoneSDK::payCommitAsync(PayStorage::PayRecord &record, PAYCOMMIT_CB handler)
{
    uuLOG("%s hhh----------payCommitAsync", LOG_TAG);
    
	if (!m_init_ok) {
		//(sender->*handler)(-3, map<string, string>());
		return;
	}

	//m_cb = cb;
	m_handler = handler;

	PayRequest request;
	request.body().m_user_id = record.user_id;
	request.body().m_role_id = record.role_id;
	request.body().m_pay_no = record.record[0];
	request.body().m_product_id = record.record[1];
	request.body().m_pay_channel = record.record[2];
	request.body().m_cp_ext = record.record[3];
	request.body().m_receipt = record.record[4];
    request.body().m_cp_role_id = record.cpID[0];
    request.body().m_cp_group_id = record.cpID[1];
	request.body().m_product_quantity = record.purchase_num;
	
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;

	/*PayResponse* response = NULL;
	int ret = apiserver_send_recv(&request, (Aone2BaseMessage**)&response);*/
	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::payCommitCallback);

}

void AoneSDK::payCommitAsync(const string& pay_no, const string& product_id,unsigned int purchaseNum, const string& pay_channel, const string& cp_ext, const string& receipt, PAYCOMMIT_CB handler)
{
    uuLOG("%s begin pay commit ... ", LOG_TAG);
    
	if (!m_init_ok)
    {
		//(sender->*handler)(-3, map<string, string>());
		return;
	}

	if (isRoleAndGroupValid() == false) {
		//(sender->*handler)(-3, map<string, string>());
		//return;
	}

	//m_cb = cb;
	m_handler = handler;

	PayRequest request;
	request.body().m_user_id = m_user_id;
	request.body().m_role_id = m_select_role.ID;
	request.body().m_receipt = receipt;
	request.body().m_pay_channel = pay_channel;
	request.body().m_cp_ext = cp_ext;
	request.body().m_pay_no = pay_no;
	request.body().m_product_id = product_id;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_cp_role_id = m_select_role_cp.ID;
	request.body().m_cp_group_id = m_select_group_cp.ID;
	request.body().m_product_quantity = m_purchase_num;

	/*PayResponse* response = NULL;
	int ret = apiserver_send_recv(&request, (Aone2BaseMessage**)&response);*/
	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::payCommitCallback);
}

void AoneSDK::payCommitCallback(int ret, byte *responseBytes, int len)
{
	if (ret < 0)
	{
		uuLOG("%s pay commit fail, apiserver send_recv error or timeout", LOG_TAG);
		//m_cb(ret, map<string, string>());
		(this->*m_handler)(ret, map<string, string>());
        logException(ret, "pay commit fail, apiserver send_recv error or timeout");
		return;
	}

	PayResponse *response = (PayResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	// 判断返回码
	if (response->get_result() != 0)
	{
		uuLOG("%s pay commit fail, apiserver return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		//m_cb(code, map<string, string>());
		(this->*m_handler)(code, map<string, string>());
        logException(code, "pay commit fail, apiserver return code");
		return;
	}

	uuLOG("%s pay commit ok, request success !!!", LOG_TAG);

	delete response;
	//m_cb(0, map<string, string>());
	(this->*m_handler)(0, map<string, string>());
}

void AoneSDK::payCommitResult(int result, map<string, string> params)
{
	if (result < 0) {
		uuLOG("%s Error! pay commit failed ,ret code: %d", LOG_TAG, result);
	}
	else {
        /*
         result < 0 :提交收据失败；
         result == 0 :提交收据成功，重复收据会忽略，并返回0；
         result > 0 :报错，收据为空、查不到订单号等。
         */
		uuLOG("%s pay commit order success!", LOG_TAG);
        if (result == 0 && AonesdkConfig::getInstance()->getAgainstFakePay() == "true") {
            uuSdkManager::getInstance()->callFunc(m_sdkName, "finishPay");
            return;
        }
		m_storage.load();
		int i = 0;
		for (i = 0; i < (int)m_storage.m_vecData.size(); i++) {
			if (m_last_payno == m_storage.m_vecData[i].record[0]) {
				m_storage.m_vecData.erase(m_storage.m_vecData.begin() + i);
				m_storage.saveToFile();//payCommitResult提交订单或收据结果完成----删除订单
				break;
			}
		}
		if (result != 0) {
			string msg = "pay commit order error!";
			recordSDKPayException(result, msg);
		}
	}
}

void AoneSDK::payReCommitResult(int result, map<string, string> params)
{
    if (result < 0) {
        m_storage.m_store_payno_fail.push_back(m_storage.m_store_payno);
        m_storage.m_store_payno = "";
    }
    else if (result > 0) {
        string msg = "repay commit order error!";
        recordSDKPayException(result, msg);
    }
    
    // commit twice also return 0, and then delete the data in file.
    if (result >= 0) {
        for (int i = 0; i < m_storage.m_vecData.size(); ++i)
        {
            if (m_storage.m_store_payno == m_storage.m_vecData[i].record[0]) {
                m_storage.m_vecData.erase(m_storage.m_vecData.begin() + i);
                m_storage.saveToFile();//payReCommitResult再次提交收据---删除订单
                break;
            }
        }
        
        m_storage.m_store_payno = "";
    }
    
    m_storage.checkRecords(this);
}
    
void AoneSDK::receiptCommitAsync(PayStorage::PayRecord &record, PAYCOMMIT_CB handler)
{
    uuLOG("%s hhh----------receiptCommitAsync PayRecord", LOG_TAG);
    if (!m_init_ok)
        return;
    
    m_handler = handler;
    
    CommitReceiptRequest request;
    request.body().m_pay_no = record.record[0];
    request.body().m_receipt = record.record[4];
    request.body().m_pay_channel = record.record[2];
    
    request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
    request.body().m_device_info = m_device_info;
    request.body().m_version_info = m_version_info;
    
    apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::receiptCommitCallback);
}

void AoneSDK::receiptCommitAsync(const string& pay_no, const string& receipt, const string& pay_channel, PAYCOMMIT_CB handler)
{
	uuLOG("%s hhh----------receiptCommitAsync", LOG_TAG);
	if (!m_init_ok)
		return;

	m_handler = handler;

	CommitReceiptRequest request;
	request.body().m_pay_no = pay_no;
	request.body().m_receipt = receipt;
	request.body().m_pay_channel = pay_channel;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;

	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::receiptCommitCallback);
}

void AoneSDK::receiptCommitCallback(int ret, byte *responseBytes, int len)
{
	if (ret < 0)
	{
		uuLOG("%s receipt commit fail, apiserver send_recv error or timeout", LOG_TAG);
		(this->*m_handler)(ret, map<string, string>());
		return;
	}

	CommitReceiptResponse *response = (CommitReceiptResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	std::map<string, string> info;
	std::vector<PluginParam*> params;
	char buf[128] = { 0 };
	sprintf(buf, "%d", response->get_result());
	info["code"] = buf;
	info["receipt"] = m_recipt;
	PluginParam *pluginParam = new PluginParam(info);
	params.push_back(pluginParam);
	uuLOG("%s receiptCommitCallback ", LOG_TAG);
	uuSdkManager::getInstance()->callFuncWithParam("receiptCommitCallback", params);
	if (response->get_result() != 0)
	{
		uuLOG("%s receipt commit fail, apiserver return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		(this->*m_handler)(code, map<string, string>());
		return;
	}

	uuLOG("%s receipt commit ok, request success !!!", LOG_TAG);
	
	delete response;
	
	(this->*m_handler)(0, map<string, string>());
}

void AoneSDK::payRequestAsync(const string& pay_no, const string& product_id, unsigned int purchaseNum, const string& pay_channel, const string& cp_ext, AONESDK_CB cb)
{
	if (!m_init_ok || isRoleAndGroupValid() == false) {
		m_begin_pay_time = m_ineffective_time;
		cb(-3, map<string, string>());
        logException(-3, "[payRequestAsync]");
		return;
	}

	m_cb = cb;
    uuLOG("%s hhh --- pay reauest, pay_no = %s", LOG_TAG, pay_no.c_str());
	PayRequest request;
	request.body().m_user_id = m_user_id;
	request.body().m_role_id = m_select_role.ID;
	request.body().m_receipt = "";
	request.body().m_pay_channel = pay_channel;
	request.body().m_cp_ext = cp_ext;
	request.body().m_pay_no = pay_no;
	request.body().m_product_id = product_id;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_cp_role_id = m_select_role_cp.ID;
	request.body().m_cp_group_id = m_select_group_cp.ID;
	request.body().m_channel_user_id = m_channel_user_id;
	request.body().m_product_quantity = purchaseNum;


	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::payRequestCallback);
}

void AoneSDK::payRequestCallback(int ret, byte *responseBytes, int len)
{
	if (ret < 0)
	{
		uuLOG("%s pay request fail, apiserver send_recv error or timeout", LOG_TAG);
		if (m_cb) {
			m_begin_pay_time = m_ineffective_time;

			m_cb(ret, map<string, string>());
            logException(ret, "pay request fail, apiserver send_recv error or timeout");
		}
		return;
	}

	PayResponse *response = (PayResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	// 判断返回码
	if (response->get_result() != 0)
	{
		m_begin_pay_time = m_ineffective_time;

		uuLOG("%s pay request fail, apiserver return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		m_cb(code, map<string, string>());
        logException(code, "aonesdk pay request fail, apiserver return code");
		return;
	}

	uuLOG("%s pay request ok, request success !!! server_pay_no = %s", LOG_TAG, (response -> body().m_pay_no).c_str());

	m_actual_discount = response->body().m_discount;
	if (m_actual_discount <= 0 || m_actual_discount > 100)
		m_actual_discount = 100;
	uuLOG("%s actual discount is:%d", LOG_TAG, m_actual_discount);

	delete response;

    uuLOG("%s pay sdk name is = %s\n", LOG_TAG, m_sdkName.c_str());
	this -> pay(m_sdkName.c_str(), m_select_product_id,m_purchase_num, m_cp_ext, false, m_cb);
}
    
void AoneSDK::queryOrderStatus(AONESDK_CB cb)
{
    if (!m_init_ok) {
        cb(-3, map<string, string>());
        logException(-3, "[queryOrderStatus]");
        return;
    }
    
    m_cb = cb;
    
    QueryOrderStatusRequest request;
  
    request.body().m_user_id = m_user_id;
    request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
    request.body().m_device_info = m_device_info;
    request.body().m_version_info = m_version_info;
    
    apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::queryOrderStatusCallback);

}
    
void AoneSDK::queryOrderStatusCallback(int ret, byte *responseBytes, int len)
{
    if (ret < 0)
    {
        uuLOG("%s  query order status fail, api server send_recv error or timeout", LOG_TAG);
        m_cb(ret, map<string, string>());
        logException(ret, "query order status fail, api server send_recv error or timeout");
        return;
    }
    
    QueryOrderStatusResponse *response = (QueryOrderStatusResponse*)s_apiParser->get_message(responseBytes, len);
    double amount = response->body().m_amount;
    double amount_usd = response->body().m_amount_usd;
    int coin = response->body().m_coin;
    string currency = response->body().m_currency;
    string pay_no = response->body().m_pay_no;
    
    map<string, string>queryOrderMap;
    
    queryOrderMap.insert(make_pair("current", currency));
    queryOrderMap.insert(make_pair("pay_no", pay_no));

	char amountBuf[5];
	sprintf(amountBuf, "%f", amount);
	string c_amount = amountBuf;
    queryOrderMap.insert(make_pair("amount", c_amount));
    
	char amountUsdBuf[5];
	sprintf(amountUsdBuf, "%f", amount_usd);
	string c_amount_usd = amountUsdBuf;
    queryOrderMap.insert(make_pair("amount_usd", c_amount_usd));
   
    char   temp[64];
    sprintf(temp,"%d",coin);
    string c_coin=temp;
    queryOrderMap.insert(make_pair("coin", c_coin));
    
    if (responseBytes) delete responseBytes;
 
    // 判断返回码
    if (response->get_result() != 0)
    {
        uuLOG("%s query order status, apiserver return code is %d", LOG_TAG, response->get_result());
        int code = response->get_result();
        delete response;
        m_cb(code, queryOrderMap);
        logException(code, "query order status, apiserver return code");
        return;
    }
    
    uuLOG("%s query order status ok, request success !!!", LOG_TAG);
    
    m_cb(0, queryOrderMap);
}
    
void AoneSDK::queryPayNoStatus(uint32 appId, const std::string &payNO, aonesdk::AONESDK_CB cb)
{
    if (!m_init_ok) {
        cb(-3, map<string, string>());
        logException(-3, "[queryPayNoStatus]");
        return;
    }
    m_queryPayNoCb = cb;
    
    QueryPayNoStatusRequest request;
    request.body().m_app_id = appId;
    request.body().m_pay_no = payNO;
    uuLOG("%s begin query pay number, appId = %d, lasy_pay_no = %s", LOG_TAG, appId, payNO.c_str());
    apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::queryPayNoStatusCallback);
}
    
void AoneSDK::queryPayNoStatusCallback(int ret, byte *responseBytes, int len)
{
    map<string, string>data;
    if (ret < 0)
    {
        uuLOG("%s query pay number fail, api server send_recv error or timeout", LOG_TAG);
        data["aone_msg"] = "query oerder fail, api server send_recv error";
        m_queryPayNoCb(ret, data);
        logException(ret, "query pay number fail, api server send_recv error or timeout");
        return;
    }
    
     QueryPayNoStatusResponse *response = (QueryPayNoStatusResponse *)s_apiParser->get_message(responseBytes, len);
    if (responseBytes) delete responseBytes;
    
    int result = response -> get_result();
    if (result != 0) {
        uuLOG("%s query pay number fail, result code is %d", LOG_TAG, result);
        delete response;
        data["aone_msg"] = ("query oerder fail, code = %d" , result);
        m_queryPayNoCb(result, data);
        logException(result, "query pay number fail, result code is not OK!");
        return;
    }
    string status = response -> body().m_status;
    int code = 1;
    if (status.compare("ok") == 0 || status.compare("OK") == 0) {
        code = 0;
    } else {
        data["order"] =  m_last_payno;
    }
    uuLOG("%s query pay number success, status = [%s]", LOG_TAG, status.c_str());
    
    m_queryPayNoCb(code, data);
}

//查询角色是否达到切支付条件
void AoneSDK::queryPayMode(AONESDK_CB cb)
{
    if (!m_init_ok) {
        cb(-3, map<string, string>());
        logException(-3, "[queryPayMode]");
        return;
    }
    m_queryPayModeCb = cb;
    
    QueryPayModeRequest request;
    request.body().m_user_id = m_user_id;
    request.body().m_cp_role_id = m_select_role_cp.ID;
    request.body().m_cp_group_id = m_select_group_cp.ID;
    request.body().m_device_info = m_device_info;
    request.body().m_version_info = m_version_info;
    uuLOG("%s begin query pay mode, m_user_id = %d, m_select_group_cp.id = %s", LOG_TAG, m_user_id, m_select_group_cp.ID.c_str());
    apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::queryPayModeCallback);
    
}

void AoneSDK::queryPayModeCallback(int ret, byte *responseBytes, int len)
{
    map<string, string>data;
    if (ret < 0)
    {
        uuLOG("%s query pay mode fail, api server send_recv error or timeout", LOG_TAG);
        data["aone_msg"] = "query mode fail, api server send_recv error";
        m_queryPayModeCb(ret, data);
        logException(ret, "query pay mode fail, api server send_recv error or timeout");
        return;
    }
    
    QueryPayModeResponse *response = (QueryPayModeResponse *)s_apiParser->get_message(responseBytes, len);
    if (responseBytes) delete responseBytes;
    
    int result = response -> get_result();
    
    if (result != 0) {
        uuLOG("%s query pay mode fail, result code is %d", LOG_TAG, result);
        delete response;
        data["aone_msg"] = ("query mode fail, code = %d" , result);
        m_queryPayModeCb(result, data);
        logException(result, "query pay mode fail, result code is not OK!");
        return;
    }
    
    int mode = response->body().m_pay_mode;
    
    char temp[64];
    sprintf(temp,"%d",mode);
    string c_mode=temp;
    data["mode"] = c_mode;
    uuLOG("%s query pay mode success, mode = [%d]", LOG_TAG, mode);

    m_queryPayModeCb(ret, data);
}

int AoneSDK::AoneChangePassword(const string oldpass, const string newpass)
{
	if (!m_init_ok) return -3; // 未初始化
	m_new_password = newpass;
	AoneChangePasswordRequest request;
	request.body().m_user_id = m_user_id;
	request.body().m_token = m_token;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	request.body().m_old_password = oldpass;
	request.body().m_new_password = newpass;
	AoneChangePasswordResponse* response = NULL;
	int ret = apiserver_send_recv(&request, (Aone2BaseMessage**)&response);
	if (ret < 0)
	{
		return ret;
	}

	// 判断返回码
	if (response->get_result() != 0)
	{
		int code = response->get_result();
		delete response;
		return code;
	}
	//get old account and password from aone_userinfo.json
	if (m_is_create_inheritancecode){
		m_is_create_inheritancecode = false;
		AoneSDK::AoneUserInfo aoneUserInfo = getUserInfo();
		string account = aoneUserInfo.account;
		string password = aoneUserInfo.password;
		char buf[128] = { 0 };
		sprintf(buf, "%d", m_user_id);
		//deleteUserInfo();
		saveUserInfo(account, m_new_password, "jp_special", buf, "", "","");
	}
	return 0;
}

void AoneSDK::AoneChangePasswordAsync(const string oldpass, const string newpass, AONESDK_CB cb)
{
	if (!m_init_ok) {
		cb(-3, map<string, string>());
        logException(-3, "[AoneChangePasswordAsync]");
		return;
	}
	m_new_password = newpass;
	m_cb = cb;
	uuLOG("(AoneChangePasswordAsync)m_user_id:%d",m_user_id);
	
	if (isJpSpecial()){
		string m_jp_prefix = "jp_special=";
		m_version_info.m_ext = m_jp_prefix.append("jp_special");//value is jp_special
	}
	
	AoneChangePasswordRequest request;
	request.body().m_user_id = m_user_id;
	request.body().m_token = m_token;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	request.body().m_old_password = oldpass;
	request.body().m_new_password = newpass;
	//AoneChangePasswordResponse* response = NULL;
	//int ret = apiserver_send_recv(&request, (Aone2BaseMessage**)&response);
	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::AoneChangePasswordCallback);
}

void AoneSDK::AoneChangePasswordCallback(int ret, byte *responseBytes, int len)
{
	if (ret < 0)
	{
		uuLOG("%s aone change password fail, api server send_recv error or timeout", LOG_TAG);
		m_cb(ret, map<string, string>());
        logException(ret, "aone change password fail, api server send_recv error or timeout");
		return;
	}
	AoneChangePasswordResponse* response = (AoneChangePasswordResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	// 判断返回码
	if (response->get_result() != 0)
	{
		uuLOG("%s aone change password fail, apiserver return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		m_cb(code, map<string, string>());
        logException(code, "aone change password fail, apiserver return code");
		return;
	}

	uuLOG("%s aone change password ok, request success !!!", LOG_TAG);
	//get old account and password from aone_userinfo.json
	map<string, string> infos;
	if (m_is_create_inheritancecode){
		m_is_create_inheritancecode = false;
		AoneSDK::AoneUserInfo aoneUserInfo = getUserInfo();
		string account = aoneUserInfo.account;
		string password = aoneUserInfo.password;
		//deleteFile("aone_userinfo.json");
		char buf[128] = { 0 };
		sprintf(buf, "%d", m_user_id);
		//deleteUserInfo();
		saveUserInfo(account, m_new_password, "jp_special", buf, "", "","");
		infos["account"] = account;
		infos["password"] = m_new_password;
	}
	m_cb(0, infos);
}

void AoneSDK::AoneCreateInheritance(const string password, AONESDK_CB cb){
	AoneSDK::AoneUserInfo aoneUserInfo = getUserInfo();
	string oldPassword = aoneUserInfo.password;
	m_is_create_inheritancecode = true;
	AoneChangePasswordAsync(oldPassword,password,cb);
}

int AoneSDK::AoneBind(const string account, const string password)
{
	if (!m_init_ok) return -3; // 未初始化

	AoneBindRequest request;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	request.body().m_account = account;
	request.body().m_password = password;
	AoneBindResponse* response = NULL;
	int ret = apiserver_send_recv(&request, (Aone2BaseMessage**)&response);
	if (ret < 0)
	{
		return ret;
	}

	// 判断返回码
	if (response->get_result() != 0)
	{
		int code = response->get_result();
		delete response;
		return code;
	}

	m_user_id = response->body().m_user_id;
	m_user_account = response->body().m_user_account;
	m_token = response->body().m_token;
	delete response;

	return 0;
}

void AoneSDK::AoneBindAsync(const string account, const string password, AONESDK_CB cb)
{
	if (!m_init_ok) {
		cb(-3, map<string, string>());
        logException(-3, "[AoneBindAsync]");
		return;
	}

	m_cb = cb;

	AoneBindRequest request;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	request.body().m_account = account;
	request.body().m_password = password;
	/*AoneBindResponse* response = NULL;
	int ret = apiserver_send_recv(&request, (Aone2BaseMessage**)&response);*/
	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::AoneBindCallback);
}

void AoneSDK::AoneBindCallback(int ret, byte *responseBytes, int len)
{
	if (ret < 0)
	{
		uuLOG("%s aone bind fail, api server send_recv error or timeout", LOG_TAG);
		m_cb(ret, map<string, string>());
        logException(ret, "aonesdk aone bind fail, api server send_recv error or timeout");
		return;
	}
	AoneBindResponse* response = (AoneBindResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	// 判断返回码
	if (response->get_result() != 0)
	{
		uuLOG("%s aone bind fail, apiserver return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		m_cb(code, map<string, string>());
        logException(code, "aone bind fail, apiserver return code");
		return;
	}

	m_user_id = response->body().m_user_id;
	m_user_account = response->body().m_user_account;
	m_token = response->body().m_token;
	delete response;

	uuLOG("%s aone bind ok, request success !!!", LOG_TAG);

	m_cb(0, map<string, string>());
}

void AoneSDK::OAuthBindAsync(AONESDK_CB cb)
{
	if (!m_init_ok) {
		cb(-3, map<string, string>());
        logException(-3, "[OAuthBindAsync]");
		return;
	}

	m_cb = cb;

	OAuthBindRequest request;
	request.body().m_oauth_type = m_oauth_type;
	request.body().m_oauth_token = m_oauth_token;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
    //德国12暂用---服务器更新之后删除
    if (m_oauth_type == "apple") {
        request.body().m_oauth_type = "gamecenter";
    }

	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::OAuthBindCallback);
}

void AoneSDK::OAuthBindCallback(int ret, byte *responseBytes, int len)
{
	if (ret < 0)
	{
		uuLOG("%s oauth bind fail, api server send_recv error or timeout", LOG_TAG);
		m_cb(ret, map<string, string>());
        logException(ret, "oauth bind fail, api server send_recv error or timeout");
		return;
	}
	OAuthBindResponse* response = (OAuthBindResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	// 判断返回码
	if (response->get_result() != 0)
	{
		uuLOG("%s oauth bind fail, apiserver return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		m_cb(code, map<string, string>());
        logException(code, "oauth bind fail, apiserver return code");
		return;
	}

	m_user_id = response->body().m_user_id;
	m_user_account = response->body().m_user_account;
	m_token = response->body().m_token;
	delete response;

	uuLOG("%s oauth bind ok, request success !!!", LOG_TAG);

	m_cb(0, map<string, string>());
}

/*
 *	third party login begin
 */
void AoneSDK::thirdPartyLogin(AONESDK_CB cb){
	assert(cb);
	uuLOG("%s native AoneSdk thirdPartyLogin begin !!!", LOG_TAG);
	
	if (m_init_ok == false)
	{
		cb(-3, map<string, string>());
        logException(-3, "[thirdPartyLogin]");
		return;
	}
	
	if (getAuthTypeIsTest())
	{
		quickLogin();
		cb(0 , map<string, string>());
		return;
	}

	if (getChannelIsTest() && m_oauth_type == "")
	{
		mLoginSdkName = "";
		sdkLoginAsync(AonesdkConfig::getInstance()->getTestToken(), AonesdkConfig::getInstance()->getTestToken(), cb);
		return;
	}

	if (m_oauth_type != "") {
		mLoginSdkName = uuSdkConfigHelper::getSdkNameByParam("oauth_type", m_oauth_type.c_str()); // 第三方登陆方式
		if (mLoginSdkName == "") {
			uuLOG("%s no default user sdk!, it maybe a channel that not need a user sdk.", LOG_TAG);
			quickLogin();
			cb(0 , map<string, string>());
			return;
		}
	}
	if (mLoginSdkName == "") {
		mLoginSdkName = uuSdkManager::getInstance()->getDefaultUserSdkName();
	}

	this->thirdPartyLogin(mLoginSdkName.c_str(), cb);
}

void AoneSDK::thirdPartyLogin(const string& sdkName, AONESDK_CB cb)
{
	uuLOG("%s native AoneSdk thirdPartyLogin2 begin !!!", LOG_TAG);
	m_LogIncb = cb;
	m_sdkName = sdkName;
	uuSdkManager::getInstance()->login(sdkName, this, (uuSdkResultHandler)&AoneSDK::thirdPartyLoginCallback);
}

void AoneSDK::thirdPartyLoginCallback(uuESdkResultType type, map<string, string> params)
{
	uuLOG("%s native AoneSdk thirdPartyLoginCallback begin !!!", LOG_TAG);
	string session = "";
	string uid = "";
	int retCode = -1;

	switch (type)
	{
	case eSDK_RESULT_SUCCESS:
	case eSDK_RESULT_SWITCHACCOUNT:
		if (params.find("session") != params.end())
		{
			if (m_oauth_type == "")
			{
				m_loginResult = type;

				session = params["session"];
				uid = params["uid"];
				/*uuLOG("session is: %s", session.c_str());
				uuLOG("uid is: %s", uid.c_str());*/
				if (m_version_info.m_channel.find("aone_android") == 0 || m_version_info.m_channel.find("aone_ios") == 0) {
					uuLOG("thirdPartyLoginCallback find aone_android or aone_ios");
					m_user_id = atoi(uid.c_str());
					m_user_account = "";
					m_token = session;

					const char* eventId = uuSdkConfig::getInstance()->getContentValueByKey(m_sdkName.c_str(), "event_login");
					if (eventId != NULL) {
						m_eventLoginType = eventId;
					} else {
						m_eventLoginType = EVENT_LOGIN_TYPE_NORMAL;
					}
					m_LogIncb(m_loginResult, map<string, string>());
				} else {
					thirdPartySdkLoginAsync(uid, session, m_LogIncb);
				}
				return;
			}
			else
			{
				uuLOG("m_version_info.m_ext:%s", m_version_info.m_ext.c_str());
				
				if ((m_oauth_type == "facebook" || m_oauth_type == "google") && params["session"] != ""){
					m_thirdparty_account = params["session"];
				}else if (params["uid"] != ""){//m_oauth_type：twitter/apple/gamecenter,jp use
					m_thirdparty_account = params["uid"];
				}
				if (params["uid"] != ""){
					m_thirdparty_user_id = params["uid"];
				}
				if (isJpSpecial()){
					//此时该设备没有登录其他任何账号，直接调用第三方登录
					if (0 == m_user_id){
						m_is_jp_first_third_login = true;
						//AoneSDK::AoneUserInfo aoneUserInfo = getUserInfo();
						//if (aoneUserInfo.thirdUserId == m_thirdparty_userid){
						uuLOG("(thirdPartyLoginCallback)m_thirdparty_account:%s",m_thirdparty_account.c_str());
						aoneLoginAsync(m_thirdparty_account,"",m_LogIncb);
						return;
						//}
						//else{
						//	if (m_LogIncb){
						//		m_LogIncb(type, params);
						//		return;
						//	}
						//}
					}
					else{

						//1.开始进行绑定第三方账号的操作
						//1.1 在游戏里，此时有aone账号的userId。根据userId查询是否有第三方进行绑定
						m_thirdparty_user_id = uid = params["uid"];
						m_thirdparty_token = session = params["session"];
						uuLOG("(thirdPartyLoginCallback)m_oauth_type:%s",m_oauth_type.c_str());
						aoneBindOtherAccount(m_thirdparty_account,m_LogIncb);
						return;
					}
					
				}
                uuLOG("%s third party login success, begin to oauth", LOG_TAG);
				m_oauth_token = params["session"];
				uuLOG("%s oauth token is: %s", LOG_TAG, session.c_str());
				OAuthInfoAsync(m_LogIncb);
				return;
			}
		}
		else
		{
			uuLOG("%s login success, but no session!!!", LOG_TAG);
			type = eSDK_RESULT_FAILED;
		}
		break;
	case eSDK_RESULT_LOGOUT:
		//cleanRoleData();
		break;
	default:
		uuLOG("%s login failed, the result type is %d", LOG_TAG, type);
		break;
	}

	if (m_LogIncb)
	{
		m_LogIncb(type, params);
//		m_LogIncb = NULL;
	}
}

void AoneSDK::thirdPartySdkLoginAsync(const string& userId, const string& token, AONESDK_CB cb)
{
	if (!m_init_ok) {
		cb(-3, map<string, string>());
        logException(-3, "[thirdPartySdkLoginAsync]");
		return;
	}

	m_LogIncb = cb;

	//uuLOG("token is: %s", token.c_str());
	//uuLOG("userId is: %s", userId.c_str());

	SDKLoginRequest request;
	request.body().m_sdk_userid = userId;
	request.body().m_sdk_token = token;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	string sdkVersion = uuSdkManager::getInstance()->callStringFunc(mLoginSdkName, "getSDKVersion");
	uuLOG("%s commit sdk name: %s, sdk version: %s", LOG_TAG, mLoginSdkName.c_str(), sdkVersion.c_str());
	request.body().m_sdk_version = sdkVersion;
	
	//SDKLoginResponse* response = NULL;
	//int ret = apiserver_send_recv(&request, (Aone2BaseMessage**)&response);

	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::thirdPartySdkLoginCallback);
}

void AoneSDK::thirdPartySdkLoginCallback(int ret, byte *responseBytes, int len)
{
	if (ret < 0)
	{
		uuLOG("%s channel thirdPartySdk login fail, apiserver send_recv error or timeout", LOG_TAG);
		m_LogIncb(ret, map<string, string>());
        logException(ret, "channel thirdPartySdk login fail, apiserver send_recv error or timeout");
		return;
	}

	SDKLoginResponse *response = (SDKLoginResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	// 判断返回码
	if (response->get_result() != 0)
	{
		uuLOG("%s channel thirdPartySdk login fail, apiserver return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		m_LogIncb(code, map<string, string>());
        logException(code, "channel thirdPartySdk login fail, apiserver return code");
		return;
	}

	uuLOG("%s channel thirdPartySdk login ok, request success !!!", LOG_TAG);

	m_user_id = response->body().m_user_id;
	m_user_account = response->body().m_user_account;
	m_token = response->body().m_token;
	delete response;

	if (m_user_id == 0) {
		uuLOG("%s thirdPartySdkLogin receive user id is null!", LOG_TAG);
	}
	if (m_user_account == "") {
		uuLOG("%s thirdPartySdkLogin receive user account is null!", LOG_TAG);
	}
	if (m_token == "") {
		uuLOG("%s thirdPartySdkLogin receive token is null!", LOG_TAG);
	}
	
	const char* eventId = uuSdkConfig::getInstance()->getContentValueByKey(m_sdkName.c_str(), "event_login");
	if (eventId != NULL) {
		m_eventLoginType = eventId;
	} else {
		m_eventLoginType = EVENT_LOGIN_TYPE_NORMAL;
	}
	m_LogIncb(m_loginResult, map<string, string>());
	//userLoginAsync(m_LogIncb);
}
/*
 *	third party login  end
 */

void AoneSDK::login(AONESDK_CB cb)
{
	if (m_begin_login_time != m_ineffective_time){
		std::chrono::duration<double, std::ratio<1, 1>> interval(std::chrono::high_resolution_clock::now() - m_begin_login_time);
		uuLOG("%s login, interval is:%f", LOG_TAG, interval.count());

		if (interval.count() <= 3 && interval.count() >= 0){
			uuLOG("%s login, don't frequent login!", LOG_TAG);
			m_begin_login_time = std::chrono::high_resolution_clock::now();
			return;
		}
	}
	m_begin_login_time = std::chrono::high_resolution_clock::now();

	assert(cb);
	uuLOG("%s login, native AoneSdk login begin !!!", LOG_TAG);
    
	if (m_init_ok == false)
	{
		m_begin_login_time = m_ineffective_time;

		cb(-3, map<string, string>());
		uuLOG("%s login, login failed, init not ok", LOG_TAG);
        logException(-3, "login failed, init not ok");
		return;
	}

	if (getAuthTypeIsTest())
	{
		uuLOG("%s login, auth type is test, use quick login", LOG_TAG);
		m_begin_login_time = m_ineffective_time;

		quickLogin();
		cb(0 , map<string, string>());
		return;
	}

	if (getChannelIsTest() && m_oauth_type == "")
	{
		uuLOG("%s login, channel is test, auth type is empty, use sdk login", LOG_TAG);
		mLoginSdkName = "";
		sdkLoginAsync(AonesdkConfig::getInstance()->getTestToken(), AonesdkConfig::getInstance()->getTestToken(), cb);
		return;
	}

	if (m_oauth_type != "") {
		mLoginSdkName = uuSdkConfigHelper::getSdkNameByParam("oauth_type", m_oauth_type.c_str()); // 第三方登陆方式
        uuLOG("%s oauth type is not nil ,oauth type = %s , login sdk name = %s", LOG_TAG, m_oauth_type.c_str(), mLoginSdkName.c_str());
		if (mLoginSdkName == "") {
			uuLOG("%s no default user sdk!, it maybe a channel that not need a user sdk.", LOG_TAG);
			quickLogin();
			m_begin_login_time = m_ineffective_time;
			cb(0 , map<string, string>());
			return;
		}
	} else
    {// add for third party and 01Game SDK
        mLoginSdkName = uuSdkManager::getInstance()->getDefaultUserSdkName();
    }
    
	if (mLoginSdkName == "")
    {
		mLoginSdkName = uuSdkManager::getInstance()->getDefaultUserSdkName();
        uuLOG("%s default login sdk name is %s", LOG_TAG, mLoginSdkName.c_str());
	}
    
    uuLOG("%s oauth type = %s , login sdk name = %s", LOG_TAG, m_oauth_type.c_str(), mLoginSdkName.c_str());
	this -> login(mLoginSdkName.c_str(), cb);
}

void AoneSDK::login(const string& sdkName, AONESDK_CB cb)
{
	m_LogIncb = cb;
	m_sdkName = sdkName;
	if(!isAppOK("-12")){
		return;
	}
	uuSdkManager::getInstance()->login(sdkName, this, (uuSdkResultHandler)&AoneSDK::loginCallback);
}

void AoneSDK::logout(AONESDK_CB cb)
{
	assert(cb);
	uuLOG("%s native AoneSdk logout begin !!!", LOG_TAG);

	if (m_init_ok == false)
	{
		cb(-3, map<string, string>());
        logException(-3, "[logout]");
		return;
	}
	

	if (getChannelIsTest())
	{
		cb(4, map<string, string>());
        logException(4, "m_channel == test");
		return;
	}

	if (mLoginSdkName == "")
    { // quickLogin or aoneLogin
        uuLOG("%s login sdk name is nil", LOG_TAG);
		cb(0, map<string, string>());
		return;
	}

    uuLOG("%s logout sdk = %s", LOG_TAG, mLoginSdkName.c_str());
	this->logout(mLoginSdkName.c_str(), cb);
}

void AoneSDK::aoneBindOtherAccount(string newAccount,AONESDK_CB cb){
	if (!m_init_ok) {
		cb(-3, map<string, string>());
		logException(-3, "[aoneBindOtherAccount]");
		return;
	}
	if(isJpSpecial()){
		string m_jp_prefix = "jp_special=";
		m_version_info.m_ext = m_jp_prefix.append(m_oauth_type);
	}
	
	m_LogIncb = cb;
	m_thirdparty_account = newAccount;
	AoneBindOtherAccountRequest request;
	request.body().m_user_id = m_user_id;
	request.body().m_type = m_oauth_type;
	request.body().m_new_account = newAccount;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	
	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::aoneBindOtherAccountCallback);
}

void AoneSDK::aoneBindOtherAccountCallback(int ret, byte *responseBytes, int len){
	if (ret < 0)
	{
		uuLOG("%s aone Bind OtherAccount fail, apiserver send_recv error or timeout", LOG_TAG);
		m_LogIncb(ret, map<string, string>());
		logException(ret, "aone Bind OtherAccount fail, apiserver send_recv error or timeout");
		return;
	}
	uuLOG("(aoneBindOtherAccountCallback)m_version_info.m_ext:%s",m_version_info.m_ext.c_str());
	AoneBindOtherAccountResponse *response = (AoneBindOtherAccountResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	// 判断返回码
	if (response->get_result() != 0)
	{
		uuLOG("%s aone Bind OtherAccount fail, apiserver return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		m_LogIncb(code, map<string, string>());
		logException(code, "aone Bind OtherAccount fail, apiserver return code");
		return;
	}

	uuLOG("%s aone Bind OtherAccount login ok, request success !!!", LOG_TAG);
	
	delete response;
	
	if (isJpSpecial()){
		AoneSDK::AoneUserInfo aoneUserInfo = getUserInfo();
		string account = aoneUserInfo.account;
		string password = aoneUserInfo.password;
		string type = aoneUserInfo.type;
		string userId = aoneUserInfo.userId;
		
		if (m_thirdparty_account != ""){//第三方账号登录成功后返回的sessio或者uid，如：Twitter等，华为等(主要用于第三方的绑定操作)
			//deleteUserInfo();
			saveUserInfo(account, password, "jp_special", userId, m_thirdparty_account, m_thirdparty_user_id, m_oauth_type);
			//if (m_is_jp_third_login){
				//aoneLoginAsync(m_thirdparty_account,"",m_LogIncb);
				//return;
			//}
		}
		
	}
	m_LogIncb(m_loginResult, map<string, string>());
}

void AoneSDK::aoneUnbindOtherAccount(AONESDK_CB cb){
	if (!m_init_ok) {
		cb(-3, map<string, string>());
		logException(-3, "[aoneUnbindOtherAccount]");
		return;
	}

	m_LogIncb = cb;
	AoneUnbindOtherAccountRequest request;
	request.body().m_user_id = m_user_id;
	request.body().m_type = m_oauth_type;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::aoneUnbindOtherAccountCallback);
}

void AoneSDK::aoneUnbindOtherAccountCallback(int ret, byte *responseBytes, int len){
	if (ret < 0)
	{
		uuLOG("%s aone unBind OtherAccount fail, apiserver send_recv error or timeout", LOG_TAG);
		m_LogIncb(ret, map<string, string>());
		logException(ret, "aone unBind OtherAccount fail, apiserver send_recv error or timeout");
		return;
	}

	AoneUnbindOtherAccountResponse *response = (AoneUnbindOtherAccountResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	// 判断返回码
	if (response->get_result() != 0)
	{
		uuLOG("%s aone unBind OtherAccount fail, apiserver return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		m_LogIncb(code, map<string, string>());
		logException(code, "aone unBind OtherAccount fail, apiserver return code");
		return;
	}

	uuLOG("%s aone unBind OtherAccount login ok, request success !!!", LOG_TAG);

	delete response;
	
	if (isJpSpecial()){
		AoneSDK::AoneUserInfo aoneUserInfo = getUserInfo();
		string account = aoneUserInfo.account;
		string password = aoneUserInfo.password;
		string type = aoneUserInfo.type;
		string userId = aoneUserInfo.userId;

		deleteUserInfo();
		saveUserInfo(account, password, "jp_special", userId, "", "","");
		uuLOG("aoneUnbindOtherAccount:========================");
		//aoneBindOtherAccount(m_thirdparty_account,m_LogIncb);
		//return;
	}

	m_LogIncb(m_loginResult, map<string, string>());
}

int AoneSDK::aoneQueryBindOtherAccount(){
	if (!m_init_ok) {
		//cb(-3, map<string, string>());
		logException(-3, "[aoneQueryBindOtherAccount_]");
		return -3;
	}
	
	AoneQueryBindOtherAccountRequest request;
	request.body().m_user_id = m_user_id;
	request.body().m_type = m_oauth_type;
	//
	AoneQueryBindOtherAccountResponse *response = NULL;
	int ret = apiserver_send_recv(&request, (Aone2BaseMessage**)&response);
	if (ret < 0)
	{
		uuLOG("%s quick other account fail, apiserver send_recv error or timeout", LOG_TAG);
		return ret;
	}

	if (response->get_result() != 0)
	{
		int code = response->get_result();
		uuLOG("%s quick other account, apiserver return code is %d", LOG_TAG, code);
		delete response;
		return code;
	}

	uuLOG("%s quick other account ok, request seccess!", LOG_TAG);

	string account = response->body().m_account;
	uuLOG("aone query Bind OtherAccount login ok, request success:%s", account.c_str());
	delete response;
	return (account != "") ? 0 : 1;
}

void AoneSDK::aoneQueryBindOtherAccount(AONESDK_CB cb){
	if (!m_init_ok) {
		cb(-3, map<string, string>());
		logException(-3, "[aoneQueryBindOtherAccount]");
		return;
	}

	m_LogIncb = cb;
	AoneQueryBindOtherAccountRequest request;
	request.body().m_user_id = m_user_id;
	request.body().m_type = m_oauth_type;
	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::aoneQueryBindOtherAccountCallback);
}

void AoneSDK::aoneQueryBindOtherAccountCallback(int ret, byte *responseBytes, int len){
	if (ret < 0)
	{
		uuLOG("%s aone unBind OtherAccount fail, apiserver send_recv error or timeout", LOG_TAG);
		m_LogIncb(ret, map<string, string>());
		logException(ret, "aone query Bind OtherAccount fail, apiserver send_recv error or timeout");
		return;
	}

	AoneQueryBindOtherAccountResponse *response = (AoneQueryBindOtherAccountResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	// 判断返回码
	if (response->get_result() != 0)
	{
		uuLOG("%s aone query Bind OtherAccount fail, apiserver return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		m_LogIncb(code, map<string, string>());
		logException(code, "aone query Bind OtherAccount fail, apiserver return code");
		return;
	}

	uuLOG("%s aone query Bind OtherAccount login ok, request success !!!", LOG_TAG);
	string account = response->body().m_account;
	uuLOG("aone query Bind OtherAccount login ok, request success:%s", account.c_str());
/**
	if ("" == account){
		//如果查询到的账号为"",则是没有进行过绑定，现在绑定
		if ((m_oauth_type == "twitter" || m_oauth_type == "gameCenter" || m_oauth_type == "apple") && m_thirdparty_account != ""){
			uuLOG("begin bind after,query bind");
			aoneBindOtherAccount(m_thirdparty_account, m_LogIncb);
			return;
		}
		else if ((m_oauth_type == "facebook" || m_oauth_type == "google") && m_thirdparty_account != ""){
			aoneBindOtherAccount(m_thirdparty_account, m_LogIncb);
			return;
		}
		
	}
	else{
		//如果查询到的账号不为"",则是有进行过绑定，需要先解绑
		//aoneBindOtherAccount();
		aoneUnbindOtherAccount(m_LogIncb);
		return;
	}**/
	delete response;
	map<string, string> data;
	data["otherAccount"] = account;
	m_LogIncb(m_loginResult, data);
}

void AoneSDK::logout(const string& sdkName, AONESDK_CB cb)
{
	m_LogIncb = cb;
	m_sdkName = sdkName;

	uuSdkManager::getInstance()->logout(sdkName, this, (uuSdkResultHandler)&AoneSDK::logoutCallback);
}

void AoneSDK::exit()
{
	std::vector<PluginParam*> params;
	uuSdkManager::getInstance()->callFuncWithParam("exit", params);
	cplog_uninit();
}

bool AoneSDK::hasExitUI()
{
	std::vector<PluginParam*> params;
	return uuSdkManager::getInstance()->callBoolFuncWithParam("hasExitUI", params);
}

void AoneSDK::setShareSdk( const char* sdkName )
{
	m_shareSdk = (sdkName != NULL) ? sdkName : "";
}

void AoneSDK::share(map<string, string> params, AONESDK_CB cb)
{
    string useSystemShare = "";
    string shareSdkName = "";
    map<string, string>::iterator it;
    it = params.find("useSystemShare");
    if (it != params.end()) {
        useSystemShare = it -> second;
    }
    
    if (useSystemShare != "" && strcmp(useSystemShare.c_str(), "true") == 0) {
        systemShare(params, cb);
        return;
    }
	uuLOG("m_shareSdk:%s",m_shareSdk.c_str());
	share(m_shareSdk, params, cb);
}

void AoneSDK::share(const string& sdkName, map<string, string> params, AONESDK_CB cb)
{
	m_sdkName = sdkName;
	m_cb = cb;
	m_params = params;
	uuSdkManager::getInstance()->share(sdkName, m_params, this, (uuSdkResultHandler)&AoneSDK::shareCallback);
}

void AoneSDK::systemShare(map<std::string, std::string> params, aonesdk::AONESDK_CB cb)
{
    //uuDevice::systemShare(params, cb);
}
    
#include "uuSdkResultListener.h"

void AoneSDK::pay(const string& product_id,unsigned int purchaseNum, const string& cp_ext, AONESDK_CB cb)
{
	if (m_begin_pay_time != m_ineffective_time){
		std::chrono::duration<double, std::ratio<1, 1>> interval(std::chrono::high_resolution_clock::now() - m_begin_pay_time);
		uuLOG("%s pay interval is:%f", LOG_TAG, interval.count());

		if (interval.count() <= 3 && interval.count() >= 0){
			uuLOG("%s don't frequent pay!", LOG_TAG);
			m_begin_pay_time = std::chrono::high_resolution_clock::now();
            cb(-3, map<string, string>());
			return;
		}
	}
	
	m_cp_product_id = product_id;

	m_begin_pay_time = std::chrono::high_resolution_clock::now();

	uuLOG("%s native AoneSdk pay begin !!!", LOG_TAG);

	if (m_init_ok == false || isRoleAndGroupValid() == false)
	{
		m_begin_pay_time = m_ineffective_time;

        uuLOG("%s select_role ID = %d , select_group ID = %d, select_role_cp ID = %s , select_role_cp ID = %s", LOG_TAG, m_select_role.ID, m_select_group.ID, m_select_role_cp.ID.c_str(), m_select_group_cp.ID.c_str());
		cb(-3, map<string, string>());
        logException(-3, "[pay]");
		return; // 未正确初始化
	}

	string _product_id = product_id;
	uuLOG("product_id:%s",product_id.c_str());
	if (m_productIdMap.size() > 0) 
	{
		if (m_productIdMap.find(product_id) != m_productIdMap.end())
		{
			_product_id = m_productIdMap[product_id];
		}
		else
		{
			uuLOG("%s can't find cp product %s in extInfo", LOG_TAG, product_id.c_str());
		}
	}

	if (_product_id != "")
    {
		map<string, GameProduct>::iterator it = m_channel_products.find(_product_id);
		if (it == m_channel_products.end())
		{
			m_begin_pay_time = m_ineffective_time;

            uuLOG("%s pay fail, not find the  product id = %s", LOG_TAG, product_id.c_str());
			cb(-9, map<string, string>());
            logException(-9, "no products");
			return;
		}
	}

	m_select_product_id = _product_id;
	m_cp_ext = cp_ext;
	m_purchase_num = purchaseNum;

	std::map<string, string> info2;
	char buf[128] = {0};
	sprintf(buf, "%d", m_select_role.ID);
	info2["aoneRoleId"] = buf;
    
    std::vector<PluginParam *> allParams;
    PluginParam *pluginParam = new PluginParam(info2);
    allParams.push_back(pluginParam);
    m_last_payno = uuSdkManager::getInstance()->callStringFuncWithParam("generateSpecialOrder", allParams);
    uuLOG("%s callStringFunc return order = [%s]", LOG_TAG, m_last_payno.c_str());
    
	const char *ret = uuSdkConfig::getInstance() -> getInitValueByKey(m_sdkName.c_str(), "short_order");
	const char *ret2 = uuSdkConfig::getInstance() -> getInitValueByKey(m_sdkName.c_str(), "num_order");
    
    string groupID = m_select_group_cp.ID;
    string roleID = m_select_role_cp.ID;
    
	if (ret != NULL && m_last_payno.empty()) {
		
        m_last_payno = generate_payno_unicom(m_appid, atoi(groupID.c_str()), atoi(roleID.c_str()));
        
	} else if (ret2 != NULL && m_last_payno.empty()){
		
        m_last_payno = generate_payno_num(m_appid, atoi(groupID.c_str()), atoi(roleID.c_str()));
        
	} else if (m_last_payno.empty()){
       
        m_last_payno = generate_payno(m_appid, atoi(groupID.c_str()), atoi(roleID.c_str()));
		
	}
    
	if (getChannelIsTest() && (m_pay_channel == "" || getPayChannelIsTest()))
	{
		m_begin_pay_time = m_ineffective_time;
		cb(0, map<string, string>());
		return;
	}
	string payChannel = (m_pay_channel == "") ? m_version_info.m_channel : m_pay_channel;
	m_commitAfterPay = false;
	m_sdkName = uuSdkConfigHelper::getSdkNameByParam("payChannel", m_pay_channel.c_str());
    
	if (m_sdkName == "")
    { // 渠道支付方式
		m_sdkName = uuSdkManager::getInstance()->getDefaultIapSdkName(); // not a good solution
		if (m_sdkName == "")
        {
			m_begin_pay_time = m_ineffective_time;
			uuLOG("%s no default iap sdk!, it maybe a channel that not need a iap sdk.", LOG_TAG);
			cb(-8, map<string, string>());
            logException(-8, "no default iap sdk!, it maybe a channel that not need a iap sdk.");
			return;
		}
	}
    uuLOG("%s hhh ----- m_sdkName is = %s", LOG_TAG, m_sdkName.c_str());
	if (isUse01gameAndNotBackEnd(uuESdkType::eSDK_PAY))
	{// outside aonesdk called, and invoke the inside aoensdk, just for use ios 01Game SDK
        uuLOG("%s sdkName = %s , productId = %s ,purchaseNum = %u, cpext = %s", LOG_TAG,m_sdkName.c_str(), m_select_product_id.c_str() ,m_purchase_num, m_cp_ext.c_str());
		this -> pay(m_sdkName.c_str(), m_select_product_id,purchaseNum, m_cp_ext, false, cb);
		return;
	}
    
    PayStorage::PayRecord info;
    info.record[0] = m_last_payno;
    info.record[1] = _product_id;
    info.record[2] = payChannel;
    info.record[3] = m_cp_ext;
    info.record[4] = "";
    info.cpID[0] = m_select_role_cp.ID;
    info.cpID[1] = m_select_group_cp.ID;
    info.user_id = m_user_id;
    info.role_id = m_select_role.ID;
	info.purchase_num= purchaseNum;
    m_storage.save(info);

    ret = uuSdkConfig::getInstance()->getInitValueByKey(m_sdkName.c_str(), "commit_after_pay");
    if (ret != NULL) { // 第三方支付方式，判断是否需要在支付完成后提交订单
        m_commitAfterPay = true;
    }
    
	if (AonesdkConfig::getInstance()->getAgainstFakePay() == "true")
	{// general order in server
		uuLOG("%s hhh----------againstFakePay", LOG_TAG);
        uuLOG("%s [cpext] = %s", LOG_TAG,m_cp_ext.c_str());
		payRequestAsync(m_last_payno, m_select_product_id, purchaseNum, payChannel, m_cp_ext, cb);
		return;
	} 

	if (!m_commitAfterPay && m_select_product_id != "")
    {// general order in server
        uuLOG("%s ------payRequestAsync--------", LOG_TAG);
        uuLOG("%s [cpext] = %s", LOG_TAG, m_cp_ext.c_str());
		payRequestAsync(m_last_payno, m_select_product_id, purchaseNum, payChannel, m_cp_ext, cb);
	} else
    {
		this->pay(m_sdkName.c_str(), m_select_product_id, purchaseNum, m_cp_ext, false, cb);
	}
}

string __json_value_to_quoted_string(const string& str)
{
	string result = "\"";
	for (size_t i = 0; i < str.size(); i++)
	{
		switch(str[i])
		{
		case '\"':
			result += "\\\"";
			break;
		case '\\':
			result += "\\\\";
			break;
		case '\b':
			result += "\\b";
			break;
		case '\f':
			result += "\\f";
			break;
		case '\n':
			result += "\\n";
			break;
		case '\r':
			result += "\\r";
			break;
		case '\t':
			result += "\\t";
			break;
		default:
			result += str[i];
		}
	}
	result += "\"";
	return result;
}

void AoneSDK::pay(const string &sdkName, const string &product_id,unsigned int purchaseNum, const string &cp_ext, const bool thirdPartyCallback, AONESDK_CB cb)
{
	assert(cb);
    
    LogEvent("Inital P", true);
    if(!isAppOK("-15")){
		return;
	}
	m_cb = cb;
	m_sdkName = sdkName;
	GameProduct product;
	map<string, string> params;
	if (product_id != "") {
		map<string, GameProduct>::iterator it = m_channel_products.find(product_id);
		if (it == m_channel_products.end())
		{
			m_begin_pay_time = m_ineffective_time;

			cb(-9, map<string, string>());
            logException(-9, "no products");
			return;
		}
		product = it->second;
	} else {
		product.Amount = 1.0f;
		product.Amount_usd = 1.0f;
		product.Describe = "none";
		product.Name = "none";
		product.ID = "";
		product.Coin = 1;
		product.Currency = "none";
		map<string, GameProduct>::iterator it;
		string jsonStr = "[";
		for (it = m_channel_products.begin(); it != m_channel_products.end(); ) {
			jsonStr.append("{");
			GameProduct product = it->second;
			
			jsonStr.append("\"id\":");
			jsonStr.append(__json_value_to_quoted_string(product.ID));
			jsonStr.append(",");
			
			jsonStr.append("\"name\":");
			jsonStr.append(__json_value_to_quoted_string(parse_for_lang(product.Name)));
			jsonStr.append(",");
			
			jsonStr.append("\"describe\":");
			jsonStr.append(__json_value_to_quoted_string(parse_for_lang(product.Describe)));
			jsonStr.append(",");
			
			jsonStr.append("\"currency\":");
			jsonStr.append(__json_value_to_quoted_string(product.Currency));
			jsonStr.append(",");
			
			jsonStr.append("\"amount\":");
			char buf[30] = {};
			snprintf(buf, 29, "%.2f", product.Amount);
			jsonStr.append(__json_value_to_quoted_string(buf));
			jsonStr.append(",");
			
			jsonStr.append("\"amount_usd\":");
			snprintf(buf, 29, "%.2f", product.Amount_usd);
			jsonStr.append(__json_value_to_quoted_string(buf));
			jsonStr.append(",");

			jsonStr.append("\"coin\":");
			snprintf(buf, 29, "%u", product.Coin);
			jsonStr.append(__json_value_to_quoted_string(buf));
			jsonStr.append(",");

			jsonStr.append("\"type\":");
            jsonStr.append(__json_value_to_quoted_string(product.Type));
			jsonStr.append("");

			++it;
			if (it != m_channel_products.end()) {
				jsonStr.append("},");
			} else {
				jsonStr.append("}");
			}
		}
		jsonStr.append("]");
		params["product_list"] = jsonStr;
	}
	char price[30] = {};
	char price_usd[30] = {};
	char coins[30] = {};
	char purchase_num[30] = {};
	snprintf(price, 29, "%.2f", product.Amount);
	snprintf(price_usd, 29, "%.2f", product.Amount_usd);
	params["order"] = m_last_payno;
	params["price"] = price;
	params["price_usd"] = price_usd;
	params["cp_ext"] = cp_ext;
	params["desc"] = parse_for_lang(product.Describe);
	params["name"] = parse_for_lang(product.Name);
	params["id"] = product.ID;
	params["cp_product_id"] = m_cp_product_id;
	snprintf(purchase_num, 29, "%d", purchaseNum);
	params["purchase_num"] = purchase_num;
	params["group_id"] = m_select_group_cp.ID;
	params["role_id"] = m_select_role_cp.ID;
	snprintf(coins, 29, "%u", product.Coin);
	params["coin"] = coins;
	params["discount"] = product.Discount;

	char actualDiscount[30] = {};
	snprintf(actualDiscount, 29, "%d", m_actual_discount);
	params["actual_discount"] = actualDiscount;
    
    char level[30] = {};
    snprintf(level, 29, "%d", m_select_role_cp.Level);
    params["Level"] = level;
    
	int index = m_user_account.find("@");
	params["currency"] = product.Currency;
	if (index != m_user_account.npos)
	{
		string sdk_user_id = m_user_account.substr(0, index);
		params["user_id"] = sdk_user_id;
	}
	uuLOG("%s pay m_user_id is: %d", LOG_TAG, m_user_id);
	char buf[128] = { 0 };
	sprintf(buf, "%d", m_user_id);
	params["aone_user_id"] = buf;
	if(thirdPartyCallback)
    {
		uuSdkManager::getInstance()->pay(sdkName, params, this, (uuSdkResultHandler)&AoneSDK::thirdPartyWorkCallback);
	} else
    {
		uuSdkManager::getInstance()->pay(sdkName, params, this, (uuSdkResultHandler)&AoneSDK::payCallback);
	}
}

void AoneSDK::shareCallback(uuESdkResultType type, map<string, string> params)
{
	switch (type)
	{
	case eSDK_RESULT_SUCCESS:
		{
			uuLOG("%s share success!", LOG_TAG);
			if (m_params.find("option") != m_params.end()) {
				string option = m_params["option"];
				if (option == "share") {
					const char* eventId = uuSdkConfig::getInstance()->getContentValueByKey(m_sdkName.c_str(), "event_share");
					if (eventId != NULL) {
						LogEvent(eventId, true);
					} else {
						LogEvent("Share", true);
					}
				} else if (option == "invite") {
					const char* eventId = uuSdkConfig::getInstance()->getContentValueByKey(m_sdkName.c_str(), "event_invite");
					if (eventId != NULL) {
						LogEvent(eventId, true);
					}
				}
			}
		}
		break;
	default:
		uuLOG("%s share failed, the result type is %d", LOG_TAG, type);
		break;
	}

	if (m_cb)
	{
		m_cb(type, params);
//		m_cb = NULL;
	}
}

void AoneSDK::loginCallback(uuESdkResultType type, map<string, string> params)
{
	string session = "";
	string uid = "";
//	int retCode = -1;

	uuLOG("%s login call back\n", LOG_TAG);

	switch (type)
	{
	case eSDK_RESULT_SUCCESS:
	case eSDK_RESULT_SWITCHACCOUNT:
		if (params.find("session") != params.end())
		{
			if (m_oauth_type == "")
			{
				m_loginResult = type;

				session = params["session"];
				uid = params["uid"];
				uuLOG("%s hhh---------session is: %s", LOG_TAG, session.c_str());
				uuLOG("%s hhh---------uid is: %s", LOG_TAG, uid.c_str());
				m_channel_user_id = uid;
				//
				if(!isAppOK("-13")){
					return;
				}
				if (isUse01gameAndNotBackEnd())
				{
					uuLOG("%s isUse01gameSdk true\n", LOG_TAG);
					m_user_id = atoi(uid.c_str());
					m_user_account = "";
					m_token = session;

					const char* eventId = uuSdkConfig::getInstance()->getContentValueByKey(m_sdkName.c_str(), "event_login");
					if (eventId != NULL)
                    {
						m_eventLoginType = eventId;
					} else {
						m_eventLoginType = EVENT_LOGIN_TYPE_NORMAL;
					}

					submitUserInfo();
					onAoneLoginEndAsync();
					return;
				}

				sdkLoginAsync(uid, session, m_LogIncb);
				return;
			}
			else
			{
				m_oauth_token = params["session"];
				//uuLOG("oauth token is: %s", session.c_str());
				OAuthInfoAsync(m_LogIncb);
				return;
			}
		}
		else
		{
			uuLOG("%s login success, but no session!!!", LOG_TAG);
			type = eSDK_RESULT_FAILED;
		}
		break;
	case eSDK_RESULT_LOGOUT:
		//cleanRoleData();
		break;
	default:
		uuLOG("%s login failed, the result type is %d", LOG_TAG, type);
            logException(type, "login fail");
		break;
	}

	if (m_LogIncb)
	{
		m_begin_login_time = m_ineffective_time;
		m_LogIncb(type, params);
//		m_LogIncb = NULL;
	}
}

void AoneSDK::logoutCallback(uuESdkResultType type, map<string, string> params)
{
	switch (type)
	{
	case eSDK_RESULT_SUCCESS:
		loginCallback(type, params);
		return;
	case eSDK_RESULT_LOGOUT:
	{
		uuLOG("%s logout success!", LOG_TAG);
		// 角色信息传递给sdk
		std::map<string, string> info2;
		char buf[128] = { 0 };
		sprintf(buf, "%d", m_user_id);
		info2["aoneUserId"] = buf;
		sprintf(buf, "%d", m_select_role.ID);
		info2["aoneRoleId"] = buf;
		info2["aoneRoleName"] = m_select_role.Name;
		sprintf(buf, "%d", m_select_role.Level);
		info2["aoneRoleLevel"] = buf;
		sprintf(buf, "%d", getSelectedGroup().ID);
		info2["aoneGroupId"] = buf;
		info2["aoneGroupName"] = getSelectedGroup().Name;
		sprintf(buf, "%d", m_select_role.VIP);
		info2["aoneRoleVip"] = buf;
		std::vector<PluginParam*> allParams;
		PluginParam* pluginParam = new PluginParam(info2);
		allParams.push_back(pluginParam);
		uuSdkManager::getInstance()->callFuncWithParam("roleLogout", allParams);
		delete pluginParam;
		pluginParam = NULL;
		//cleanRoleData();
		break;
	}
	default:
		uuLOG("%s logout failed, the result type is %d", LOG_TAG, type);
		break;
	}

	if (m_LogIncb)
	{
		m_LogIncb(type, params);
//		m_LogIncb = NULL;
	}
}

void AoneSDK::thirdPartyWorkCallback(uuESdkResultType type, map<string, string> params)
{
	string payChannel;
	switch (type)
	{
	case eSDK_RESULT_SUCCESS:
		{
			uuLOG("%s pay success!", LOG_TAG);
			map<string, string> purchaseParams;
			if (params.find("currency") != params.end() && params.find("price") != params.end())
            {
				purchaseParams["eventCurrency"] = params["currency"];
				purchaseParams["eventValue"] = params["price"];
				LogEvent("purchase", purchaseParams, true);
			} else {
				uuLOG("%s Error! no pay params: currency or price", LOG_TAG);
			}
			
			params["roleId"] = m_select_role_cp.ID;
			params["payType"] = m_pay_channel;

			std::vector<PluginParam*> allParams;
			PluginParam* pluginParam = new PluginParam(params);
			allParams.push_back(pluginParam);
			uuSdkManager::getInstance()-> callFuncWithParam("payResultCommit", allParams);
			delete pluginParam;
			pluginParam = NULL;
		}
		break;
	default:
		uuLOG("%s pay failed, the result type is %d", LOG_TAG, type);
		if (type != eSDK_RESULT_CANCEL) {
			string msg = "";
			if (params.find("aone_msg") != params.end()) {
				msg = params["aone_msg"];
			}
			recordSDKPayException(type, msg);
		}
		
		break;
	}
	if (m_cb)
	{
		m_begin_pay_time = m_ineffective_time;

		m_cb(type, params);
	}
}


void AoneSDK::payCallback(uuESdkResultType type, map<string, string> params)
{
	string payChannel;
	switch (type)
	{
	case eSDK_RESULT_SUCCESS:
		{
			uuLOG("%s pay success !!!", LOG_TAG);
			map<string, string> purchaseParams;
            //***
            string productId;
            if (params.find("id") != params.end()) {
                productId = params["id"];
            }
            if (productId == "")
            //***
            {//如果params没有商品信息的话，调用插件里的getCurrency和getPrice方法获取currency和price的值
				//some sdk only know price after purchase, for example, 01game card pay
				string currency = uuSdkManager::getInstance()->callStringFunc(m_sdkName, "getCurrency");
				string price = uuSdkManager::getInstance()->callStringFunc(m_sdkName, "getPrice");
				uuLOG("%s get price and currency from plugin, currency is [%s], price is [%s]", LOG_TAG, currency.c_str(), price.c_str());
				if (currency != "") params["currency"] = currency;
				if (price != "") params["price"] = price;
			}
			
			if (params.find("currency") != params.end() && params.find("price") != params.end())
            {
				purchaseParams["eventCurrency"] = params["currency"];
				purchaseParams["eventValue"] = params["price"];
				LogEvent("purchase", purchaseParams, true);
			} else
            {
				uuLOG("%s Error! no pay params: currency or price", LOG_TAG);
			}
			
			char buf[128] = {0};
			sprintf(buf, "%d", m_select_role.ID);
			params["roleId"] = buf;
			params["payType"] = m_pay_channel;
            
            char bufs[128]={0};
            sprintf(bufs, "%d", m_select_role.Level);
            params["Level"] =bufs;
            
            params["role_name"] =m_select_role.Name;
#ifdef CC_TARGET_OS_IPHONE
			if (is01gameBackEnd() || !isUse01game())
//#else
#endif
            {
                std::vector<PluginParam*> allParams;
                PluginParam* pluginParam = new PluginParam(params);
                allParams.push_back(pluginParam);
                uuSdkManager::getInstance()->callFuncWithParam("payResultCommit", allParams);
                delete pluginParam;
                pluginParam = NULL;
            }

			if (!isUse01gameAndNotBackEnd(uuESdkType::eSDK_PAY) && (m_commitAfterPay || m_select_product_id == ""))
            {   // for two aonesdk, use the internal aonesdk to commit receipt
                // for only one aonesdk, use the shared aonesdk instance to commit receipt
				uuLOG("%s payCallback: not use01gameSdk", LOG_TAG);
				payChannel = (m_pay_channel == "") ? m_version_info.m_channel : m_pay_channel;
				m_recipt = uuSdkManager::getInstance()->callStringFunc(m_sdkName, "getRecipt");
                if (m_recipt.length() < 1) {
                    string msg = "recipt is nil";
                    recordSDKPayException(type, msg);
                }
				string productId = uuSdkManager::getInstance()->callStringFunc(m_sdkName, "getProductId");
				if (productId == "") {
					productId = m_select_product_id;
				}
                
                bool isSave = false;
    
                m_storage.load();
                
                for (int i = 0; i < (int)m_storage.m_vecData.size(); i++) {
                    if (productId == m_storage.m_vecData[i].record[1]
                        && m_storage.m_vecData[i].record[4].empty()) {
                        isSave = true;
                        m_storage.m_vecData[i].record[4] = m_recipt;
                        m_storage.saveToFile();//payCallback支付成功----保存订单
                        break;
                    }
                }
                
                if (isSave == false) {
                    PayStorage::PayRecord info;
                    info.record[0] = m_last_payno;
                    info.record[1] = productId;
                    info.record[2] = payChannel;
                    info.record[3] = m_cp_ext;
                    info.record[4] = m_recipt;
                    info.cpID[0] = m_select_role_cp.ID;
                    info.cpID[1] = m_select_group_cp.ID;
                    info.user_id = m_user_id;
                    info.role_id = m_select_role.ID;
					info.purchase_num = m_purchase_num;
                    m_storage.save(info);
                }
				
                if (AonesdkConfig::getInstance()->getAgainstFakePay() == "true")
                {
                    receiptCommitAsync(m_last_payno, m_recipt, payChannel, (PAYCOMMIT_CB)&AoneSDK::payCommitResult);
                } else
                {
                    payCommitAsync(m_last_payno, productId,m_purchase_num, payChannel, m_cp_ext, m_recipt, (PAYCOMMIT_CB)&AoneSDK::payCommitResult);
                }
					
				if (m_cb)
				{
					m_begin_pay_time = m_ineffective_time;

					m_cb(type, params);
                    if (type != eSDK_RESULT_SUCCESS) {
                        logException(type, "pay unsucess");
                    }
				}
				return;
			}
		}
		break;
	default:
		uuLOG("%s pay failed, the result type is %d", LOG_TAG, type);
		if (type != eSDK_RESULT_CANCEL) {
			string msg = "";
			if (params.find("aone_msg") != params.end()) {
				msg = params["aone_msg"];
			}
			recordSDKPayException(type, msg);
		}
            
        m_storage.load();
     
        for (int i = 0; i < (int)m_storage.m_vecData.size(); i++) {
            if (m_last_payno == m_storage.m_vecData[i].record[0]) {
                m_storage.m_vecData.erase(m_storage.m_vecData.begin() + i);
                m_storage.saveToFile();//payCallback支付失败---删除订单
                break;
            }
        }
		
		break;
	}
	if (m_cb)
	{
		m_begin_pay_time = m_ineffective_time;

		m_cb(type, params);
        if (type != eSDK_RESULT_SUCCESS) {
            logException(type, "pay unsuccess");
        }
	}
}

void AoneSDK::initCallback(uuESdkResultType type, map<string, string> params)
{
	uuLOG("%s initCallback, type: [%u]", LOG_TAG, type);
	switch (type)
	{
	case eSDK_RESULT_SUCCESS:
		 uuLOG("%s initCallback, init all sdk success!", LOG_TAG);
         if (checkFirstOpen()){
            LogEvent("Install", true);
         }else{
         	uuLOG("%s not the first Install", LOG_TAG);
         }
		break;
	default:
        m_init_ok = false;
		uuLOG("%s initCallback, init all sdk failed, the result type is %d", LOG_TAG, type);
		break;
	}

	m_initResultType = type;
	m_initResultParams = params;

	bool disablePlatformVersionCheck = false;
	const std::string key = "disablePlatformVersionCheck";
	if (m_channel_params.find(key) != m_channel_params.end())
	{
		disablePlatformVersionCheck = (m_channel_params[key] == "true");
	}

	if (disablePlatformVersionCheck)
	{
		afterCheckVersion();
	}
	else
	{
		if (is01gameBackEnd() == false)
			checkVersionPlatform();
		else
			afterCheckVersion();
	}
}

void AoneSDK::initPayCallback(uuESdkResultType type, map<string, string> params)
{
    uuLOG("%s initPayCallback, init pay callback begin!", LOG_TAG);

    string sdkName = "";
    //目前苹果支付的成功回调传的msg是"[IAPOnlineApple] pay success"
    if (params.begin()->second == "[IAPOnlineApple] pay success") {
        sdkName = "IAPOnlineApple";
    }else{
        //走原来的代码流程
        sdkName = uuSdkConfigHelper::getSdkNameByParam("payChannel", m_pay_channel.c_str());
        if (sdkName == "") { // 渠道支付方式
            sdkName = uuSdkManager::getInstance()->getDefaultIapSdkName(); // not a good solution
            if (sdkName == "") {
                uuLOG("%s initPayCallback, no default iap sdk!, it maybe a channel that not need a iap sdk.", LOG_TAG);
                return;
            }
        }
    }
    m_sdkName = sdkName;
    
    string productID = uuSdkManager::getInstance()->callStringFunc(sdkName, "getProductID");
    int coin = 0;
    if(productID.empty() && sdkName != "IAPOnlineApple")
    {
        uuLOG("%s initPayCallBack, product id is null\n", LOG_TAG);

        std::vector<PluginParam*> params = std::vector<PluginParam*>();
        coin = uuSdkManager::getInstance()->callIntFuncWithParam("getCoin", params);
        if (coin == 0)
        {
            uuLOG("%s initPayCallBack, coin is zero", LOG_TAG);
            return;
        }
    }
    
    if (type == eSDK_RESULT_SUCCESS) {
        if (sdkName == "IAPOnlineApple") {
            string payChannel = "appstore";

            m_recipt = uuSdkManager::getInstance()->callStringFunc(sdkName, "getRecipt");
            if (m_recipt.length() < 1) {
                string msg = "recipt is nil";
                recordSDKPayException(type, msg);
            }
            string productId = uuSdkManager::getInstance()->callStringFunc(sdkName, "getProductID");
            if (productId == "") {
                productId = m_select_product_id;
            }
            string order = uuSdkManager::getInstance()->callStringFunc(sdkName, "getOrder");
            if (order == "") {
                order = m_last_payno;
            }
            if (AonesdkConfig::getInstance()->getAgainstFakePay() == "true")
            {//提交收据
                receiptCommitAsync(order, m_recipt, payChannel, (PAYCOMMIT_CB)&AoneSDK::payCommitResult);
            } else
            {
                payCommitAsync(order, productId,m_purchase_num, payChannel, m_cp_ext, m_recipt, (PAYCOMMIT_CB)&AoneSDK::payCommitResult);
            }
            return;
        }
        
        string receipt = uuSdkManager::getInstance()->callStringFunc(sdkName,"getRecipt");
        uuLOG("%s initPayCallBack, productID = %s,receipt = %s", LOG_TAG,productID.c_str(),receipt.c_str());
        
        if (!receipt.empty()) {
			uuLOG("%s initPayCallBack, check storage pay record, record number is %u", LOG_TAG, m_storage.m_vecData.size());
            for (int i = 0; i < m_storage.m_vecData.size(); ++i)
            {
				uuLOG("%s initPayCallBack, pay record product id is %s, receipt is %s", LOG_TAG, m_storage.m_vecData[i].record[1].c_str(), m_storage.m_vecData[i].record[4].c_str());
				if ((m_storage.m_vecData[i].record[1] == productID || isProductOKForCoin(m_storage.m_vecData[i].record[1], coin)) &&
                    m_storage.m_vecData[i].record[4].empty()) {
                    
                    m_storage.m_vecData[i].record[4] = receipt;
                    m_storage.saveToFile();//initPayCallback初始化插件获取收据不为空getRecipt---保存订单
                    
                    if (m_storage.m_store_payno.empty()) {
                        m_storage.checkRecords(this);
                    }
                    
                    break;
                }
            }
        }
    }
    else{
        uuLOG("%s initPayCallBack, callback failed!\n", LOG_TAG);
        for (int i = 0; i < m_storage.m_vecData.size(); ++i)
        {
            if (m_storage.m_vecData[i].record[1] == productID &&
                m_storage.m_vecData[i].record[4].empty()) {
                m_storage.m_vecData.erase(m_storage.m_vecData.begin() + i);
                m_storage.saveToFile();//initPayCallback初始化插件获取收据为空getRecipt---删除订单
                break;
            }
        }
    }
}

bool AoneSDK::isProductOKForCoin(string productId, int coin)
{
	if (coin <= 0)
		return false;

	if (m_channel_products.find(productId) == m_channel_products.end())
		return false;

	return m_channel_products[productId].Coin == coin;
}

void AoneSDK::adsCallback(uuESdkResultType type, map<string, string> params)
{
	uuLOG("(adsCallback)type:%d",type);
	switch (type)
	{
	case eSDK_RESULT_SUCCESS:
		uuLOG("%s ads success!", LOG_TAG);
		break;
	default:
		if (type == eSDK_RESULT_SWITCHACCOUNT){//此时表示网络错误或者未知错误
			uuLOG("%s ads failed====!", LOG_TAG);
			if(!m_platform_adsSdks.empty() && m_ad_num < m_platform_adsSdks.size()-1){
				const char* adsSdkName = m_platform_adsSdks[++m_ad_num].c_str();
				uuLOG("m_platform_adsSdks--m_adsSdk:%s",adsSdkName);
				setAdsSdk(adsSdkName);
				
				uuLOG("m_platform_adsSdks--m_adsSdk2:%s",m_adsSdk.c_str());
				//if (m_config_adsSdks.contains(m_adsSdk)) {
				if (std::find(m_config_adsSdks.begin(), m_config_adsSdks.end(), m_adsSdk) != m_config_adsSdks.end()){
					uuSdkManager::getInstance()->showAds(m_adsSdk, m_ads_info, m_pos, this, (uuSdkResultHandler)&AoneSDK::adsCallback);
					return;
				}else{
					uuLOG("Error(m_platform_adsSdks),uusdkConfig.json has no sdk:%s" , m_adsSdk.c_str());
					if (std::find(m_config_adsSdks.begin(), m_config_adsSdks.end(), m_adsSdk) == m_config_adsSdks.end()){
						//数据中心的SDK不在uusdkConfig.json中
						m_adsSdk = uuSdkConfigHelper::getDefalutAdsSdkName();
					}
					uuSdkManager::getInstance()->showAds(m_adsSdk, m_ads_info, m_pos, this, (uuSdkResultHandler)&AoneSDK::adsCallback);
					return;
				}
				
				
			}else if(!m_config_adsSdks.empty() && m_ad_num < m_config_adsSdks.size()-1){
				setAdsSdk(m_config_adsSdks[++m_ad_num].c_str());
				uuLOG("m_config_adsSdks--m_adsSdk:%s",m_adsSdk.c_str());
				//if (m_config_adsSdks.contains(m_adsSdk)) {
				if (std::find(m_config_adsSdks.begin(), m_config_adsSdks.end(), m_adsSdk) != m_config_adsSdks.end()){
					uuSdkManager::getInstance()->showAds(m_adsSdk, m_ads_info, m_pos, this, (uuSdkResultHandler)&AoneSDK::adsCallback);
					return;
				}else{
					uuLOG("Error(m_config_adsSdks),uusdkConfig.json has no sdk:%s" , m_adsSdk.c_str());
				}
			}
			
		}
		uuLOG("%s ads failed, the result type is %d", LOG_TAG, type);
		break;
	}

	if (m_cb)
	{
		m_cb(type, params);
//		m_cb = NULL;
	}
}

void AoneSDK::pushCallback(uuESdkResultType type, map<string, string> params)
{
    uuLOG("%s push %d", LOG_TAG, (int)type);
	if (m_cb) {
		m_cb(type, params);
	}
}

void AoneSDK::callFunc(const char* funcName)
{
	uuSdkManager::getInstance()->callFunc(funcName);
}

void AoneSDK::callFuncWithStringParam( const char* funcName, const char* param )
{
	std::vector<PluginParam*> params;
	PluginParam* pluginParam = new PluginParam(param);
	params.push_back(pluginParam);
	uuSdkManager::getInstance()->callFuncWithParam(funcName, params);
	delete pluginParam;
	pluginParam = NULL;
}

void AoneSDK::callFuncWithParam( const char* funcName, void* param )
{
    std::vector<PluginParam*> params;
    PluginParam* pluginParam = new PluginParam(param);
    params.push_back(pluginParam);
    uuSdkManager::getInstance()->callFuncWithParam(funcName, params);
    delete pluginParam;
    pluginParam = NULL;
}
    
void AoneSDK::callFuncWithParam(const char* funcName, PluginParam* param)
{
    std::vector<PluginParam*> params;
    params.push_back(param);
    uuSdkManager::getInstance()->callFuncWithParam(funcName, params);
}

int AoneSDK::callIntFunc( const char* funcName, void* param )
{
	std::vector<PluginParam*> params;
	PluginParam* pluginParam = NULL;
	if (param != NULL) {
		PluginParam* pluginParam = new PluginParam(param);
		params.push_back(pluginParam);
	}
	int ret = uuSdkManager::getInstance()->callIntFuncWithParam(funcName, params);
	if (pluginParam != NULL) {
		delete pluginParam;
		pluginParam = NULL;
	}
	return ret;
}

string AoneSDK::callStringFunc( const char* funcName)
{
	string ret = uuSdkManager::getInstance()->callStringFunc(funcName);
	return ret;
}

string AoneSDK::callStringFunc(const char *funcName, const char *sdkName)
{
    string ret = uuSdkManager::getInstance()->callStringFunc(sdkName, funcName);
    return ret;
}
    
string AoneSDK::callStringFuncWithParam( const char* funcName, void* param)
{
	std::vector<PluginParam*> params;
	PluginParam* pluginParam = NULL;
	if (param != NULL) {
		PluginParam* pluginParam = new PluginParam(param);
		params.push_back(pluginParam);
	}
	string ret = uuSdkManager::getInstance()->callStringFuncWithParam(funcName, params);
	if (pluginParam != NULL) {
		delete pluginParam;
		pluginParam = NULL;
	}
	return ret;
}

/*
 *	third party submitRoleData
 */
void AoneSDK::thirdPartySubmitRoleData(const std::string &roleID, const std::string &name, int level, const std::string &groupID, const std::string &groupName, int roleVip, bool isLogin, std::string action,unsigned long long roleCTime)
{
	uuLOG("%s third party submit role Data: roleID = %s , groupID = %s", LOG_TAG, roleID.c_str(), groupID.c_str());
    
	// 角色添加到列表中
	GameRoleCP role;
	role.ID = roleID;
	role.Name = name;
	role.Type = 0;
	role.Level = level;
	role.VIP = roleVip;
	role.CreateTime = "";
	role.LastLoginTime = "";
	role.LastGroupID = "";
	role.OwnerGroupID = "";
	m_roles_cp.push_back(role);

	GameGroupCP group;
	group.ID = groupID;
	group.Name = groupName;
	m_groups_cp.push_back(group);

	// 自动选中分区和角色
	m_select_role_cp = role;
	m_select_group_cp = group;

	bool isInner = isUse01game() && is01gameBackEnd();
	if ( isInner == false )
    {// outside
        
        std::map<string, string> info;
        char buf[128] = { 0 };
		sprintf(buf, "%d", m_user_id);
		info["userId"] = info["aoneUserId"] = buf;
		
        info["roleId"] = info["aoneRoleId"] = roleID;
        info["roleName"] = info["aoneRoleName"] = name;
        sprintf(buf, "%d", level);
        info["roleLevel"] = info["aoneRoleLevel"] = buf;
        info["groupId"] = info["aoneGroupId"] = groupID;
        info["groupName"] = info["aoneGroupName"] = groupName;
		info["action"] = action;
        sprintf(buf, "%d", roleVip);
        info["roleVip"] = info["aoneRoleVip"] = buf;
        info["roleCreateTime"] = info["aoneRoleCreateTime"] = m_select_role.CreateTime;
		sprintf(buf, "%llu", roleCTime);
		info["roleCTime"] = info["aoneRoleCTime"] = buf;
        info["userAccount"] = m_user_account;
        std::vector<PluginParam*> allParams;
        PluginParam* pluginParam = new PluginParam(info);
        allParams.push_back(pluginParam);
        uuSdkManager::getInstance() -> callFuncWithParam("submitRoleData", allParams);
        
        delete pluginParam;
        pluginParam = NULL;
    }

	if (isUse01gameAndNotBackEnd() == false && (isLogin == true || (strcmp(action.c_str(), "createRole") == 0)))
	{
		// 上传用户信息
		RecordRoleLoginRequest request;
		request.body().m_user_id = m_user_id;
		request.body().m_group_id = groupID;
		request.body().m_role_id = roleID;
		request.body().m_role_name = name;
		request.body().m_device_info = m_device_info;
		request.body().m_version_info = m_version_info;
		request.body().m_group_name = groupName;
		apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::thirdPartySubmitRoleDataCallback);
        
        uuLOG("%s thirdPartySubmitRoleData record role login", LOG_TAG);
	}
    
    if (strcmp(action.c_str(), "createRole") == 0)
    {
        createRoleCP(roleID, name, level, map<string, string>(), groupID, groupName, roleVip);
    }
	return;
}

/*
 *	third party submitRoleData callBack
 */
void AoneSDK::thirdPartySubmitRoleDataCallback(int ret, byte *responseBytes, int len)
{
	uuLOG("%s RecordRoleLogin begin!!! %d", LOG_TAG, len);
	if (ret < 0)
	{
		uuLOG("%s RecordRoleLogin fail, apiserver send_recv error or timeout", LOG_TAG);
		return;
	}
    
	RecordRoleLoginResponse *response = (RecordRoleLoginResponse*)s_apiParser->get_message(responseBytes, len);
	//if (responseBytes) delete responseBytes;
	// 判断返回码
	if (response->get_result() != 0)
	{
		ret = response->get_result();
		uuLOG("%s RecordRoleLogin fail, apiserver return code is %d", LOG_TAG, ret);
		delete response;
		return;
	}
	uuLOG("%s RecordRoleLogin ok, request success !!!", LOG_TAG);
}

void AoneSDK::submitUserInfo() 
{
	string channelAccount = uuSdkManager::getInstance()->callStringFunc("getUserAccount");
	std::map<string, string> info;

	info["userAccount"] = channelAccount;

	char buf[128] = { 0 };
	sprintf(buf, "%d", m_user_id);
	info["userId"] = buf;

	string isCreateAccount = "false";
	if (uuSdkManager::getInstance()->callBoolFunc("isCreateAccount"))
	{
		isCreateAccount = "true";
	}
	info["isCreateAccount"] = isCreateAccount;

	std::vector<PluginParam*> allParams;
	PluginParam* pluginParam = new PluginParam(info);
	allParams.push_back(pluginParam);

	uuLOG("%s submit user info , account is %s, user id is %d, isCreateAccount is %s", LOG_TAG, channelAccount.c_str(), m_user_id, isCreateAccount.c_str());
	uuSdkManager::getInstance()->callFuncWithParam("submitUserInfo", allParams);
	delete pluginParam;
	pluginParam = NULL;
}

void AoneSDK::submitRoleData()
{
	std::map<string, string> info;
	char buf[128] = {0};
	sprintf(buf, "%d", m_select_role.ID);
	m_select_role_cp.ID = buf;
	info["aoneRoleId"] = buf;
    info["roleId"] = buf;
	info["aoneRoleName"] = m_select_role.Name;
    info["roleName"] = m_select_role.Name;
	sprintf(buf, "%d", m_select_role.Level);
	info["aoneRoleLevel"] = buf;
    info["roleLevel"] = buf;
	sprintf(buf, "%d", getSelectedGroup().ID);
	info["aoneGroupId"] = buf;
    info["groupId"] = buf;
	info["aoneGroupName"] = getSelectedGroup().Name;
    info["groupName"] = getSelectedGroup().Name;
	sprintf(buf, "%d", m_select_role.VIP);
	info["aoneRoleVip"] = buf;
    info["roleVip"] = buf;
	info["aoneRoleCreateTime"] = m_select_role.CreateTime;
    info["roleCreateTime"] = m_select_role.CreateTime;
	sprintf(buf, "%d", m_user_id);
	info["aoneUserId"] = buf;
    info["userId"] = buf;
	info["userAccount"] = m_user_account;
    info["action"] = "login";
	char groupId[20];
	sprintf(groupId, "%d", m_select_group.ID);
	string c_groupId = groupId;
	m_select_group_cp.ID = c_groupId;
    uuLOG("%s m_select_group_cp.ID = %s", LOG_TAG, c_groupId.c_str());

    if (isUse01gameAndNotBackEnd() == false) {
        // 上传用户信息
        RecordRoleLoginRequest request;
        request.body().m_user_id = m_user_id;
        request.body().m_group_id = info["groupId"];
        request.body().m_role_id = info["roleId"];
        request.body().m_device_info = m_device_info;
        request.body().m_version_info = m_version_info;
		request.body().m_group_name = getSelectedGroup().Name;
        request.body().m_role_name = info["roleName"];
        apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::thirdPartySubmitRoleDataCallback);
        uuLOG("%s submitRoleData here", LOG_TAG);
    }
    
    std::vector<PluginParam*> allParams;
    PluginParam* pluginParam = new PluginParam(info);
    allParams.push_back(pluginParam);
    uuSdkManager::getInstance()->callFuncWithParam("submitRoleData", allParams);
    delete pluginParam;
    pluginParam = NULL;
	
   	return;
}

void AoneSDK::cleanRoleData()
{
	// 清空数据
	m_user_id = 0;
	m_user_account.clear();
	m_aone_account.clear();
	m_aone_password.clear();
	m_token.clear();
	m_user_token = 0;
	m_groups.clear();
	m_roles.clear();
	m_select_role.ID = 0;
	m_select_group.ID = 0;
	m_purchase_num = 1;
	m_select_product_id.clear();
	m_cp_ext.clear();
	m_last_payno.clear();
}


void AoneSDK::onAoneLoginEndAsync()
{
	uuLOG("%s onAoneLoginEndAsync, mUseUserLogin:%d",LOG_TAG, mUseUserLogin);
	if (mUseUserLogin)
	{
		userLoginAsync(m_LogIncb);
	}
	else
	{
		m_begin_login_time = m_ineffective_time;

		LogEvent(m_eventLoginType.c_str(), true);
		m_LogIncb(m_loginResult, map<string, string>());
	}
}


int AoneSDK::onAoneLoginEnd()
{
	if (mUseUserLogin)
	{
		return userLogin();
	}
	return true;
}

bool AoneSDK::isRoleAndGroupValid()
{
	bool aoneOk = (m_select_role.ID != 0 && m_select_group.ID != 0);
	bool cpOk = (m_select_role_cp.ID != "" && m_select_group_cp.ID != "");
	return aoneOk || cpOk;
}

bool AoneSDK::is01gameBackEnd()
{
	return this != getInstance();
}
    
void AoneSDK::setDefaultUserSdkName(const char *sdkName)
{
    uuSdkConfigHelper::setDefaultUserSdkName(sdkName);
}
    
// this case is for two aonesdk ,and used the outside aonesdk
bool AoneSDK::isUse01gameAndNotBackEnd(uuESdkType inWhere)
{
	return isUse01game(inWhere) && (is01gameBackEnd() == false);
}
    
/** 
有两个01Game,是外部调用；有一个01Game，但是内部调用
*/
bool AoneSDK::isUse01game(uuESdkType inWhere)
{
	string val;
	if (inWhere == uuESdkType::eSDK_PAY) {
		val = uuSdkConfigHelper::getInitParams(uuSdkConfigHelper::getDefaultIapSdkName(), "Is01gameSdk");
	}
	else {
		val = uuSdkConfigHelper::getInitParams(uuSdkConfigHelper::getDefaultUserSdkName(), "Is01gameSdk");
	}
	return val != "";
}

bool AoneSDK::shouldLog(bool useInternal)
{
	//user call log , always should log
	if (useInternal == false) 
		return true;
#ifdef CC_TARGET_OS_IPHONE
	//ios 01game sdk use inner aonesdk to log event
    if (isUse01gameAndNotBackEnd())
		return false;	
#endif
    return true;
}

void AoneSDK::LogPageStart(const char* pageName)
{
	for (vector<string>::iterator it = m_analyticsSdks.begin(); it != m_analyticsSdks.end(); ++it)
		uuSdkManager::getInstance()->logPageStart((*it).c_str(), pageName);
}

void AoneSDK::LogPageEnd( const char* pageName )
{
	for (vector<string>::iterator it = m_analyticsSdks.begin(); it != m_analyticsSdks.end(); ++it)
		uuSdkManager::getInstance()->logPageEnd((*it).c_str(), pageName);
}

void AoneSDK::LogEvent(const char* eventId, bool useInternal)
{
	if (shouldLog(useInternal) == false)
	{
		uuLOG("%s not log event %s in this aonesdk", LOG_TAG, eventId);
		return;
	}
	
	if (eventId != NULL) {
		uuLOG("%s log event: %s", LOG_TAG, eventId);
	}
	for (vector<string>::iterator it = m_analyticsSdks.begin(); it != m_analyticsSdks.end(); ++it)
		uuSdkManager::getInstance()->logEvent((*it).c_str(), eventId);
}

void AoneSDK::LogEvent(const char* eventId, map<string, string> info, bool useInternal)
{
	if (shouldLog(useInternal) == false) 
	{
		uuLOG("%s not log event %s in this aonesdk", LOG_TAG, eventId);
		return;
	}

	if (eventId != NULL) {
		uuLOG("%s log event with params: %s", LOG_TAG, eventId);
	}
	for (vector<string>::iterator it = m_analyticsSdks.begin(); it != m_analyticsSdks.end(); ++it)
	{
		uuLOG("%s hhh----------log event with sdk:%s", LOG_TAG, (*it).c_str());
		uuSdkManager::getInstance()->logEvent((*it).c_str(), eventId, info);
	}
}

void AoneSDK::LogEventBegin( const char* eventId )
{
	for (vector<string>::iterator it = m_analyticsSdks.begin(); it != m_analyticsSdks.end(); ++it)
		uuSdkManager::getInstance()->logEventBegin((*it).c_str(), eventId);
}

void AoneSDK::LogEventEnd( const char* eventId )
{
	for (vector<string>::iterator it = m_analyticsSdks.begin(); it != m_analyticsSdks.end(); ++it)
		uuSdkManager::getInstance()->logEventEnd((*it).c_str(), eventId);
}

void AoneSDK::LogKVEventBegin( const char* eventId, map<string, string> info )
{
	for (vector<string>::iterator it = m_analyticsSdks.begin(); it != m_analyticsSdks.end(); ++it)
		uuSdkManager::getInstance()->logKVEventBegin((*it).c_str(), eventId, info);
}

void AoneSDK::LogKVEventEnd( const char* eventId )
{
	for (vector<string>::iterator it = m_analyticsSdks.begin(); it != m_analyticsSdks.end(); ++it)
		uuSdkManager::getInstance()->logKVEventEnd((*it).c_str(), eventId);
}

void AoneSDK::LogError( const char* errorId, const char* message )
{
	for (vector<string>::iterator it = m_analyticsSdks.begin(); it != m_analyticsSdks.end(); ++it)
		uuSdkManager::getInstance()->logError((*it).c_str(), errorId, message);
}

vector<std::string> AoneSDK::GetOnlineParams( const char* key )
{
	vector<std::string> onlineParams;
	for (vector<string>::iterator it = m_analyticsSdks.begin(); it != m_analyticsSdks.end(); ++it)
	{
		onlineParams.push_back(uuSdkManager::getInstance()->getOnlineParams((*it).c_str(), key));
	}

	return onlineParams;
}

void AoneSDK::setAnalyticsSdk( const char* sdkName )
{
}

void AoneSDK::addFileSearchPath( const string path )
{
	cpfile_add_search_path(path.c_str());
}


void AoneSDK::setConfigData( const char* data )
{
	uuSdkConfigHelper::setConfigData(data);
	AonesdkConfig::getInstance()->loadData(data);
}


void AoneSDK::setUseUserLogin(bool useUserLogin)
{
    uuLOG("%s setUseUserLogin = %d", LOG_TAG, useUserLogin);
	mUseUserLogin = useUserLogin;
}

void AoneSDK::setDeviceId(const string deviceId)
{
	m_device_id = deviceId;
}

void AoneSDK::showAds(map<string, string> info, int pos, AONESDK_CB cb)
{
	m_cb = cb;
	m_pos = pos;
	m_ads_info = info;
	m_ad_num = 0;//播放成功后,下次播放还从第一个开始
	m_adsSdk = m_default_adsSdk;
	uuLOG("(showAds)m_adsSdk:%s ", m_adsSdk.c_str());
	if (std::find(m_config_adsSdks.begin(), m_config_adsSdks.end(), m_adsSdk) == m_config_adsSdks.end()){
		//数据中心的SDK不在uusdkConfig.json中
		m_adsSdk = uuSdkConfigHelper::getDefalutAdsSdkName();
	}
	uuSdkManager::getInstance()->showAds(m_adsSdk, info, pos, this, (uuSdkResultHandler)&AoneSDK::adsCallback);
}

void AoneSDK::hideAds( map<string, string> info, AONESDK_CB cb )
{
	m_cb = cb;
	uuSdkManager::getInstance()->hideAds(m_adsSdk, info, this, (uuSdkResultHandler)&AoneSDK::adsCallback);
}

void AoneSDK::queryPoints( AONESDK_CB cb )
{
	m_cb = cb;
	uuSdkManager::getInstance()->queryPoints(m_adsSdk, this, (uuSdkResultHandler)&AoneSDK::adsCallback);
}

void AoneSDK::spendPoints( int points, AONESDK_CB cb )
{
	m_cb = cb;
	uuSdkManager::getInstance()->spendPoints(m_adsSdk, points, this, (uuSdkResultHandler)&AoneSDK::adsCallback);
}

void AoneSDK::setAdsSdk( const char* sdkName )
{
	m_adsSdk = (sdkName != NULL) ? sdkName : "";
}

void AoneSDK::startPush( AONESDK_CB cb )
{
	m_cb = cb;
	uuSdkManager::getInstance()->startPush(m_pushSdk, this, (uuSdkResultHandler)&AoneSDK::pushCallback);
}

void AoneSDK::setPushSdk( const char* sdkName )
{
	m_pushSdk = (sdkName != NULL) ? sdkName : "";
}

void AoneSDK::closePush( AONESDK_CB cb )
{
	m_cb = cb;
	uuSdkManager::getInstance()->closePush(m_pushSdk, this, (uuSdkResultHandler)&AoneSDK::pushCallback);
}

void AoneSDK::setAlias( const char* alias, AONESDK_CB cb )
{
	m_cb = cb;
	uuSdkManager::getInstance()->setAlias(m_pushSdk, alias, this, (uuSdkResultHandler)&AoneSDK::pushCallback);
}

void AoneSDK::delAlias( const char* alias, AONESDK_CB cb )
{
	m_cb = cb;
	uuSdkManager::getInstance()->delAlias(m_pushSdk, alias, this, (uuSdkResultHandler)&AoneSDK::pushCallback);
}

void AoneSDK::setTags( const char* tags, AONESDK_CB cb )
{
	m_cb = cb;
	uuSdkManager::getInstance()->setTags(m_pushSdk, tags, this, (uuSdkResultHandler)&AoneSDK::pushCallback);
}

void AoneSDK::delTags( const char* tags, AONESDK_CB cb )
{
	m_cb = cb;
	uuSdkManager::getInstance()->delTags(m_pushSdk, tags, this, (uuSdkResultHandler)&AoneSDK::pushCallback);
}

int AoneSDK::loadAllSdks()
{
    //cplog_close_nslog(1);
	return uuSdkManager::getInstance() -> loadAllSdks();
}

void AoneSDK::recordSDKPayException(int code, string& msg )
{
    uuLOG("%s pay recorde , code = %d, msg = %s", LOG_TAG, code, msg.c_str());
	if (!m_init_ok)
    {
		return;
	}

	recordSDKPayExceptionRequest request;
	char buff[20];
	sprintf(buff, "%d", code);
	request.body().m_code = buff;
	request.body().m_detail = msg;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;

	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::recordSDKPayExceptionCallback);
}

void AoneSDK::recordSDKPayExceptionCallback( int ret, byte *responseBytes, int len )
{
    uuLOG("%s recordSDKPayException %s", LOG_TAG, (ret == 0) ? "SUCCESS" : "FAIL");
	if (responseBytes) delete responseBytes;
}

void AoneSDK::httpRequest(string host, AONESDK_CB cb)
{
	m_cb = cb;
	m_httpUrl = host;

	int pos = host.find("http://");
	if (pos != host.npos) {
		host = host.substr(pos + 7);
	}

	string subPath = "";
	pos = host.find("/");
	if (pos != host.npos) {
		subPath = host.substr(pos);
	} else {
		uuLOG("%s Error! url format is invalid!", LOG_TAG);
		if (m_cb){
			map<string, string> versionInfoMap;
			versionInfoMap["MajorVersion"] = m_game_version.MajorVersion;
			versionInfoMap["MinorVersion"] = m_game_version.MinorVersion;
			versionInfoMap["RevisionVersion"] = m_game_version.RevisionVersion;
			versionInfoMap["Notice"] = m_game_version.Notice;
			versionInfoMap["DownloadURL"] = m_game_version.DownloadURL;
			versionInfoMap["UpdateURL"] = m_game_version.UpdateURL;
			versionInfoMap["BigPatcherURL"] = m_game_version.BigPatcherURL;
			m_cb(0, versionInfoMap);
		}
		return;
	}
	host = host.substr(0, pos);
	
	const char* str1= "GET ";
	const char* str2= " HTTP/1.1\r\nHost: ";
	const char* str3= "\r\nUser-Agent: a-one client\r\n\r\n";
	uint32 requestLen = strlen(str1) + strlen(str2) + strlen(str3) + subPath.size() + host.size();
	char *requestBytes = new char[requestLen + 1];
	requestBytes[0] = '\0';
	strcat(requestBytes, str1);
	strcat(requestBytes, subPath.c_str());
	strcat(requestBytes, str2);
	strcat(requestBytes, host.c_str());
	strcat(requestBytes, str3);

	uuNetAsync::http_send_recv(this, (HTTP_CB)&AoneSDK::httpRequestCallback, host, 80, "", requestBytes, requestLen);
}

void AoneSDK::httpRequestCallback(int ret, map<string, string> params)
{
	string strCode = params["code"];
	int iCode = 0;
	if (strCode != "") {
		iCode = atoi(strCode.c_str());
	}
	if (ret != 0 || iCode < 0 || iCode >= 400) {
		params["url"] = m_httpUrl;
		recordSDKDownloadException(params);
	} else {
		if (m_cb){
			map<string, string> versionInfoMap;
			versionInfoMap["MajorVersion"] = m_game_version.MajorVersion;
			versionInfoMap["MinorVersion"] = m_game_version.MinorVersion;
			versionInfoMap["RevisionVersion"] = m_game_version.RevisionVersion;
			versionInfoMap["Notice"] = m_game_version.Notice;
			versionInfoMap["DownloadURL"] = m_game_version.DownloadURL;
			versionInfoMap["UpdateURL"] = m_game_version.UpdateURL;
			versionInfoMap["BigPatcherURL"] = m_game_version.BigPatcherURL;
			m_cb(0, versionInfoMap);
		}
	}
}

void AoneSDK::recordSDKDownloadException(map<string, string> params)
{
	RecordSDKDownloadExceptionRequest request;
	request.body().m_dns = params["dns"];
	request.body().m_ip = params["ip"];
	request.body().m_url = params["url"];
	request.body().m_code = params["code"];
	request.body().m_detail = params["detail"];
	if (params.find("use_ms") != params.end()) {
		request.body().m_use_ms = atoi(params["use_ms"].c_str());
	}

	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;

	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::recordSDKDownloadExceptionCallback);
}

void AoneSDK::recordSDKDownloadExceptionCallback( int ret, byte *responseBytes, int len )
{
    uuLOG("%s recordSDKDownloadException %s", LOG_TAG, (ret == 0) ? "SUCCESS" : "FAIL");
	if (responseBytes) delete responseBytes;
	if (m_cb) m_cb(0, map<string, string>());
}

void AoneSDK::recordGameDownloadException(const string url, const string code, const string detail, AONESDK_CB cb)
{
	m_exceptionCb = cb;

	RecordGameDownloadExceptionRequest request;
	request.body().m_url = url;
	request.body().m_code = code;
	request.body().m_detail = detail;
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;

	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::recordGameDownloadExceptionCallback);
}

void AoneSDK::recordGameDownloadExceptionCallback( int ret, byte *responseBytes, int len )
{
    uuLOG("%s recordGameDownloadException %s", LOG_TAG, (ret == 0) ? "SUCCESS" : "FAIL");
	if (responseBytes) delete responseBytes;
	if (m_exceptionCb) {
		m_exceptionCb(ret, map<string, string>());
	}
}

void AoneSDK::checkIsEnableLocalPay(AONESDK_CB cb)
{

	m_cb = cb;

	QueryIsEnableLocalPayRequest request;
	string type = uuDevice::getNetworkType();
	if (type == "") {
		uuLOG("%s check is enable local pay fail, the network type is null, maybe no network now!", LOG_TAG);
		m_cb(-1, map<string, string>());
        logException(-1, "check is enable local pay fail, the network type is null, maybe no network now!");
		return;
	}
	if (type == "wifi") {
		request.body().m_is_wifi = true;
	} else {
		request.body().m_is_wifi = false;
	}
	request.body().m_code = uuDevice::getOpertorCode();
	request.body().m_auth_key = AoneSDK::make_authkeyA(m_device_info.m_device_id + m_appkey);
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;

	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::checkIsEnableLocalPayCallback);
}

void AoneSDK::checkIsEnableLocalPayCallback(int ret, byte *responseBytes, int len)
{
	if (ret < 0)
	{
		uuLOG("%s check is enable local pay fail, api server send_recv error or timeout", LOG_TAG);
		m_cb(ret, map<string, string>());
        logException(ret, "check is enable local pay fail, api server send_recv error or timeout");
		return;
	}
	QueryIsEnableLocalPayResponse* response = (QueryIsEnableLocalPayResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	
	if (response->get_result() != 0)
	{
		uuLOG("%s check is enable local pay fail, apiserver return code is %d", LOG_TAG, response->get_result());
		int code = response->get_result();
		delete response;
		m_cb(code, map<string, string>());
        logException(code, "check is enable local pay fail, apiserver return code");
		return;
	}

	bool enable = response->body().m_is_enable;
	delete response;

	uuLOG("%s check is enable local pay ok, request success !!!", LOG_TAG);
	map<string, string> params;
	if (enable) {
		params["enable"] = "1";
	} else {
		params["enable"] = "0";
	}

	m_cb(0, params);
}

void AoneSDK::addNotify( std::map<std::string, std::string> info )
{
	std::vector<PluginParam*> allParams;
	PluginParam* pluginParam = new PluginParam(info);
	allParams.push_back(pluginParam);
	uuSdkManager::getInstance()->callFuncWithParam("addNotify", allParams);
	delete pluginParam;
	pluginParam = NULL;
}

void AoneSDK::clearNotify( int notifyId )
{
	std::vector<PluginParam*> allParams;
	PluginParam* pluginParam = new PluginParam(notifyId);
	allParams.push_back(pluginParam);
	uuSdkManager::getInstance()->callFuncWithParam("clearNotify", allParams);
	delete pluginParam;
	pluginParam = NULL;
}

void AoneSDK::clearAllNotify()
{
	std::vector<PluginParam*> allParams;
	uuSdkManager::getInstance()->callFuncWithParam("clearAllNotify", allParams);
}

void AoneSDK::logException(uint32 code, const char *msg)
{
	char buf[1024];
	sprintf(buf, "%d",code);
	cplog_report_exception(buf, msg);
}

string AoneSDK::getChannelParamFromKey(string key)
{
    map<string, string>::iterator it;
    for (it = m_channel_params.begin(); it != m_channel_params.end(); it++)
    {
        string first = it -> first;
        string second = it -> second;
        if (first == key)
        {
            return second;
        }
    }
    return "";
}

void AoneSDK::openUrl(const char* url){
	uuDevice::openUrl(url);
}

void AoneSDK::copyToClipboard(const char* text){
	uuDevice::copyToClipboard(text);
}
void AoneSDK::joinQGroup(){
	string joinQGroupKey = getChannelParamFromKey("JoinQGroupKey");
	uuLOG("joinQGroupKey:%s",joinQGroupKey.c_str());
	string joinQGroupCode = getChannelParamFromKey("JoinQGroupCode");
	uuDevice::joinQGroup(joinQGroupCode,joinQGroupKey);
}

void AoneSDK::starReviewInApp(const char* appid){
    uuDevice::starReviewInApp(appid);
}

//应用内评星
void AoneSDK::starInApp(){
    uuDevice::starInApp();
}
//应用内调起苹果商店评论
void AoneSDK::reviewInApp(const char* appId){
    uuDevice::reviewInApp(appId);
}

int AoneSDK::isInstallApp(const string& packageName){
	return uuDevice::isInstallApp(packageName);
}
    
void AoneSDK::GetAdTrackLink(string userData, AONESDK_CB cb)
{
    m_cb = cb;
    if (!m_init_ok)
    {
        uuLOG("%s GetAdTrackLink failed, SDK not inited", LOG_TAG);
        m_cb(-1, map<string, string>());
        return;
    }
    
    PublishShareInfoRequest request;
    request.body().m_device_info = m_device_info;
    request.body().m_version_info = m_version_info;
    
//    request.body().m_app_id = m_appid;
//    request.body().m_channel = getChannelID();
    request.body().m_user_data = userData;
    
    apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::getAdTrackLinkCallback);
}
    
void AoneSDK::getAdTrackLinkCallback(int ret, byte *responseBytes, int len)
{
    if (ret < 0)
    {
        uuLOG("%s GetAdTrackLink failed, send request fail, code = %d", LOG_TAG, ret);
        m_cb(ret, map<string, string>());
        logException(ret, "GetAdTrackLink failed, send request fail, error or timeout");
        return;
    }
    
    PublishShareInfoResponse * response = (PublishShareInfoResponse *)s_apiParser -> get_message(responseBytes, len);
    if (responseBytes) delete responseBytes;
    
    if (response -> get_result() != 0)
    {
        ret = response -> get_result();
        uuLOG("%s GetAdTrackLink failed, get response fail, code = %d", LOG_TAG, response -> get_result());
        m_cb(ret, map<string, string>());
        logException(ret, "aonesdk GetAdSource failed, get response fail");
        return;
    }
    
    string path = response -> body().m_url;
    string host = getChannelParamFromKey("app_internal_share_domain");
    
    if (host.length() < 1)
    {
        host = "http://share.smartspace-game.com";
        uuLOG("%s use default host = %s", LOG_TAG, host.c_str());
    }
    string url = host + path;
    delete response;
    map<string, string>info;
    info["url"] = url;
    info["AdSource"] = path;
    uuLOG("%s get AdTrackLink url = %s", LOG_TAG, url.c_str());
    m_cb(0, info);
}

    
string AoneSDK::GetAppLink()
{
    if (!m_init_ok)
    {
        uuLOG("%s GetAdTrackLink failed, SDK not inited", LOG_TAG);
        return "";
    }
    
    return getChannelParamFromKey("app_internal_redirect_url");
}
    
   
void AoneSDK::GetAdSource(string &userData, AONESDK_CB cb)
{
    m_cb = cb;
    if (!m_init_ok)
    {
        uuLOG("%s GetAdSource failed, SDK not inited", LOG_TAG);
        m_cb(-1, map<string, string>());
        return;
    }
    
    QueryShareInfoRequest request;
    request.body().m_device_info = m_device_info;
    request.body().m_version_info = m_version_info;
    
    apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::getAdSourceCallback);
}

    
void AoneSDK::getAdSourceCallback(int ret, byte *responseBytes, int len)
{
    if (ret < 0)
    {
        uuLOG("%s GetAdSource failed, send request fail, code = %d", LOG_TAG, ret);
        m_cb(ret, map<string, string>());
        logException(ret, "GetAdSource failed, send request fail, error or timeout");
        return;
    }
    
    QueryShareInfoResponse * response = (QueryShareInfoResponse *)s_apiParser -> get_message(responseBytes, len);
    if (responseBytes) delete responseBytes;
    
    if (response -> get_result() != 0)
    {
        ret = response -> get_result();
        uuLOG("%s GetAdSource failed, get response fail, code = %d", LOG_TAG, response -> get_result());
        m_cb(ret, map<string, string>());
        logException(ret, "GetAdSource failed, get response fail");
        return;
    }
    
    string token = response -> body().m_url;
    string user_data = response -> body().m_user_data;
    delete response;
    
    map<string, string>info;
    info["token"] = token;
    info["userData"] = user_data;
    m_cb(0, info);
}
    
//string AoneSDK::EmbedQR(string srcImgPath, string info, int x, int y, int w, int h)
//{
//    return generateQRCode(srcImgPath, info, x, y, w, h);
//}

void AoneSDK::recordDevicePoint(uint32 sdkPoint){
	uuLOG("%s recordDevicePoint sdkPoint:%d", LOG_TAG, sdkPoint);
	
	uuLOG("%s recordDevicePoint checkFirstRecord:%d", LOG_TAG, mChceckFirstRecord);
	RecordDevicePointRequest request;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_sdk_point = sdkPoint;
	request.body().m_is_first_login = mChceckFirstRecord;
	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::recordDevicePointCallback);
}

void AoneSDK::recordDevicePointCallback(int ret, byte *responseBytes, int len){
	if (ret < 0)
	{
		uuLOG("%s recordDevicePoint failed, send request fail, code = %d", LOG_TAG, ret);
		logException(ret, "recordDevicePoint failed, send request fail, error or timeout");
		return;
	}

	RecordDevicePointResponse * response = (RecordDevicePointResponse *)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;

	if (response->get_result() != 0)
	{
		ret = response->get_result();
		uuLOG("%s recordDevicePoint failed, get response fail, code = %d", LOG_TAG, response->get_result());
		logException(ret, "recordDevicePoint failed, get response fail");
		return;
	}
	uuLOG("recordDevicePoint success");
}

void AoneSDK::recordRolePoint(uint32 gamePoint){
	uuLOG("%s recordRolePoint gamePoint:%d",LOG_TAG,gamePoint);
	uuLOG("m_user_id:%d", m_user_id);
	uuLOG("m_cp_role_id:%s", m_select_role_cp.ID.c_str());
	uuLOG("m_cp_role_name:%s", m_select_role_cp.Name.c_str());
	uuLOG("m_cp_group_id:%s", m_select_group_cp.ID.c_str());
	RecordRolePointRequest request;
	request.body().m_user_id = m_user_id;
	request.body().m_cp_role_id = m_select_role_cp.ID; 
	request.body().m_cp_role_name = m_select_role_cp.Name;
	request.body().m_cp_group_id = m_select_group_cp.ID;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_game_point = gamePoint;
	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::recordRolePointCallback);
}

void AoneSDK::recordRolePointCallback(int ret, byte *responseBytes, int len){
	if (ret < 0)
	{
		uuLOG("%s recordRolePoint failed, send request fail, code = %d", LOG_TAG, ret);
		logException(ret, "recordRolePoint failed, send request fail, error or timeout");
		return;
	}

	RecordRolePointResponse * response = (RecordRolePointResponse *)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;

	if (response->get_result() != 0)
	{
		ret = response->get_result();
		uuLOG("%s recordRolePoint failed, get response fail, code = %d", LOG_TAG, response->get_result());
		logException(ret, "recordRolePoint failed, get response fail");
		return;
	}
	uuLOG("recordRolePoint success");
}

void AoneSDK::IdcardLogin(){
	IdcardLoginRequest request;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_user_id = m_user_id;
	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::IdcardLoginCallback);
}

void AoneSDK::IdcardLoginCallback(int ret, byte *responseBytes, int len){
	if (ret < 0)
	{
		uuLOG("%s IdcardLogin failed, send request fail, code = %d", LOG_TAG, ret);
		logException(ret, "IdcardLogin failed, send request fail, error or timeout");
		return;
	}
	IdcardLoginResponse* response = (IdcardLoginResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;

	if (response->get_result() != 0)
	{
		ret = response->get_result();
		uuLOG("%s IdcardLogin failed, get response fail, code = %d", LOG_TAG, response->get_result());
		logException(ret, "IdcardLogin failed, get response fail");
		return;
	}
	m_idcardlogin_token = response->body().m_token;
	uuLOG("IdcardLogin success");

}

void AoneSDK::IdcardLogout(){
	IdcardLogoutRequest request;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_user_id = m_user_id;
	apiserver_send_recv_async(&request,(NET_CB)&AoneSDK::IdcardLogoutCallback);
}
    
void AoneSDK::IdcardLogoutCallback(int ret, byte* responseBytes, int len){
	if (ret < 0)
	{
		uuLOG("%s IdcardLogout failed, send request fail, code = %d", LOG_TAG, ret);
		logException(ret, "IdcardLogout failed, send request fail, error or timeout");
		return;
	}
	IdcardLogoutResponse* response = (IdcardLogoutResponse*)s_apiParser->get_message(responseBytes,len);
	if (responseBytes) delete responseBytes;

	if (response->get_result() != 0){
		ret = response->get_result();
		uuLOG("%s IdcardLogout failed, get response fail, code = %d", LOG_TAG, response->get_result());
		logException(ret, "IdcardLogout failed, get response fail");
		return;
	}
	uuLOG("IdcardLogout success");
}

void AoneSDK::IdcardHeart(){
	IdcardHeartRequest request;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_user_id = m_user_id;
	request.body().m_token = m_idcardlogin_token;
	apiserver_send_recv_async(&request,(NET_CB)&AoneSDK::IdcardHeartCallback);
}

void AoneSDK::IdcardHeartCallback(int ret,byte* responseBytes,int len){
	if (ret < 0)
	{
		uuLOG("%s IdcardHeart failed, send request fail, code = %d", LOG_TAG, ret);
		logException(ret, "IdcardHeart failed, send request fail, error or timeout");
		return;
	}
	IdcardHeartResponse* response = (IdcardHeartResponse*)s_apiParser->get_message(responseBytes,len);
	if (responseBytes) delete responseBytes;
	if (response->get_result() != 0){
		ret = response->get_result();
		uuLOG("%s IdcardHeart failed, get response fail, code = %d", LOG_TAG, response->get_result());
		logException(ret, "IdcardHeart failed, get response fail");
		return;
	}
	uuLOG("IdcardHeart success");
}

void AoneSDK::IdcardCertificate(const char* identity_number, const char* real_name, AONESDK_CB cb){
	IdcardCertificateRequest request;
	request.body().m_device_info = m_device_info;
	request.body().m_version_info = m_version_info;
	request.body().m_user_id = m_user_id;
	request.body().m_identity_number = identity_number;
	request.body().m_real_name = real_name;
	apiserver_send_recv_async(&request, (NET_CB)&AoneSDK::IdcardCertificateCallback);
}

void AoneSDK::IdcardCertificateCallback(int ret, byte* responseBytes, int len){
	if (ret < 0)
	{
		uuLOG("%s IdcardCertificate failed, send request fail, code = %d", LOG_TAG, ret);
		logException(ret, "IdcardCertificate failed, send request fail, error or timeout");
		return;
	}
	IdcardCertificateResponse* response = (IdcardCertificateResponse*)s_apiParser->get_message(responseBytes, len);
	if (responseBytes) delete responseBytes;
	if (response->get_result() != 0){
		ret = response->get_result();
		uuLOG("%s IdcardCertificate failed, get response fail, code = %d", LOG_TAG, response->get_result());
		logException(ret, "IdcardCertificate failed, get response fail");
		return;
	}
	uuLOG("IdcardCertificate success");
}

void AoneSDK::requestPlayerExtraInfo(AONESDK_CB cb){
	uuDevice::requestPlayerExtraInfo(cb);
}

bool AoneSDK::getChannelIsTest() {
	if(m_version_info.m_channel == "test") return true;
//	else if(m_version_info.m_channel == "handygame_hyhgand_cs") return true;
	return false;
}
bool AoneSDK::getPayChannelIsTest() {
	if(m_pay_channel == "test") return true;
//	else if(m_pay_channel == "handygame_hyhgand_cs") return true;
	return false;
}
bool AoneSDK::getAuthTypeIsTest() {
	if(m_oauth_type == "test") return true;
//	else if(m_oauth_type == "handygame_hyhgand_cs") return true;
	return false;
}

char *getStrData(char *buffer, uint16 &pos) {
	uint16 size = 0;
	memcpy(&size, buffer + pos, 2);
	char *data = (char *)malloc(size + 1);
	pos += 2;
	memcpy(data, buffer + pos, size);
	data[size] = '\0';
	pos += size;

	return data;
}

boolean PayStorage::load()
{
	string filePath = cpfile_write_path();
	filePath += "aone_pay_list";
	uuLOG("%s hhh path is %s", LOG_TAG, filePath.c_str());
	FILE *f = fopen(filePath.c_str(), "rb");
	if (f == NULL) {
		uuLOG("no paylist file %s", filePath.c_str());
		return true;
	}
	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);
	if (size <= 2) { // no record， 2 for the null data
		uuLOG("%s paylist file no record", LOG_TAG);
		fclose(f);
		return true;
	}
	char *buffer = (char *)malloc(size);
	fread(buffer, 1, size, f);
	fclose(f);

	RC4 parse;
	parse.init("aonesoft123456ftosenoa");
	parse.update((byte*)buffer, (byte*)buffer, size);

	uint16 length;
	memcpy(&length, buffer, 2);
	if (length != size) {
		uuLOG("%s Error! the file: aone_pay_list, has wrong data length! length is %d, size is %d", LOG_TAG, length, size);
		return false;
	}
	uint16 pos = 2;
	PayRecord payData;
	char * curData;
	int count = 0;
	int num = payData.getNumber();
    int cpCount = 0;
    int cpNum = payData.getCpNum();
	m_vecData.clear();
	while (pos < length) {
        if (count < num) {
            curData = getStrData(buffer, pos);
            payData.record[count++] = curData;
			//uuLOG("load record[%d] size is %d, str is %s", count - 1, payData.record[count - 1].size(), payData.record[count - 1].c_str());
        }
        
        if (count >= num && cpCount < cpNum)
        {
            curData = getStrData(buffer, pos);
            payData.cpID[cpCount++] = curData;
        }
        
		if (count >= num && cpCount >= cpNum) {
            count = 0;
            cpCount = 0;
			memcpy(&payData.user_id, buffer + pos, 4);
			pos += 4;
			memcpy(&payData.role_id, buffer + pos, 4);
			pos += 4;
			memcpy(&payData.purchase_num, buffer + pos, 4);
			pos += 4;

			m_vecData.push_back(payData);
		}
		free(curData);
	}

	uuLOG("%s load pay recod count is %d", LOG_TAG, m_vecData.size());
	return true;
}

boolean PayStorage::save(PayRecord data, boolean toSave)
{
	m_vecData.push_back(data);
	if (m_vecData.size() > 20) {
		vector<PayRecord>::iterator itr = m_vecData.begin();
		m_vecData.erase(itr);
	}
	if (!toSave) {
		uuLOG("%s not save pay store", LOG_TAG);
		return true;
	}

	return saveToFile();
}

void PayStorage::checkRecords(AoneSDK *sdk)
{
    load();
    
    int index = -1;
    
    for (int i = 0; i < m_vecData.size(); ++i)
    {
        bool isFail = false;
        if (!m_vecData[i].record[4].empty()) {// check receipt is empty
            for (int j = 0; j < m_store_payno_fail.size(); ++j)
            {
                // `last_pay_no` compared with the pay failed store_payno_fail
                if (m_vecData[i].record[0] == m_store_payno_fail[j]) {
                    isFail = true;
                    break;
                }
            }
            
            if (!isFail) {
                index = i;
                break;
            }
        }
    }
    //检查本地保存的该订单信息，再次调用提交收据协议
    if ((int)m_vecData.size() > index && index >= 0) {
        PayRecord record = m_vecData[index];
        m_store_payno = record.record[0];
        if (AonesdkConfig::getInstance()->getAgainstFakePay() == "true")
            sdk->receiptCommitAsync(record, (PAYCOMMIT_CB)&AoneSDK::payReCommitResult);
        else
            sdk->payCommitAsync(record, (PAYCOMMIT_CB)&AoneSDK::payReCommitResult);
    }
}

boolean PayStorage::saveToFile()
{
	string path = cpfile_write_path();
	path += "aone_pay_list";
	vector<PayRecord>::iterator itr;
	int length = 0;
	for (itr = m_vecData.begin(); itr != m_vecData.end(); ++itr) {
		length += (*itr).getLength();
	}
	length += 2;
	char *buffer = (char*)malloc(length);
	memcpy(buffer, &length, 2);
	length = 2;
	int size;
	for (itr = m_vecData.begin(); itr != m_vecData.end(); ++itr) {
		int num = (*itr).getNumber();
		for (int i = 0; i < num; ++i) {
			size = (*itr).record[i].size();
			memcpy(buffer + length, &size, 2);
			length += 2;
			memcpy(buffer + length, (*itr).record[i].c_str(), size);
			length += size;
			//uuLOG("save record[%d], size is %d, str is %s", i, size, (*itr).record[i].c_str());
		}
        
        int cpNum = (*itr).getCpNum();
        for (int j = 0; j < cpNum; ++j){
            size = (*itr).cpID[j].size();
            memcpy(buffer + length, &size, 2);
            length += 2;
            memcpy(buffer + length, (*itr).cpID[j].c_str(), size);
            length += size;
        }
        
		memcpy(buffer + length, &(*itr).user_id, 4);
		length += 4;
		memcpy(buffer + length, &(*itr).role_id, 4);
		length += 4;
		memcpy(buffer + length, &(*itr).purchase_num, 4);
		length += 4;
	}

	RC4 parse;
	parse.init("aonesoft123456ftosenoa");
	parse.update((byte*)buffer, (byte*)buffer, length);

	FILE *f = fopen(path.c_str(), "wb");
	if (f == NULL) {
        free(buffer);
		return false;
	}
	fwrite(buffer, 1, (size_t)length, f);
	fclose(f);
	free(buffer);

    uuLOG("%s save pay record count is %u", LOG_TAG, m_vecData.size());

	return true;
}

}
