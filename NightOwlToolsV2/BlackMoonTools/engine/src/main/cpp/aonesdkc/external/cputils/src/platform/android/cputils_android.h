#ifndef __CPUTILS_ANDROID_H__
#define __CPUTILS_ANDROID_H__

#ifdef __cplusplus
extern "C" {
#endif

	#include <jni.h>
	JavaVM* cputils_get_jvm();

#ifdef __cplusplus
};
#endif

#endif