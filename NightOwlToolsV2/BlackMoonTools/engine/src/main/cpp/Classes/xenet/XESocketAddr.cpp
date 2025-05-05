
// Sock地址类

#include "XESocketAddr.h"


NS_XE_BEGIN

XESocketAddr::XESocketAddr(uint16 family)
{
	assert(family == AF_INET || family == AF_INET6);
	reset();
	m_data.m_inaddr.sin_family = family;
}

XESocketAddr::XESocketAddr(const XESocketAddr& addr)
{
	memcpy(this, &addr, sizeof(addr));
}

XESocketAddr::XESocketAddr(const sockaddr_in& addr)
{
	reset();
	memcpy(&m_data.m_inaddr, &addr, sizeof(addr));
	m_data.m_inaddr.sin_family = AF_INET;
}

XESocketAddr::XESocketAddr(const sockaddr_in6& addr)
{
	reset();
	memcpy(&m_data.m_inaddr6, &addr, sizeof(addr));
	m_data.m_inaddr6.sin6_family = AF_INET6;
}

XESocketAddr::XESocketAddr(const string& host, uint16 port)
{
	reset();
	set_host(host);
	set_port(port);
}

XESocketAddr::~XESocketAddr(void)
{
	reset();
}

XESocketAddr& XESocketAddr::operator = (const XESocketAddr& addr)
{
	if (this == &addr) return *this;
	memcpy(&m_data, &addr.m_data, sizeof(m_data));
	return *this;
}

XESocketAddr& XESocketAddr::operator = (const sockaddr_in& addr)
{
	reset();
	memcpy(&m_data.m_inaddr, &addr, sizeof(addr));
	m_data.m_inaddr.sin_family = AF_INET;
	return *this;
}

XESocketAddr& XESocketAddr::operator = (const sockaddr_in6& addr)
{
	reset();
	memcpy(&m_data.m_inaddr6, &addr, sizeof(addr));
	m_data.m_inaddr6.sin6_family = AF_INET6;
	return *this;
}

XESocketAddr::operator const sockaddr* () const
{
	return (const sockaddr*)&m_data;
}

int XESocketAddr::size() const
{
	if (m_data.m_inaddr.sin_family == AF_INET)
		return sizeof(m_data.m_inaddr);
	else
		return sizeof(m_data.m_inaddr6);
}

uint16 XESocketAddr::family() const
{
	return m_data.m_inaddr.sin_family;
}

void XESocketAddr::set_port(uint16 port)
{
	m_data.m_inaddr.sin_port = htons(port);
}

bool XESocketAddr::set_host(const string& host)
{
	string host_ = host;

	// 从DNS服务查询
	addrinfo* servinfo = NULL;
	if (0 == getaddrinfo(host_.c_str(), NULL, NULL, &servinfo) && servinfo)
	{
		addrinfo* info = servinfo;
		while (info)
		{
			if (info->ai_family == AF_INET)
			{
				m_data.m_inaddr.sin_family = AF_INET;
				m_data.m_inaddr = *(struct sockaddr_in *)info->ai_addr;
				assert(info->ai_addrlen == sizeof(m_data.m_inaddr));
				freeaddrinfo(servinfo);
				return true;
			}
			else if (info->ai_family == AF_INET6)
			{
				m_data.m_inaddr.sin_family = AF_INET6;
				m_data.m_inaddr6 = *(struct sockaddr_in6 *)info->ai_addr;
				assert(info->ai_addrlen == sizeof(m_data.m_inaddr6));
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

void XESocketAddr::reset()
{
	memset(&m_data, 0, sizeof(m_data));
	m_data.m_inaddr.sin_family = AF_INET;
	return;
}

NS_XE_END