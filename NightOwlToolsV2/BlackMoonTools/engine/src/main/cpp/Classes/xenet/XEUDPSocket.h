#ifndef _XEUDP_SOCKET_H_
#define _XEUDP_SOCKET_H_
	
//*********************************************************************
// FileName		:	XEUDPSocket.h
// Author		:	majianwei
// Date			:	2017/12/11
// Description	:	udp socket 封装
//*********************************************************************
	
#include <queue>
#include <mutex>
#include <string>
#include "xenet/XEStdC.h"
#include "xenet/XEEvent.h"
#include "xenet/XEAtomic.h"
#include "xenet/XESocket.h"
#include "xenet/XERC4.h"
#include "xenet/kcp/ikcp.h"
#include "xenet/XESelector.h"
#include "xenet/XEMessage.h"
#include "xenet/XESocketAddr.h"

	
NS_XE_BEGIN
	
class XEUDPSocket : public XESocketBase
{
public:
	
	XEUDPSocket();
	~XEUDPSocket();
	
	void init(XESelector* pSelector, const std::string& rc4_key);
	
	// 链接
	socket_operate_result connect(const std::string& ip_, unsigned short port_);

	// 等待链接上
	wait_result wait_connect(int ms_wait);
	
	// 发送消息

	void sendMessage(XEMessage* pMessage, uint32 timeout_ms = 5 * 1000);
	bool send_message(XEMessage* pMessage, uint32 timeout_ms = 5 * 1000);

	// 获取消息
	XEMessage* receiveMessage();
	bool recv_message(XEMessage* &pMessage);
	
	void disconnect();
	
	int getSocketState();
	
	// 是否链接还在
	bool is_alive();
	
	// 关闭连接
	bool close();
	
	// 等待关闭
	wait_result wait_close(int ms_wait);
	
	// 错误信息
	void get_error_info(uint32& error_code, uint32& error_info);
	
	// 获取句柄
	SOCKET get_handle();
	
	// 获取当前链接状态
	long get_status();
	
	// 是否有必要切换到TCP
	bool is_tcp_necessary();
	
	static  XEUDPSocket* createSocket(XESelector* pSelector, const std::string &rc4_key);
	
	static void releaseSocket(XEUDPSocket* pSelector);
	
private:
	
	// 回调 有数据可以接收
	void on_can_recv();

	// 回调 有数据可以发送
	void on_can_send();

	// 回调 关闭
	void on_close();
	
	// 定时器回调
	virtual void on_timer(int id_);

	// 回调 有些数据需要处理
	void on_can_handle(int event_);
	
private:
	
	// udp 输出
	static int _udp_output(const char *buf, int len, struct IKCPCB *kcp, void *user);
	
	// 尝试链接
	socket_operate_result _try_connect(int& error_code);
	
	// 系统udp发送
	int _system_udp_output(const char *buf, int len);
	
	// kcp 用到的udp接口，增加了对send返回-1的判断
	int _kcp_udp_output(const char *buf, int len);
	
	// kcp 定时更新
	void _on_timer_update_kcp(int key_);
	
	// 尝试连接 定时器
	void _on_timer_test_connect(int key_);
	
	// 检测消息超时
	void _on_timer_check_timeout(int key_);
	
	// 检测是否需要发送心跳
	void _check_heart_beats_required(uint32 ts_now);
	
	// 
	void _do_connect();
	
	// 接收消息
	uint32 _do_recv();
	
	// 发送消息
	uint32 _do_send();
	
	// 主动关闭socket
	void _active_close_socket();
	
	// 关闭socket
	void _close_socket();
	
	// 增加错误信息
	void _push_except_msg(uint32 error_code, uint32 protocol_id = 0, uint32 msg_seq = 0);
	void _push_except_msg(uint32 error_code, const vector<uint32>& protocol_ids);
	
	// 移动超时消息到异常消息
	void _move_timeout_to_except();
	
	// 删除异常队列中消息
	void _clear_except_queue_msg();
	
	// 设置错误码
	void _set_error(uint32 error_code, uint32 error_info);

private:
	
	XEAtomic				m_status_;			// 
	XESocket				m_sk_client;		// 
	string					m_rc4_key;			// 
	XESocketAddr			m_rmt_addr;			// 远端地址
	XEEvent					m_event_connect;	// 链接事件
	XEEvent					m_event_close;		// 关闭控制
	std::mutex				m_mtx_recv;			// 接收令牌
	std::mutex				m_mtx_send;			// 发送令牌
	std::mutex				m_mtx_except;		// 
	std::mutex				m_mtx_timeout;		// 超时相关管理
	queue<XEMessage*>		m_queue_recv;		// 
	queue<XEMessage*>		m_queue_send;		// 
	queue<XEMessage*>		m_queue_except;		// 异常消息
	std::unordered_map<uint32, message_timer>	m_timeout_check;	// 
	
	ikcpcb*					m_p_kcp;			// 
	XERC4					m_rc4_recv;			// 
	XERC4					m_rc4_send;			// 
	char*					m_rcv_buff;			// 
	char*					m_snd_buff;			// 发送缓冲
	uint32					m_try_connect_times;// 尝试链接次数 
	
	uint32					m_error_code;		// 
	uint32					m_error_info;		// 
	
	uint32					m_last_send_ts;		// 最后一条消息发送的时间戳
	uint32					m_last_recv_ts;		// 最后一条消息接收的时间戳
	
	mutex					m_mtx_ping;			// 
	volatile long			m_ping_times;		// 
	volatile long			m_ping_total;		// 
	float					m_last_loss;		// 最近的网络包丢失情况
	static map<XEUDPSocket*, shared_ptr<XEUDPSocket>> m_map_sockets;
};
	
typedef shared_ptr<XEUDPSocket> XEUDPSocketPtr;
	
NS_XE_END
	
	
#endif /*_XEUDP_SOCKET_H_*/