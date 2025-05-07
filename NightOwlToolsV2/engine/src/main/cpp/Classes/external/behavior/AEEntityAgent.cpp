#include "AEEntityAgent.h"
#include "behaviac_generated/types/behaviac_types.h"

NS_AE_BEGIN

AEEntityAgent::AEEntityAgent()
{
	m_pEntityAgent = behaviac::Agent::Create<EntityAgent>();
	bool bRet = m_pEntityAgent->btload("Entity");
	m_pEntityAgent->btsetcurrent("Entity");

	m_pEntityAgent->setEntityAgent(this);
}

AEEntityAgent::~AEEntityAgent()
{
	
}

AEEntityAgent* AEEntityAgent::create()
{	
	return new AEEntityAgent();
}

void AEEntityAgent::release()
{
	behaviac::Agent::Destroy(m_pEntityAgent);
	delete this;
}

void AEEntityAgent::update(float delta)
{
	m_pEntityAgent->btexec();
}

void AEEntityAgent::setEntityAgentListener(AEEntityAgentListener listener)
{
	m_pEntityAgentListener = listener;
}

int AEEntityAgent::onEntityAgentListener(std::string funcName)
{
	if (nullptr != m_pEntityAgentListener)
	{
		return m_pEntityAgentListener(funcName);
	}
	return 1;
}

NS_AE_END
