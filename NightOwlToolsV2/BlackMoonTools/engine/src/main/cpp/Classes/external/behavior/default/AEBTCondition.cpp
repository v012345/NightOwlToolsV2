#include "external/behavior/default/AEBTCondition.h"
#include "scripting/lua-bindings/manual/cocos2d/LuaScriptHandlerMgr.h"

NS_AE_BEGIN

AEBTCondition::AEBTCondition() 
{

}

AEBTCondition::~AEBTCondition()
{
#if CC_ENABLE_SCRIPT_BINDING
	if (nullptr != m_pNodeListener)
	{
		m_pNodeListener("dispose", 0);
	}
#endif
}

AEBTCondition* AEBTCondition::create()
{
	return new AEBTCondition();
}

void AEBTCondition::update(float32 delta)
{

}

bool AEBTCondition::check()
{
#if CC_ENABLE_SCRIPT_BINDING
	if (nullptr != m_pNodeListener)
	{
		return m_pNodeListener("check", 0);
	}
#endif
	return true;
}

void AEBTCondition::enter()
{
	m_eState = AEBTNodeStatus::RUNNING;

#if CC_ENABLE_SCRIPT_BINDING
	if (nullptr != m_pNodeListener)
	{
		m_pNodeListener("enter", 0);
	}
#endif
}

void AEBTCondition::exit()
{
#if CC_ENABLE_SCRIPT_BINDING
	if (nullptr != m_pNodeListener)
	{
		m_pNodeListener("exit", 0);
	}
#endif

	m_eState = AEBTNodeStatus::READIED;
}

NS_AE_END
