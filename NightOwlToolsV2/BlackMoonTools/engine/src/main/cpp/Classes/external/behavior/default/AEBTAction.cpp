#include "external/behavior/default/AEBTAction.h"
#include "scripting/lua-bindings/manual/cocos2d/LuaScriptHandlerMgr.h"

NS_AE_BEGIN

AEBTAction::AEBTAction()
{

}

AEBTAction::~AEBTAction()
{
#if CC_ENABLE_SCRIPT_BINDING
	if (nullptr != m_pNodeListener)
	{
		m_pNodeListener("dispose", 0);
	}
#endif
}

AEBTAction* AEBTAction::create()
{
	return new AEBTAction();
}

void AEBTAction::update(float32 delta)
{
	if (AEBTNodeStatus::RUNNING != m_eState) return;

#if CC_ENABLE_SCRIPT_BINDING
	if (nullptr != m_pNodeListener)
	{
		m_pNodeListener("update", delta);
	}
#endif
}

void AEBTAction::enter()
{
	m_eState = AEBTNodeStatus::RUNNING;

#if CC_ENABLE_SCRIPT_BINDING
	if (nullptr != m_pNodeListener)
	{
		m_pNodeListener("enter", 0);
	}
#endif
}

void AEBTAction::exit()
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
