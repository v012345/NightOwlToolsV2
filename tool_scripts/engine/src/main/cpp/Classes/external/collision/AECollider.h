/************************************************************************** 
    *  @Copyright (c) 2017, A-one Soft, All rights reserved. 
 
    *  @file			: AECollider.h 
    *  @version	: ver 1.0.0 
 
    *  @author	: andy-zhao 
    *  @date		: 2017/3/30 
    *  @brief		: 碰撞体 
**************************************************************************/
#ifndef _AECollider_H_
#define _AECollider_H_

#include "platform/AEStdC.h"
#include "external/collision/AECollision.h"
#include "math/AEGeometry.h"
#include <Box2D/Box2D.h>

NS_AE_BEGIN

class AECollider;
class AEColliderData : public Ref
{
public:
	AEColliderData();
	AEColliderData(AECollider* collider, AECollider* otherCollider);
	virtual ~AEColliderData();

	/**
		碰撞体。
	*/
	AECollider* getCollider() { return m_pOtherCollider; }

	/**
		接触点。
	*/
	int32 getContactPointX();
	int32 getContactPointY();

private:
	// AECollider
	AECollider* m_pCollider;
	// AECollider Other
	AECollider* m_pOtherCollider;

};

typedef enum _CollisionEvent
{
	eCollisionBegan = 0,
	eCollisionContinue = 1,
	eCollisionEnd = 2,

} AECollisionEvent;

typedef std::function<void(AECollisionEvent event, AEColliderData* colliderData)> AECollisionListener;

class AESpine;
class AECollider
{
public:
	friend AECollision;
	AECollider();
	AECollider(AECollision* collision);
	AECollider(AECollision* collision, uint32 type, int32 tag, uint16 category, uint16 mask);
	virtual ~AECollider();

	/**
		释放。
	*/
	void release();

	/**
		创建。
	*/
	static AECollider* create(AECollision* collision, uint32 type, int32 tag, uint16 category, uint16 mask);

	/**
		刷新。
	*/
	void update(float32 delta);

	/**
		更新。
	*/
	void updateVertices(AESpine* spine, float32 delta);

	void updateVerticesSync(string spineKey, string spineAnimationName, float32 time, float32 x, float32 y, float32 scaleX, float32 scaleY);

	/**
		设置碰撞监听。
	*/
	void setCollisionListener(AECollisionListener listener);

	/**
		碰撞回调。
	*/
	void onCollision(AECollisionEvent event, AEColliderData* colliderData);

	/**
		添加碰撞体。
	*/
	void insertCollider(AECollider* collider);

	/**
		移除碰撞体。
	*/
	void eraseCollider(AECollider* collider);

	/**
		获取碰撞体外框。
	*/
	AERect<int32> getBoundingBox();

	/**
		设置b2Fixture。
	*/
	void setFixture(uint16 category, uint16 mask);

	/**
		碰撞体类型。
	*/
	AE_SYNTHESIZE_REF(uint32, m_iType, Type);

	/**
		碰撞体tag。
	*/
	AE_SYNTHESIZE_REF(int32, m_iTag, Tag);

protected:
	/**
		碰撞体坐标。
	*/
	void setPosition(float32 x, float32 y);

private:
	// AECollision
	AECollision* m_pCollision;
	// Listener
	AECollisionListener m_pCollisionListener;
	// b2Body
	b2Body* m_pB2Body;
	// b2Fixture
	b2Fixture* m_pFixture;
	// Position
	b2Vec2 m_pPosition;
	// Vertices
	b2Vec2 m_pVertices[b2_maxPolygonVertices];
	// Vertices Count
	uint32 m_iVerticesCount;

	// 碰撞开始数据集合
	std::vector<AEColliderData*> m_pCollideBeganDataList;
	// 碰撞中数据集合
	std::vector<AEColliderData*> m_pCollideContinueDataList;
	// 碰撞结束数据集合
	std::vector<AEColliderData*> m_pCollideEndDataList;
};

NS_AE_END

#endif // _AECollider_H_