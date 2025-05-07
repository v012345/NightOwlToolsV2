#ifndef _X_SOCKET_EVENT_MNGR_H_
#define _X_SOCKET_EVENT_MNGR_H_
	
#include <set>
#include <list>
#include <map>
#include <mutex>
#include <memory>
#include <functional>
#include <unordered_map>
#include "xenet/XESocketBase.h"
using namespace std;
	
NS_XE_BEGIN
// 事件操作(非IO)
class XEEventOperate
{
	friend class XESockEventMngr;
public:
	
	~XEEventOperate();
	
private:
	
	XEEventOperate();
	
	// 设置相关操作
	bool _add_operate(int type_, int event_, bool& need_interrupt);
	
	// 获取标志
	int _get_flag();
	
	//获取需要处理的event
	void _get_event(set<int>& event_);
	
	// 清除 
	void _clear();
	
private:
	
	int					 m_flag_;
	set<int>			 m_events_;
};
	
typedef shared_ptr<XEEventOperate> XEEventOperatePtr;
	
class XESockEventMngr
{
public:
	XESockEventMngr();
	~XESockEventMngr();

	// 开启
	bool start();

	// 添加异步事件
	bool add_sync_event(XESocketBasePtr pBaseSocket, int type_, int event_, bool& need_interrupt);

	// 移除socket
	void remove_socket(XESocketBasePtr pBaseSocket);

	// 处理事件
	void process_event(vector<XESocketBasePtr>& sRemoved);

	// stop
	void stop();

private:

	std::mutex									m_mutex_;
	SockMngrStatus								m_status_;
	std::map<XESocketBasePtr, XEEventOperatePtr>	m_events;
};
	
	
NS_XE_END
	
	
#endif /*_X_SOCKET_EVENT_MNGR_H_*/
