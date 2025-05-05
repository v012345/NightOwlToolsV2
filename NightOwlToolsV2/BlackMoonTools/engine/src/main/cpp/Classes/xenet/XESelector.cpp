#include <iostream>
#include "xenet/XEInclude.h"
#include "xenet/XESelector.h"
	
NS_XE_BEGIN
	
XESelector* XESelector::m_instance = nullptr;

XESelector* XESelector::getInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new XESelector();
	}
	return m_instance;
}

XESelector::XESelector() :m_stop(false), m_status_(SS_NOT_RUNING), m_interrupter_(this)
{
}
	
XESelector::~XESelector()
{
	if (SS_STOPED != m_status_)
		stop();
}
	
// 初始化
bool XESelector::start()
{
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    signal(SIGPIPE ,SIG_IGN);
#endif
	std::lock_guard<std::mutex> locker_(m_mutex_);
	if (!m_thread_.joinable()) m_thread_ = std::thread(std::bind(&XESelector::_run, this));
	
	m_status_ = SS_RUNING;
	
	return true;
}

void XESelector::stop()
{
	std::lock_guard<std::mutex> locker_(m_mutex_);
	m_stop = true;
	m_interrupter_.interrupt();
	m_thread_.join();
	m_status_ = SS_STOPED;
}
	
// 异步接收
bool XESelector::async_recv(XESocketBasePtr pBaseSocket, OPERATE_RUN_TYPE run_type_)
{
	// 
	bool need_interrupt = false;
	int type_ = (run_type_ == OPERATE_RUN_TYPE::OPERATE_RUN_ENDLESS) ? 
				SOCKET_FLAG_OPERATE_READ_ENDLESS : SOCKET_FLAG_OPERATE_READ_ONCE;
	if (!m_io_mngr.add_sync_io(pBaseSocket, type_, need_interrupt))
		return false;
	
	if (need_interrupt) m_interrupter_.interrupt();
	
	return true;
}

// 异步发送
bool XESelector::async_send(XESocketBasePtr pBaseSocket, OPERATE_RUN_TYPE run_type_)
{
	// 
	bool need_interrupt = false;
	
	int type_ = (run_type_ == OPERATE_RUN_TYPE::OPERATE_RUN_ENDLESS) ?
				SOCKET_FLAG_OPERATE_WRITE_ENDLESS : SOCKET_FLAG_OPERATE_WRITE_ONCE;
	if (!m_io_mngr.add_sync_io(pBaseSocket, type_, need_interrupt))
		return false;
	
	if (need_interrupt) m_interrupter_.interrupt();
	
	return true;
}
	
// 异步关闭
bool XESelector::async_close(XESocketBasePtr pBaseSocket)
{
	bool need_interrupt = false;
	
	m_timer_.remove_times_(pBaseSocket);
	m_event_mngr.add_sync_event(pBaseSocket, SOCKET_FLAG_OPERATE_CLOSE, 0, need_interrupt);
	
	if (need_interrupt) m_interrupter_.interrupt();
	
	return true;
}
	
// 异步做一些事情
bool XESelector::async_do_something(XESocketBasePtr pBaseSocket, int event_)
{
	bool need_interrupt = false;

	m_event_mngr.add_sync_event(pBaseSocket, SOCKET_FLAG_OPERATE_ASYNC, event_, need_interrupt);

	if (need_interrupt)
	{
		m_interrupter_.interrupt();
	}
	else
	{
		XELOGWRITE("[XESelector::async_do_something] not use to interrupt.");
	}

	return true;
}
	
// 移除操作
void XESelector::remove_socket(XESocketBasePtr pBaseSocket)
{
	// 
	bool need_interrupt = false;
	m_timer_.remove_times_(pBaseSocket);
	m_event_mngr.remove_socket(pBaseSocket);
	m_io_mngr.remove_socket(pBaseSocket, need_interrupt);
	
	if (need_interrupt) m_interrupter_.interrupt();
}
	

// 添加定时器
int XESelector::add_timer(XESocketBasePtr pBaseSocket, OPERATE_RUN_TYPE run_type_, int interval_, int timer_id)
{
	bool need_interrupt = false;
	int ret_ = m_timer_.add_timer_(pBaseSocket, timer_id, run_type_, interval_, need_interrupt);
	if (need_interrupt) m_interrupter_.interrupt();
	return ret_;
}
	
// 移除定时器
void XESelector::remove_timer(int timer_id)
{
	// 
	m_timer_.remove_timer_by_key(timer_id);
}
	

void XESelector::_run()
{

	XELOGWRITE("XESelector::_run");

	if (!m_interrupter_.init())
	{
		XELOGWRITE("XESelector::_run m_interrupter_.init  fail");
		return;
	}


	if (!m_timer_.start())
	{
		XELOGWRITE("XESelector::_run m_timer_.start  fail");
		return;
	}


	if (!m_io_mngr.start())
	{
		XELOGWRITE("XESelector::_run m_io_mngr.start fail");
		return;
	}


	if (!m_event_mngr.start())
	{
		XELOGWRITE("XESelector::_run m_event_mngr.start fail");
		return;
	}

    for (;;)
    {
        SOCKET max_socket = 0;
        _init_select_fd_set(max_socket);
        uint32 wait_ms = m_timer_.get_wait_tm();
        
        timeval tm_;
        tm_.tv_sec = wait_ms / 1000;
        tm_.tv_usec = (wait_ms % 1000) * 1000;
		//XELOGWRITE("XESelector::_run for before ::select tm_.tv_sec %ld tm_.tv_usec %ld", tm_.tv_sec, tm_.tv_usec);
        int nRet = ::select(max_socket + 1, &m_r_sets, &m_w_sets, &m_e_sets, &tm_);
		//XELOGWRITE("XESelector::_run for end ::select %d %d", nRet, m_stop);
        bool is_interrupted = false;
        
        if (m_stop){
            return _on_stop();
        }
        else if (SOCKET_ERROR == nRet){
            return _on_select_error();
        }
        else if (0 == nRet)
        {
            //
            m_timer_.process_timer();
        }
        else
        {
            is_interrupted = !!FD_ISSET(m_interrupter_.get_handle(), &m_r_sets);
            
            if (is_interrupted)
            {
                m_interrupter_.reset();
                vector<XESocketBasePtr> sRemoved;
                m_event_mngr.process_event(sRemoved);
                if (!sRemoved.empty())
                {
                    auto iter_remove = sRemoved.begin();
                    for (; iter_remove != sRemoved.end(); ++ iter_remove)
                    {
                        bool no_need_interrupt;
                        m_timer_.remove_times_(*iter_remove);
                        m_io_mngr.remove_socket(*iter_remove, no_need_interrupt);
                    }
                }
            }
            m_timer_.process_timer();
            
            if (!is_interrupted || nRet > 1)
                m_io_mngr.process_io(m_r_sets, m_w_sets, m_e_sets);
            

        }
    }
}
	
// 
void XESelector::_init_select_fd_set(SOCKET& max_socket)
{
	m_io_mngr.init_select_fd_set(max_socket, m_r_sets, m_w_sets, m_e_sets);
	
	FD_SET(m_interrupter_.get_handle(), &m_r_sets);
	if (max_socket < m_interrupter_.get_handle()) max_socket = m_interrupter_.get_handle();
}
	
// 停止
void XESelector::_on_stop()
{
	m_timer_.stop();
	m_io_mngr.stop();
	m_event_mngr.stop();
	m_interrupter_.release();
}

// 解决 错误
void XESelector::_on_select_error()
{
	XELOGWRITE("XESelector::_on_select_error:%d", errno);
	std::cout << "_on_select_error happened : " << ERRORNO << endl;
}
	
NS_XE_END
