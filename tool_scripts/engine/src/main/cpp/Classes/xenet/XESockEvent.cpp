#include "xenet/XESockEvent.h"
	
NS_XE_BEGIN
		
// 事件操作(非IO)
XEEventOperate::XEEventOperate() : m_flag_(0)
{
}
	
XEEventOperate::~XEEventOperate()
{
}
	
bool XEEventOperate::_add_operate(int type_, int event_, bool& need_interrupt)
{
	ADD_OPERATE_FLAG(m_flag_, type_);
	if (SOCKET_FLAG_OPERATE_CLOSE & type_)
	{
		need_interrupt = true;
		return true;
	}
	XELOGWRITE("[XEEventOperate::_add_operate] add event.");
	pair<set<int>::iterator, bool> ret_ = m_events_.insert(event_);
	need_interrupt = ret_.second;
	if (!need_interrupt)
	{
		XELOGWRITE("[XEEventOperate::_add_operate] insert failed.");
	}
	return true;
}
	
	
// 获取标志
int XEEventOperate::_get_flag()
{
	return m_flag_;
}
	
//获取需要处理的event
void XEEventOperate::_get_event(set<int>& event_)
{
	event_.insert(m_events_.begin(), m_events_.end());
}
	
// 清除 
void XEEventOperate::_clear()
{
	m_flag_ = 0;
	m_events_.clear();
	XELOGWRITE("[XEEventOperate::_add_operate] clear event.");
}
	
XESockEventMngr::XESockEventMngr() : m_status_(SMS_NOT_RUNNING)
{
}

XESockEventMngr::~XESockEventMngr()
{
}

// 开启
bool XESockEventMngr::start()
{
	std::lock_guard<std::mutex> locker_(m_mutex_);
	if (SMS_NOT_RUNNING != m_status_) return false;

	m_status_ = SMS_RUNNING;

	return true;
}

// 添加操作信息
bool XESockEventMngr::add_sync_event(XESocketBasePtr pBaseSocket, int type_, int event_, bool& need_interrupt)
{
	std::lock_guard<std::mutex> locker_(m_mutex_);
	if (SMS_RUNNING != m_status_)
	{
		XELOGWRITE("[XESockEventMngr::add_sync_event] not running.");
		return false;
	}


	typedef map<XESocketBasePtr, XEEventOperatePtr>::iterator opt_iter;
	opt_iter it = m_events.find(pBaseSocket);

	bool need_interrupt_s = false;
	if (m_events.end() == it)
	{
		pair<opt_iter, bool> ret_ = m_events.insert(make_pair(pBaseSocket, XEEventOperatePtr(new XEEventOperate)));
		if (ret_.second == false)
		{
			XELOGWRITE("[XESockEventMngr::add_sync_event] insert failed.");
			return false;
		}
		it = ret_.first;
		need_interrupt_s = true;
	}
	bool need_interupt_o = false;
	it->second->_add_operate(type_, event_, need_interupt_o);

	need_interrupt = need_interupt_o || need_interrupt_s;
	
	return true;
}


// 移除socket
void XESockEventMngr::remove_socket(XESocketBasePtr pBaseSocket)
{
	std::lock_guard<std::mutex> locker_(m_mutex_);
	if (SMS_RUNNING != m_status_) return;

	map<XESocketBasePtr, XEEventOperatePtr>::iterator it_ = m_events.find(pBaseSocket);
	if (m_events.end() != it_)
	{
		// 
		m_events.erase(it_);
	}
}

// 处理事件
void XESockEventMngr::process_event(vector<XESocketBasePtr>& sRemoved)
{
	vector<pair<XESocketBasePtr, set<int>>> need_handle;
	
	m_mutex_.lock();
	std::map<XESocketBasePtr, XEEventOperatePtr>::iterator it_event = m_events.begin();
	for (; it_event != m_events.end();)
	{
		int flag_ = it_event->second->_get_flag();
		if (flag_ == 0) 
		{ 
			++it_event;
			continue; 
		}

		if (IS_OPERAGE_FLAG_SET(flag_, SOCKET_FLAG_OPERATE_CLOSE))
		{
			sRemoved.push_back(it_event->first);
			m_events.erase(it_event++);
			continue;
		}
		else if (IS_OPERAGE_FLAG_SET(flag_, SOCKET_FLAG_OPERATE_ASYNC))
		{
			set<int> events_;
			it_event->second->_get_event(events_);
			need_handle.push_back(make_pair(it_event->first, events_));
		}
		it_event->second->_clear();
		++it_event;
	}
	m_mutex_.unlock();

	vector<XESocketBasePtr>::iterator it_close = sRemoved.begin();
	for (; it_close != sRemoved.end(); ++it_close)
	{
		(*it_close)->on_close();
	}

	vector<pair<XESocketBasePtr, set<int>>>::iterator it_handle = need_handle.begin();
	for (; it_handle != need_handle.end(); ++it_handle)
	{
		set<int>::iterator it_event = it_handle->second.begin();
		for (; it_event != it_handle->second.end(); ++it_event)
			it_handle->first->on_can_handle(*it_event);
	}
}

// stop
void XESockEventMngr::stop()
{
	std::lock_guard<std::mutex> locker_(m_mutex_);
	if (SMS_RUNNING != m_status_) return;

	m_events.clear();
	m_status_ = SMS_NOT_RUNNING;
}

	
NS_XE_END