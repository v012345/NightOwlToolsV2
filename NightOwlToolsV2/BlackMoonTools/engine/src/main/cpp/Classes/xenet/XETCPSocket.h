#ifndef _XTCP_SOCKET_H_
#define _XTCP_SOCKET_H_
	
//*********************************************************************
// FileName		:	XETCPSocket.h
// Author		:	majianwei
// Date			:	2017/12/9
// Description	:	tcp socket的封装
//*********************************************************************
#include <queue>
#include <mutex>
#include <string>
#include "xenet/XERC4.h"
#include "xenet/XEEvent.h"
#include "xenet/XEAtomic.h"
#include "xenet/XESocket.h"
#include "xenet/XESocketBase.h"
#include "xenet/XESelector.h"
#include "xenet/XEMessage.h"
#include "xenet/XESocketAddr.h"
	
NS_XE_BEGIN
	
class XETCPSocket : public XESocketBase
{
public:
	XETCPSocket();
	~XETCPSocket();
	
	// 
	void init(XESelector* pSelector, const std::string& rc4_key);
	
	// 链接
	socket_operate_result connect(const std::string& ip_, unsigned short port_);
	
	// 等待链接上
	wait_result wait_connect(int ms_wait);
	
	// 发送消息
	bool send_message(XEMessage* pMessage, uint32 timeout_ms = 5 * 1000);
	
	void sendMessage(XEMessage* pMessage, uint32 timeout_ms = 5 * 1000);
	
	// 获取消息
	bool recv_message(XEMessage* &pMessage);
	
	XEMessage* receiveMessage();
	
	void disconnect();
	
	int getSocketState();
	
	// 是否链接还在
	bool is_alive();
	
	// 关闭连接
	bool close();
	
	// 等待关闭
	wait_result wait_close(int ms_wait);
	
	// 获取句柄
	SOCKET get_handle();
	
	// 错误信息
	void get_error_info(uint32& error_code, uint32& error_info);

	static XETCPSocket* createSocket(XESelector* pSelector, const std::string &rc4_key);

	static void  releaseSocket(XETCPSocket* p_socket);

	string getName();
	void setName(string _name);
	
private:
	
	// 回调 有数据可以接收
	virtual void on_can_recv();
	
	// 回调 有数据可以发送
	virtual void on_can_send();
	
	// 回掉 关闭处理
	virtual void on_close();
	
	// 定时器回调
	virtual void on_timer(int id_);
	
	// 回调 有些数据需要处理
	virtual void on_can_handle(int event_);
	
private:
	
	// 获取状态信息
	long get_status();
	
	// 主动关闭socket
	void _active_close_socket();
	
	// 增加错误信息
	void _push_except_msg(uint32 error_code, uint32 protocol_id = 0, uint32 msg_seq = 0);
	void _push_except_msg(uint32 error_code, const vector<uint32>& protocol_ids);
	
	// 错误信息
	void _set_error_info(uint32 error_code, uint32 error_info);
	
	// 关闭socket
	void _close_socket();
	
	// 检测消息超时
	void _check_msg_timeout();
	
	// 移动超时消息到异常消息
	void _move_timeout_to_except();
	
	// 删除异常队列中消息
	void _clear_except_queue_msg();
	
private:
	
	XEAtomic				m_status_;
	XESocket				m_sk_client;		// 
	string					m_rc4_key;			// 
	XESocketAddr			m_rmt_addr;			// 远端地址
	std::mutex				m_mtx_send;			// 
	std::mutex				m_mtx_recv;			// 
	std::mutex				m_mtx_except;		// 
	std::mutex				m_mtx_timeout;		// 超时相关管理
	queue<XEMessage*>		m_queue_send;			// 
	queue<XEMessage*>		m_queue_recv;		// 
	queue<XEMessage*>		m_queue_except;		// 异常消息
	std::unordered_map<uint32, message_timer>	m_timeout_check;	// 
	XEByteBuffer			m_sendBuffer;		// 
	XEByteBuffer			m_recvBuffer;		// 
	XERC4					m_rc4_send;			// 
	XERC4					m_rc4_recv;			// 
	XEEvent					m_event_connect;	// 链接事件
	XEEvent					m_event_close;		// 关闭控制
	uint32					m_error_code;		// 
	uint32					m_error_info;		// 
	string					m_name;				// 
	static map<XETCPSocket*, shared_ptr<XETCPSocket>> m_map_sockets;
};
	
typedef shared_ptr<XETCPSocket> XETCPSocketPtr;
	
NS_XE_END
	
#endif /*_XTCP_SOCKET_H_*/