#include "AEVibrator.h"

NS_AE_BEGIN

AEVibrator::AEVibrator()
{
}

AEVibrator::~AEVibrator()
{
}

void AEVibrator::vibrateOnce()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	AEVibrator::vibrate(50);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	AEVibrator::vibrate(1);
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
}

void AEVibrator::vibrate(int time)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniHelper::callStaticVoidMethod(helperClassName, "vibrate", time);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//iosVibrator::vibrate(time);
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
}

void AEVibrator::cancelVibrate()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniHelper::callStaticVoidMethod(helperClassName, "cancelVibrate");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//iosVibrator::cancelVibrate();
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
}

void AEVibrator::vibrateWithCadence(int interval, int duration, bool immediate, bool repeat)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniHelper::callStaticVoidMethod(helperClassName, "vibrateWithPattern", interval, duration, immediate, repeat);
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
}

void AEVibrator::vibrateWithPattern(int pattern[], int size, bool repeat)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
}

NS_AE_END
