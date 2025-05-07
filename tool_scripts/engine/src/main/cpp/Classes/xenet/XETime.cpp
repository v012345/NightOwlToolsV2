#include "xenet/XEPlatformConfig.h"
#include "xenet/XETime.h"
#include "common/AEUtil.h"
#include "xenet/XEInclude.h"
#include "time.h"
	
NS_XE_BEGIN
	
/* get clock in millisecond 64 */
int64 iclock64(void)
{
	long s, u;
	int64 value;
	ae::AEUtil::itimeofday(&s, &u);
	value = ((int64)s) * 1000 + (u / 1000);
	return value;
}

uint32 iclock()
{
	return (uint32)(iclock64() & 0xfffffffful);
}

/* sleep in millisecond */
void isleep(unsigned long millisecond)
{
#if XE_TARGET_PLATFORM != XE_PLATFORM_WIN32
	usleep((millisecond << 10) - (millisecond << 4) - (millisecond << 3));
#else
	Sleep(millisecond);
#endif
}
	
NS_XE_END
