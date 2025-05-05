#include "scripting/lua-bindings/manual/lua_behavior_manual.hpp"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"
#include "scripting/lua-bindings/manual/cocos2d/LuaScriptHandlerMgr.h"
#include "scripting/lua-bindings/manual/CCLuaValue.h"
#include "external/behavior/AEEntityAgent.h"
#include "external/behavior/default/AEBTAction.h"
#include "external/behavior/default/AEBTCondition.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"

int executeEntityAgentEvent(int handler, std::string funcName)
{
	if (0 == handler)
        return 0;
    
    LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
    if (nullptr == stack)
        return 0;
    
    lua_State* L = LuaEngine::getInstance()->getLuaStack()->getLuaState();
    if (nullptr == L)
        return 0;
    
	stack->pushString(funcName.c_str());

	int reslult = 0;
	stack->executeFunction(handler, 1, 1, [&](lua_State* L, int numReturn) {
		reslult = tolua_tonumber(L, -1, 0);
		lua_pop(L, 1);
	});
	return reslult;
}

int tolua_Behavior_registerEntityAgentEventHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"AEEntityAgent",0,&tolua_err) ||
        !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
		ae::AEEntityAgent* self = (ae::AEEntityAgent*)tolua_tousertype(tolua_S, 1, 0);
        if (NULL != self ) {
            int handler = (toluafix_ref_function(tolua_S,2,0));
			{
				self->setEntityAgentListener([=](std::string funcName) {
					return executeEntityAgentEvent(handler, funcName);
				});
				ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, ScriptHandlerMgr::HandlerType(10002));
			}
        }
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'registerEntityAgentEventHandler'.",&tolua_err);
    return 0;
#endif
}

int tolua_Behavior_unregisterEntityAgentEventHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"AEEntityAgent",0,&tolua_err) ||
        !tolua_isnumber(tolua_S, 2, 0, &tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
		ae::AEEntityAgent* self = (ae::AEEntityAgent*)tolua_tousertype(tolua_S, 1, 0);
        if (NULL != self) {
			ScriptHandlerMgr::getInstance()->removeObjectHandler((void*)self, ScriptHandlerMgr::HandlerType(10002));
        }
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'unregisterEntityAgentEventHandler'.",&tolua_err);
    return 0;
#endif
}

int register_behavior_module(lua_State* L)
{
	if (nullptr == L)
		return 0;

	lua_pushstring(L, "AEEntityAgent");
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (lua_istable(L, -1))
	{
		tolua_function(L, "registerEntityAgentEventHandler", tolua_Behavior_registerEntityAgentEventHandler00);
		tolua_function(L, "unregisterEntityAgentEventHandler", tolua_Behavior_unregisterEntityAgentEventHandler00);
	}
	lua_pop(L, 1);

	std::string typeName = typeid(ae::AEEntityAgent).name();
	g_luaType[typeName] = "AEEntityAgent";
	g_typeCast["AEEntityAgent"] = "AEEntityAgent";

	return 0;
}

bool executeBTNodeScript(int handler, std::string funcName, float delta)
{
	if (0 == handler)
		return 0;

	LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
	if (nullptr == stack)
		return 0;

	lua_State* L = LuaEngine::getInstance()->getLuaStack()->getLuaState();
	if (nullptr == L)
		return 0;

	stack->pushString(funcName.c_str());
	stack->pushFloat(delta);

	bool reslult = false;
	stack->executeFunction(handler, 2, 1, [&](lua_State* L, int numReturn) {
		reslult = tolua_toboolean(L, -1, 0);
		lua_pop(L, 1);
	});
	return reslult;
}

static int tolua_BTCondition_registerScriptHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S, 1, "AEBTCondition", 0, &tolua_err) ||
		!toluafix_isfunction(tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		ae::AEBTCondition* self = (ae::AEBTCondition*)tolua_tousertype(tolua_S, 1, 0);
		if (NULL != self) {
			int handler = (toluafix_ref_function(tolua_S, 2, 0));
			{
				self->setNodeListener([=](std::string funcName, float delta) {
					return executeBTNodeScript(handler, funcName, delta);
				});
				ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, ScriptHandlerMgr::HandlerType(10003));
			}
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror :
	tolua_error(tolua_S, "#ferror in function 'registerScriptHandler'.", &tolua_err);
	return 0;
#endif
}

static int tolua_BTCondition_unregisterScriptHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S, 1, "AEBTCondition", 0, &tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		ae::AEBTCondition* self = (ae::AEBTCondition*)tolua_tousertype(tolua_S, 1, 0);
		if (NULL != self) {
			ScriptHandlerMgr::getInstance()->removeObjectHandler((void*)self, ScriptHandlerMgr::HandlerType(10003));
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror :
	tolua_error(tolua_S, "#ferror in function 'unregisterScriptHandler'.", &tolua_err);
	return 0;
#endif
}

int register_btcondition_module(lua_State* L)
{
	if (nullptr == L)
		return 0;

	lua_pushstring(L, "AEBTCondition");
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (lua_istable(L, -1))
	{
		tolua_function(L, "registerScriptHandler", tolua_BTCondition_registerScriptHandler00);
		tolua_function(L, "unregisterScriptHandler", tolua_BTCondition_unregisterScriptHandler00);
	}
	lua_pop(L, 1);

	std::string typeName = typeid(ae::AEBTCondition).name();
	g_luaType[typeName] = "AEBTCondition";
	g_typeCast["AEBTCondition"] = "AEBTCondition";

	return 0;
}

static int tolua_BTAction_registerScriptHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S, 1, "AEBTAction", 0, &tolua_err) ||
		!toluafix_isfunction(tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		ae::AEBTAction* self = (ae::AEBTAction*)tolua_tousertype(tolua_S, 1, 0);
		if (NULL != self) {
			int handler = (toluafix_ref_function(tolua_S, 2, 0));
			{
				self->setNodeListener([=](std::string funcName, float delta) {
					return executeBTNodeScript(handler, funcName, delta);
				});
				ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, ScriptHandlerMgr::HandlerType(10004));
			}
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror :
	tolua_error(tolua_S, "#ferror in function 'registerScriptHandler'.", &tolua_err);
	return 0;
#endif
}

static int tolua_BTAction_unregisterScriptHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S, 1, "AEBTAction", 0, &tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		ae::AEBTAction* self = (ae::AEBTAction*)tolua_tousertype(tolua_S, 1, 0);
		if (NULL != self) {
			ScriptHandlerMgr::getInstance()->removeObjectHandler((void*)self, ScriptHandlerMgr::HandlerType(10004));
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror :
	tolua_error(tolua_S, "#ferror in function 'unregisterScriptHandler'.", &tolua_err);
	return 0;
#endif
}

int register_btaction_module(lua_State* L)
{
	if (nullptr == L)
		return 0;

	lua_pushstring(L, "AEBTAction");
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (lua_istable(L, -1))
	{
		tolua_function(L, "registerScriptHandler", tolua_BTAction_registerScriptHandler00);
		tolua_function(L, "unregisterScriptHandler", tolua_BTAction_unregisterScriptHandler00);
	}
	lua_pop(L, 1);

	std::string typeName = typeid(ae::AEBTAction).name();
	g_luaType[typeName] = "AEBTAction";
	g_typeCast["AEBTAction"] = "AEBTAction";

	return 0;
}

int register_all_behavior_manual(lua_State* L)
{
    lua_getglobal(L, "_G");
    if (lua_istable(L,-1))
    {
		register_behavior_module(L);
		register_btcondition_module(L);
		register_btaction_module(L);
    }
    lua_pop(L, 1);

    return 1;
}
