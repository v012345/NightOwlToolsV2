// 2008-12-03
// xcore_socket.cpp
// guosh
// socket简单封装



#include "xnet_socket.h"
#include <errno.h>
#ifdef __WINDOWS__
#include "mstcpip.h"
#else
#include <sys/uio.h>
#endif//__WINDOWS__

namespace aonesdk_xnet
{

class IdleSock
{
public:
	IdleSock()
	{
		m_sock.open();
	}

	~IdleSock()
	{
		m_sock.close();
	}

	void attach(SOCKET sock)
	{
		m_sock.attach(sock);
	}

private:
	XSocket m_sock;
};
static IdleSock  g_idleSock;

///////////////////////////////////////////////////////////////////////
// class XSocket
///////////////////////////////////////////////////////////////////////
XSocket::~XSocket()
{
	ASSERT((m_sock == INVALID_SOCKET) && "Not call close() or detach() befor call destructor of XSocket");
	this->close();
}

void XSocket::attach(SOCKET sock)
{
	if (m_sock != sock)
	{
		this->close();
		m_sock = sock;
	}
}

SOCKET XSocket::detach()
{
	SOCKET sock = m_sock;
	m_sock = INVALID_SOCKET;
	return sock;
}

bool XSocket::is_open() const
{
	return (m_sock != INVALID_SOCKET);
}


///////////////////////////////////////////////////////////////////////
// base interfaces
///////////////////////////////////////////////////////////////////////
bool XSocket::open(int type, uint16 family)
{
	ASSERT(family == AF_INET || family == AF_INET6);
	this->close();
	m_sock = ::socket(family, type, 0);
	return (INVALID_SOCKET != m_sock);
}

bool XSocket::bind(const XSockAddr& addr)
{
	if (m_sock == INVALID_SOCKET) return false;

	ASSERT((local_addr().get_port() == 0) && "Socket cann't bind two address.");
	return (0 == ::bind(m_sock, addr, addr.size()));
}

bool XSocket::connect(const XSockAddr& addr)
{
	if (m_sock == INVALID_SOCKET) return false;

	while (true)
	{
		int ret = ::connect(m_sock, addr, addr.size());
		if (ret < 0)
		{
			if (_is_can_restore())
			{
				continue;
			}
			else if (_is_already() || _is_would_block())
			{
				return true;
			}
			return false;
		}
		return true;
	}

	return false;
}

bool XSocket::listen(const XSockAddr& addr, int backlog)
{
	if (m_sock == INVALID_SOCKET) return false;
	ASSERT((local_addr().get_port() == 0) && "Socket already bind a address before call listen.");

	if (backlog < 0) backlog = 0X7FFFFFFF;
	if (!set_reuse_addr(true)) return false;
	if (!this->bind(addr)) return false;
	if (0 != ::listen(m_sock, backlog)) return false;
	return true;
}

bool XSocket::accept(XSocket& sock, XSockAddr* remote_addr)
{
	ASSERT(this != &sock);
	if (m_sock == INVALID_SOCKET) return false;

	SOCKET s = INVALID_SOCKET;
	sock.attach(INVALID_SOCKET);

	while (true)
	{
		struct sockaddr_storage addr;
		socklen_t addrlen = sizeof(sockaddr_storage);
		s = ::accept(m_sock, (sockaddr*)&addr, &addrlen);
		if (remote_addr)
		{
			if (addr.ss_family == AF_INET)
				*remote_addr = XSockAddr(*(sockaddr_in*)&addr);
			else if (addr.ss_family == AF_INET6)
				*remote_addr = XSockAddr(*(sockaddr_in6*)&addr);
			else
				*remote_addr = XSockAddr();
		}
		if (s == INVALID_SOCKET)
		{
			if (_is_can_restore())
			{
				continue;
			}
			else if (_is_would_block())
			{
				break;
			}
			else if (_is_emfile())
			{
				g_idleSock.attach(INVALID_SOCKET);
				g_idleSock.attach(::accept(m_sock, NULL, NULL));
				g_idleSock.attach(INVALID_SOCKET);
				g_idleSock.attach(::socket(AF_INET, SOCK_STREAM, 0));
				break;
			}
			return false;
		}
		break;
	}

	sock.attach(s);
	return true;
}

bool XSocket::abort()
{
	return close(0);
}

XSockAddr XSocket::local_addr() const
{
	if (m_sock == INVALID_SOCKET) return XSockAddr();

	struct sockaddr_storage addr;
	socklen_t addrlen = sizeof(sockaddr_storage);
	if (0 != getsockname(m_sock, (sockaddr*)&addr, &addrlen)) return XSockAddr();
	if (addr.ss_family == AF_INET)
		return XSockAddr(*(sockaddr_in*)&addr);
	else if (addr.ss_family == AF_INET6)
		return XSockAddr(*(sockaddr_in6*)&addr);
	else
		return XSockAddr();
}

XSockAddr XSocket::remote_addr() const
{
	if (m_sock == INVALID_SOCKET) return XSockAddr();

	struct sockaddr_storage addr;
	socklen_t addrlen = sizeof(sockaddr_storage);
	if (0 != getpeername(m_sock, (sockaddr*)&addr, &addrlen)) return XSockAddr();
	if (addr.ss_family == AF_INET)
		return XSockAddr(*(sockaddr_in*)&addr);
	else if (addr.ss_family == AF_INET6)
		return XSockAddr(*(sockaddr_in6*)&addr);
	else
		return XSockAddr();
}

///////////////////////////////////////////////////////////////////////
// option interfaces
///////////////////////////////////////////////////////////////////////
bool XSocket::set_reuse_addr(bool bl)
{
	if (m_sock == INVALID_SOCKET) return false;

	int nVal = (int)bl;
	int ret = setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&nVal, sizeof(int));
	return (0 == ret);
}

bool XSocket::set_keep_alive(bool bl)
{
	if (m_sock == INVALID_SOCKET) return false;

	int nVal = (int)bl;
	int ret = setsockopt(m_sock, SOL_SOCKET, SO_KEEPALIVE, (const char*)&nVal, sizeof(int));
	if (0 != ret) return false;
	
	#ifdef __WINDOWS__
	tcp_keepalive inKeepAlive = {0};
	unsigned long ulInLen = sizeof(tcp_keepalive);
	tcp_keepalive outKeepAlive = {0};
	unsigned long ulOutLen = sizeof(tcp_keepalive);
	unsigned long ulBytesReturn = 0;
	inKeepAlive.onoff = bl ? 1 : 0;
	inKeepAlive.keepalivetime = 60000; // 如该连接在60秒内没有任何数据往来,则进行探测
	inKeepAlive.keepaliveinterval = 5000; // 探测时发包的时间间隔为5秒(默认探测10次)
	if (SOCKET_ERROR == WSAIoctl(m_sock, SIO_KEEPALIVE_VALS, (LPVOID)&inKeepAlive, ulInLen, (LPVOID)&outKeepAlive, ulOutLen, &ulBytesReturn, NULL, NULL)) return false;
	#endif//__WINDOWS__

    #ifdef CC_TARGET_OS_IPHONE
    //Idle time used when SO_KEEPALIVE is enabled. Sets how long connection must be idle before keepalive is sent
    int keepaliveIdle = 60;
    //Interval between keepalives when there is no reply. Not same as idle time
    int keepaliveIntvl = 5;
    //Number of keepalives before close (including first keepalive packet)
    int keepaliveCount = 3;
    
    if (setsockopt(m_sock, IPPROTO_TCP, TCP_KEEPCNT, &keepaliveCount, sizeof(int)) == -1)
    {
        return false;
    }
    
    if (setsockopt(m_sock, IPPROTO_TCP, TCP_KEEPALIVE, &keepaliveIdle, sizeof(int)) == -1)
    {
        return false;
    }
    
    if (setsockopt(m_sock, IPPROTO_TCP, TCP_KEEPINTVL, &keepaliveIntvl, sizeof(int)) == -1)
    {
        return false;
    }

    #endif
	return true;
}

bool XSocket::set_tcp_nodelay(bool bl)
{
	if (m_sock == INVALID_SOCKET) return false;

	int nVal = (int)bl;
	int ret = setsockopt(m_sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&nVal, sizeof(int));
	return (0 == ret);
}

bool XSocket::set_linger(uint16 delay_sec, bool bl)
{
	struct linger lingerStruct = {(uint16)bl, delay_sec};
	int ret = setsockopt(m_sock, SOL_SOCKET, SO_LINGER, (char*)&lingerStruct, sizeof(lingerStruct));
	return (0 == ret);
}

bool XSocket::set_nonblock(bool bl)
{
	if (m_sock == INVALID_SOCKET) return false;

	#ifdef __WINDOWS__
	unsigned long ulOption = (unsigned long)bl;
	int ret = ioctlsocket(m_sock, FIONBIO, (unsigned long*)&ulOption);
	#endif//__WINDOWS__

	#ifdef __POSIX__
	int ret = fcntl(m_sock, F_GETFL, 0);
	if (ret == -1) return false;
	if (bl)
		ret = fcntl(m_sock, F_SETFL, ret | O_NONBLOCK);
	else
		ret = fcntl(m_sock, F_SETFL, ret & (~O_NONBLOCK));
	#endif//__POSIX__

	return (0 == ret);
}

bool XSocket::set_send_bufsize(uint32 nSize)
{
	if (m_sock == INVALID_SOCKET) return false;

	int ret = setsockopt(m_sock, SOL_SOCKET, SO_SNDBUF, (const char*)&nSize, sizeof(int));
	return (0 == ret);
}

bool XSocket::get_send_bufsize(uint32& nSize) const
{
	nSize = 0;
	if (m_sock == INVALID_SOCKET) return false;

	socklen_t optlen = sizeof(int);
	int ret = getsockopt(m_sock, SOL_SOCKET, SO_SNDBUF, (char*)&nSize, &optlen);
	return (0 == ret);
}

bool XSocket::set_recv_bufsize(uint32 nSize)
{
	if (m_sock == INVALID_SOCKET) return false;

	int ret = setsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, (const char*)&nSize, sizeof(int));
	return (0 == ret);
}

bool XSocket::get_recv_bufsize(uint32& nSize) const
{
	nSize = 0;
	if (m_sock == INVALID_SOCKET) return false;

	socklen_t optlen = sizeof(int);
	int ret = getsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, (char*)&nSize, &optlen);
	return (0 == ret);
}

///////////////////////////////////////////////////////////////////////
// date interfaces
///////////////////////////////////////////////////////////////////////
int XSocket::send(const void* buf, int len)
{
	if ((m_sock == INVALID_SOCKET)) return -1;
	if ((buf == NULL) || (len <= 0)) return 0;

	while (true)
	{
		int ret = ::send(m_sock, (const char*)buf, len, 0);
		if (ret < 0)
		{
			if (_is_can_restore())
			{
				continue;
			}
			if (_is_would_block())
			{
				return 0;
			}
			return -1;
		}
		return ret;
	}

	return 0;
}

int XSocket::recv(void* buf, int len)
{
	if (m_sock == INVALID_SOCKET) return -1;
	if ((buf == NULL) || (len <= 0)) return 0;

	while (true)
	{
		int ret = ::recv(m_sock, (char*)buf, len, 0);
		if (ret == 0) return -2; // closed by remote host
		if (ret < 0)
		{
			if (_is_can_restore())
			{
				continue;
			}
			if (_is_would_block())
			{
				return 0;
			}
			return -1;
		}
		return ret;
	}

	return 0;
}

int XSocket::sendto(const void* buf, int len, const XSockAddr& addr)
{
	if (m_sock == INVALID_SOCKET) return -1;
	if ((buf == NULL) || (len <= 0)) return 0;

	while (true)
	{
		int ret = ::sendto(m_sock, (const char*)buf, len, 0, addr, addr.size());
		if (ret < 0)
		{
			if (_is_can_restore())
			{
				continue;
			}
			if (_is_would_block())
			{
				return 0;
			}
			return -1;
		}
		return ret;
	}

	return 0;
}

int XSocket::recvfrom(void* buf, int len, XSockAddr& addr)
{
	if (m_sock == INVALID_SOCKET) return -1;
	if ((buf == NULL) || (len <= 0)) return 0;
	addr.reset();

	while (true)
	{
		struct sockaddr_storage addr_;
		socklen_t addrlen = sizeof(sockaddr_storage);
		int ret = ::recvfrom(m_sock, (char*)buf, len, 0, (sockaddr*)&addr_, &addrlen);
		if (ret == 0) return -2; // shutdown by remote host
		if (ret < 0)
		{
			if (_is_can_restore())
			{
				continue;
			}
			if (_is_would_block())
			{
				return 0;
			}
			return -1;
		}

		if (addr_.ss_family == AF_INET)
			addr = XSockAddr(*(sockaddr_in*)&addr);
		else if (addr_.ss_family == AF_INET6)
			addr = XSockAddr(*(sockaddr_in6*)&addr);
		else
			addr = XSockAddr();
		return ret;
	}

	return 0;

}

int XSocket::send_n(const void* buf, int len, int timeout_ms)
{
	if (m_sock == INVALID_SOCKET) return -1;
	if ((buf == NULL) || len <= 0) return 0;

	int sendsize = 0;
	do
	{
		if (!can_send(timeout_ms)) return sendsize;
		int ret = this->send((const char*)buf + sendsize, len - sendsize);
		if (ret < 0)
		{
			if (sendsize > 0)
				return sendsize;
			else
				return ret;
		}
		sendsize += ret;
	}
	while (sendsize < len);

	ASSERT(sendsize == len);
	return sendsize;
}

int XSocket::recv_n(void* buf, int len, int timeout_ms)
{
	if (m_sock == INVALID_SOCKET) return -1;
	if ((buf == NULL) || len <= 0) return 0;

	int recvsize = 0;
	do
	{
		if (!can_recv(timeout_ms)) return recvsize;
		int ret = this->recv((char*)buf + recvsize, len - recvsize);
		if (ret < 0)
		{
			if (recvsize > 0)
				return recvsize;
			else
				return ret;
		}
		recvsize += ret;
	}
	while (recvsize < len);

	ASSERT(recvsize == len);
	return recvsize;
}

///////////////////////////////////////////////////////////////////////
// private interfaces
///////////////////////////////////////////////////////////////////////
#ifdef __WINDOWS__
bool XSocket::can_recv(int timeout_ms)
{
	if (m_sock == INVALID_SOCKET) return true;

	struct timeval tv = {};
	timeval* pcTimeout = NULL;
	if (timeout_ms >= 0)
	{
		tv.tv_sec = timeout_ms / 1000;
		tv.tv_usec = (timeout_ms % 1000) * 1000;
		pcTimeout = &tv;
	}

	while (true)
	{
		fd_set rdset, exceptset;
		FD_ZERO(&rdset);
		FD_ZERO(&exceptset);
		FD_SET(m_sock, &rdset);
		FD_SET(m_sock, &exceptset);

		// If timeout is NULL (no timeout), select can block indefinitely.
		// In windows, pcTimeout not altered; In linux, pcTimeout may update.
		int ret = select((int)m_sock + 1, &rdset, NULL, &exceptset, pcTimeout);
		if (ret == 0) return false;
		if (FD_ISSET(m_sock, &rdset) || FD_ISSET(m_sock, &exceptset)) return true;
		if (_is_can_restore()) continue;
		fprintf(stderr, "socket select return %d, errno:%d\n", ret, errno);
		ASSERT(!"socket select exception.");
		break;
	}

	return false;
}

bool XSocket::can_send(int timeout_ms)
{
	if (m_sock == INVALID_SOCKET) return false;

	struct timeval tv = {};
	timeval* pcTimeout = NULL;
	if (timeout_ms >= 0)
	{
		tv.tv_sec = timeout_ms / 1000;
		tv.tv_usec = (timeout_ms % 1000) * 1000;
		pcTimeout = &tv;
	}

	while (true)
	{
		fd_set wrset, exceptset;
		FD_ZERO(&wrset);
		FD_ZERO(&exceptset);
		FD_SET(m_sock, &wrset);
		FD_SET(m_sock, &exceptset);

		// If timeout is NULL (no timeout), select can block indefinitely.
		// In windows, pcTimeout not altered; In linux, pcTimeout may update.
		int ret = select((int)m_sock + 1, NULL, &wrset, &exceptset, pcTimeout);
		if (ret == 0) return false;
		if (FD_ISSET(m_sock, &wrset)) return true;
		if (FD_ISSET(m_sock, &exceptset)) return false;
		if (_is_can_restore()) continue;
		fprintf(stderr, "socket select return %d, errno:%d\n", ret, errno);
		ASSERT(!"socket select exception.");
		break;
	}

	return false;
}

bool XSocket::send_v(const iovec* iov, int cnt)
{
	if (m_sock == INVALID_SOCKET) return false;
	if (iov == NULL || cnt == 0) return true;
	ASSERT(cnt <= 20);

	WSABUF bufs[20];
	ULONG total = 0;
	for (int i = 0; i < cnt; i++)
	{
		ASSERT(iov->iov_base);
		bufs[i].buf = (CHAR*)iov->iov_base;
		bufs[i].len = (ULONG)iov->iov_len;
		total += bufs[i].len;
	}

	DWORD snds = 0;
	if (0 != WSASend(m_sock, bufs, cnt, &snds, 0, NULL, NULL)) return false;
	if (snds != total) return false;
	return true;
}

bool XSocket::shutdown()
{
	if (m_sock != INVALID_SOCKET)
	{
		return (-1 != ::shutdown(m_sock, SD_SEND));
	}
	return true;
}

bool XSocket::close(int delay)
{
	if (m_sock != INVALID_SOCKET)
	{
		SOCKET sock = m_sock;
		m_sock = INVALID_SOCKET;

		if (delay >= 0)
		{
			struct linger linger_ = { 1, delay };
			setsockopt(sock, SOL_SOCKET, SO_LINGER, (const char*)&linger_, sizeof(linger_));
		}

		return (-1 != ::closesocket(sock));
	}
	return true;
}

bool XSocket::_is_can_restore()
{
	return (WSAEINTR == WSAGetLastError());
}

bool XSocket::_is_already()
{
	return ((WSAEALREADY == WSAGetLastError()) ||
	        (WSAEINPROGRESS == WSAGetLastError()) ||
	        (WSAEISCONN == WSAGetLastError()));
}

bool XSocket::_is_would_block()
{
	return ((WSAEWOULDBLOCK == WSAGetLastError()) ||
	        (WSA_IO_PENDING == WSAGetLastError()));
}

bool XSocket::_is_emfile()
{
	return (WSAEMFILE == WSAGetLastError());
}
#endif//__WINDOWS__

#ifdef __POSIX__
bool XSocket::can_recv(int timeout_ms)
{
	if (timeout_ms < 0) timeout_ms = -1;
	if (m_sock == INVALID_SOCKET) return true;

	while (true)
	{
		struct pollfd  event;
		event.fd = m_sock;
		event.events = POLLIN;  // 不考虑带外数据
		int ret = poll(&event, 1, timeout_ms);
		if (ret > 0)
		{
			if (event.revents & POLLIN)
			{
				int err = 0;
				socklen_t len = (socklen_t)sizeof(err);
				if (getsockopt(m_sock, SOL_SOCKET, SO_ERROR, &err, &len) < 0) return false;
				return (err == 0);
			}
			else
			{
				ASSERT(false);
				return false;
			}
		}
		if (ret == 0) return false;
		if (_is_can_restore()) continue;
		fprintf(stderr, "socket poll return %d, errno:%d\n", ret, errno);
		ASSERT(!"socket poll exception.");
		break;
	}

	return false;
}

bool XSocket::can_send(int timeout_ms)
{
	if (timeout_ms < 0) timeout_ms = -1;
	if (m_sock == INVALID_SOCKET) return false;

	while (true)
	{
		struct pollfd  event;
		event.fd = m_sock;
		event.events = POLLOUT;
		int ret = poll(&event, 1, timeout_ms);
		if (ret > 0)
		{
			if (event.revents & POLLOUT)
			{
				int err = 0;
				socklen_t len = (socklen_t)sizeof(err);
				if (getsockopt(m_sock, SOL_SOCKET, SO_ERROR, &err, &len) < 0) return false;
				return (err == 0);
			}
			else
			{
				return false;
			}
		}
		if (ret == 0) return false;
		if (_is_can_restore()) continue;
		fprintf(stderr, "socket poll return %d, errno:%d\n", ret, errno);
		ASSERT(!"socket poll exception.");
		break;
	}

	return false;
}

bool XSocket::send_v(const iovec* iov, int cnt)
{
	if (m_sock == INVALID_SOCKET) return false;
	if (iov == NULL || cnt == 0) return true;

	size_t total = 0;
	for (int i = 0; i < cnt; i++)
	{
		ASSERT(iov->iov_base);
		total += iov->iov_len;
	}

	int ret = ::writev(m_sock, iov, cnt);
	if (ret < 0) return false;
	if (ret != (int)total) return false;
	return true;
}

bool XSocket::shutdown()
{
	if (m_sock != INVALID_SOCKET)
	{
		return (-1 != ::shutdown(m_sock, SHUT_WR));
	}
	return true;
}

bool XSocket::close(int delay)
{
	if (m_sock != INVALID_SOCKET)
	{
		SOCKET sock = m_sock;
		m_sock = INVALID_SOCKET;

		if (delay >= 0)
		{
			struct linger linger_ = { 1, delay };
			setsockopt(sock, SOL_SOCKET, SO_LINGER, (const char*)&linger_, sizeof(linger_));
		}

		return (-1 != ::close(sock));
	}
	return true;
}

bool XSocket::_is_can_restore()
{
	return (EINTR == errno);
}

bool XSocket::_is_already()
{
	return ((EALREADY == errno) ||
	        (EINPROGRESS == errno) ||
	        (EISCONN == errno));
}

bool XSocket::_is_would_block()
{
	return ((EAGAIN == errno) ||
	        (EWOULDBLOCK == errno));
}

bool XSocket::_is_emfile()
{
	return (EMFILE == errno);
}
#endif//__POSIX__

} // namespace xcore

