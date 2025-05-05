#ifndef _CPLOG_
#define _CPLOG_

#ifdef __cplusplus
extern "C" {
#endif

	#include "zf_log.h"

	#define CP_LOG_VERBOSE 1
	#define CP_LOG_DEBUG   2
	#define CP_LOG_INFO    3
	#define CP_LOG_WARN    4
	#define CP_LOG_ERROR   5
	#define CP_LOG_FATAL   6
	#define CP_LOG_NONE    0xFF

	#define CP_LOGE ZF_LOGE
	#define CP_LOGW ZF_LOGW
	#define CP_LOGI ZF_LOGI
	#define CP_LOGV ZF_LOGV
	#define CP_LOGD ZF_LOGD

	typedef void(*CPLOG_OUTPUT_FUN)(int lvl, const char* msg);

	void cplog_init();
    void cplog_close_nslog(int isClose);
	void cplog_uninit();
	void cplog_report_exception(const char* type, const char* msg);
	void cplog_custom_output(CPLOG_OUTPUT_FUN cb);

#if defined(ANDROID)
	#define ZF_LOG_USE_ANDROID_LOG
#endif //(ANDROID)


#ifdef __cplusplus
};
#endif

#endif
