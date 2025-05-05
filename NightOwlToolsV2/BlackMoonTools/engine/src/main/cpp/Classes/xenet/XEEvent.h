#ifndef _AEEVENT_H_
#define _AEEVENT_H_
	
//*********************************************************************
// FileName		:	tk_event.h
// Author		:	majianwei
// Date			:	2018/1/20
// Description	:	event 的 跨平台实现
//*********************************************************************
	
#include "xenet/XEPlatformConfig.h"
#include "xenet/XEInclude.h"

NS_XE_BEGIN
#if XE_TARGET_PLATFORM == XE_PLATFORM_WIN32
	typedef HANDLE event_handle;
	#define INVALID_EVENT_HANDLE	INVALID_HANDLE_VALUE
#else
#include <pthread.h>
	typedef struct
	{
		bool state;
		bool manual_reset;
		pthread_mutex_t mutex;
		pthread_cond_t cond;
	}event_t;
#define INVALID_EVENT_HANDLE	NULL
	typedef event_t* event_handle;
#endif
	
enum wait_result
{
	wait_one = 0,  // 等待到了一个
	wait_timeout = 1,  // 等待超时了
	wait_failed = 2,  // 等待失败了
};
	
class XEEvent
{
public:
	XEEvent(bool manual_reset, bool init_state);
	~XEEvent();
	
	bool set_event();
	
	bool reset_event();
	
	wait_result wait();
	
	wait_result timed_wait(unsigned int ums_);
	
private:
	
	event_handle		m_hEvent;
};
	
NS_XE_END
	
#endif /*_AEEVENT_H_*/