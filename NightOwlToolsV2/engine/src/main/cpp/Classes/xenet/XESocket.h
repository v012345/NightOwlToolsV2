#ifndef _AESOCKET_H_
#define _AESOCKET_H_
	
#include "xenet/XEPlatformDefine.h"
#include "XEInclude.h"
#include "xenet/XESocketAddr.h"
	
NS_XE_BEGIN
	
class XESocket
{
public:
	XESocket();
	~XESocket();
	
	// 创建
	bool create(int domain_, int type_, int protocol_);
	
	// bind
	bool bind(const XESocketAddr& addr_);
	
	// listen
	bool listen(int backlog);

	// accept
	bool accept(XESocketAddr& addr_, XESocket& new_socket_);
	
	// connect
	bool connect(const XESocketAddr& dest_addr_);
	
	// 发送 
	ssize_t send(const void* buf_, size_t len_, int flags_);
	ssize_t send_to(const void* buf_, size_t len_, int flags_, const XESocketAddr& dest_addr_);
	
	// 接收
	ssize_t recv(void* buf_, size_t len_, int flags_);
	ssize_t recv_from(void* buf_, size_t len_, int flags_, XESocketAddr& addr_);
	
	// 设置阻塞与否
	bool set_nonblock(bool is_nonblock_);
	
	// 设置重用地址与否
	bool set_reuseaddr(bool is_reuse);
	
	// 获取当前socket所在地址，需在bind或者connect之后调用
	bool get_sock_name(XESocketAddr& dest_addr_);
	
	// 获取socket内部错误码
	uint32 get_sock_error();
	
	// 获取 句柄 / 描述符
	SOCKET get_handle();
	
	// 是否有效
	bool is_valid();
	
	// 设置发送超时时间
	bool set_send_timeo(uint32 timeo_ms);
	
	// 设置接收超时时间
	bool set_recv_timeo(uint32 timeo_ms);
	
	// 销毁
	void close();
	
private:
	
	 SOCKET		m_sk;
};
	
NS_XE_END
	
#endif // !_AESOCKET_H_
