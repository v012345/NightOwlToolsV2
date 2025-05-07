/************************************************************************** 
    *  @Copyright (c) 2015, A-one Soft, All rights reserved. 
 
    *  @file			: AEPlatformConfig.h 
    *  @version	: ver 1.0.0 
 
    *  @author	: andy-zhao 
    *  @date		: 2015/11/28 
    *  @brief		: 平台配置 
**************************************************************************/
#ifndef _XEPlatformConfig_H_
#define _XEPlatformConfig_H_
#include "platform/AEPlatformConfig.h"
//#include "cocos2d.h"

//USING_NS_CC;
using namespace std;



/**
	定义平台
*/
#define XE_PLATFORM_UNKNOWN	0
#define XE_PLATFORM_IOS 1
#define XE_PLATFORM_ANDROID 2
#define XE_PLATFORM_WIN32 3



#if AE_TARGET_PLATFORM == XE_PLATFORM_IOS
#define XE_TARGET_PLATFORM XE_PLATFORM_IOS
#elif AE_TARGET_PLATFORM == XE_PLATFORM_ANDROID
#define XE_TARGET_PLATFORM XE_PLATFORM_ANDROID
#elif AE_TARGET_PLATFORM == XE_PLATFORM_WIN32
#define XE_TARGET_PLATFORM XE_PLATFORM_WIN32
#endif

/**
	字节序
*/
#define XE_BYTE_BUFFER_JAVA 1
#define XE_BYTE_BUFFER_C_PLUS_PLUS 2

#define XE_BYTE_BUFFER XE_BYTE_BUFFER_C_PLUS_PLUS


#endif // _XEPlatformConfig_H_