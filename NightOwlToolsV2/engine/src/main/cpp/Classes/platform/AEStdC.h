/**************************************************************************
*  @Copyright (c) 2015, A-one Soft, All rights reserved.

*  @file			: AEPlatformConfig.h
*  @version	: ver 1.0.0

*  @author	: andy-zhao
*  @date		: 2015/11/28
*  @brief		: »´∆ΩÃ®≈‰÷√
**************************************************************************/
#ifndef _AEStdC_H_
#define _AEStdC_H_

#include "platform/AEPlatformMacros.h"
#if (AE_TARGET_PLATFORM == AE_PLATFORM_WIN32)
#include "platform/win32/AEStdC-win32.h"
#endif
#include "json_inc.h"
#include "md5_inc.h"

#endif // _AEStdC_H_