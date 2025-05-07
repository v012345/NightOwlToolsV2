#include <iostream>
#include <memory>
#include "xenet/XEStdC.h"
#include "xenet/XESelectTimer.h"
using namespace std;
	
NS_XE_BEGIN
	
XETimerMngr::XETimerMngr() : m_status_(TS_NOT_RUNNING)
{
}
	
XETimerMngr::~XETimerMngr()
{
}
	
// 开始
bool XETimerMngr::start()
{
	std::lock_guard<std::mutex> locker_(m_mutex_);
	if (m_status_ != TS_NOT_RUNNING) 
		return false;
	
	m_status_ = TS_RUNNING;
	return true;
}
	
int XETimerMngr::add_timer_(XESocketBasePtr ptrSocket, int key_, OPERATE_RUN_TYPE run_type_,
									int interval_, bool& need_interrupt)
{
	std::lock_guard<std::mutex> locker_(m_mutex_);
	if (TS_RUNNING != m_status_)
		return -1;
	
	XTimerOperationPtr new_operation(new XETimerOperation(ptrSocket, run_type_, key_, interval_));
	_add_timer(new_operation, need_interrupt);
	
	return key_;
}
	
// 移除定时器
void XETimerMngr::remove_times_(XESocketBasePtr ptrSocket)
{
	lock_guard<mutex> locker_(m_mutex_);

	if (TS_RUNNING != m_status_)
		return;

	list<XTimerOperationPtr>::iterator it_tm = m_timers.begin();
	for (; it_tm != m_timers.end();)
	{
		if ((*it_tm)->m_ptrSocket == ptrSocket)
			it_tm = m_timers.erase(it_tm);
		else
			++ it_tm;
	}
}
	

void XETimerMngr::remove_timer_by_key(int key_)
{
	lock_guard<mutex> locker_(m_mutex_);
	if (TS_RUNNING != m_status_) return;
	
	list<XTimerOperationPtr>::iterator it_tm = m_timers.begin();
	for (; it_tm != m_timers.end(); ++it_tm)
	{
		if ((*it_tm)->m_key_ == key_)
		{
			m_timers.erase(it_tm);
			break;
		}
	}
	
}
	
// 处理timer
void XETimerMngr::process_timer()
{
	vector<XTimerOperationPtr> reached_timer;
	
	m_mutex_.lock();
	_get_reached_timer(reached_timer);
	m_mutex_.unlock();
	
	vector<XTimerOperationPtr>::iterator it_timer = reached_timer.begin();
	for (; it_timer != reached_timer.end(); ++it_timer)
	{
		(*it_timer)->process();
	}
}
	
// 获取最长等待时间(ms)
uint32 XETimerMngr::get_wait_tm()
{
	std::lock_guard<std::mutex> locker_(m_mutex_);
	if (TS_RUNNING != m_status_|| m_timers.empty())
		return 0x7fffffff;
	
	XTimerOperationPtr pTimerOpr = *(m_timers.begin());
	uint32 now_ = iclock();
	uint32 ret_ = pTimerOpr->m_dead_line > now_ ? pTimerOpr->m_dead_line - now_ : 0; // for debug.
	return ret_;
}
	
bool XETimerMngr::stop()
{
	m_mutex_.lock();
	if (TS_RUNNING != m_status_){
		m_mutex_.unlock();
		return false;
	}
	
	m_timers.clear();
	m_status_ = TS_NOT_RUNNING;
	
	m_mutex_.unlock();
	
	return true;
}
	
// timer 入队列
void XETimerMngr::_add_timer(XTimerOperationPtr ptrTimer, bool& is_first)
{
	if (NULL == ptrTimer) return;
	
	is_first = true;
	list<XTimerOperationPtr>::iterator it_tm = m_timers.begin();
	for (; it_tm != m_timers.end(); ++it_tm)
	{
		if (ptrTimer->m_dead_line < (*it_tm)->m_dead_line)
		{
			m_timers.insert(it_tm, ptrTimer);
			return ;
		}
		is_first = false;
	}
	m_timers.push_back(ptrTimer);
}

	
// 处理
void XETimerMngr::_get_reached_timer(vector<XTimerOperationPtr>& reached_timer)
{
	vector<XTimerOperationPtr> recovery_timer;
	list<XTimerOperationPtr>::iterator it_tm = m_timers.begin();
	for (; it_tm != m_timers.end();)
	{
		XTimerOperationPtr ptrTimer = *it_tm;
		uint32 tm_now = iclock();
		if (tm_now < ptrTimer->m_dead_line)
			break;
		reached_timer.push_back(ptrTimer);
		if (ptrTimer->m_run_type_ == OPERATE_RUN_TYPE::OPERATE_RUN_ENDLESS)
		{
			ptrTimer->m_dead_line = max(tm_now, ptrTimer->m_dead_line + ptrTimer->m_interval);
			recovery_timer.push_back(ptrTimer);
		}
		it_tm = m_timers.erase(it_tm);
	}
	
	// 重新入队
	vector<XTimerOperationPtr>::iterator it_recover_ = recovery_timer.begin();
	for (; it_recover_ != recovery_timer.end(); ++it_recover_)
	{
		bool is_first = false;
		_add_timer(*it_recover_, is_first);
	}
}
	
NS_XE_END