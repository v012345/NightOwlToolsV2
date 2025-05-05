#include "external/collision/AECollision.h"
#include "external/collision/AECollider.h"

NS_AE_BEGIN

AECollision::AECollision()
{
	b2Vec2 gravity;
	gravity.Set(0.0f, 0.0f);
	m_pB2World = new b2World(gravity);

	m_pB2World->SetContactFilter(&m_pFilter);
	m_pB2World->SetContactListener(this);
	m_pB2World->SetDebugDraw(&m_pDebugDraw);
	m_pDebugDraw.SetFlags(true);

	m_pWorldManifold = new b2WorldManifold();

	for (int32 i = 0; i < MaxColliderCount; i++)
	{
		AECollider* pCollider = new AECollider(this);
		m_pVectorFreeCollider.push_back(pCollider);
	}
}

AECollision::~AECollision()
{
	for (int32 i = 0; i < m_pVectorFreeCollider.size(); i++)
	{
		AESafe_delete(m_pVectorFreeCollider[i]);
	}

	AESafe_delete(m_pB2World);
	AESafe_delete(m_pWorldManifold);
}

void AECollision::release()
{
	delete this;
}

AECollision* AECollision::getInstance()
{
	return AECollision::instance();
}

void AECollision::init()
{

}

void AECollision::purge()
{

}

void AECollision::update(float delta)
{
	if (nullptr != m_pB2World)
	{
		m_pB2World->Step(delta, 8, 3);

		std::list<AECollider*>::iterator iter = m_pListCollider.begin();
		for (; iter != m_pListCollider.end(); ++iter)
		{
			(*iter)->update(delta);
		}
	}
}

void AECollision::insertCollider(AECollider* collider)
{
	m_pListCollider.push_back(collider);
}

void AECollision::eraseCollider(AECollider* collider)
{
	std::list<AECollider*>::iterator iter = m_pListCollider.begin();
	for (; iter != m_pListCollider.end(); ++iter)
	{
		if ((*iter)->getTag() == collider->getTag())
		{
			m_pListCollider.erase(iter);
			break;
		}
	}
}

void AECollision::BeginContact(b2Contact* contact)
{	
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();

	AECollider* pColliderA = static_cast<AECollider*>(bodyA->GetUserData());
	AECollider* pColliderB = static_cast<AECollider*>(bodyB->GetUserData());

	if (nullptr != pColliderA && nullptr != pColliderB)
	{	
		pColliderA->insertCollider(pColliderB);
		pColliderB->insertCollider(pColliderA);
	}
}

void AECollision::EndContact(b2Contact* contact)
{
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();

	AECollider* pColliderA = static_cast<AECollider*>(bodyA->GetUserData());
	AECollider* pColliderB = static_cast<AECollider*>(bodyB->GetUserData());

	if (nullptr != pColliderA && nullptr != pColliderB)
	{
		pColliderA->eraseCollider(pColliderB);
		pColliderB->eraseCollider(pColliderA);
	}
}

AECollider* AECollision::getFreeCollider()
{
	return m_pVectorFreeCollider.empty() ? nullptr : m_pVectorFreeCollider.back();
}

void AECollision::pushFreeCollider(AECollider* pCollider)
{
	m_pVectorFreeCollider.push_back(pCollider);
}

void AECollision::popFreeCollider()
{
	m_pVectorFreeCollider.pop_back();
}

NS_AE_END
