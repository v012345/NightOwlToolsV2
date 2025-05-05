/************************************************************************** 
    *  @Copyright (c) 2017, A-one Soft, All rights reserved. 
 
    *  @file			: AEColliderFilter.h 
    *  @version	: ver 1.0.0 
 
    *  @author	: andy-zhao 
    *  @date		: 2017/3/30 
    *  @brief		: 碰撞体 
**************************************************************************/
#ifndef _AEColliderFilter_H_
#define _AEColliderFilter_H_

#include "platform/AEStdC.h"
#include <Box2D/Box2D.h>

NS_AE_BEGIN

class AEColliderFilter : public b2ContactFilter
{
public:
	AEColliderFilter() { }
	virtual ~AEColliderFilter() { }

	/*
	bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
	{
		return true;
	}
	*/
};

NS_AE_END

#endif // _AEColliderFilter_H_