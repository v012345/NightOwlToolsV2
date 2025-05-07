#include "xenet/XEStdC.h"
#if XE_TARGET_PLATFORM == XE_PLATFORM_WIN32
	#include "windows.h"
#else
	#include "errno.h"
	#include "sys/time.h"
#endif
#include <new>
#include "xenet/XEEvent.h"
	
NS_XE_BEGIN
	
XEEvent::XEEvent(bool manual_reset, bool init_state) : m_hEvent(INVALID_EVENT_HANDLE)
{
#ifdef _MSC_VER
	m_hEvent = CreateEvent(NULL, manual_reset, init_state, NULL);
#else
	m_hEvent = new(std::nothrow) event_t;
	if (m_hEvent == NULL)
		return;

	m_hEvent->state = init_state;
	m_hEvent->manual_reset = manual_reset;
	if (pthread_mutex_init(&m_hEvent->mutex, NULL))
	{
		delete m_hEvent;
		return;
	}
	if (pthread_cond_init(&m_hEvent->cond, NULL))
	{
		pthread_mutex_destroy(&m_hEvent->mutex);
		delete m_hEvent;
		return;
	}
#endif
}
	
XEEvent::~XEEvent()
{
#ifdef _MSC_VER
	CloseHandle(m_hEvent);
#else
	pthread_cond_destroy(&m_hEvent->cond);
	pthread_mutex_destroy(&m_hEvent->mutex);
	delete m_hEvent;
#endif
}
	
// 是否有效
bool XEEvent::set_event()
{
#ifdef _MSC_VER
	return !!SetEvent(m_hEvent);
#else
	if (pthread_mutex_lock(&m_hEvent->mutex) != 0)
	{
		return false;
	}
	m_hEvent->state = true;
	if (m_hEvent->manual_reset)
	{
		if (pthread_cond_broadcast(&m_hEvent->cond))
		{
			return false;
		}
	}
	else
	{
		if (pthread_cond_signal(&m_hEvent->cond))
		{
			return false;
		}
	}
	if (pthread_mutex_unlock(&m_hEvent->mutex) != 0)
	{
		return false;
	}
	
	return true;
#endif
}
	
bool XEEvent::reset_event()
{
#ifdef _MSC_VER
	return !!ResetEvent(m_hEvent);
#else
	if (pthread_mutex_lock(&m_hEvent->mutex) != 0)
	{
		return false;
	}
	m_hEvent->state = false;
	if (pthread_mutex_unlock(&m_hEvent->mutex) != 0)
	{
		return false;
	}
	return true;
#endif
}
	
wait_result XEEvent::wait()
{
#ifdef _MSC_VER
	DWORD ret = WaitForSingleObject(m_hEvent, INFINITE);
	if (ret == WAIT_OBJECT_0)
	{
		// 
		return wait_result::wait_one;
	}
	return wait_result::wait_failed;
#else
	int tmp_ret =pthread_mutex_lock(&m_hEvent->mutex); 
	if (tmp_ret)
	{
		return wait_result::wait_failed;
	}
	while (!m_hEvent->state)
	{
		if (pthread_cond_wait(&m_hEvent->cond, &m_hEvent->mutex))
		{
			pthread_mutex_unlock(&m_hEvent->mutex);
			return wait_result::wait_failed;
		}
	}
	if (!m_hEvent->manual_reset)
	{
		m_hEvent->state = false;
	}
	if (pthread_mutex_unlock(&m_hEvent->mutex))
	{
		return wait_result::wait_failed;
	}
	return wait_result::wait_one;
#endif
}
	
wait_result XEEvent::timed_wait(unsigned int ums_)
{
#ifdef _MSC_VER
	DWORD ret = WaitForSingleObject(m_hEvent, ums_);
	if (ret == WAIT_OBJECT_0)
	{
		return wait_result::wait_one;
	}
	if (ret == WAIT_TIMEOUT)
	{
		return wait_result::wait_timeout;
	}
	return wait_result::wait_failed;
#else

	int rc = 0;
	struct timespec abstime;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	abstime.tv_sec = tv.tv_sec + ums_ / 1000;
	abstime.tv_nsec = tv.tv_usec * 1000 + (ums_ % 1000) * 1000000;
	if (abstime.tv_nsec >= 1000000000)
	{
		abstime.tv_nsec -= 1000000000;
		abstime.tv_sec++;
	}

	if (pthread_mutex_lock(&m_hEvent->mutex) != 0)
	{
		return wait_result::wait_failed;
	}
	while (!m_hEvent->state)
	{
		if ((rc = pthread_cond_timedwait(&m_hEvent->cond, &m_hEvent->mutex, &abstime)))
		{
			if (rc == ETIMEDOUT) break;
			pthread_mutex_unlock(&m_hEvent->mutex);
			return wait_result::wait_failed;
		}
	}
	if (rc == 0 && !m_hEvent->manual_reset)
	{
		m_hEvent->state = false;
	}
	if (pthread_mutex_unlock(&m_hEvent->mutex) != 0)
	{
		return wait_result::wait_failed;
	}
	if (rc == ETIMEDOUT)
	{
		return wait_result::wait_timeout;
	}
		
	return wait_result::wait_one;
#endif
}
	
NS_XE_END
