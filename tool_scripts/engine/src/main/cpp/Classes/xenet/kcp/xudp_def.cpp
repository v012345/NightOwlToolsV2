#include "xenet/XEStdC.h"
#include "xenet/kcp/xudp_def.hpp"

namespace xudp
{
	
#define CONV_LEN						11

#define AONE2_KCP_CONNECT_SYN			"aone2_kcp_connect_syn"
#define AONE2_KCP_CONNECT_SYN_LEN		21

#define AONE2_KCP_CONNECT_ACK			"aone2_kcp_connect_ack_"
#define AONE2_KCP_CONNECT_ACK_LEN		22

#define AONE2_KCP_DISCONNECT			"aone2_kcp_disconnect_"
#define AONE2_KCP_DISCONNECT_LEN		21
	
#if AE_TARGET_PLATFORM == AE_PLATFORM_WIN32
#define SAFE_FORMATE	sprintf_s
#else 
#define SAFE_FORMATE	snprintf
#endif 

	
const char* eventTypeStr(eEventType eventType)
{
	switch (eventType)
	{
		case eConnect: return "eConnect";
		case eDisconnect: return "eDisconnect";
		case eRcvMsg: return "eRcvMsg";
		case eLagNotify: return "eLagNotify";
		default: return "unknown";
	}
}
	
// kcp 链接命令
std::string make_kcp_key_connect()
{
	return AONE2_KCP_CONNECT_SYN;
}

bool is_kcp_key_connect(const char* data, size_t len)
{
	return (len == AONE2_KCP_CONNECT_SYN_LEN &&
		memcmp(data, AONE2_KCP_CONNECT_SYN, AONE2_KCP_CONNECT_SYN_LEN) == 0);
}

// kcp 链接回复命令
std::string make_kcp_key_connect_response(uint32_t conv)
{
	std::string str_result;
	str_result.resize(AONE2_KCP_CONNECT_ACK_LEN + CONV_LEN);
	SAFE_FORMATE((char*)str_result.c_str(), AONE2_KCP_CONNECT_ACK_LEN + CONV_LEN, "%s%010u", AONE2_KCP_CONNECT_ACK, conv);
	return str_result;
}

bool is_kcp_key_connect_response(const char* data, size_t len)
{
	return (len == (AONE2_KCP_CONNECT_ACK_LEN + CONV_LEN) &&
		memcmp(data, AONE2_KCP_CONNECT_ACK, AONE2_KCP_CONNECT_ACK_LEN) == 0);
}

uint32_t get_conv_from_key_connect_response(const char* data)
{
	// 
	return strtoul(data + AONE2_KCP_CONNECT_ACK_LEN, 0, 10);
}

// 断开连接命令
std::string make_kcp_key_disconnect(uint32_t conv)
{
	std::string str_result;
	str_result.resize(AONE2_KCP_DISCONNECT_LEN + CONV_LEN);
	SAFE_FORMATE((char*)str_result.c_str(), AONE2_KCP_DISCONNECT_LEN + CONV_LEN, "%s%010u", AONE2_KCP_DISCONNECT, conv);
	return str_result;
}

bool is_kcp_key_disconnect(const char* data, size_t len)
{
	return (len == (CONV_LEN + AONE2_KCP_DISCONNECT_LEN) &&
		memcmp(data, AONE2_KCP_DISCONNECT, AONE2_KCP_DISCONNECT_LEN) == 0);
}

uint32_t get_conv_from_key_disconnect(const char* data)
{
	return strtoul(data + AONE2_KCP_DISCONNECT_LEN, 0, 10);
}
	
}
