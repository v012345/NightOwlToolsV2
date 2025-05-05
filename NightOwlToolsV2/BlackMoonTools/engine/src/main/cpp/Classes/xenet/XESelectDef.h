#ifndef _X_SELECT_DEF_H_
#define _X_SELECT_DEF_H_

//*********************************************************************
// FileName		:	XESelectDef.h
// Author		:	majianwei
// Date			:	2017/12/11
// Description	:	Select模型需要的一些定义
//*********************************************************************
	
//#include "net/XEMessage.h"
#include "xenet/XEStdC.h"
#include <string>
#include <map>
#if XE_TARGET_PLATFORM == XE_PLATFORM_WIN32
#include "winsock.h"
#else 
#include "errno.h"
#endif 
	
using namespace std;
NS_XE_BEGIN
	
#if XE_TARGET_PLATFORM == XE_PLATFORM_WIN32
#define ERRORNO					WSAGetLastError()
#define ERROR_WOULD_BLOCK		WSAEWOULDBLOCK
#define ERROR_INPROGRESS		WSAEWOULDBLOCK 

#else 
#define ERRORNO					errno
#define ERROR_WOULD_BLOCK		EWOULDBLOCK
#define ERROR_INPROGRESS		EINPROGRESS 

#endif 
	
#define SOCKET_FLAG_OPERATE_MIN				0x0001
#define SOCKET_FLAG_OPERATE_READ_ONCE		0x0001  // 
#define SOCKET_FLAG_OPERATE_WRITE_ONCE		0x0002  // 
#define SOCKET_FLAG_OPERATE_READ_ENDLESS	0x0040  // 
#define SOCKET_FLAG_OPERATE_WRITE_ENDLESS	0x0080  //
#define SOCKET_FLAG_OPERATE_ERROR			0x0010  //
#define SOCKET_FLAG_OPERATE_CLOSE			0x0020  // 关闭操作
#define SOCKET_FLAG_OPERATE_ASYNC			0x0040  // 让IO线程异步的执行一个函数
#define SOCKET_FLAG_OPERATE_MAX				0x0040  //
	
#define SOCKET_FLAG_READ_OPERATE			(SOCKET_FLAG_OPERATE_READ_ONCE | SOCKET_FLAG_OPERATE_READ_ENDLESS)
#define SOCKET_FLAG_WRITE_OPERATE			(SOCKET_FLAG_OPERATE_WRITE_ONCE | SOCKET_FLAG_OPERATE_WRITE_ENDLESS)
#define SOCKET_FLAG_IO_OPERARE				(SOCKET_FLAG_READ_OPERATE | SOCKET_FLAG_WRITE_OPERATE)
	
#define ADD_OPERATE_FLAG(flag_, operate_type_) (flag_ |= (operate_type_))
#define IS_OPERAGE_FLAG_SET(flag_, operate_type_) (flag_ & (operate_type_))
#define REMOVE_OPERATE_FLAG(flag_, operate_type_) (flag_ &= ~(operate_type_))
	
	
enum net_error_type
{
	XENET_OK							= 0,  // 正常
	XENET_CREATE_SOCKET_FAILED			= -20000,  // 创建socket失败
	XENET_XTCP_CONNECT_FAILED			= -20001,  // tcp链接失败
	XENET_XTCP_SET_NONBLOCK_FAILED		= -20002,  // 设置非阻塞失败
	XENET_XTCP_CONNECT_TIMEOUT			= -20003,  // tcp链接超时
	XENET_XUDP_CONNECT_ABOVE_LIMITS		= -20004,  // 尝试连接超过最大次数
	XENET_XUDP_KEEPLIVE_TIMEOUT			= -20005,  // udp心跳超时，链接断开了
	XENET_CLOSED_NORMAL					= -20006,  // 对端正常关闭了连接
	XENET_CLOSED_ABNORMAL				= -20007,  // 对端非正常关闭了链接
	XENET_MESSAGE_TIMEOUT				= -20008,  // 消息超时
	XENET_MESSAGE_NOT_CONNECT			= -20009,  // 消息发送时，网络处于非正常阶段
};
	
enum SockMngrStatus
{
	SMS_NOT_RUNNING = 1,
	SMS_RUNNING = 2,
};
	
enum OPERATE_RUN_TYPE
{
	OPERATE_RUN_ONE_TIME = 1,  // 只运行一次
	OPERATE_RUN_ENDLESS = 2,  // 不断地运行
};
	
#define XE_SOCKET_STATUS_NOT_CONNECT		1
#define XE_SOCKET_STATUS_CONNECTING			2
#define XE_SOCKET_STATUS_CONNECTED			3
#define XE_SOCKET_STATUS_CLOSING			4
	
#define MSG_SEND_BUFF_SIZE				(2 * 1024 * 1024)	// 消息的发送缓冲大小
#define MSG_RECV_BUFF_SIZE				(2 * 1024 * 1024)	// 接收缓冲大小
	
// socket 事件 定义
#define SOCKET_EVENT_BASE				1000
#define UDP_SOCKET_EVENT_CONNECT		(SOCKET_EVENT_BASE + 1)
#define TCP_SOCKET_EVENT_CONNECT		(SOCKET_EVENT_BASE + 2)
	
// 定时器id 定义
#define TIMER_BASE						10086
#define UDP_TIMER_TRY_CONNECT			(TIMER_BASE + 1)
#define UDP_TIMER_UPDATE_KCP			(TIMER_BASE + 2)
#define TCP_TIMER_CONNECT_TIMEOUT		(TIMER_BASE + 3)
#define TCP_TIMER_MSG_TIMEOUT_CHECK		(TIMER_BASE + 4)
#define UDP_TIMER_MSG_TIMEOUT_CHECK		(TIMER_BASE + 5)
	
#define UDP_CONNECT_TRY_TIMES			3	// udp尝试连接的次数
#define UDP_CONNECT_TRY_INTERVAL		300	// udp每次尝试连接的间隔(ms)
#define KCP_UPDATE_INTERVAL				5
#define KCP_HEARTBEATS_INTERVAL			(5 * 1000)	// kcp的心跳间隔
#define KCP_MESSAGE_MAX_PING			(10 * 1000)	// 最长的 kcp ping值
#define XXP_CHECK_TIMEOUT_INTERVAL		(1 * 1000)	// 检测消息超时时间的间隔
	
	
// 错误格式化信息
class error_system
{
public:
	
	static void format_error(const std::string& file_name, uint32 line_, net_error_type error_type, 
								uint32 error_code, std::string& error_msg);
	
private:
	
	// 获取系统
	static void _get_system_error(uint32 error_code, std::string& error_msg);
	
	static const map<net_error_type, string> m_error_types;
};
	
// 格式化错误信息
#define FORMAT_ERROR_MSG(error_type, error_code, error_msg) \
			error_system::format_error(__FILE__, __LINE__, error_type, error_code, error_msg)
	
}
	
#endif /*_X_SELECT_DEF_H_*/