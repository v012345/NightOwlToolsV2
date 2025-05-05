#include "xenet/XESelectDef.h"
#include "xenet/XEStdC.h"
#include <stdio.h>	
NS_XE_BEGIN
	
typedef pair<net_error_type, string> error_array_item;
static const map<net_error_type, string>::value_type error_array[] = 
{
	error_array_item(XENET_OK, "成功."),
	error_array_item(XENET_CREATE_SOCKET_FAILED, "创建socket失败."),
	error_array_item(XENET_XTCP_CONNECT_FAILED, "tcp链接失败."),
	error_array_item(XENET_XTCP_SET_NONBLOCK_FAILED, "设置非阻塞失败."),
	error_array_item(XENET_XTCP_CONNECT_TIMEOUT, "tcp链接超时."),
	error_array_item(XENET_XUDP_CONNECT_ABOVE_LIMITS, "udp尝试连接超过了最大次数."),
	error_array_item(XENET_XUDP_KEEPLIVE_TIMEOUT, "udp心跳超时，链接断开了."),
	error_array_item(XENET_CLOSED_NORMAL, "对端正常关闭了连接."),
	error_array_item(XENET_CLOSED_ABNORMAL, "对端非正常关闭了链接."),
};
const map<net_error_type, string> error_system::m_error_types(error_array, 
						error_array + sizeof(error_array) / sizeof(error_array[0]));
	
void error_system::format_error(const string& file_name, uint32 line_, net_error_type error_type,
									uint32 error_code, string& error_msg)
{
	string error_type_str, error_code_str;
	map<net_error_type, string>::const_iterator it_error_type = m_error_types.find(error_type);
	if (m_error_types.end() != it_error_type)
	{
		error_type_str = it_error_type->second;
	}
	else
	{
		error_type_str = "未知";
	}
	_get_system_error(error_code, error_code_str);
	
#define ERROR_BUFFER_SIZE 1024
	char tmp_msg[ERROR_BUFFER_SIZE] = { 0 };
	SAFE_FORMATE(tmp_msg, ERROR_BUFFER_SIZE, "%s:%u error type(%u:%s), error code(%u:%s). \n", file_name.c_str(), line_, \
				(uint32)error_type, error_type_str.c_str(), error_code, error_code_str.c_str());
	
	error_msg = tmp_msg;
}
	
// 获取系统
void error_system::_get_system_error(uint32 error_code, string& error_msg)
{
#define ERROR_BUFFER_SIZE 1024
	char tmp_msg[ERROR_BUFFER_SIZE] = { 0 };
#if XE_TARGET_PLATFORM == XE_PLATFORM_WIN32
	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, error_code, LANG_NEUTRAL, tmp_msg, ERROR_BUFFER_SIZE, NULL);
#else
	strerror_r(error_code, tmp_msg, ERROR_BUFFER_SIZE);
#endif
	error_msg = tmp_msg;
}
	
NS_XE_END