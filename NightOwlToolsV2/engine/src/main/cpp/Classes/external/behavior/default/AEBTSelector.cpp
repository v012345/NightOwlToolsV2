#include "external/behavior/default/AEBTSelector.h"
#include "external/behavior/default/AEBTCondition.h"

NS_AE_BEGIN

AEBTSelector::AEBTSelector()
: m_iSelectorIndex(-1)
{

}

AEBTSelector::~AEBTSelector()
{

}

AEBTSelector* AEBTSelector::create()
{
	return new AEBTSelector();
}

void AEBTSelector::update(float32 delta)
{
	if (AEBTNodeStatus::RUNNING != m_eState) return;

	for (AEBTNodeVector::iterator iter = m_pCondition.begin(); iter != m_pCondition.end(); ++iter)
	{
		AEBTCondition* condition = dynamic_cast<AEBTCondition*>(*iter);
		if (!condition->check())
		{
			AEBTNode* child = m_pChildren[m_iSelectorIndex];
			if (AEBTNodeStatus::RUNNING == child->getState())
				child->exit();
			m_eState = AEBTNodeStatus::SUCCESS;
			return;
		}
	}

	updateSelector(delta);
}

void AEBTSelector::updateSelector(float32 delta)
{
	AEBTNode* child = m_pChildren[m_iSelectorIndex];
	child->update(delta);

	if (AEBTNodeStatus::SUCCESS == child->getState())
	{
		child->exit();

		uint32 childCount = m_pChildren.size();
		uint32 length = m_iSelectorIndex + m_pChildren.size();
		for (uint32 index = m_iSelectorIndex + 1; index < length; ++index)
		{
			m_iSelectorIndex = index % childCount;
			AEBTNode* childBrother = dynamic_cast<AEBTNode*>(m_pChildren[m_iSelectorIndex]);
			childBrother->enter();

			if (AEBTNodeStatus::RUNNING != childBrother->getState())
				childBrother->exit();
			else
				return;
		}

		m_eState = AEBTNodeStatus::SUCCESS;
	}
}

void AEBTSelector::enter()
{
	if (!check())
	{
		m_eState = AEBTNodeStatus::FAILURE;
		return;
	}

	uint32 childCount = m_pChildren.size();
	for (uint32 index = 0; index < childCount; ++index)
	{
		AEBTNode* child = dynamic_cast<AEBTNode*>(m_pChildren[index]);
		child->enter();

		if (AEBTNodeStatus::RUNNING == child->getState())
		{
			m_iSelectorIndex = index;
			m_eState = AEBTNodeStatus::RUNNING;
			return;
		}
		else
			child->exit();
	}

	m_eState = AEBTNodeStatus::FAILURE;
}

void AEBTSelector::exit()
{
	if (AEBTNodeStatus::RUNNING == m_eState)
	{
		AEBTNode* child = m_pChildren[m_iSelectorIndex];
		if (AEBTNodeStatus::RUNNING == child->getState())
			child->exit();
	}

	m_iSelectorIndex = -1;
	m_eState = AEBTNodeStatus::READIED;

	AEBTComposite::exit();
}

NS_AE_END
