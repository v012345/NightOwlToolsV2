#ifndef _X_SELECT_INTERRUPTER_
#define _X_SELECT_INTERRUPTER_
	
//*********************************************************************
// FileName		:	XESelectInterupter.h
// Author		:	majianwei
// Date			:	2017/12/8
// Description	:	打断Select等待的接口
//*********************************************************************

#include <mutex>
#include "xenet/XEStdC.h"
#include "xenet/XESocket.h"
	
NS_XE_BEGIN
	
enum InterruptStatus
{
	IS_NOT_RUNNING = 1,
	IS_RUNNING = 2,
};
	
class XESelector;
class XESelectInterupter
{
	
public:
	XESelectInterupter(XESelector* selector_);
	~XESelectInterupter();
	
	// 初始化
	bool init();
	
	// 获取等待句柄
	SOCKET get_handle();
	
	// 获取当前的状态
	long get_status();
	
	// 重置
	void reset();
	
	// 打断
	void interrupt();
	
	// 释放
	void release();
	
	
private:
	
	std::mutex			m_mutex_;
	InterruptStatus		m_status_;
	bool				m_have_written;
	XESocket			m_sock_client;
	XESocket			m_sock_server;
	XESelector*			m_Selector;
};
	
NS_XE_END
	
#endif /*_X_SELECT_INTERRUPTER_*/
