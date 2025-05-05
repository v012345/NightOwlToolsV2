#include "external/behavior/default/AEBTNode.h"

NS_AE_BEGIN

AEBTNode::AEBTNode()
: m_pParent(nullptr)
, m_eState(AEBTNodeStatus::READIED)
{

}

AEBTNode::~AEBTNode()
{

}

bool AEBTNode::init()
{
	return true;
}

void AEBTNode::enter()
{

}

void AEBTNode::exit()
{

}

void AEBTNode::update(float32 delta)
{

}

void AEBTNode::setNodeListener(AEBTNodeListener listener)
{
	m_pNodeListener = listener;
}

NS_AE_END
