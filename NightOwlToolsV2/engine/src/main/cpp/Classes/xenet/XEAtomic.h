#ifndef _AEATOMIC_H_
#define _AEATOMIC_H_
	
#include "xenet/XEStdC.h"
#include "XEPlatformDefine.h"
	
NS_XE_BEGIN
	
class XEAtomic
{
public:
	XEAtomic(long lValue);
	~XEAtomic();
	
	// 
	long get_value();
	
	long set_value(long lValue);
	
	long compare_and_set(long compare_, long exchange_);
	
private:
	
	atomic_t m_atomic_;
};
	
NS_XE_END
	
#endif /*_AEATOMIC_H_*/