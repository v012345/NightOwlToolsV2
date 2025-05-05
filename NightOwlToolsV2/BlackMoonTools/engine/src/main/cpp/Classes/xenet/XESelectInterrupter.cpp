#include "string.h"
#include "xenet/XEPlatformConfig.h"
#include "xenet/XESelectInterrupter.h"
#include "xenet/XESocketAddr.h"
	
NS_XE_BEGIN
	
#define INTERRUPT_SOCKET_TIMEOUT_TIME						(2 * 1000)  // 超时时间2S
#define INTERRUPT_TEST_STRING								"test"
#define INTERRUPT_INTERRUPT_CHAR							'b'
	
XESelectInterupter::XESelectInterupter(XESelector* selector_) : m_status_(IS_NOT_RUNNING),
																m_sock_client(),
																m_sock_server(),
																m_Selector(selector_)
{
	m_have_written = false;
}

XESelectInterupter::~XESelectInterupter()
{
}
	
bool XESelectInterupter::init()
{
	// 
	std::lock_guard<std::mutex> locker_(m_mutex_);
	
	if (m_status_ != IS_NOT_RUNNING) return false;
	if (!m_sock_server.create(AF_INET, SOCK_DGRAM, IPPROTO_UDP) ||
		!m_sock_client.create(AF_INET, SOCK_DGRAM, IPPROTO_UDP))
	{
		// 
		return false;
	}
	
	m_sock_server.set_send_timeo(INTERRUPT_SOCKET_TIMEOUT_TIME);
	m_sock_client.set_send_timeo(INTERRUPT_SOCKET_TIMEOUT_TIME);
	
	XESocketAddr inAddr;
	inAddr.set_host("127.0.0.1");
	inAddr.set_port(0);
	//sockaddr_in inAddr;
	//inAddr.sin_family = AF_INET;
	//inAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//inAddr.sin_port = 0;
	
	if (!m_sock_server.bind(inAddr))
	{
		// 
		return false;
	}
	
	if (!m_sock_server.get_sock_name(inAddr))
	{
		// 
		return false;
	}
	
	
	if (!m_sock_client.connect(inAddr))
	{
		
		// 
		return false;
	}
	
	if (SOCKET_ERROR == m_sock_client.send(INTERRUPT_TEST_STRING, strlen(INTERRUPT_TEST_STRING), 0))
	{
		return false;
	}
	
#define TEST_BUFF_SIZE	1024
	
	char szBuff[TEST_BUFF_SIZE] = {};
	if (SOCKET_ERROR == m_sock_server.recv(szBuff, TEST_BUFF_SIZE, 0) || (0 != strcmp(szBuff, INTERRUPT_TEST_STRING)))
	{
		
		return false;
	}
	
	if (!m_sock_server.set_nonblock(true)) return false;
	if (!m_sock_client.set_nonblock(true)) return false;
	if (!m_sock_server.set_reuseaddr(true)) return false;
	if (!m_sock_client.set_reuseaddr(true)) return false;
	
	m_status_ = IS_RUNNING;
	
	return true;
}
	
// 获取等待句柄
SOCKET XESelectInterupter::get_handle()
{
	std::lock_guard<std::mutex> locker_(m_mutex_);
	return m_sock_server.get_handle();
}
	
// 获取当前的状态
long XESelectInterupter::get_status()
{
	// 
	std::lock_guard<std::mutex> locker_(m_mutex_);
	return m_status_;
}
	
// 重置
void XESelectInterupter::reset()
{
	std::lock_guard<std::mutex> locker_(m_mutex_);
	if (m_status_ != IS_RUNNING) return;
	
#define BUFF_LEN	1024
	
	int ret_ = 0;
	char buff_[BUFF_LEN] = {};
	do
	{
		ret_ = m_sock_server.recv(buff_, BUFF_LEN, 0);
		
	} while (BUFF_LEN == ret_);
	m_have_written = false;
	
}

void XESelectInterupter::interrupt()
{
	std::lock_guard<std::mutex> locker_(m_mutex_);
	if (m_status_ != IS_RUNNING) return;
	if (m_have_written) return ;
	
	char c = INTERRUPT_INTERRUPT_CHAR;
	m_sock_client.send(&c, sizeof c, 0);
	m_have_written = true;
}
	
// 释放
void XESelectInterupter::release()
{
	std::lock_guard<std::mutex> locker_(m_mutex_);
	
	m_sock_client.close();
	m_sock_server.close();
	
	m_status_ = IS_NOT_RUNNING;
}
	
NS_XE_END