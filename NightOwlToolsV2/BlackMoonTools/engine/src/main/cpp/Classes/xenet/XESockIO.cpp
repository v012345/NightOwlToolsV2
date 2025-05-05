#include "xenet/XESockIO.h"
#include <vector>	
NS_XE_BEGIN
	
// IO操作
XEIOOperate::XEIOOperate() : m_flags(0)
{
}
	
XEIOOperate::~XEIOOperate()
{
}
	
// 设置相关操作
bool XEIOOperate::_add_operate(int type_, bool& need_interrupt)
{
	if (type_ < SOCKET_FLAG_OPERATE_MIN || type_ > SOCKET_FLAG_OPERATE_MAX)
		return false;
	
	need_interrupt = !IS_OPERAGE_FLAG_SET(m_flags, type_);
	ADD_OPERATE_FLAG(m_flags, type_);
	
	return true;
}
	
// 移除操作
void XEIOOperate::_remove_operate(int type_, bool& need_interrupt)
{
	need_interrupt = false;
	REMOVE_OPERATE_FLAG(m_flags, type_);
}
	
// 
void XEIOOperate::_clear_flag(int flag_)
{
	REMOVE_OPERATE_FLAG(m_flags, flag_);
}
	
// 获取 flag
int XEIOOperate::_get_flag()
{
	// 
	return m_flags;
}
	
// 清除
void XEIOOperate::_clear()
{
	m_flags = 0;
}

	
XESockIOMngr::XESockIOMngr() : m_status_(SMS_NOT_RUNNING)
{
}
	
XESockIOMngr::~XESockIOMngr()
{
}
	
// 开启
bool XESockIOMngr::start()
{
	std::lock_guard<std::mutex> locker_(m_mutex_);
	if (SMS_NOT_RUNNING != m_status_) return false;
	
	m_status_ = SMS_RUNNING;
	
	return true;
}
	
// 添加操作信息
bool XESockIOMngr::add_sync_io(XESocketBasePtr pBaseSocket, int type_, bool& need_interrupt)
{
	std::lock_guard<std::mutex> locker_(m_mutex_);
	if (SMS_RUNNING != m_status_) return false;
	
	typedef map<XESocketBasePtr, XEIOOperatePtr>::iterator opt_iter;
	opt_iter it = m_operates_.find(pBaseSocket);

	bool need_interrupt_s = false;
	if (m_operates_.end() == it)
	{
		pair<opt_iter, bool> ret_ = m_operates_.insert(make_pair(pBaseSocket, XEIOOperatePtr(new XEIOOperate)));
		if (ret_.second == false) return false;

		it = ret_.first;
		need_interrupt_s = true;
	}
	bool need_interupt_o = false;
	it->second->_add_operate(type_, need_interupt_o);

	need_interrupt = need_interupt_o || need_interrupt_s;
	
	return true;
}
	
// 移除socket
void XESockIOMngr::remove_socket(XESocketBasePtr pBaseSocket, bool& need_interrupt)
{
	need_interrupt = false;
	std::lock_guard<std::mutex> locker_(m_mutex_);
	if (SMS_RUNNING != m_status_) return ;
	
	map<XESocketBasePtr, XEIOOperatePtr>::iterator it_ = m_operates_.find(pBaseSocket);
	if (m_operates_.end() != it_)
	{
		// 
		need_interrupt = true;
		m_operates_.erase(it_);
	}
}
	
// 初始化Select文件描述符集合
void XESockIOMngr::init_select_fd_set(SOCKET& max_socket, fd_set& rd_set, fd_set& wr_set, fd_set& er_set)
{
	FD_ZERO(&rd_set);
	FD_ZERO(&wr_set);
	FD_ZERO(&er_set);
	
	std::lock_guard<std::mutex> locker_(m_mutex_);
	if (SMS_RUNNING != m_status_) return ;
	map<XESocketBasePtr, XEIOOperatePtr>::iterator it_operate = m_operates_.begin();
	for (; it_operate != m_operates_.end(); ++it_operate)
	{
		XESocketBasePtr ptrBaseSocket = it_operate->first;
		if (!ptrBaseSocket) continue;
		
		SOCKET fd_ = ptrBaseSocket->get_handle();
		int flag_ = it_operate->second->_get_flag();
		if (IS_OPERAGE_FLAG_SET(flag_, SOCKET_FLAG_OPERATE_READ_ONCE | SOCKET_FLAG_OPERATE_READ_ENDLESS))
			FD_SET(fd_, &rd_set);
		
		if (IS_OPERAGE_FLAG_SET(flag_, SOCKET_FLAG_OPERATE_WRITE_ONCE | SOCKET_FLAG_OPERATE_WRITE_ENDLESS))
			FD_SET(fd_, &wr_set);
		
		if (fd_ > max_socket) max_socket = fd_;
	}
}
	

	
// 处理IO
void XESockIOMngr::process_io(fd_set& rd_set, fd_set& wr_set, fd_set& er_set)
{
	m_mutex_.lock();
	if (SMS_RUNNING != m_status_)
	{
		m_mutex_.unlock();
		return;
	}
	
	std::vector<XESocketBasePtr> vecRead, vecWrite;
	std::map<XESocketBasePtr, XEIOOperatePtr>::iterator it_operate_ = m_operates_.begin();
	for (; it_operate_ != m_operates_.end(); )
	{
		int flag_ = it_operate_->second->_get_flag();
		if (0 == flag_)
		{
			++ it_operate_;
			continue;
		}
		
		SOCKET s_ = it_operate_->first->get_handle();
		if (IS_OPERAGE_FLAG_SET(flag_, SOCKET_FLAG_READ_OPERATE) && FD_ISSET(s_, &rd_set))
		{
			vecRead.push_back(it_operate_->first);
			it_operate_->second->_clear_flag(SOCKET_FLAG_OPERATE_READ_ONCE);
		}
		
		if (IS_OPERAGE_FLAG_SET(flag_, SOCKET_FLAG_WRITE_OPERATE) && FD_ISSET(s_, &wr_set))
		{
			vecWrite.push_back(it_operate_->first);
			it_operate_->second->_clear_flag(SOCKET_FLAG_OPERATE_WRITE_ONCE);
		}
		
		++it_operate_;
	}
	m_mutex_.unlock();
	
	std::vector<XESocketBasePtr>::iterator it_socket = vecRead.begin();
	for (; it_socket != vecRead.end(); ++it_socket)
		(*it_socket)->on_can_recv();
	
	it_socket = vecWrite.begin();
	for (; it_socket != vecWrite.end(); ++it_socket)
		(*it_socket)->on_can_send();
}
	
// stop
void XESockIOMngr::stop()
{
	std::lock_guard<std::mutex> locker_(m_mutex_);
	if (SMS_RUNNING != m_status_) return ;
	
	m_operates_.clear();
	m_status_ = SMS_NOT_RUNNING;
}
	
	
NS_XE_END
	
