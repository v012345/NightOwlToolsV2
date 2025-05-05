#ifndef _X_SOCKET_IO_MNGR_H_
#define _X_SOCKET_IO_MNGR_H_

#include <map>
#include <mutex>
#include "xenet/XESocketBase.h"
	
NS_XE_BEGIN
	
// IO操作
class XEIOOperate
{
	friend class XESockIOMngr;
public:

	~XEIOOperate();

private:

	XEIOOperate();
	XEIOOperate(const XEIOOperate&){};

	// 设置相关操作
	bool _add_operate(int type_, bool& need_interrupt);

	// 移除操作
	void _remove_operate(int type_, bool& need_interrupt);

	// 获取flag
	int _get_flag();

	// 
	void _clear_flag(int flag_);

	// 清除
	void _clear();

private:

	int					 m_flags;
};
	
typedef shared_ptr<XEIOOperate> XEIOOperatePtr;

	
class XESockIOMngr
{
public:
	XESockIOMngr();
	~XESockIOMngr();
	
	// 开启
	bool start();
	
	// 添加操作信息
	bool add_sync_io(XESocketBasePtr pBaseSocket, int type_, bool& need_interrupt);
	
	// 移除socket
	void remove_socket(XESocketBasePtr pBaseSocket, bool& need_interrupt);
	
	// 初始化Select文件描述符集合
	void init_select_fd_set(SOCKET& max_socket, fd_set& rd_set, fd_set& wr_set, fd_set& er_set);
	
	// 处理IO
	void process_io(fd_set& rd_set, fd_set& wr_set, fd_set& er_set);
	
	// stop
	void stop();
	
private:
	
	std::mutex									m_mutex_;
	SockMngrStatus								m_status_;
	std::map<XESocketBasePtr, XEIOOperatePtr>		m_operates_;  // 
};
	
NS_XE_END
	
#endif /*_X_SOCKET_IO_MNGR_H_*/