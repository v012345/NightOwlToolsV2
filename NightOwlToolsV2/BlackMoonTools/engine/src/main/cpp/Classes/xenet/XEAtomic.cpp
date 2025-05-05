#include "xenet/XEInclude.h"
#include "xenet/XEAtomic.h"
	
NS_XE_BEGIN
	
XEAtomic::XEAtomic(long lValue) 
		: m_atomic_(lValue)
{
}
	
XEAtomic::~XEAtomic()
{
}
	
// 
long XEAtomic::get_value()
{
#ifdef _MSC_VER
	return InterlockedExchangeAdd(&m_atomic_, 0);
#else
	return __sync_fetch_and_add(&m_atomic_, 0);
#endif
}
	
long XEAtomic::set_value(long lValue)
{
#ifdef _MSC_VER
	return InterlockedExchange(&m_atomic_, lValue);
#else
	return __sync_lock_test_and_set(&m_atomic_, lValue);
#endif
}

long XEAtomic::compare_and_set(long compare_, long exchange_)
{
#ifdef _MSC_VER
	return InterlockedCompareExchange(&m_atomic_, exchange_, compare_);
#else
	return  __sync_val_compare_and_swap(&m_atomic_, compare_, exchange_);
#endif
}
	
NS_XE_END