#include "external/collision/AECollider.h"
#include "scripting/lua-bindings/manual/cocos2d/LuaScriptHandlerMgr.h"
#include "external/spine/AESpine.h"
#include "external/spine/AESpineColliderManager.h"

NS_AE_BEGIN

AEColliderData::AEColliderData()
: m_pCollider(nullptr)
, m_pOtherCollider(nullptr)
{

}

AEColliderData::AEColliderData(AECollider* collider, AECollider* otherCollider)
: m_pCollider(collider)
, m_pOtherCollider(otherCollider)
{
	
}

AEColliderData::~AEColliderData()
{

}

int32 AEColliderData::getContactPointX()
{
	AERect<int32> rect = AERect<int32>::intersect(m_pCollider->getBoundingBox(), m_pOtherCollider->getBoundingBox());
	return rect.getMidX();
}

int32 AEColliderData::getContactPointY()
{
	AERect<int32> rect = AERect<int32>::intersect(m_pCollider->getBoundingBox(), m_pOtherCollider->getBoundingBox());
	return rect.getMidY();
}

AECollider::AECollider()
: m_pCollision(nullptr)
, m_pB2Body(nullptr)
, m_iType(0)
, m_iTag(0)
, m_iVerticesCount(0)
{

}

AECollider::AECollider(AECollision* collision)
: m_pCollision(collision)
, m_pB2Body(nullptr)
, m_iType(0)
, m_iTag(0)
, m_iVerticesCount(0)
{
	b2BodyDef def;
	def.type = b2_dynamicBody;
	def.allowSleep = false;
	def.awake = false;
	m_pB2Body = m_pCollision->getB2World()->CreateBody(&def);

	b2PolygonShape shape;
	shape.SetAsBox(1.0f, 1.0f);
	m_pFixture = m_pB2Body->CreateFixture(&shape, 0);
	m_pFixture->SetSensor(true);

	b2Filter filter;
	filter.categoryBits = 0x0001;
	filter.maskBits = 0xFFFF;
	m_pFixture->SetFilterData(filter);

	m_pB2Body->SetUserData(this);
	m_pB2Body->SetActive(false);
}

AECollider::AECollider(AECollision* collision, uint32 type, int32 tag, uint16 category, uint16 mask)
: m_pCollision(collision)
, m_pB2Body(nullptr)
, m_iType(type)
, m_iTag(tag)
, m_iVerticesCount(0)
{
	b2BodyDef def;
	def.type = b2_dynamicBody;
	def.allowSleep = false;
	def.awake = false;
	m_pB2Body = m_pCollision->getB2World()->CreateBody(&def);

	b2PolygonShape shape;
	shape.SetAsBox(1.0f, 1.0f);
	m_pFixture = m_pB2Body->CreateFixture(&shape, 0);
	m_pFixture->SetSensor(true);

	b2Filter filter;
	filter.categoryBits = category;
	filter.maskBits = mask;
	m_pFixture->SetFilterData(filter);

	m_pB2Body->SetUserData(this);
}

AECollider::~AECollider()
{
	m_pCollision->getB2World()->DestroyBody(m_pB2Body);
}

void AECollider::release()
{
	m_pCollideBeganDataList.clear();
	m_pCollideContinueDataList.clear();
	m_pCollideEndDataList.clear();

	m_pCollision->pushFreeCollider(this);
	m_pCollision->eraseCollider(this);
	m_pB2Body->SetActive(false);
}

AECollider* AECollider::create(AECollision* collision, uint32 type, int32 tag, uint16 category, uint16 mask)
{
	AECollider* pCollider = collision->getFreeCollider();
	if (nullptr != pCollider)
	{
		pCollider->setType(type);
		pCollider->setTag(tag);
		pCollider->setFixture(category, mask);

		collision->insertCollider(pCollider);
		collision->popFreeCollider();
	}
	else
	{
		pCollider = new AECollider(collision, type, tag, category, mask);
		collision->insertCollider(pCollider);
	}
	return pCollider;
}

void AECollider::update(float32 delta)
{
	std::vector<AEColliderData*>::iterator iter = m_pCollideBeganDataList.begin();
	for (; iter != m_pCollideBeganDataList.end(); ++iter)
	{
		onCollision(eCollisionBegan, (*iter));
	}

	iter = m_pCollideContinueDataList.begin();
	for (; iter != m_pCollideContinueDataList.end(); ++iter)
	{
		onCollision(eCollisionContinue, (*iter));
	}

	iter = m_pCollideEndDataList.begin();
	for (; iter != m_pCollideEndDataList.end(); ++iter)
	{
		onCollision(eCollisionEnd, (*iter));
		AESafe_delete((*iter));
	}

	m_pCollideBeganDataList.clear();
	m_pCollideEndDataList.clear();
}

void AECollider::updateVerticesSync(string spineKey, string spineAnimationName, float32 time, float32 x, float32 y, float32 scaleX, float32 scaleY)
{
	float32 vertices[b2_maxPolygonVertices << 1];
	memset(vertices, 0, b2_maxPolygonVertices << 3);
	AESpineColliderManager::getInstance()->getVertices(spineKey, spineAnimationName, time, &m_iVerticesCount, vertices);
	m_iVerticesCount = b2_maxPolygonVertices;
	
	m_iVerticesCount = m_iVerticesCount >> 1;

	for (uint32 i = 0; i < m_iVerticesCount; i++)
	{
		uint32 n = i << 1;		
		m_pVertices[i] = b2Vec2((int32)(vertices[n] * scaleX), (int32)(vertices[n + 1] * scaleY));
	}

	setPosition(x, y);

	auto shape = dynamic_cast<b2PolygonShape*>(m_pFixture->GetShape());
	bool ret = shape->Set(m_pVertices, m_iVerticesCount);
	m_pB2Body->SetActive(ret);
}

void AECollider::updateVertices(AESpine* spine, float32 delta)
{	
	float32 vertices[b2_maxPolygonVertices << 1];
	memset(vertices, 0, b2_maxPolygonVertices << 3);

	m_iVerticesCount = b2_maxPolygonVertices;
	spine->getVertices(vertices, &m_iVerticesCount);
	m_iVerticesCount = m_iVerticesCount >> 1;

	for (uint32 i = 0; i < m_iVerticesCount; i++)
	{
		uint32 n = i << 1;
		m_pVertices[i] = b2Vec2((int32)vertices[n], (int32)vertices[n + 1]);
	}

	Vec2 pos = spine->getPosition();
	setPosition(pos.x, pos.y);

	auto shape = dynamic_cast<b2PolygonShape*>(m_pFixture->GetShape());
	bool ret = shape->Set(m_pVertices, m_iVerticesCount);
	m_pB2Body->SetActive(ret);
}

void AECollider::setCollisionListener(AECollisionListener listener)
{
	m_pCollisionListener = listener;
}

void AECollider::onCollision(AECollisionEvent event, AEColliderData* colliderData)
{
	if (nullptr != m_pCollisionListener)
	{
		m_pCollisionListener(event, colliderData);
	}
}

void AECollider::insertCollider(AECollider* collider)
{
	AEColliderData* colliderData = new AEColliderData(this, collider);
	m_pCollideBeganDataList.push_back(colliderData);
	m_pCollideContinueDataList.push_back(colliderData);

	std::sort(m_pCollideBeganDataList.begin(), m_pCollideBeganDataList.end(), [](AEColliderData* colliderA, AEColliderData* colliderB) -> bool {
		return colliderA->getCollider()->getTag() < colliderB->getCollider()->getTag();
	});

	std::sort(m_pCollideContinueDataList.begin(), m_pCollideContinueDataList.end(), [](AEColliderData* colliderA, AEColliderData* colliderB) -> bool {
		return colliderA->getCollider()->getTag() < colliderB->getCollider()->getTag();
	});
}

void AECollider::eraseCollider(AECollider* collider)
{
	std::vector<AEColliderData*>::iterator iter = m_pCollideContinueDataList.begin();
	for (; iter != m_pCollideContinueDataList.end(); ++iter)
	{
		if ((*iter)->getCollider()->getTag() == collider->getTag())
		{
			m_pCollideEndDataList.push_back(*iter);
			m_pCollideContinueDataList.erase(iter);

			break;
		}
	}

	std::sort(m_pCollideEndDataList.begin(), m_pCollideEndDataList.end(), [](AEColliderData* colliderA, AEColliderData* colliderB) -> bool {
		return colliderA->getCollider()->getTag() < colliderB->getCollider()->getTag();
	});
}

AERect<int32> AECollider::getBoundingBox()
{
	int32 minX = 0, minY = 0, maxX = 0, maxY = 0;
	for (uint32 i = 0; i < m_iVerticesCount; i += 1) 
	{
		int32 x = m_pVertices[i].x, y = m_pVertices[i].y;
		minX = min(minX, x);
		minY = min(minY, y);
		maxX = max(maxX, x);
		maxY = max(maxY, y);
	}
	return AERect<int32>(m_pPosition.x + minX, m_pPosition.y + minY, maxX - minX, maxY - minY);
}

void AECollider::setFixture(uint16 category, uint16 mask)
{
	b2Filter filter;
	filter.categoryBits = category;
	filter.maskBits = mask;
	m_pFixture->SetFilterData(filter);
}

void AECollider::setPosition(float32 x, float32 y)
{
	m_pPosition = b2Vec2((int32)x, (int32)y);
	m_pB2Body->SetTransform(m_pPosition, 0);
}

NS_AE_END
