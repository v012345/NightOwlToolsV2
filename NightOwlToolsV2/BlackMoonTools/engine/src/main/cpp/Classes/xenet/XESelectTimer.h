#ifndef _X_SELECT_TIMER_H_
#define _X_SELECT_TIMER_H_
	
#include <list>
#include <queue>
#include <mutex>
#include <memory>
#include "xenet/XESocketBase.h"
#include "xenet/XETime.h"
	
NS_XE_BEGIN
	
typedef void(TimerCallBack)(int);
	
class XETimerOperation
{
	friend class XETimerMngr;
	
public:
	
	~XETimerOperation()
	{
	}
	
private:
	
	XETimerOperation(XESocketBasePtr ptrSocket, OPERATE_RUN_TYPE run_type_, int key_, int interval_)
		: m_ptrSocket(ptrSocket), m_run_type_(run_type_), m_key_(key_), m_interval(interval_), m_dead_line(iclock() + interval_)
	{
	}
	
	// 
	void process()
	{
		XESocketBasePtr ptrSocket = m_ptrSocket;
		if (ptrSocket) ptrSocket->on_timer(m_key_);
	}
	
private:
	
	XESocketBasePtr		m_ptrSocket;
	OPERATE_RUN_TYPE	m_run_type_;
	int					m_key_;
	int					m_interval;
	unsigned int		m_dead_line;
};
	
typedef shared_ptr<XETimerOperation> XTimerOperationPtr;
	
enum TimerStatus
{
	TS_NOT_RUNNING = 1,
	TS_RUNNING = 2,
};
	
class XETimerMngr
{
public:
	XETimerMngr();
	~XETimerMngr();
	
	// 开始
	bool start();
	
	// 添加定时器
	int add_timer_(XESocketBasePtr ptrSocket, int key_, OPERATE_RUN_TYPE run_type_,
					int interval_, bool& need_interrupt);
	
	// 移除key指定的多个定时器
	void remove_times_(XESocketBasePtr ptrSocket);
	
	// 按照id删除定时器
	void remove_timer_by_key(int key_);
	
	// 处理timer
	void process_timer();
	
	// 获取最长等待时间(ms)
	uint32 get_wait_tm();
	
	// 停止
	bool stop();
	
private:
	
	// timer 入队列
	void _add_timer(XTimerOperationPtr pNewTimer_, bool& is_first);
	
	// 处理
	void _get_reached_timer(vector<XTimerOperationPtr>& reached_timer);
	
	std::mutex					m_mutex_;
	TimerStatus					m_status_;
	std::list<XTimerOperationPtr>		m_timers;
};
	
	
NS_XE_END
	
#endif /*_X_SELECT_TIMER_H_*/