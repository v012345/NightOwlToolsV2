#include "xenet/XEInclude.h"
#include "xenet/XEPlatformConfig.h"
#include "xenet/XETime.h"
#include "common/AEUtil.h"
#include <sstream>
#include <iostream>
#include "xenet/XESelectDef.h"
#include "xenet/kcp/xudp_def.hpp"
#include "xenet/XEUDPSocket.h"
	
NS_XE_BEGIN
	
map<XEUDPSocket*, shared_ptr<XEUDPSocket>> XEUDPSocket::m_map_sockets;
XEUDPSocket::XEUDPSocket():m_status_(XE_SOCKET_STATUS_NOT_CONNECT)
						, m_sk_client()
						, m_event_connect(true, false)
						, m_event_close(true, false)
						, m_p_kcp(NULL)
						, m_rcv_buff(new char[MSG_RECV_BUFF_SIZE])
						, m_snd_buff(new char[MSG_SEND_BUFF_SIZE])
						, m_last_send_ts(0)
						, m_last_recv_ts(0)
						, m_last_loss(0.0)
{
	
}
	
XEUDPSocket::~XEUDPSocket()
{
	auto clear_queue = [](queue<XEMessage*>& queue_msg, std::mutex& mutex_, bool b_del)
	{
		mutex_.lock();
		while (!queue_msg.empty())
		{
			XEMessage* p_message = queue_msg.front();
			if (b_del && p_message)
				delete p_message;
			queue_msg.pop();
		}
		mutex_.unlock();
	};

	clear_queue(m_queue_send, m_mtx_send, true);
	clear_queue(m_queue_recv, m_mtx_recv, false);
	
	_clear_except_queue_msg();
	
	delete[] m_rcv_buff;
	delete[] m_snd_buff;
}
	
void XEUDPSocket::init(XESelector* pSelector, const std::string& rc4_key)
{
	XESocketBase::init(pSelector);
	m_rc4_key = rc4_key;
	assert(NULL != pSelector);
}
	
// 链接
socket_operate_result XEUDPSocket::connect(const std::string& ip_, unsigned short port_)
{
	//XELOGWRITE("XEUDPSocket::connect ip:%s , port_:%d", ip_.c_str(), port_);
	cout << "XEUDPSocket:connect: ip_[" << ip_ << "] port_[" << port_ << "]" << endl;
	// 
	int status_ = m_status_.compare_and_set(XE_SOCKET_STATUS_NOT_CONNECT, XE_SOCKET_STATUS_CONNECTING);
	if (XE_SOCKET_STATUS_NOT_CONNECT != status_)
	{
		if (XE_SOCKET_STATUS_CONNECTING == status_)
		{
			return socket_operate_result::socket_work_handling;
		}
		else
		{
			return socket_operate_result::socket_status_not_expected;
		}
	}
	
	m_try_connect_times = 0;
	m_event_close.reset_event();
	m_event_connect.reset_event();
	
	//m_rmt_addr.sin_family = AF_INET;
	//m_rmt_addr.sin_addr.s_addr = inet_addr(ip_.c_str());
	//m_rmt_addr.sin_port = htons(port_);
	m_rmt_addr.set_host(ip_);
	m_rmt_addr.set_port(port_);
	
	_clear_except_queue_msg();
	
	m_pSelector->async_do_something(shared_from_this(), UDP_SOCKET_EVENT_CONNECT);
	/*
	if (wait_result::wait_failed == wait_connect(-1))
	{
		uint32 error_code = 0, error_info = 0;
		if (!is_alive())
		{
			get_error_info(error_code, error_info);
			cout << "error_code:" << error_code << " error_info:" << error_info << endl;
			//XELOGWRITE("XEUDPSocket::connect error error_code:%d, error_info:%d", error_code, error_info);
			return socket_operate_result::operate_failed;
		}
	}
	*/
	//XELOGWRITE("XEUDPSocket::connect success");
	return socket_operate_result::operate_done_;
}
	
// 等待链接上
wait_result XEUDPSocket::wait_connect(int ms_wait)
{
	long status_ = get_status();
	switch (status_)
	{
	case XE_SOCKET_STATUS_CONNECTING:
		return m_event_connect.timed_wait(ms_wait);
	case XE_SOCKET_STATUS_CONNECTED:
		return wait_result::wait_one;
	default:
		break;
	}
	return wait_result::wait_failed;
}
	
void XEUDPSocket::sendMessage(XEMessage* pMessage, uint32 timeout_ms/* = 5 * 1000*/)
{
	if (NULL == pMessage)
		return ;
	
	if (get_status() != XE_SOCKET_STATUS_CONNECTED)
	{
		_push_except_msg(XENET_MESSAGE_NOT_CONNECT, pMessage->getProtocolID(), pMessage->getSequenceID());
		return ;
	}
	
	// 消息副本
	XEMessage * p_message_copy = pMessage->clone();
	if (NULL == p_message_copy)
	{
		return ;
	}
	
	m_mtx_send.lock();
	bool is_queue_empty = m_queue_send.empty();
	m_queue_send.push(p_message_copy);
	m_mtx_send.unlock();
	
	if (0 != timeout_ms)
	{
		uint32 ts_deadline = iclock() + timeout_ms;
		m_mtx_timeout.lock();
		m_timeout_check.emplace(p_message_copy->getSequenceID(), message_timer(p_message_copy->getProtocolID(), ts_deadline));
		m_mtx_timeout.unlock();
	}
	
	if (is_queue_empty)
	{
		m_pSelector->async_send(shared_from_this(), OPERATE_RUN_TYPE::OPERATE_RUN_ONE_TIME);
	}
}
	
// 发送消息
bool XEUDPSocket::send_message(XEMessage* pMessage, uint32 timeout_ms/* = 5 * 1000*/)
{
	if (NULL == pMessage) return false;
	
	if (XE_SOCKET_STATUS_CONNECTED != get_status())
	{
		_push_except_msg(XENET_MESSAGE_NOT_CONNECT, pMessage->getProtocolID(), pMessage->getSequenceID());
		return false;
	}
	
	XEMessage * p_message_copy = pMessage->clone();
	if (NULL == p_message_copy)
	{
		return false;
	}
	
	m_mtx_send.lock();
	
	bool is_queue_empty = m_queue_send.empty();
	m_queue_send.push(p_message_copy);
	m_mtx_send.unlock();
	
	if (0 != timeout_ms)
	{
		uint32 ts_deadline = iclock() + timeout_ms;

		m_mtx_timeout.lock();
		m_timeout_check.emplace(p_message_copy->getSequenceID(), message_timer(p_message_copy->getProtocolID(), ts_deadline));
		m_mtx_timeout.unlock();
	}
	
	if (is_queue_empty)
	{
		m_pSelector->async_send(shared_from_this(), OPERATE_RUN_TYPE::OPERATE_RUN_ONE_TIME);
	}
	
	return true;
}
	
int XEUDPSocket::getSocketState()
{
	int state = 0;
	if (get_status() == XE_SOCKET_STATUS_CONNECTED)
	{
		state = 1;
	}
	else if (get_status() <= XE_SOCKET_STATUS_CONNECTING){
		state = 0;
	}
	else{
		state = 2;
	}
	//XELOGWRITE("XEUDPSocket::getSocketState state:%d , get_status:%d", state, get_status());
	return get_status();
}

// 获取消息
bool XEUDPSocket::recv_message(XEMessage* &pMessage)
{
	pMessage = receiveMessage();
	
	return true;
}
	
XEMessage* XEUDPSocket::receiveMessage()
{
	auto peek_message = [this](std::mutex& mtx_, queue<XEMessage*>& msg_queue)->XEMessage*
	{
		XEMessage* p_ret_ = NULL;
		std::lock_guard<std::mutex> locker_(mtx_);
		if (!msg_queue.empty())
		{
			p_ret_ = msg_queue.front();
			msg_queue.pop();
		}
		return p_ret_;
	};

	if (get_status() != XE_SOCKET_STATUS_CONNECTED)
	{
		return peek_message(m_mtx_except, m_queue_except);
	}

	XEMessage* p_ret_ = peek_message(m_mtx_recv, m_queue_recv);
	if (NULL == p_ret_)
	{
		p_ret_ = peek_message(m_mtx_except, m_queue_except);
	}
	
	return p_ret_;
}

bool XEUDPSocket::is_alive()
{
	// 
	return XE_SOCKET_STATUS_CONNECTED == get_status();
}
	
// 关闭连接
bool XEUDPSocket::close()
{
	if (XE_SOCKET_STATUS_CONNECTING == m_status_.compare_and_set(XE_SOCKET_STATUS_CONNECTING, XE_SOCKET_STATUS_CLOSING)
		|| XE_SOCKET_STATUS_CONNECTED == m_status_.compare_and_set(XE_SOCKET_STATUS_CONNECTED, XE_SOCKET_STATUS_CLOSING))
	{
		m_pSelector->async_close(shared_from_this());
	}
	return true;
}
	
// 等待关闭
wait_result XEUDPSocket::wait_close(int ms_wait)
{
	long status_ = get_status();
	switch (status_)
	{
	case XE_SOCKET_STATUS_CLOSING:
		return m_event_close.timed_wait(ms_wait);
	case XE_SOCKET_STATUS_NOT_CONNECT:
		return wait_result::wait_one;
	default:
		break;
	}
	return wait_result::wait_failed;
}
	
// 错误信息
void XEUDPSocket::get_error_info(uint32& error_code, uint32& error_info)
{
	error_code = m_error_code;
	error_info = m_error_info;
}
	
// 获取句柄
SOCKET XEUDPSocket::get_handle()
{
	return m_sk_client.get_handle();
}
	
long XEUDPSocket::get_status()
{
	// 
	return m_status_.get_value();
}
	
// 是否有必要切换到TCP
bool XEUDPSocket::is_tcp_necessary()
{
	// 
	return m_last_loss > 0.3; // 30 % 
}
	
// udp 输出
int XEUDPSocket::_udp_output(const char *buf, int len, struct IKCPCB *kcp, void *user)
{
	// 
	return ((XEUDPSocket*)user)->_kcp_udp_output(buf, len);
}
	
// 尝试链接
socket_operate_result XEUDPSocket::_try_connect(int& error_code)
{
	if (get_status() != XE_SOCKET_STATUS_CONNECTING)
		return socket_operate_result::socket_status_not_expected;
	
	string key_connect = xudp::make_kcp_key_connect();
	int ret_ = _system_udp_output(key_connect.c_str(), key_connect.length());
	if (-1 == ret_)
	{
		// 
		error_code = ERRORNO;
		return socket_operate_result::operate_failed;
	}
	
	return socket_operate_result::operate_done_;
}
	
int XEUDPSocket::_kcp_udp_output(const char *buf, int len)
{
	//
	int ret_ = this->_system_udp_output(buf, len);
	if (-1 == ret_)
	{
		_set_error(net_error_type::XENET_CLOSED_ABNORMAL, ERRORNO);
		_active_close_socket();
	}
	
	if (ret_ != len)
	{
		
		cout << "ret( "<< ret_ << ") is not equal len (" << len<<")." << endl;
	}
	
	//cout << "time : " << iclock() << ", send package : " << len << endl;
	
	return ret_;
}
	
void XEUDPSocket::on_timer(int id_)
{
	switch (id_)
	{
	case UDP_TIMER_TRY_CONNECT:
		_on_timer_test_connect(id_);
		break;
	case UDP_TIMER_UPDATE_KCP:
		_on_timer_update_kcp(id_);
		break;
	case UDP_TIMER_MSG_TIMEOUT_CHECK:
		_on_timer_check_timeout(id_);
		break;
	default:
		break;
	}
}
	
// 
int XEUDPSocket::_system_udp_output(const char *buf, int len)
{
	
	// udp socket send
	return m_sk_client.send(buf, len, 0);
}
	
void XEUDPSocket::_on_timer_update_kcp(int key_)
{
	if (get_status() != XE_SOCKET_STATUS_CONNECTED)
	{
		return;
	}
	
	uint32 ts_now_ = iclock();
	
	// 内部单线程调用，不需要加锁
	if (-1 == ikcp_update(m_p_kcp, ts_now_))
	{
		return;
	}
	
	// 检测是否掉线
	if (ts_now_ - m_last_recv_ts >= KCP_MESSAGE_MAX_PING)
	{
		_set_error(net_error_type::XENET_XUDP_KEEPLIVE_TIMEOUT, 0);
		_active_close_socket();
	}
	
}
	
// 尝试连接 定时器
void XEUDPSocket::_on_timer_test_connect(int key_)
{
	if (get_status() != XE_SOCKET_STATUS_CONNECTING) return;

	if (m_try_connect_times++ > UDP_CONNECT_TRY_TIMES)
	{
		_set_error(net_error_type::XENET_XUDP_CONNECT_ABOVE_LIMITS, 0);
		_active_close_socket();
		return ;
	}
	
	int error_code = 0;
	socket_operate_result result_ = _try_connect(error_code);
	if (socket_operate_result::operate_failed == result_
		&& net_error_type::XENET_CLOSED_ABNORMAL == error_code)
	{
		_set_error(net_error_type::XENET_CLOSED_ABNORMAL, ERRORNO);
		_active_close_socket();
		return ;
	}
	
	m_pSelector->add_timer(shared_from_this(), OPERATE_RUN_TYPE::OPERATE_RUN_ONE_TIME, UDP_CONNECT_TRY_INTERVAL, UDP_TIMER_TRY_CONNECT);
}
	
// 检测消息超时
void XEUDPSocket::_on_timer_check_timeout(int key_)
{
	uint32 ts_now = iclock();
	vector<uint32> timeout_proto;

	m_mtx_timeout.lock();
	std::unordered_map<uint32, message_timer>::iterator it_timeout = m_timeout_check.begin();
	for (; it_timeout != m_timeout_check.end();)
	{
		if (ts_now > it_timeout->second.m_deadline)
		{
			timeout_proto.push_back(it_timeout->second.m_proto_id);
			it_timeout = m_timeout_check.erase(it_timeout);
		}
		else
		{
			++it_timeout;
		}
	}
	m_mtx_timeout.unlock();

	if (!timeout_proto.empty())
	{
		_push_except_msg(XENET_MESSAGE_TIMEOUT, timeout_proto);
	}
}
	
// 检测是否需要发送心跳
void XEUDPSocket::_check_heart_beats_required(uint32 ts_now)
{
	if (ts_now - m_last_send_ts >= KCP_HEARTBEATS_INTERVAL)
	{
		// 空包做心跳
		ikcp_send(m_p_kcp, "", 0);
		m_last_send_ts = ts_now;
	}
}
	
// 回调 有数据可以接收
void XEUDPSocket::on_can_recv()
{
	if (get_status() != XE_SOCKET_STATUS_CONNECTED && get_status() != XE_SOCKET_STATUS_CONNECTING)
		return ;
	
	int ret_ = m_sk_client.recv(m_rcv_buff, MSG_RECV_BUFF_SIZE, 0);
	
	if (-1 == ret_)
	{
		_set_error(net_error_type::XENET_CLOSED_ABNORMAL, ERRORNO);
		_active_close_socket();
	}
	else if (0 == ret_)
	{
		_set_error(net_error_type::XENET_CLOSED_NORMAL, ERRORNO);
		_active_close_socket();
		
	}else{
		
		// 检查是否是命令
		m_rcv_buff[ret_] = 0;
		if (xudp::is_kcp_key_connect_response(m_rcv_buff, ret_))
		{
			int conv_ = xudp::get_conv_from_key_connect_response(m_rcv_buff);
			if (m_status_.compare_and_set(XE_SOCKET_STATUS_CONNECTING, XE_SOCKET_STATUS_CONNECTED) == XE_SOCKET_STATUS_CONNECTING)
			{
				m_p_kcp = ikcp_create(conv_, this);
				ikcp_wndsize(m_p_kcp, 256, 256);
				
				m_p_kcp->output = &XEUDPSocket::_udp_output;
				ikcp_nodelay(m_p_kcp, 1, 5, 2, 1);
				
				m_pSelector->add_timer(shared_from_this(), OPERATE_RUN_TYPE::OPERATE_RUN_ENDLESS, 
											KCP_UPDATE_INTERVAL, UDP_TIMER_UPDATE_KCP);
				
				m_pSelector->add_timer(shared_from_this(), OPERATE_RUN_TYPE::OPERATE_RUN_ENDLESS,
											XXP_CHECK_TIMEOUT_INTERVAL, UDP_TIMER_MSG_TIMEOUT_CHECK);
				
				m_last_loss = 0;
				m_last_recv_ts = m_last_send_ts = iclock();
				
				m_event_connect.set_event();
			}
		}
		else
		{
			ikcp_input(m_p_kcp, m_rcv_buff, ret_);
			++ (m_p_kcp->statistics_value[IKCP_STATICTICS_RECV_TIMES]);
			
			_do_recv();
			m_last_recv_ts = iclock();
		}
	}
	
}

// 回调 有数据可以发送
void XEUDPSocket::on_can_send()
{
	// 
	if (get_status() != XE_SOCKET_STATUS_CONNECTED)
	{
		return;
	}
	
	uint32 ts_now_ = iclock();
	uint32 snt_count_ = _do_send();
	if (0 != snt_count_)
		m_last_send_ts = ts_now_;
	
	if (-1 == ikcp_flush(m_p_kcp))
	{
		// closed by local
		return ;
	}
	
	m_p_kcp->ts_flush = ts_now_;

	// 检测是否需要心跳
	if (0 == snt_count_)
		_check_heart_beats_required(ts_now_);
}
	
// 回调 关闭
void XEUDPSocket::on_close()
{
	if (get_status() != XE_SOCKET_STATUS_CLOSING) return;
	
	if (m_sk_client.is_valid() && NULL != m_p_kcp)
	{
		string key_disconnect = xudp::make_kcp_key_disconnect(m_p_kcp->conv);
		
		// 
		for (int i = 0; i < 3; ++i)
		{
			// 
			_system_udp_output(key_disconnect.c_str(), key_disconnect.length());
		}
	}
	
	// 
	_close_socket();
}
	
// 回调 有些数据需要处理
void XEUDPSocket::on_can_handle(int event_)
{
	if (event_ == UDP_SOCKET_EVENT_CONNECT)
		_do_connect();
}
	
void XEUDPSocket::_do_connect()
{
	if (XE_SOCKET_STATUS_CONNECTING != get_status())
	{
		// 
		return ;
	}
	
	m_rc4_send.init(m_rc4_key);
	m_rc4_recv.init(m_rc4_key);
	
	if (!m_sk_client.create(m_rmt_addr.family(), SOCK_DGRAM, IPPROTO_UDP))
	{
		_set_error(net_error_type::XENET_CREATE_SOCKET_FAILED, ERRORNO);
		_active_close_socket();
		return ;
	}
	
#if XE_TARGET_PLATFORM == XE_PLATFORM_WIN32
	
	DWORD dwBytesReturned = 0;
	BOOL bNewBehavior = FALSE;
	WSAIoctl(m_sk_client.get_handle(), SIO_UDP_CONNRESET, &bNewBehavior, sizeof(bNewBehavior),
						NULL, 0, &dwBytesReturned, NULL, NULL);

#endif //
	
	// 发送请求连接的
	if (!m_sk_client.connect(m_rmt_addr))
	{
		_set_error(net_error_type::XENET_XUDP_CONNECT_ABOVE_LIMITS, ERRORNO);
		_active_close_socket();
		m_status_.set_value(XE_SOCKET_STATUS_NOT_CONNECT);
		return;
	}
	
	// 开始读取
	m_pSelector->async_recv(shared_from_this(), OPERATE_RUN_ENDLESS);

	// 尝试连接一次
	int error_code = 0;
	if (socket_operate_result::operate_failed == _try_connect(error_code)
			&& net_error_type::XENET_CLOSED_ABNORMAL == error_code)
	{
		_set_error(net_error_type::XENET_CLOSED_ABNORMAL, ERRORNO);
		_active_close_socket();
		m_status_.set_value(XE_SOCKET_STATUS_NOT_CONNECT);
		return ;
	}
	
	m_pSelector->add_timer(shared_from_this(), OPERATE_RUN_ONE_TIME, 300, UDP_TIMER_TRY_CONNECT);
}
	
// 接收消息
uint32 XEUDPSocket::_do_recv()
{
	uint32 ret_ = 0;
	for (;;)
	{
		int kcp_ret_ = ikcp_recv(m_p_kcp, m_rcv_buff, MSG_RECV_BUFF_SIZE);
		if (kcp_ret_ < 0) break;
		
		m_rc4_recv.update((const xe::byte*)m_rcv_buff, (xe::byte*)m_rcv_buff, kcp_ret_);
		
		assert(kcp_ret_ >= (sizeof(uint32) + sizeof(Aone2CmdHeader_)));
		if (kcp_ret_ < (sizeof(uint32)+sizeof(Aone2CmdHeader_)))
		{
			std::cout << "XEUDPSocket::_do_recv: not enough for a message (" << kcp_ret_ <<")." << endl;
			break;
		}
		
		uint32 msg_len = *(uint32*)m_rcv_buff;
		Aone2CmdHeader_* header_ = (Aone2CmdHeader_*)(m_rcv_buff + sizeof(uint32));
			
		if (msg_len <= kcp_ret_)
		{
			XEMessage* pMessage = XEMessage::receive(msg_len);
			XEByteBuffer* pBuffer = pMessage->getByteBuffer();
			
			// 
			if (28002 == header_->m_cmdNum)
			{
				// 
				header_->m_reserve = static_cast<uint32>(ae::AEUtil::getMS());
			}
			
			pBuffer->setBuffer((const char*)m_rcv_buff, sizeof(uint32), msg_len - sizeof(uint32));
			pBuffer->flip();
			
			m_mtx_recv.lock();
			m_queue_recv.push(pMessage);
			m_mtx_recv.unlock();
			
			m_mtx_except.lock();
			m_timeout_check.erase(header_->m_cmdSeq);
			m_mtx_except.unlock();
			
			++ ret_;
		}
	}
	
	return ret_;
}
	
// 发送消息
xe::uint32 XEUDPSocket::_do_send()
{
	xe::uint32 ret_ = 0;
	XEMessage* pMessage = NULL;
	for (;;)
	{
		m_mtx_send.lock();
		if (m_queue_send.empty())
		{
			m_mtx_send.unlock();
			break;
		}
		
		pMessage = m_queue_send.front();
		m_queue_send.pop();
		m_mtx_send.unlock();
		
		if (NULL == pMessage) continue;
		
		XEByteBuffer *pBuffer = pMessage->getByteBuffer();
		if (NULL == pBuffer) continue;
		
		int len_ = pBuffer->getCapacity();
		
		const char* sz_buff = pBuffer->getBuffer();
		if ( 27004 == pMessage->getProtocolID()
			&& len_ > (sizeof(uint32) + sizeof(Aone2CmdHeader_))
			&& NULL != sz_buff)
		{
			
			Aone2CmdHeader_* header_ = (Aone2CmdHeader_*)(sz_buff + sizeof(uint32));
			
			float64 f_time_now = ae::AEUtil::getMS();
			header_->m_reserve = static_cast<uint32>(f_time_now);
		}
		
		
		memcpy(m_snd_buff, pBuffer->getBuffer(), len_);

		delete pMessage;

		m_rc4_send.update((const xe::byte*)m_snd_buff, (xe::byte*)m_snd_buff, len_);
		ikcp_send(m_p_kcp, m_snd_buff, len_);
		
		++ ret_;
	}
	
	return ret_;
}
	
// 主动关闭socket
void XEUDPSocket::_active_close_socket()
{
	// 
	_push_except_msg(m_error_code);
	_move_timeout_to_except();
	
	_close_socket();
}
	
// 关闭socket
void XEUDPSocket::_close_socket()
{
	// 
	if (NULL != m_p_kcp)
	{
		m_last_loss = ikcp_getlastresenttimes(m_p_kcp);
		ikcp_release(m_p_kcp);
		m_p_kcp = NULL;
	}
	
	m_pSelector->remove_socket(shared_from_this());
	m_sk_client.close();

	m_mtx_send.lock();
	while (!m_queue_send.empty())
	{
		XEMessage *pMessage = m_queue_send.front();
		if (pMessage) delete pMessage;
		
		m_queue_send.pop();
	}
	m_mtx_send.unlock();

	m_mtx_recv.lock();
	while (!m_queue_recv.empty())
	{
		XEMessage *pMessage = m_queue_recv.front();
		if (pMessage) delete pMessage;
		m_queue_recv.pop();
	}
	m_mtx_recv.unlock();
	
	m_mtx_timeout.lock();
	m_timeout_check.clear();
	m_mtx_timeout.unlock();
	
	m_event_close.set_event();
	m_event_connect.set_event();

	m_status_.set_value(XE_SOCKET_STATUS_NOT_CONNECT);
}
	
// 增加错误信息
void XEUDPSocket::_push_except_msg(uint32 error_code, uint32 protocol_id/* = 0*/, uint32 msg_seq/* = 0*/)
{
	// 
	m_mtx_except.lock();
	m_queue_except.push(XEMessage::errorMessage(error_code, protocol_id + 1, msg_seq));
	m_mtx_except.unlock();
}

// 增加错误信息
void XEUDPSocket::_push_except_msg(uint32 error_code, const vector<uint32>& protocol_ids)
{
	m_mtx_except.lock();
	auto it_proto = protocol_ids.begin();
	for (; it_proto != protocol_ids.end(); ++it_proto)
	{
		m_queue_except.push(XEMessage::errorMessage(error_code, *it_proto + 1));
	}
	m_mtx_except.unlock();
}
	
// 移动超时消息到异常消息
void XEUDPSocket::_move_timeout_to_except()
{
	vector<uint32> timeout_proto;

	m_mtx_timeout.lock();
	std::unordered_map<uint32, message_timer>::iterator it_timeout = m_timeout_check.begin();
	for (; it_timeout != m_timeout_check.end(); ++it_timeout)
	{
		timeout_proto.push_back(it_timeout->second.m_proto_id);
	}
	m_timeout_check.clear();
	m_mtx_timeout.unlock();

	if (!timeout_proto.empty())
	{
		_push_except_msg(XENET_MESSAGE_TIMEOUT, timeout_proto);
	}
}
	
// 删除异常队列中消息
void XEUDPSocket::_clear_except_queue_msg()
{
	m_mtx_except.lock();
	while (!m_queue_except.empty())
	{
		XEMessage*p_message = m_queue_except.front();
		if (p_message) delete p_message;
		m_queue_except.pop();
	}
	m_mtx_except.unlock();
}
	
// 设置错误码
void XEUDPSocket::_set_error(uint32 error_code, uint32 error_info)
{
	m_error_code = error_code;
	m_error_info = error_info;
}
	
XEUDPSocket* XEUDPSocket::createSocket(XESelector* pSelector, const std::string &rc4_key)
{
	shared_ptr<XEUDPSocket> ptr_socket(new XEUDPSocket());
	ptr_socket.get()->init(pSelector, rc4_key);
	if (ptr_socket)
	{
		m_map_sockets.insert(make_pair(ptr_socket.get(), ptr_socket));
	}
	return ptr_socket.get();
}

void XEUDPSocket::releaseSocket(XEUDPSocket* p_socket)
{
	auto it = m_map_sockets.find(p_socket);
	if (it != m_map_sockets.end())
	{
		m_map_sockets.erase(it);
	}
}

void XEUDPSocket::disconnect()
{
	close();
}

NS_XE_END