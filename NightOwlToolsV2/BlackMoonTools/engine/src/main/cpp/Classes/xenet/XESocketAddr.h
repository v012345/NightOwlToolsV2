
// Sock地址类


#ifndef _XE_SOCK_ADDR_H_
#define _XE_SOCK_ADDR_H_

#include "XEInclude.h"
#include "XEPlatformDefine.h"


NS_XE_BEGIN

class XESocketAddr
{
public:
	XESocketAddr(uint16 family = AF_INET/* or AF_INET6*/);
	XESocketAddr(const XESocketAddr& addr);
	XESocketAddr(const sockaddr_in& addr);
	XESocketAddr(const sockaddr_in6& addr);
	XESocketAddr(const string& host, uint16 port); // host: ip或域名
	~XESocketAddr(void);

	XESocketAddr& operator = (const XESocketAddr& addr);
	XESocketAddr& operator = (const sockaddr_in& addr);
	XESocketAddr& operator = (const sockaddr_in6& addr);
	operator const sockaddr* () const;
	int size() const;
	uint16 family() const;

	void   set_port(uint16 port);
	// host such as "192.168.4.12" or "www.sina.com.cn"
	// 任意的ipv4地址为"0.0.0.0"
	// 任意的ipv6地址为"::"或"0:0:0:0:0:0:0:0"
	// 不允许为""，因为""同时指代任意的ipv4和ipv6地址
	bool   set_host(const string& host);
	void   reset();

private:
	union Data
	{
		sockaddr_in  m_inaddr;
		sockaddr_in6  m_inaddr6;
	} m_data;
};
NS_XE_END

#endif//_XE_SOCK_ADDR_H_
