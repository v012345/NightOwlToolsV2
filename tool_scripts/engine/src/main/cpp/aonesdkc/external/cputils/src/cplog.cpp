#include "cplog.h"
#include "cpfile.h"
#include <cstdio>
#include <cstring>
#include "platform/CCPlatformDefine.h"
#include "platform/CCFileUtils.h"
#include "cJSON.h"
#include <string>

#ifndef DISABLE_LOG_FILE
#include <thread>
#include <functional>
#include <mutex>
#include <chrono>
#include <list>
#endif

using namespace std;

const char* logDir = "logDir";

static zf_log_output _old_output;
static bool _init = false;
static int _log_lvl = CP_LOG_VERBOSE;
static std::string _log_dir = "";
static int _log_keep_days = 7;
static int _log_max_line = 65535;
static std::string _bugly_id = "";
static bool _bugly_debug = false;
static bool _write_to_file = true;
static CPLOG_OUTPUT_FUN _cplog_custom_cb = NULL;

extern "C"
{
    extern float GetSystemVersion();
}

static void _init_config()
{
	#ifdef IOS
    //under ios 6.0, json parser may load __exp10__ fail, skip it, use default setting
    if (GetSystemVersion() < 7.0) {
        return;
    }
	#endif
    
	char* d = (char*)cpfile_readstr("cplog_config.json");

	cJSON* root = cJSON_Parse(d);
	if (root != NULL)
	{
		cJSON* item = cJSON_GetObjectItem(root, "log_dir");
		if (item != NULL)
		{
			_log_dir = item->valuestring;
		}

		item = cJSON_GetObjectItem(root, "write_to_file");
		if (item != NULL)
		{
			_write_to_file = item->valueint;
		}

		item = cJSON_GetObjectItem(root, "log_keep_days");
		if (item != NULL)
		{
			_log_keep_days= item->valueint;
		}

		item = cJSON_GetObjectItem(root, "log_max_line");
		if (item != NULL)
		{
			_log_max_line = item->valueint;
		}

		item = cJSON_GetObjectItem(root, "bugly_id");
		if (item != NULL)
		{
			_bugly_id = item->valuestring;
		}

		item = cJSON_GetObjectItem(root, "bugly_debug");
		if (item != NULL)
		{
			_bugly_debug = item->valueint;
		}

		cJSON_Delete(root);
	}
}

#ifndef DISABLE_LOG_FILE
static bool logExit = false;
class LogFileController
{
public:
	LogFileController()
	{
		_log_exit = false;
		_log_line = 0;
	}

	~LogFileController()
	{
		cplog_uninit();
	}

	void start_loop()
	{
		create_log_file();
		_log_thread = thread(bind(&LogFileController::loop, this));
#ifdef WIN32
		//calling join after main in vs would cause deadlock
		//https://connect.microsoft.com/VisualStudio/feedback/details/747145
		_log_thread.detach();
#endif
	}

	void end_loop()
	{
		ZF_LOGI("end log file");
		{
			lock_guard<mutex> lock(_log_mutex);
			_log_exit = true;
		}

		if (_log_thread.joinable())
		{
			_log_thread.join();
		}
	}

	void add_msg(string msg)
	{
		{
			lock_guard<mutex> lock(_log_mutex);
			_log_buf.push_back(msg);
		}
	}

private:
	void create_log_file()
	{
		ZF_LOGI("create log file");
		time_t timer;
		time(&timer);
		tm* t_tm = localtime(&timer);

		char currentTime[50];
		sprintf(currentTime, "%d-%d-%d-%d-%d-%d", t_tm->tm_year + 1900, t_tm->tm_mon + 1, t_tm->tm_mday,
			t_tm->tm_hour, t_tm->tm_min, t_tm->tm_sec);


		if (_log_dir != "")
		{
			std::string logDirPath = cpfile_external_path() + _log_dir + "/" + logDir;
			cputils::FileUtils::getInstance()->createDirectory(logDirPath);
			std::string logPath = logDirPath + "/cplog" + currentTime + ".txt";
			_log_file = fopen(logPath.c_str(), "w");
			ZF_LOGI("log path is %s", logPath.c_str());
		}
		else
		{
			char logDirPath[1024];
			sprintf(logDirPath, "%s%s", cpfile_external_path(), logDir);
			cputils::FileUtils::getInstance()->createDirectory(logDirPath);
			char logPath[1024];
			sprintf(logPath, "%s/cplog%s.txt", logDirPath, currentTime);
			_log_file = fopen(logPath, "w");

			ZF_LOGI("log path is %s", logPath);
		}

		if (_log_file == NULL)
		{
			ZF_LOGE("create log file fail");
		}
	}

	void loop()
	{
		try
		{
			while (true)
			{
				{
					lock_guard<mutex> lock(_log_mutex);
					if (_log_exit) 
						break;
				}

				consume_msg();
				this_thread::sleep_for(chrono::seconds(1));
			}
		}
		catch (exception& e)
		{
			ZF_LOGI("occur exception %s", e.what());
		}

		printf("already exit\n");
	}

	void consume_msg()
	{
		list<string> _buf;
		{
			lock_guard<mutex> lock(_log_mutex);
			_buf.swap(_log_buf);
		}

		if (_log_file != NULL)
		{
			for (auto& msg : _buf)
			{
				if (_log_line >= _log_max_line)
				{
					fclose(_log_file);
					_log_line = 0;
					create_log_file();	
				}

				fprintf(_log_file, "%s\n", msg.c_str());
				_log_line++;
			}
			fflush(_log_file);
		}
	}

	FILE* _log_file = NULL;
	thread _log_thread;
	bool _log_exit;
	mutex _log_mutex;
	list<string> _log_buf;
	int _log_line;
};

static LogFileController _logFileController;
#endif

#ifdef USE_BUGLY
#include "CrashReport.h"
static CrashReport::CRLogLevel mapCRLvl[] =
{
	CrashReport::CRLogLevel::Verbose,
	CrashReport::CRLogLevel::Debug,
	CrashReport::CRLogLevel::Info,
	CrashReport::CRLogLevel::Warning,
	CrashReport::CRLogLevel::Error,
	CrashReport::CRLogLevel::Error,
	CrashReport::CRLogLevel::Error
};

bool isEnableBugly()
{
    if (_bugly_id == "")
    {
        return false;
    }
    
#ifdef IOS
    if (GetSystemVersion() < 7.0) {
        return false;
    }
#endif
    return true;
}

void init_3rd()
{
	if (isEnableBugly() == false)
	{
		return;
	}

    CrashReport::initCrashReport(_bugly_id.c_str(), _bugly_debug);
}

void log_3rd(const int lvl, const char* tag, const char* msg)
{
    if (isEnableBugly() == false)
    {
        return;
    }

	CrashReport::CRLogLevel logLvl = CrashReport::Error;
	if (lvl < sizeof(mapCRLvl))
	{
		logLvl = mapCRLvl[lvl];
	}
	CrashReport::log(logLvl, tag, msg);
}

extern "C"
{
	void cplog_report_exception(const char* type, const char* msg)
	{
        if (isEnableBugly() == false)
        {
            return;
        }

		CrashReport::reportException(5, type, msg, "");
	}
}
#else
void init_3rd() { ; }
void log_3rd(const int lvl, const char* tag, const char* msg) { ; }
extern "C"
{
	void cplog_report_exception(const char* type, const char* msg)
	{
		CP_LOGE("not config 3rd report module");
	}
}
#endif // USE_BUGLY

static void custom_output_callback(const zf_log_message *msg, void *arg)
{
	(void)arg;
	/* p points to the log message end. By default, message is not terminated
	* with 0, but it has some space allocated for EOL area, so there is always
	* some place for terminating zero in the end (see ZF_LOG_EOL_SZ define in
	* zf_log.c).
	*/
	
	*msg->p = 0;
#ifndef DISABLE_LOG_FILE
    if (logExit == false) {
        _logFileController.add_msg(msg->buf);
    }
#endif

	if (_cplog_custom_cb != NULL)
	{
		_cplog_custom_cb(msg->lvl, msg->buf);
	}
	log_3rd(msg->lvl, msg->tag, msg->msg_b);
	_old_output.callback(msg, _old_output.arg);
}

long long getNowTime()
{
	long long now_time;
	now_time = time(NULL);
	return now_time;
}

int cplog_delete_expiry_log_cache(const char* path, const struct cpfile_stat* s)
{
	if (s->file_status == CPFILE_STATUS_F && cpfile_exist(path))
	{
		long long last_write_time = s->last_write_time;
		long long now_time = getNowTime();

		if (now_time - last_write_time >= _log_keep_days * 24 * 3600) {
			bool ret = cpfile_remove(path);
			const char* log = (ret == true) ? "success" : "fail";
			ZF_LOGW("\ndelele expiry log cache, filename=[%s]\nresult=[%s]", path, log);
		}
	}
	return 0;
}

void cplog_delete_expiry_log(const char* path)
{
	cpfile_fws(path, cplog_delete_expiry_log_cache, CPFILE_WALK_RECURSION);
}

extern "C"
{
	void cplog_init()
	{
		if (_init == false)
		{
			_init = true;
			_init_config();
			init_3rd();

			if (_log_dir != "")
			{
				std::string dirPath = cpfile_external_path() + _log_dir + "/" + logDir;
				cputils::FileUtils::getInstance()->createDirectory(dirPath);
				cplog_delete_expiry_log(dirPath.c_str());
			}
			else
			{
				char logDirPath[1024];
				sprintf(logDirPath, "%s%s", cpfile_external_path(), logDir);
				cputils::FileUtils::getInstance()->createDirectory(logDirPath);
				cplog_delete_expiry_log(logDirPath);
			}		

			const unsigned put_mask = ZF_LOG_PUT_STD;
			_old_output = _zf_log_global_output;
			zf_log_set_output_v(put_mask, 0, custom_output_callback);

#ifndef DISABLE_LOG_FILE
			if (_write_to_file)
			{
				_logFileController.start_loop();
			}
#endif
		}
	}
    
	void cplog_uninit()
	{
#ifndef DISABLE_LOG_FILE
		_logFileController.end_loop();
        logExit = true;
#endif
	}
}

void cplog_custom_output(CPLOG_OUTPUT_FUN cb)
{
	_cplog_custom_cb = cb;
}

void cplog_close_nslog(int isClose){
}

