#ifndef _AE_LOG_H_
#define _AE_LOG_H_
	
#include <list>
#include <mutex>
#include <string>
#include <fstream>
#include "xenet/XEStdC.h"	
#include <stdio.h>
#if XE_TARGET_PLATFORM == XE_PLATFORM_WIN32 
#define SAFE_FORMATE	sprintf_s
#else 
#define SAFE_FORMATE	snprintf
#endif 
using namespace std;
NS_XE_BEGIN
	
class XELog
{
public:

	static bool start();
	
	static bool write(const std::string& log_);
	
	static void stop();
#if XE_TARGET_PLATFORM == XE_PLATFORM_ANDROID 
	static int createDir(const   char   *sPathName);
#endif
	
private:
	
	XELog();
	~XELog();
	
	static void _write_thread();
	
	static bool							m_is_running;
	static std::mutex					m_mutex_;
	static fstream						m_log_stream;
	static std::list<std::string>		m_lst_logs;
};

#define LOG_BUFF_SIZE	1024
#define XE_LOG(format, ...) {char sz_buff[LOG_BUFF_SIZE] = {}; \
								SAFE_FORMATE(sz_buff, LOG_BUFF_SIZE, format, ##__VA_ARGS__);\
								XELog::write(sz_buff);}
	
	
NS_XE_END
	
	
#endif /*_AE_LOG_H_*/