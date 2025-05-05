#ifndef _X_SELECTOR_H_
#define _X_SELECTOR_H_
	
//*********************************************************************
// FileName		:	XESelector.h
// Author		:	majianwei
// Date			:	2017/12/8
// Description	:	Select IO 模型, 包含 timer，目前只单线程
//*********************************************************************

#include <map>
#include <list>
#include <vector>
#include <thread>
#include "xenet/XESocketBase.h"
#include "xenet/XEPlatformDefine.h"
#include "xenet/XESockIO.h"
#include "xenet/XESockEvent.h"
#include "xenet/XESelectTimer.h"
#include "xenet/XESelectInterrupter.h"
	
using namespace std;
	
NS_XE_BEGIN
	
enum SelectorStatus
{
	SS_NOT_RUNING = 1,
	SS_RUNING = 2,
	SS_NEED_STOP = 3,
	SS_STOPED = 4,
};
	
class XESelector
{
public:
	static XESelector* getInstance();
	
	// 
	bool start();

	// 
	void stop();
	
	// 异步接收
	bool async_recv(XESocketBasePtr pBaseSocket, OPERATE_RUN_TYPE run_type_);
	
	// 异步发送
	bool async_send(XESocketBasePtr pBaseSocket, OPERATE_RUN_TYPE run_type_);
	
	// 异步关闭
	bool async_close(XESocketBasePtr pBaseSocket);
	
	// 异步做一些事情
	bool async_do_something(XESocketBasePtr pBaseSocket, int event_);
	
	// 移除socket
	void remove_socket(XESocketBasePtr pBaseSocket);
	
	// 添加定时器
	int add_timer(XESocketBasePtr pBaseSocket, OPERATE_RUN_TYPE run_type_, int interval_, int timer_id);
	
	// 移除定时器
	void remove_timer(int timer_id);
	
	

private:
	XESelector();
	~XESelector();
	// 
	void _run();
	
	// 
	void _init_select_fd_set(SOCKET& max_socket);
	
	// 停止
	void _on_stop();
	
	// 解决 错误
	void _on_select_error();
	
	std::mutex					m_mutex_;
	fd_set						m_r_sets;  // 
	fd_set						m_w_sets;  // 
	fd_set						m_e_sets;  // 
	
	bool						m_stop;		// 是否需要停止
	std::thread					m_thread_;  // 
	SelectorStatus				m_status_;  // 
	
	XETimerMngr					m_timer_;
	XESockIOMngr					m_io_mngr;  // 
	XESockEventMngr				m_event_mngr;  // 
	XESelectInterupter			m_interrupter_;  // 

	static XESelector* m_instance;
};
	
NS_XE_END
	
#endif /*_X_SELECTOR_H_*/