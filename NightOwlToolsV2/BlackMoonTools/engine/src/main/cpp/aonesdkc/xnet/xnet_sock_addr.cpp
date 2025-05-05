// 20010-09-29
// xcore_sock_addr.h
// guosh
// Sock地址类

//#pragma comment( lib, "ws2_32" )


#include "xnet_sock_addr.h"
#include "xcore_str_util.h"
#include "xcore_byte_order.h"
/*
#include "xcore_critical.h"
*/

#ifdef _MSC_VER
#pragma comment(lib, "ws2_32.lib")
#endif//_MSC_VER

namespace aonesdk_xnet
{

////////////////////////////////////////////////////////////////////////////////
// class __XSockGuarder
////////////////////////////////////////////////////////////////////////////////
class __XSockGuarder
{
public:
	__XSockGuarder()
	{
		#ifdef __WINDOWS__
		WSADATA wsaData;
		WORD wVersionRequested = MAKEWORD(2, 2);
		int nRetCode = WSAStartup(wVersionRequested, &wsaData);
		ASSERT((!nRetCode) && "WSAStartup failed!");
		#endif//__WINDOWS__

		#ifdef __POSIX__
		signal(SIGPIPE, SIG_IGN);
		#endif//__POSIX__
	}

	~__XSockGuarder()
	{
		#ifdef __WINDOWS__
		WSACleanup();
		#endif//__WINDOWS__
	}
};
static __XSockGuarder  __g_sock_guarder;

////////////////////////////////////////////////////////////////////////////////
// class XSockAddr
////////////////////////////////////////////////////////////////////////////////
XSockAddr::XSockAddr(uint16 family)
{
	ASSERT(family == AF_INET || family == AF_INET6);
	reset();
	m_data.m_inaddr.sin_family = family;
}

XSockAddr::XSockAddr(const XSockAddr& addr)
{
	memcpy(this, &addr, sizeof(addr));
}

XSockAddr::XSockAddr(const sockaddr_in& addr)
{
	reset();
	memcpy(&m_data.m_inaddr, &addr, sizeof(addr));
	m_data.m_inaddr.sin_family = AF_INET;
}

XSockAddr::XSockAddr(const sockaddr_in6& addr)
{
	reset();
	memcpy(&m_data.m_inaddr6, &addr, sizeof(addr));
	m_data.m_inaddr6.sin6_family = AF_INET6;
}

XSockAddr::XSockAddr(const string& host, uint16 port)
{
	reset();
	set_host(host);
	set_port(port);
}

XSockAddr::~XSockAddr(void)
{
	reset();
}

XSockAddr& XSockAddr::operator = (const XSockAddr& addr)
{
	if (this == &addr) return *this;
	memcpy(&m_data, &addr.m_data, sizeof(m_data));
	return *this;
}

XSockAddr& XSockAddr::operator = (const sockaddr_in& addr)
{
	reset();
	memcpy(&m_data.m_inaddr, &addr, sizeof(addr));
	m_data.m_inaddr.sin_family = AF_INET;
	return *this;
}

XSockAddr& XSockAddr::operator = (const sockaddr_in6& addr)
{
	reset();
	memcpy(&m_data.m_inaddr6, &addr, sizeof(addr));
	m_data.m_inaddr6.sin6_family = AF_INET6;
	return *this;
}

XSockAddr::operator const sockaddr* () const
{
	return (const sockaddr*)&m_data;
}

int XSockAddr::size() const
{
	if (m_data.m_inaddr.sin_family == AF_INET)
		return sizeof(m_data.m_inaddr);
	else
		return sizeof(m_data.m_inaddr6);
}

uint16 XSockAddr::family() const
{
	return m_data.m_inaddr.sin_family;
}

void XSockAddr::set_port(uint16 port)
{
	m_data.m_inaddr.sin_port = XByteOrder::local2net(port);
}

uint16 XSockAddr::get_port() const
{
	return XByteOrder::net2local((uint16)m_data.m_inaddr.sin_port);
}

bool XSockAddr::set_host(const string& host)
{
	string host_ = host;
	XStrUtil::chop(host_, "\r\n\t ");
	if (host_.empty()) host_ = "0.0.0.0"; // 空字符串暂时认为是ipv4的任意地址

	// 从DNS服务查询
	addrinfo* servinfo = NULL;
	if (0 == getaddrinfo(host_.c_str(), NULL, NULL, &servinfo) && servinfo)
	{
		addrinfo* info = servinfo;
		while (info)
		{
			if (info->ai_family == AF_INET)
			{
				struct sockaddr_in *addr = (struct sockaddr_in *)info->ai_addr;
				m_data.m_inaddr.sin_addr = addr->sin_addr;
				m_data.m_inaddr.sin_family = info->ai_family;
				ASSERT(info->ai_addrlen == sizeof(m_data.m_inaddr));
				freeaddrinfo(servinfo);
				return true;
			}
			else if (info->ai_family == AF_INET6)
			{
				struct sockaddr_in6 *addr = (struct sockaddr_in6 *)info->ai_addr;
				m_data.m_inaddr6.sin6_addr = addr->sin6_addr;
				m_data.m_inaddr6.sin6_family = addr->sin6_family;
				ASSERT(info->ai_addrlen == sizeof(m_data.m_inaddr6));
				freeaddrinfo(servinfo);
				return true;
			}
			else
			{
				info = info->ai_next;
			}
		}
		freeaddrinfo(servinfo);
	}

	// 查询失败或无效
	if (m_data.m_inaddr.sin_family == AF_INET)
	{
		m_data.m_inaddr.sin_addr.s_addr = INADDR_NONE;
	}
	else
	{
		memset(&m_data.m_inaddr6.sin6_addr, 0xff, sizeof(m_data.m_inaddr6.sin6_addr));
	}
	return false;
}

string XSockAddr::get_hostaddr() const
{
	char buf[128];
	if (m_data.m_inaddr.sin_family == AF_INET)
		inet_ntop(AF_INET, (void*)&m_data.m_inaddr.sin_addr, buf, 127);
	else
		inet_ntop(AF_INET6, (void*)&m_data.m_inaddr6.sin6_addr, buf, 127);
	return buf;
}

string XSockAddr::to_str() const
{
	if (this->family() == AF_INET)
		return get_hostaddr() + XStrUtil::to_str(get_port(), ":%u");
	else
		return "[" + get_hostaddr() + XStrUtil::to_str(get_port(), "]:%u");
}

void XSockAddr::reset()
{
	memset(&m_data, 0, sizeof(m_data));
	m_data.m_inaddr.sin_family = AF_INET;
	return;
}

bool XSockAddr::is_any() const
{
	if (m_data.m_inaddr.sin_family == AF_INET)
	{
		if (m_data.m_inaddr.sin_addr.s_addr == INADDR_ANY) return true;
	}
	else if (m_data.m_inaddr.sin_family == AF_INET6)
	{
		for (int i = 0; i < 16; i++)
		{
			if (m_data.m_inaddr6.sin6_addr.s6_addr[i] != 0) return false;
		}
		return true;
	}
	return false;
}

bool XSockAddr::is_none() const
{
	if (m_data.m_inaddr.sin_family == AF_INET)
	{
		if (m_data.m_inaddr.sin_addr.s_addr == INADDR_NONE) return true;
	}
	else if (m_data.m_inaddr.sin_family == AF_INET6)
	{
		for (int i = 0; i < 16; i++)
		{
			if (m_data.m_inaddr6.sin6_addr.s6_addr[i] != 0xff) return false;
		}
		return true;
	}
	return false;
}

bool XSockAddr::is_loop() const
{
	if (m_data.m_inaddr.sin_family == AF_INET)
	{
		// ipv4的环路 127.0.0.1
		if (m_data.m_inaddr.sin_addr.s_addr == 0X0100007F) return true;
	}
	else if (m_data.m_inaddr.sin_family == AF_INET6)
	{
		// ipv6的环路 ::1
		for (int i = 0; i < 15; i++)
		{
			if (m_data.m_inaddr6.sin6_addr.s6_addr[i] != 0) return false;
		}
		if (m_data.m_inaddr6.sin6_addr.s6_addr[15] != 1) return false;
		return true;
	}
	return false;
}

bool XSockAddr::local_addrs(vector<XSockAddr>& addrs)
{
	static vector<XSockAddr> addrs_;
	if (!addrs_.empty())
	{
		addrs = addrs_;
		return true;
	}

	addrinfo* servinfo = NULL;
	if (0 == getaddrinfo("", NULL, NULL, &servinfo) && servinfo)
	{
		addrinfo* info = servinfo;
		do
		{
			if (info->ai_family == AF_INET)
			{
				XSockAddr addr_;
				struct sockaddr_in *addr = (struct sockaddr_in *)info->ai_addr;
				addr_.m_data.m_inaddr.sin_addr = addr->sin_addr;
				addr_.m_data.m_inaddr.sin_family = info->ai_family;
				ASSERT(info->ai_addrlen == sizeof(addr_.m_data.m_inaddr));
				if (addr_.is_any() || addr_.is_none()) continue;
				addrs_.push_back(addr_);
			}
			else if (info->ai_family == AF_INET6)
			{
				XSockAddr addr_;
				struct sockaddr_in6 *addr = (struct sockaddr_in6 *)info->ai_addr;
				addr_.m_data.m_inaddr6.sin6_addr = addr->sin6_addr;
				addr_.m_data.m_inaddr6.sin6_family = addr->sin6_family;
				ASSERT(info->ai_addrlen == sizeof(addr_.m_data.m_inaddr6));
				if (addr_.is_any() || addr_.is_none()) continue;
				addrs_.push_back(addr_);
			}
		} while (info = info->ai_next);
		freeaddrinfo(servinfo);
		addrs = addrs_;
		return true;
	}

	addrs.clear();
	return false;
}

bool XSockAddr::resolve_dns(vector<XSockAddr>& addrs, const string& domain)
{
	string domain_ = domain;
	XStrUtil::chop(domain_, "\r\n\t ");
	addrs.clear();
	if (domain_.empty()) return false;

	// 从DNS服务查询
	addrinfo* servinfo = NULL;
	if (0 != getaddrinfo(domain_.c_str(), NULL, NULL, &servinfo) || servinfo == NULL)
	{
		return false;
	}

	addrinfo* info = servinfo;
	while (info)
	{
		XSockAddr addr_;
		if (info->ai_family == AF_INET)
		{
			struct sockaddr_in *addr = (struct sockaddr_in *)info->ai_addr;
			addr_.m_data.m_inaddr.sin_addr = addr->sin_addr;
			addr_.m_data.m_inaddr.sin_family = info->ai_family;
			ASSERT(info->ai_addrlen == sizeof(addr_.m_data.m_inaddr));
			addrs.push_back(addr_);
		}
		else if (info->ai_family == AF_INET6)
		{
			struct sockaddr_in6 *addr = (struct sockaddr_in6 *)info->ai_addr;
			addr_.m_data.m_inaddr6.sin6_addr = addr->sin6_addr;
			addr_.m_data.m_inaddr6.sin6_family = addr->sin6_family;
			ASSERT(info->ai_addrlen == sizeof(addr_.m_data.m_inaddr6));
			addrs.push_back(addr_);
		}
		info = info->ai_next;
	}
	freeaddrinfo(servinfo);
	return true;
}

bool operator == (const XSockAddr& addr1, const XSockAddr& addr2)
{
	if (addr1.family() != addr2.family()) return false;
	if (addr1.get_port() != addr2.get_port()) return false;
	if (addr1.get_hostaddr() != addr2.get_hostaddr()) return false;
	return true;
}

bool operator != (const XSockAddr& addr1, const XSockAddr& addr2)
{
	return !(addr1 == addr2);
}

}//namespace xcore

