#include "external/behavior/default/AEBTParallel.h"
#include "external/behavior/default/AEBTCondition.h"

NS_AE_BEGIN

AEBTParallel::AEBTParallel()
{

}

AEBTParallel::~AEBTParallel()
{
	m_pParallelIndex.clear();
}

AEBTParallel* AEBTParallel::create()
{
	return new AEBTParallel();
}

void AEBTParallel::update(float32 delta)
{
	if (AEBTNodeStatus::RUNNING != m_eState) return;

	for (AEBTNodeVector::iterator iter = m_pCondition.begin(); iter != m_pCondition.end(); ++iter)
	{
		AEBTCondition* condition = dynamic_cast<AEBTCondition*>(*iter);
		if (!condition->check())
		{
			for (std::list<uint32>::iterator iter = m_pParallelIndex.begin(); iter != m_pParallelIndex.end(); ++iter)
			{
				AEBTNode* child = m_pChildren[*iter];
				if (AEBTNodeStatus::RUNNING == child->getState())
					child->exit();
			}
			m_eState = AEBTNodeStatus::SUCCESS;
			return;
		}
	}

	updateParallel(delta);
}

void AEBTParallel::updateParallel(float32 delta)
{
	m_eState = AEBTNodeStatus::SUCCESS;

	for (std::list<uint32>::iterator iter = m_pParallelIndex.begin(); iter != m_pParallelIndex.end();)
	{
		AEBTNode* child = m_pChildren[*iter];
		child->update(delta);

		if (AEBTNodeStatus::RUNNING == child->getState())
		{
			m_eState = AEBTNodeStatus::RUNNING;
			++iter;
		}
		else
		{
			child->exit();
			iter = m_pParallelIndex.erase(iter);
		}
	}
}

void AEBTParallel::enter()
{
	if (!check())
	{
		m_eState = AEBTNodeStatus::FAILURE;
		return;
	}

	uint32 childCount = m_pChildren.size();
	for (uint32 index = 0; index < childCount; ++index)
	{
		AEBTNode* child = m_pChildren[index];
		child->enter();

		if (AEBTNodeStatus::RUNNING == child->getState())
			m_pParallelIndex.push_back(index);
		else
			child->exit();
	}

	if (m_pParallelIndex.size() > 0)
		m_eState = AEBTNodeStatus::RUNNING;
	else
		m_eState = AEBTNodeStatus::FAILURE;
}

void AEBTParallel::exit()
{
	if (AEBTNodeStatus::RUNNING == m_eState)
	{
		for (std::list<uint32>::iterator iter = m_pParallelIndex.begin(); iter != m_pParallelIndex.end(); ++iter)
		{
			AEBTNode* child = m_pChildren[*iter];
			if (AEBTNodeStatus::RUNNING == child->getState())
				child->exit();
		}
	}

	m_pParallelIndex.clear();
	m_eState = AEBTNodeStatus::READIED;

	AEBTComposite::exit();
}

NS_AE_END
