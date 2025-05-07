#ifndef _AEVibrator_H_
#define _AEVibrator_H_

#include "platform/AEStdC.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include <android/log.h>
#include "platform/android/jni/JniHelper.h"

static const std::string helperClassName = "org/cocos2dx/lib/Cocos2dxHelper";

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//#import "iosVibrator.h"

#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

NS_AE_BEGIN

class AEVibrator
{
public: AEVibrator();
public: virtual ~AEVibrator();

public: static void vibrateOnce();
public: static void vibrate(int time);
public: static void cancelVibrate();

public: static void vibrateWithCadence(int interval, int duration, bool immediate, bool repeat);
public: static void vibrateWithPattern(int pattern[], int size, bool repeat);
};

NS_AE_END

#endif // !_AEVibrator_H_