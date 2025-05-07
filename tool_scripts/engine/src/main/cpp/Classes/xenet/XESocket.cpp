#include "xenet/XEStdC.h"
#include "xenet/XESocket.h"
#include "XETime.h"
	
NS_XE_BEGIN
	
	
XESocket::XESocket() : m_sk(INVALID_SOCKET)
{
}
	
XESocket::~XESocket()
{
}
	
// 创建
bool XESocket::create(int domain_, int type_, int protocol_)
{
	XELOGWRITE("XESocket::create %d %d %d", domain_,type_,protocol_);

	m_sk = ::socket(domain_, type_, protocol_);
	XELOGWRITE("XESocket::create %d %d %d ret %u", domain_, type_, protocol_,m_sk);
	if (INVALID_SOCKET == m_sk)
	{
		return false;
	}
	return true;
}
	
// bind
bool XESocket::bind(const XESocketAddr& addr_)
{
	XELOGWRITE("XESocket::bind %u",m_sk);
	auto ret = ::bind(m_sk, (const ::sockaddr*)&addr_, addr_.size());
	XELOGWRITE("XESocket::bind %u ret %d", m_sk, ret);
	if (SOCKET_ERROR == ret)
	{
		// 
		return false;
	}
	return true;
}
	
// listen
bool XESocket::listen(int backlog)
{
	XELOGWRITE("XESocket::listen %u %d", m_sk,backlog);
	auto ret = ::listen(m_sk, backlog);
	XELOGWRITE("XESocket::listen %u %d ret %d", m_sk, backlog, ret);
	if (SOCKET_ERROR == ret)
	{
		return false;
	}
	return true;
}

// accept
bool XESocket::accept(XESocketAddr& addr_, XESocket& new_socket_)
{
	XELOGWRITE("XESocket::accept %u", m_sk);
	socklen_t len = addr_.size();
	auto sk_tmp = ::accept(m_sk, (::sockaddr*) &addr_, &len);
	XELOGWRITE("XESocket::accept %u ret %u", m_sk,sk_tmp);
	if (INVALID_SOCKET == sk_tmp)
		return false;

	new_socket_.m_sk = sk_tmp;
	return true;
}
	
// connect
bool XESocket::connect(const XESocketAddr& dest_addr_)
{
	XELOGWRITE("XESocket::connect %u", m_sk);
	//win32下bug 调用XELOGWRITE 会导致::connect返回-1 errorno == 0
	//排查后发现 如果删除掉 bool XELog::write(const std::string& log_) 中的 std::lock_guard<std::mutex> locker(m_mutex_); 可以规避这个问题
	//或者 让线程休眠一段时间也可以规避这个问题
	//原因不知道为什么
	xe::isleep(20);
	socklen_t len = dest_addr_.size();
	auto ret = ::connect(m_sk, (const sockaddr*)&dest_addr_, len);
	if (SOCKET_ERROR == ret)
	{
		return false;
	}
	return true;
}
	
// 发送 
ssize_t XESocket::send(const void* buf_, size_t len_, int flags_)
{
	return ::send(m_sk, (ae_socket_buf_type_const)buf_, len_, flags_);
}
	
ssize_t XESocket::send_to(const void* buf_, size_t len_, int flags_,
								const XESocketAddr& dest_addr_)
{
	socklen_t addr_len = dest_addr_.size();
	return ::sendto(m_sk, (ae_socket_buf_type_const)buf_, len_, flags_, (const sockaddr*)&dest_addr_, addr_len);
}
	
// 接收
ssize_t XESocket::recv(void* buf_, size_t len_, int flags_)
{ 
	return ::recv(m_sk, (ae_socket_buf_type)buf_, len_, flags_);
}
	
ssize_t XESocket::recv_from(void* buf_, size_t len_, int flags_, XESocketAddr& addr_)
{
	socklen_t addr_len = addr_.size();
	return ::recvfrom(m_sk, (ae_socket_buf_type)buf_, len_, flags_, (sockaddr*)&addr_, &addr_len);
}
	
// 设置阻塞与否
bool XESocket::set_nonblock(bool is_nonblock_)
{
	XELOGWRITE("XESocket::set_nonblock %u", m_sk);
#if XE_TARGET_PLATFORM == XE_PLATFORM_WIN32
	unsigned long lFlag = is_nonblock_ ? 1 : 0;
	auto ret = ::ioctlsocket(m_sk, FIONBIO, &lFlag);
	XELOGWRITE("XESocket::set_nonblock %u ret %d", m_sk,ret);
	return (SOCKET_ERROR != ret);
#else 
	int flags = fcntl(m_sk, F_GETFL, 0);
	auto ret = fcntl(m_sk, F_SETFL, flags | O_NONBLOCK);
	XELOGWRITE("XESocket::set_nonblock %u ret %d", m_sk, ret);
	return (-1 != ret);
#endif
}
	
// 设置重用地址与否
bool XESocket::set_reuseaddr(bool is_reuse)
{
	XELOGWRITE("XESocket::set_reuseaddr %u", m_sk);
	unsigned long lFlag = is_reuse ? 1 : 0;
	auto ret = ::setsockopt(m_sk, SOL_SOCKET, SO_REUSEADDR, (ae_socket_buf_type_const)&lFlag, sizeof lFlag);
	XELOGWRITE("XESocket::set_reuseaddr %u ret %d", m_sk, ret);
	if (SOCKET_ERROR == ret)
	{
		return false;
	}
	return true;
}
	
// 获取当前socket所在地址，需在bind或者connect之后调用
bool XESocket::get_sock_name(XESocketAddr& dest_addr_)
{
	XELOGWRITE("XESocket::get_sock_name %u", m_sk);
	socklen_t addr_len = dest_addr_.size();
	auto ret = ::getsockname(m_sk, (sockaddr*)&dest_addr_, &addr_len);
	XELOGWRITE("XESocket::get_sock_name %u ret %d", m_sk, ret);
	if (SOCKET_ERROR == ret)
	{
		// 
		return false;
	}
	return true;
}
	
// 获取socket内部错误码
uint32 XESocket::get_sock_error()
{
	XELOGWRITE("XESocket::get_sock_error %u", m_sk);
	uint32 error_code = 0;
	socklen_t error_len = sizeof(socklen_t);
	
	auto ret = getsockopt(m_sk, SOL_SOCKET, SO_ERROR, (char*)&error_code, &error_len);
	XELOGWRITE("XESocket::get_sock_error %u ret %d", m_sk, ret);

	if (SOCKET_ERROR == ret)
		return -1;
	
	return error_code;
}
	
// 获取 句柄 / 描述符
SOCKET XESocket::get_handle()
{
	return m_sk;
}
	
// 是否有效
bool XESocket::is_valid()
{
	// 
	return INVALID_SOCKET != m_sk;
}
	
// 设置发送超时时间
bool XESocket::set_send_timeo(uint32 timeo_ms)
{
	XELOGWRITE("XESocket::set_send_timeo %u", m_sk);

	auto ret = setsockopt(m_sk, SOL_SOCKET, SO_SNDTIMEO, (ae_socket_buf_type)&timeo_ms, sizeof(timeo_ms));
	XELOGWRITE("XESocket::set_send_timeo %u ret %d", m_sk, ret);
	
	if (SOCKET_ERROR == ret)
	{
		return false;
	}
	return true;
}
	
// 设置接收超时时间
bool XESocket::set_recv_timeo(uint32 timeo_ms)
{
	XELOGWRITE("XESocket::set_send_timeo %u", m_sk);
	auto ret = setsockopt(m_sk, SOL_SOCKET, SO_RCVTIMEO, (ae_socket_buf_type)&timeo_ms, sizeof(timeo_ms));
	XELOGWRITE("XESocket::set_send_timeo %u ret %d", m_sk, ret);
	if (SOCKET_ERROR == ret)
	{
		return false;
	}
	return true;
}
	
// 销毁
void XESocket::close()
{
	if (INVALID_SOCKET == m_sk) return;

	XELOGWRITE("XESocket::close %u",m_sk);

#if XE_TARGET_PLATFORM == XE_PLATFORM_WIN32
	closesocket(m_sk);
#else 
	auto ret = ::close(m_sk);
	XELOGWRITE("XESocket::close close ret %u ret %d", m_sk,ret);
	ret = ::shutdown(m_sk, SHUT_RDWR);
	XELOGWRITE("XESocket::close shutdown ret %u ret %d", m_sk, ret);
#endif
	m_sk = INVALID_SOCKET;
}
	
NS_XE_END
