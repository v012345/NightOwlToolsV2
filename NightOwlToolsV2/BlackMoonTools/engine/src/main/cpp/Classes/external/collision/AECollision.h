/************************************************************************** 
    *  @Copyright (c) 2017, A-one Soft, All rights reserved. 
 
    *  @file			: AECollision.h 
    *  @version	: ver 1.0.0 
 
    *  @author	: andy-zhao 
    *  @date		: 2017/3/30 
    *  @brief		: 碰撞管理 
**************************************************************************/
#ifndef _AECollision_H_
#define _AECollision_H_

#include "platform/AEStdC.h"
#include "common/AESingleton.h"
#include <Box2D/Box2D.h>
#include "external/collision/AEColliderFilter.h"
#include "external/collision/AEColliderDraw.h"

NS_AE_BEGIN

class AECollider;
class AECollision : public b2ContactListener, public AESingleton<AECollision>
{
friend class AECollider;
public:
	AECollision();
	virtual ~AECollision();

	/**
		释放。
	*/
	void release();

	/**
		单例。
	*/
	static AECollision* getInstance();

	/**
		初始化。
	*/
	virtual void init();

	/**
		释放。
	*/
	virtual void purge();

	/**
		碰撞刷新。
	*/
	void update(float delta);

	/**
		添加碰撞体。
	*/
	void insertCollider(AECollider* collider);

	/**
		移除碰撞体。
	*/
	void eraseCollider(AECollider* collider);

	/**
		碰撞回调。
	*/
	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
		B2_NOT_USED(contact);
		B2_NOT_USED(oldManifold);
	}
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{
		B2_NOT_USED(contact);
		B2_NOT_USED(impulse);
	}

	// Box2d
	b2World* getB2World() { return m_pB2World; }

protected:
	// Get Free Collider
	AECollider* getFreeCollider();

	// Push Free Collider
	void pushFreeCollider(AECollider* pCollider);

	// Pop Free Collider
	void popFreeCollider();

	// Max static collider count
	static const uint32 MaxColliderCount = 64;

private:
	// Filter
	AEColliderFilter m_pFilter;
	// Debug Render
	AEColliderDraw m_pDebugDraw;
	// Box2d
	b2World* m_pB2World;
	// Box2d Manifold
	b2WorldManifold* m_pWorldManifold;
	// AECollider
	std::list<AECollider*> m_pListCollider;
	// Free AECollider
	std::vector<AECollider*> m_pVectorFreeCollider;

};

NS_AE_END

#endif // _AECollision_H_