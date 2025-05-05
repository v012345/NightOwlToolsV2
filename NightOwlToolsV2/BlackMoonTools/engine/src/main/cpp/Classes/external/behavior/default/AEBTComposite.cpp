#include "external/behavior/default/AEBTComposite.h"
#include "external/behavior/default/AEBTCondition.h"

NS_AE_BEGIN

AEBTComposite::AEBTComposite()
{

}

AEBTComposite::~AEBTComposite()
{
	for (AEBTNodeVector::iterator iter = m_pCondition.begin(); iter != m_pCondition.end(); ++iter)
		AESafe_delete(*iter);
	m_pCondition.clear();

	for (AEBTNodeVector::iterator iter = m_pChildren.begin(); iter != m_pChildren.end(); ++iter)
		AESafe_delete(*iter);
	m_pChildren.clear();
}

bool AEBTComposite::check()
{
	for (AEBTNodeVector::iterator iter = m_pCondition.begin(); iter != m_pCondition.end(); ++iter)
	{
		AEBTCondition* condition = dynamic_cast<AEBTCondition*>(*iter);
		if (condition->check())
			condition->enter();
		else
			return false;
	}
	return true;
}

void AEBTComposite::exit()
{
	for (AEBTNodeVector::reverse_iterator iter = m_pCondition.rbegin(); iter != m_pCondition.rend(); ++iter)
	{
		AEBTCondition* condition = dynamic_cast<AEBTCondition*>(*iter);
		if (AEBTNodeStatus::RUNNING == condition->getState())
			condition->exit();
	}
}

AEBTNode* AEBTComposite::addCondition(AEBTNode* condition)
{
	m_pCondition.push_back(condition);
	return condition;
}

AEBTNode* AEBTComposite::removeCondition(AEBTNode* condition)
{
	for (AEBTNodeVector::iterator iter = m_pCondition.begin(); iter != m_pCondition.end(); ++iter)
	{
		if (condition == *iter)
		{
			m_pCondition.erase(iter);
			return condition;
		}
	}
	return condition;
}

AEBTNode* AEBTComposite::addChild(AEBTNode* child)
{
	m_pChildren.push_back(child);
	return child;
}

AEBTNode* AEBTComposite::removeChild(AEBTNode* child)
{
	for (AEBTNodeVector::iterator iter = m_pChildren.begin(); iter != m_pChildren.end(); ++iter)
	{
		if (child == *iter)
		{
			m_pChildren.erase(iter);
			return child;
		}
	}
	return child;
}

void AEBTComposite::removeAllChildren()
{
	for (AEBTNodeVector::iterator iter = m_pChildren.begin(); iter != m_pChildren.end(); ++iter)
		AESafe_delete(*iter);
	m_pChildren.clear();
}

AEBTNode* AEBTComposite::findChild(uint32 childIndex)
{
	return m_pChildren.at(childIndex);
}

NS_AE_END
