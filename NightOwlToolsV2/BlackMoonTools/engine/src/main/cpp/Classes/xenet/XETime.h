#ifndef _AETIME_H_
#define _AETIME_H_
	
#include "xenet/XEPlatformDefine.h"
	
NS_XE_BEGIN
	
/* get clock in millisecond 64 */
int64 iclock64(void);
uint32 iclock();
	
/* sleep in millisecond */
void isleep(unsigned long millisecond);
	
NS_XE_END
	
#endif // _AETIME_H_
