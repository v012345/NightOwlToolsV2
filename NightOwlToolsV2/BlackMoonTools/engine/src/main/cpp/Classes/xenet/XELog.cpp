#include <thread>
//#include <direct.h>
#include "xenet/XETime.h"
#include "xenet/XELog.h"
#include <iostream>
//#include "cocos2d.h"
#include "xenet/XEStdC.h"	
#if XE_TARGET_PLATFORM == XE_PLATFORM_ANDROID 
#include "platform/android/AEJava_main_Utils.h"
#include <sys/stat.h>
#include <unistd.h>
#endif

NS_XE_BEGIN
	
bool						XELog::m_is_running = false;
std::mutex					XELog::m_mutex_;
fstream						XELog::m_log_stream;
std::list<std::string>		XELog::m_lst_logs;
	
XELog::XELog()
{
}
	
XELog::~XELog()
{
}
	
bool XELog::start()
{
	std::lock_guard<std::mutex> locker(m_mutex_);
	if (m_is_running)
		return false;
#define PATH_MAX_LEN 1024
	

	string log_file = "";//cocos2d::CCFileUtils::getInstance()->getWritablePath();
#if XE_TARGET_PLATFORM == XE_PLATFORM_WIN32 
	log_file = cocos2d::FileUtils::getInstance()->getWritablePath();
	log_file += "\\C++log.txt";
#endif


#if XE_TARGET_PLATFORM == XE_PLATFORM_ANDROID
	log_file = cocos2d::CCFileUtils::getInstance()->getWritablePath();
	log_file += "/C++log.txt";
#endif
#if XE_TARGET_PLATFORM == AE_PLATFORM_IOS
    log_file = cocos2d::FileUtils::getInstance()->getWritablePath();
    cocos2d::log("XELog log_file:%s", log_file.c_str());
    log_file += "/C++log.txt";
#endif

#if XE_TARGET_PLATFORM == XE_PLATFORM_IOS
	log_file = cocos2d::CCFileUtils::getInstance()->getWritablePath();
	log_file += "/C++log.txt";
#endif
	
	
	
	m_log_stream.open(log_file.c_str(), ios::out | ios::trunc);
	if (!m_log_stream.good())
	{
		return false;
	}
	
	m_is_running = true;

	std::thread thread_(std::bind(&XELog::_write_thread));
	thread_.detach();
	
	return true;
}
	
bool XELog::write(const std::string& log_)
{
	//目前log根本没有start 下面这句加锁会导致XESocket.cpp中的connect失败返回-1 errorno == 0 没找到有错误原因
	//std::lock_guard<std::mutex> locker(m_mutex_);

	//stringstream ss;
	//ss << std::this_thread::get_id();
	//string threadIdStr = ss.str();

	//std::string val = "[thread:" + threadIdStr +"]"+ log_;
	//
	//if (!m_is_running) return false;
	//
	//m_lst_logs.push_back(val);
	//
	return true;
}
	
void XELog::stop()
{
	std::lock_guard<std::mutex> locker(m_mutex_);
	if (!m_is_running) return;
	m_is_running = false;
}
	
void XELog::_write_thread()
{
	for (;;)
	{
		m_mutex_.lock();
		if (!m_is_running) {
			m_mutex_.unlock();
			break;
		}
		if (m_lst_logs.empty())
		{
			m_mutex_.unlock();
			isleep(20);
			continue;
		}
		
		std::string tmp_log = m_lst_logs.front();
		m_lst_logs.pop_front();
		
		char tmp2[32];

		time_t timep;
		time(&timep);
		strftime(tmp2, sizeof(tmp2), "[%Y-%m-%d %H:%M:%S]: ", localtime(&timep));
		std::string year_mom_day_hour_mim_sec = tmp2;
		
		//std::cout << tmp_log << std::endl;
		m_log_stream << year_mom_day_hour_mim_sec << tmp_log << endl;
		m_mutex_.unlock();
	}
	
	
}
	
#if XE_TARGET_PLATFORM == XE_PLATFORM_ANDROID 
int XELog::createDir(const   char   *sPathName)
{
	char   DirName[256];
	strcpy(DirName, sPathName);
	int   i, len = strlen(DirName);
	if (DirName[len - 1] != '/')
		strcat(DirName, "/");

	len = strlen(DirName);

	for (i = 1; i < len; i++)
	{
		if (DirName[i] == '/')
		{
			DirName[i] = 0;
			if (access(DirName, NULL) != 0)
			{
				if (mkdir(DirName, 0755) == -1)
				{
					perror("mkdir   error");
					return   -1;
				}
			}
			DirName[i] = '/';
		}
	}

	return   0;
}
	
#endif
NS_XE_END