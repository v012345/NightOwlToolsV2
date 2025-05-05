#ifndef _X_SOCKET_BASE_H_
#define _X_SOCKET_BASE_H_
#include "xenet/XEStdC.h"	
#include "xenet/XEPlatformConfig.h"
#include <memory>
#include "xenet/XESelectDef.h"
#if XE_TARGET_PLATFORM == XE_PLATFORM_WIN32
	#include "winsock.h"
#else
	#include <netinet/in.h>
#endif
	
#include "XEMessage.h"
	
NS_XE_BEGIN
	
class XESelector;
class XESocket;

// socket 操作返回值
enum socket_operate_result
{
	operate_done_ = 0,  // 完成
	socket_status_not_expected = 1,  // socket 状态非预期，如 socket 已经连接上，还发送了链接请求
	socket_work_handling = 2,  // 操作已经在处理了，如连续调用两次close，第二次将返回这个错误
	operate_failed = 3,  // 操作失败
};
	
// 
typedef struct _tag_message_timer
{
public:
	_tag_message_timer(uint32 proto_id, uint32 deadline_)
		: m_proto_id(proto_id)
		, m_deadline(deadline_){};
	
	uint32 m_proto_id;
	uint32 m_deadline;
	
}message_timer, *ptr_message_timer;
	
// 
class XESocketBase : public std::enable_shared_from_this<XESocketBase>
{
public:
	XESocketBase(){};

	void init(XESelector* pSelector){ m_pSelector = pSelector; }

	virtual ~XESocketBase(){};
	
	// 获取 句柄
	virtual SOCKET get_handle() = 0;
	
	// 回调 有数据可以接收
	virtual void on_can_recv() = 0;

	// 回调 有数据可以发送
	virtual void on_can_send() = 0;
	
	// 回掉 关闭处理
	virtual void on_close() = 0;
	
	// 定时器回调
	virtual void on_timer(int id_) = 0;
	
	// 回调 有些数据需要处理
	virtual void on_can_handle(int event_) = 0;
	
protected:
	
	XESelector*				m_pSelector;		// 
};
	
typedef std::weak_ptr<XESocketBase> XESocketBaseWPtr;
typedef std::shared_ptr<XESocketBase> XESocketBasePtr;
	
NS_XE_END
	
#endif /*_X_SOCKET_BASE_H_*/
