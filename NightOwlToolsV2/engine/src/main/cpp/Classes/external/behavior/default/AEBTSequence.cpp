#include "external/behavior/default/AEBTSequence.h"
#include "external/behavior/default/AEBTCondition.h"

NS_AE_BEGIN

AEBTSequence::AEBTSequence()
: m_iSequenceIndex(-1)
{

}

AEBTSequence::~AEBTSequence()
{

}

AEBTSequence* AEBTSequence::create()
{
	return new AEBTSequence();
}

void AEBTSequence::update(float32 delta)
{
	if (AEBTNodeStatus::RUNNING != m_eState) return;

	for (AEBTNodeVector::iterator iter = m_pCondition.begin(); iter != m_pCondition.end(); ++iter)
	{
		AEBTCondition* condition = dynamic_cast<AEBTCondition*>(*iter);
		if (!condition->check())
		{
			AEBTNode* child = m_pChildren[m_iSequenceIndex];
			if (AEBTNodeStatus::RUNNING == child->getState())
				child->exit();
			m_eState = AEBTNodeStatus::SUCCESS;
			return;
		}
	}

	updateSequence(delta);
}

void AEBTSequence::updateSequence(float32 delta)
{
	AEBTNode* child = m_pChildren[m_iSequenceIndex];
	child->update(delta);

	if (AEBTNodeStatus::SUCCESS == child->getState())
	{
		child->exit();
		if (m_pChildren.size() - 1 != m_iSequenceIndex)
		{
			m_iSequenceIndex++;
			AEBTNode* childBrother = m_pChildren[m_iSequenceIndex];
			childBrother->enter();
			if (AEBTNodeStatus::RUNNING != childBrother->getState())
				childBrother->exit();
			else
				return;
		}
		m_eState = AEBTNodeStatus::SUCCESS;
	}
}

void AEBTSequence::enter()
{
	if (!check())
	{
		m_eState = AEBTNodeStatus::FAILURE;
		return;
	}

	AEBTNode* child = m_pChildren[0];
	child->enter();

	if (AEBTNodeStatus::RUNNING == child->getState())
	{
		m_iSequenceIndex = 0;
		m_eState = AEBTNodeStatus::RUNNING;
		return;
	}

	child->exit();
	m_eState = AEBTNodeStatus::FAILURE;
}

void AEBTSequence::exit()
{
	if (AEBTNodeStatus::RUNNING == m_eState)
	{
		AEBTNode* child = m_pChildren[m_iSequenceIndex];
		if (AEBTNodeStatus::RUNNING == child->getState())
			child->exit();
	}

	m_iSequenceIndex = -1;
	m_eState = AEBTNodeStatus::READIED;

	AEBTComposite::exit();
}

NS_AE_END
