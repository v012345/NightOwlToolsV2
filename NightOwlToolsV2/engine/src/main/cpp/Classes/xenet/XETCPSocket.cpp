#include "xenet/XEStdC.h"
#include "XETime.h"
#include <iostream>
#include "xenet/XETCPSocket.h"
	
NS_XE_BEGIN
	
#define	MESSAGE_TIMEOUT_TMS			(5 * 1000)
	
map<XETCPSocket*, shared_ptr<XETCPSocket>> XETCPSocket::m_map_sockets;

XETCPSocket::XETCPSocket() : m_status_(XE_SOCKET_STATUS_NOT_CONNECT)
												, m_sk_client()
												, m_sendBuffer(MSG_SEND_BUFF_SIZE)
												, m_recvBuffer(MSG_RECV_BUFF_SIZE)
												, m_event_connect(true, false)
												, m_event_close(true, false)
												, m_error_code(0)
												, m_error_info(0)
{
	
}
	
XETCPSocket::~XETCPSocket()
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
}

	
void XETCPSocket::init(XESelector* pSelector, const std::string& rc4_key)
{
	XESocketBase::init(pSelector);
	m_rc4_key = rc4_key;
	assert(NULL != pSelector);
}

// 链接
socket_operate_result XETCPSocket::connect(const std::string& ip_, unsigned short port_)
{
	cout << "XETCPSocket:connect: done ip_[" << ip_ << "] port_[" << port_ << "]" << endl;
	XELOGWRITE("XETCPSocket::connect done ip:%s , port_:%d", ip_.c_str(), port_);
	long result_ = m_status_.compare_and_set(XE_SOCKET_STATUS_NOT_CONNECT, XE_SOCKET_STATUS_CONNECTING);
	if (XE_SOCKET_STATUS_NOT_CONNECT != result_)
	{
		if (result_ == XE_SOCKET_STATUS_CONNECTING)
		{
			return socket_operate_result::socket_work_handling; // 已经在执行了
		}
		else
		{
			return socket_operate_result::socket_status_not_expected;  // 当前状态不允许链接
		}
	}
	
	m_event_close.reset_event();
	int connectValue = m_event_connect.reset_event();
	XELOGWRITE("XETCPSocket::connect m_event_connect reset_event:%d", connectValue);
	//m_rmt_addr.sin_family = AF_INET;
	//m_rmt_addr.sin_addr.s_addr = inet_addr(ip_.c_str());
	//m_rmt_addr.sin_port = htons(port_);
	m_rmt_addr.set_host(ip_);
	m_rmt_addr.set_port(port_);
	
	_clear_except_queue_msg();
	
	m_pSelector->async_do_something(shared_from_this(), TCP_SOCKET_EVENT_CONNECT);
	
	/**
	if (wait_result::wait_failed == wait_connect(-1))
	{
		uint32 error_code = 0, error_info = 0;
		if (!is_alive())
		{
			get_error_info(error_code, error_info);
			cout << "error_code:" << error_code << " error_info:" << error_info << endl;
			//XELOGWRITE("XETCPSocket::connect error error_code:%d, error_info:%d", error_code, error_info);
			return socket_operate_result::operate_failed;
		}
	}
	*/
	XELOGWRITE("XETCPSocket::connect done success");
	return socket_operate_result::operate_done_;
}
	
// 等待链接上
wait_result XETCPSocket::wait_connect(int ms_wait)
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
	
// 发送消息
bool XETCPSocket::send_message(XEMessage* pMessage, uint32 timeout_ms/* = 5 * 1000*/)
{
	if (NULL == pMessage)
		return false;
	
	if (get_status() != XE_SOCKET_STATUS_CONNECTED)
	{
		_push_except_msg(XENET_MESSAGE_NOT_CONNECT, 
			pMessage->getProtocolID(), pMessage->getSequenceID());
		return false;
	}
	
	XEMessage* p_message_copy = pMessage->clone();
	if (NULL == p_message_copy)
	{
		return false;
	}
	
	m_mtx_send.lock();
	bool is_first = m_queue_send.empty();
	m_queue_send.push(p_message_copy);
	m_mtx_send.unlock();
	
	if (0 != timeout_ms)
	{
		uint32 ts_deadline = iclock() + timeout_ms;
		
		m_mtx_timeout.lock();
		m_timeout_check.emplace(p_message_copy->getSequenceID(),
			message_timer(p_message_copy->getProtocolID(), ts_deadline));
		m_mtx_timeout.unlock();
	}
	
	if (is_first)
		m_pSelector->async_send(shared_from_this(), OPERATE_RUN_ONE_TIME);
	
	return true;
}
	
void XETCPSocket::sendMessage(XEMessage* pMessage, uint32 timeout_ms /*= 5 * 1000*/)
{
	if (NULL == pMessage)
		return ;
	
	if (get_status() != XE_SOCKET_STATUS_CONNECTED)
	{
		_push_except_msg(XENET_MESSAGE_NOT_CONNECT,
			pMessage->getProtocolID(), pMessage->getSequenceID());
		return ;
	}
	
	XEMessage* p_message_copy = pMessage->clone();
	if (NULL == p_message_copy)
	{
		return ;
	}
	
	m_mtx_send.lock();
	bool is_first = m_queue_send.empty();
	m_queue_send.push(p_message_copy);
	m_mtx_send.unlock();
	
	if (0 != timeout_ms)
	{
		uint32 ts_deadline = iclock() + timeout_ms;
		
		m_mtx_timeout.lock();
		m_timeout_check.emplace(p_message_copy->getSequenceID(),
			message_timer(p_message_copy->getProtocolID(), ts_deadline));
		m_mtx_timeout.unlock();
	}
	
	if (is_first)
		m_pSelector->async_send(shared_from_this(), OPERATE_RUN_ONE_TIME);
}

// 获取消息
bool XETCPSocket::recv_message(XEMessage*& pMessage)
{
	pMessage = receiveMessage();
	
	return true;
}

XEMessage* XETCPSocket::receiveMessage()
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
	
int XETCPSocket::getSocketState()
{
	int state = 0;
	if (get_status() == XE_SOCKET_STATUS_CONNECTED)
	{
		state = 1;
	}
	else if (get_status() <= XE_SOCKET_STATUS_CONNECTING)
	{
		state = 0;
	}
	else
	{
		state = 2;
	}
	
	return get_status();
}
	
// 是否链接还在
bool XETCPSocket::is_alive()
{
	// 
	lock_guard<std::mutex> lock_(m_mtx_recv);
	return XE_SOCKET_STATUS_CONNECTED == m_status_.get_value();
}

// 关闭连接
bool XETCPSocket::close()
{
	if (XE_SOCKET_STATUS_CONNECTING == m_status_.compare_and_set(XE_SOCKET_STATUS_CONNECTING, XE_SOCKET_STATUS_CLOSING)
		|| XE_SOCKET_STATUS_CONNECTED == m_status_.compare_and_set(XE_SOCKET_STATUS_CONNECTED, XE_SOCKET_STATUS_CLOSING))
	{
		m_pSelector->async_close(shared_from_this());
	}

	if (wait_result::wait_failed == wait_close(-1))
	{
		uint32 error_code = 0, error_info = 0;
		get_error_info(error_code, error_info);
		cout << "error_code:" << error_code << " error_info:" << error_info << endl;
		XELOGWRITE("XETCPSocket::connect error error_code:%d, error_info:%d", error_code, error_info);
		return false;
	}
	return true;
}
	
long XETCPSocket::get_status()
{
	// 
	return m_status_.get_value();
}
	
// 等待关闭
wait_result XETCPSocket::wait_close(int ms_wait)
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
	
// 获取句柄
SOCKET XETCPSocket::get_handle()
{
	return m_sk_client.get_handle();
}
	
// 错误信息
void XETCPSocket::get_error_info(uint32& error_code, uint32& error_info)
{
	error_code = m_error_code;
	error_info = m_error_info;
}
	
// 回调 有数据可以接收
void XETCPSocket::on_can_recv()
{
	int nRemain = m_recvBuffer.remaining();
	if (nRemain == 0) return ;

	if (get_status() != XE_SOCKET_STATUS_CONNECTED) return;

	int ret_ = m_sk_client.recv((char*)m_recvBuffer.getBuffer() + m_recvBuffer.getPosition(), nRemain, 0);
	if (-1 == ret_)
	{
		XELOGWRITE("XETCPSocket::on_can_recv ret_ == -1 error_code:%d", m_error_code);
		_set_error_info(net_error_type::XENET_CLOSED_ABNORMAL, ERRORNO);
		std::cout << "XETCPSocket::on_can_recv: "<< ERRORNO << endl;
		_active_close_socket();
	}
	else if (0 == ret_)
	{
		XELOGWRITE("XETCPSocket::on_can_recv ret_ == 0 error_code:%d", m_error_code);
		// 关闭
		_set_error_info(net_error_type::XENET_CLOSED_NORMAL, ERRORNO);
		_active_close_socket();
	}
	else
	{
		// 解密
		m_rc4_recv.update((const xe::byte*)m_recvBuffer.getBuffer() + m_recvBuffer.getPosition(),
							(xe::byte*)m_recvBuffer.getBuffer() + m_recvBuffer.getPosition(),
							ret_);
		m_recvBuffer.setPosition(m_recvBuffer.getPosition() + ret_);

		for (;;)
		{
			uint32 position_ = m_recvBuffer.getPosition();
			if (position_ < sizeof(uint32) + sizeof(Aone2CmdHeader_))
				break;
			
			const char* buffer_ = m_recvBuffer.getBuffer();
			
			// 尝试读取消息
			uint32 msg_len = *(uint32*)buffer_;
			Aone2CmdHeader_* header_ = (Aone2CmdHeader_*)(buffer_ + 4);
			
			if (msg_len > position_)
				break;
			
			XEMessage* pMessage = new XEMessage(m_recvBuffer.getBuffer(), sizeof(msg_len), msg_len - sizeof(msg_len));
			
			m_mtx_recv.lock();
			m_queue_recv.push(pMessage);
			m_mtx_recv.unlock();
			

			m_mtx_timeout.lock();
			m_timeout_check.erase(header_->m_cmdSeq);
			m_mtx_timeout.unlock();
			
			// relocate 
			m_recvBuffer.flip();
			m_recvBuffer.setPosition(msg_len);
			m_recvBuffer.compact();
		}
	}
}
	
// 回调 有数据可以发送
void XETCPSocket::on_can_send()
{
	if (get_status() == XE_SOCKET_STATUS_CONNECTING)
	{
		uint32 error_code = m_sk_client.get_sock_error();
		if (0 != error_code)
		{
			XELOGWRITE("XETCPSocket::on_can_send m_sk_client error_code:%d", error_code);
			// 关闭
			_set_error_info(net_error_type::XENET_XTCP_CONNECT_FAILED, error_code);
			_active_close_socket();
			
			return ;
		}
		else
		{
			// 开始读取
			m_pSelector->async_recv(shared_from_this(), OPERATE_RUN_ENDLESS);
			m_status_.set_value(XE_SOCKET_STATUS_CONNECTED);
			int connectValue = m_event_connect.set_event();
			
			m_pSelector->add_timer(shared_from_this(), OPERATE_RUN_ENDLESS,
					XXP_CHECK_TIMEOUT_INTERVAL, TCP_TIMER_MSG_TIMEOUT_CHECK);
			
			XELOGWRITE("XETCPSocket::on_can_send m_event_connect set_event:%d", connectValue);
		}
	}
	
	if (get_status() != XE_SOCKET_STATUS_CONNECTED) return;

	bool bDataLeft = false;
	m_mtx_send.lock();
	for (;;)
	{
		if (m_queue_send.empty()) break;

		XEMessage* pMessage = m_queue_send.front();
		XEByteBuffer* pMsgBuff = pMessage->getByteBuffer();

		uint32 msg_len = pMsgBuff->getLimit();
		uint32 snd_buf_remain = m_sendBuffer.remaining(),
			snd_buf_pos = m_sendBuffer.getPosition();

		if (msg_len > snd_buf_remain)
		{
			bDataLeft = true;
			break;
		}
		m_sendBuffer.setBuffer(pMsgBuff->getBuffer(), 0, msg_len);
		const char* sz_send_buffer = m_sendBuffer.getBuffer() + snd_buf_pos;
		m_rc4_send.update((const xe::byte*)sz_send_buffer, (xe::byte*)sz_send_buffer, msg_len);
		
		m_queue_send.pop();
		
		delete pMessage;
	}
	m_mtx_send.unlock();

	int dataLen = m_sendBuffer.getPosition();
	if (0 == dataLen) return;  // 无数据可发送
	
	int ret_ = m_sk_client.send(m_sendBuffer.getBuffer(), m_sendBuffer.getPosition(), 0);
	if (-1 == ret_)
	{
		_set_error_info(net_error_type::XENET_CLOSED_ABNORMAL, ERRORNO);
		XELOGWRITE("XETCPSocket::on_can_send m_sk_client.send -1 error_code:%d", m_error_code);
		_active_close_socket();
	}
	else if (0 == ret_)
	{
		_set_error_info(net_error_type::XENET_CLOSED_NORMAL, ERRORNO);
		XELOGWRITE("XETCPSocket::on_can_send m_sk_client.send 0 error_code:%d", m_error_code);
		_active_close_socket();
	}
	else
	{
		m_sendBuffer.flip();
		m_sendBuffer.setPosition(ret_);
		m_sendBuffer.compact();
		
		if (bDataLeft || (dataLen != ret_)) // 有数据未发送完
		{
			// 
			m_pSelector->async_send(shared_from_this(), OPERATE_RUN_ONE_TIME);
		}
	}
}
	
// 回掉 关闭处理
void XETCPSocket::on_close()
{
	if (get_status() != XE_SOCKET_STATUS_CLOSING) return;
	XELOGWRITE("XETCPSocket::on_close begin");
	_close_socket();
}
	
// 定时器回调
void XETCPSocket::on_timer(int id_)
{
	switch (id_)
	{
	case TCP_TIMER_CONNECT_TIMEOUT:
		{
			if (get_status() == XE_SOCKET_STATUS_CONNECTING)
			{
				_set_error_info(net_error_type::XENET_XTCP_CONNECT_TIMEOUT, ERRORNO);
				_active_close_socket();
			}
		}
		break;
	case TCP_TIMER_MSG_TIMEOUT_CHECK:
		_check_msg_timeout();
		break;
	default:
		break;
	}
}
	
// 回调 有些数据需要处理
void XETCPSocket::on_can_handle(int event_)
{
	XELOGWRITE("XETCPSocket::on_can_handle begin name:%s", m_name.c_str());
	if (TCP_SOCKET_EVENT_CONNECT == event_)
	{
		m_rc4_send.init(m_rc4_key);
		m_rc4_recv.init(m_rc4_key);
		
		if (!m_sk_client.create(m_rmt_addr.family(), SOCK_STREAM, IPPROTO_TCP))
		{
			// 
			_set_error_info(net_error_type::XENET_CREATE_SOCKET_FAILED, ERRORNO);
			_active_close_socket();
			return ;
		}
			
		if (!m_sk_client.set_nonblock(true))
		{
			_set_error_info(net_error_type::XENET_XTCP_SET_NONBLOCK_FAILED, ERRORNO);
			_active_close_socket();
			return;
		}
		
		m_pSelector->add_timer(shared_from_this(), OPERATE_RUN_ONE_TIME, 2000, TCP_TIMER_CONNECT_TIMEOUT);
		
		XELOGWRITE("XETCPSocket::on_can_handle connect");
		if (!m_sk_client.connect(m_rmt_addr))
		{
			int error_code = ERRORNO;
			XELOGWRITE("XETCPSocket::on_can_handle error_code1:%d", error_code);
			if (ERROR_WOULD_BLOCK != error_code && ERROR_INPROGRESS != error_code)
			{
				_set_error_info(net_error_type::XENET_XTCP_CONNECT_FAILED, error_code);
				_active_close_socket();
				XELOGWRITE("XETCPSocket::on_can_handle error_code2:%d", error_code);
				return;
			}
		}
		m_pSelector->async_send(shared_from_this(), OPERATE_RUN_ONE_TIME);
	}
	
}
	
// 主动关闭socket
void XETCPSocket::_active_close_socket()
{
	XELOGWRITE("XETCPSocket::_active_close_socket begin");
	
	_close_socket();
	
	_push_except_msg(m_error_code);
	_move_timeout_to_except();
}
	
// 增加错误信息
void XETCPSocket::_push_except_msg(uint32 error_code, uint32 protocol_id/* = 0*/, uint32 msg_seq/* = 0*/)
{
	// 
	m_mtx_except.lock();
	m_queue_except.push(XEMessage::errorMessage(error_code, protocol_id + 1, msg_seq));
	m_mtx_except.unlock();
}
	
// 增加错误信息
void XETCPSocket::_push_except_msg(uint32 error_code, const vector<uint32>& protocol_ids)
{
	m_mtx_except.lock();
	auto it_proto = protocol_ids.begin();
	for (; it_proto != protocol_ids.end(); ++it_proto)
	{
		m_queue_except.push(XEMessage::errorMessage(error_code, *it_proto + 1));
	}
	m_mtx_except.unlock();
}
	
// 错误信息
void XETCPSocket::_set_error_info(uint32 error_code, uint32 error_info)
{
	m_error_code = error_code;
	m_error_info = error_info;
}
	
// 关闭socket
void XETCPSocket::_close_socket()
{
	XELOGWRITE("XETCPSocket::_close_socket begin");
	m_pSelector->remove_socket(shared_from_this());
	m_sk_client.close();
	
	m_mtx_send.lock();
	while (!m_queue_send.empty())
	{
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
	int connectValue = m_event_connect.set_event();
	XELOGWRITE("XETCPSocket::_close_socket m_event_connect set_event:%d, error_code :%d, error_info:%d", connectValue, m_error_code, m_error_info);

	m_status_.set_value(XE_SOCKET_STATUS_NOT_CONNECT);
}
	
// 检测消息超时
void XETCPSocket::_check_msg_timeout()
{
	uint32 ts_now = iclock();
	vector<uint32> timeout_proto;
	
	m_mtx_timeout.lock();
	std::unordered_map<uint32, message_timer>::iterator it_timeout = m_timeout_check.begin();
	for (; it_timeout != m_timeout_check.end(); )
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
	
// 移动超时消息到异常消息
void XETCPSocket::_move_timeout_to_except()
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
void XETCPSocket::_clear_except_queue_msg()
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
	
XETCPSocket* XETCPSocket::createSocket(XESelector* pSelector, const std::string &rc4_key)
{
	shared_ptr<XETCPSocket> ptr_socket(new XETCPSocket());
	ptr_socket.get()->init(pSelector, rc4_key);
	if (ptr_socket)
	{
		m_map_sockets.insert(make_pair(ptr_socket.get(), ptr_socket));
	}
	return ptr_socket.get();
}

void XETCPSocket::releaseSocket(XETCPSocket* p_socket)
{
	auto it = m_map_sockets.find(p_socket);
	if (it != m_map_sockets.end())
	{
		m_map_sockets.erase(it);
	}
}
	
void XETCPSocket::disconnect()
{
	XELOGWRITE("XETCPSocket::disconnect");
	close();
}

string XETCPSocket::getName()
{
	return m_name;
}
void XETCPSocket::setName(string _name)
{
	m_name = _name;
}

NS_XE_END
	
