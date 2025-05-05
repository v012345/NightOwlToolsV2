/************************************************************************** 
    *  @Copyright (c) 2015, A-one Soft, All rights reserved. 
 
    *  @file			: AEPlatformConfig.h 
    *  @version	: ver 1.0.0 
 
    *  @author	: andy-zhao 
    *  @date		: 2015/11/28 
    *  @brief		: 平台配置 
**************************************************************************/
#ifndef _AEPlatformConfig_H_
#define _AEPlatformConfig_H_

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

/**
	定义平台
*/
#define AE_PLATFORM_UNKNOWN	0
#define AE_PLATFORM_IOS 1
#define AE_PLATFORM_ANDROID 2
#define AE_PLATFORM_WIN32 3

#if CC_TARGET_PLATFORM == AE_PLATFORM_IOS
#define AE_TARGET_PLATFORM AE_PLATFORM_IOS
#elif CC_TARGET_PLATFORM == AE_PLATFORM_ANDROID
#define AE_TARGET_PLATFORM AE_PLATFORM_ANDROID
#elif CC_TARGET_PLATFORM == AE_PLATFORM_WIN32
#define AE_TARGET_PLATFORM AE_PLATFORM_WIN32
#endif

/**
	字节序
*/
#define AE_BYTE_BUFFER_JAVA 1
#define AE_BYTE_BUFFER_C_PLUS_PLUS 2

#define AE_BYTE_BUFFER AE_BYTE_BUFFER_C_PLUS_PLUS

#endif // _AEPlatformConfig_H_