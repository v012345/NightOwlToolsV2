#include "platform/AEStdC.h"
#if AE_TARGET_PLATFORM == AE_PLATFORM_ANDROID

#ifndef __AEJava_main_Utils_H__
#define __AEJava_main_Utils_H__

#include <string>

std::string getAndroidSdcardPath();

#endif

#endif