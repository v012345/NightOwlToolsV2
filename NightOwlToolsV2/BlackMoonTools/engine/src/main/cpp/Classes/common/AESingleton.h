/************************************************************************** 
    *  @Copyright (c) 2015, A-one Soft, All rights reserved. 
 
    *  @file     	: AESingleton.h 
    *  @version	: ver 1.0.0 
 
    *  @author	: andy-zhao 
    *  @date		: 2015/06/25 
    *  @brief		:  
**************************************************************************/
#ifndef _AESingleton_H_
#define _AESingleton_H_

#include "platform/AEStdC.h"

NS_AE_BEGIN

template<class classT>
class AESingleton
{
public:
	static classT* instance() { static classT s_instance; return &s_instance; }
	virtual void init() = 0;
	virtual void purge() = 0;

protected:
	AESingleton() { }
	virtual ~AESingleton() { }

private:
	AESingleton(const AESingleton&);
	AESingleton& operator = (const AESingleton&);

};

NS_AE_END

#endif // _AESingleton_H_