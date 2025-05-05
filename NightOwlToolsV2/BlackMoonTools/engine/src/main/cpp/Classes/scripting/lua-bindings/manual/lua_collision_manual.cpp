#include "scripting/lua-bindings/manual/lua_collision_manual.hpp"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"
#include "scripting/lua-bindings/manual/cocos2d/LuaScriptHandlerMgr.h"
#include "scripting/lua-bindings/manual/CCLuaValue.h"
#include "external/collision/AECollider.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"

int executeCollisionEvent(ae::AECollider* pCollider, int handler, ae::AECollisionEvent event, ae::AEColliderData* pColliderData)
{
	if (nullptr == pCollider || 0 == handler)
        return 0;
    
    LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
    if (nullptr == stack)
        return 0;
    
    lua_State* L = LuaEngine::getInstance()->getLuaStack()->getLuaState();
    if (nullptr == L)
        return 0;
    
    int ret = 0;
	stack->pushInt((int)event);
	stack->pushObject(pColliderData, typeid(ae::AEColliderData).name());
    ret = stack->executeFunctionByHandler(handler, 2);
    return ret;
}

int tolua_Collision_registerCollisionEventHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"AECollider",0,&tolua_err) ||
        !toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
		ae::AECollider* self = (ae::AECollider*)tolua_tousertype(tolua_S, 1, 0);
        if (NULL != self ) {
            int handler = (toluafix_ref_function(tolua_S,2,0));
			self->setCollisionListener([=](ae::AECollisionEvent event, ae::AEColliderData* pColliderData) {
				executeCollisionEvent(self, handler, event, pColliderData);
			});
			ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, ScriptHandlerMgr::HandlerType(10001));
        }
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'registerCollisionEventHandler'.",&tolua_err);
    return 0;
#endif
}

int tolua_Collision_unregisterCollisionEventHandler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"AECollider",0,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
		ae::AECollider* self = (ae::AECollider*)tolua_tousertype(tolua_S, 1, 0);
        if (NULL != self ) {
			self->setCollisionListener(nullptr);
			ScriptHandlerMgr::getInstance()->removeObjectHandler((void*)self, ScriptHandlerMgr::HandlerType(10001));
        }
    }
    return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'unregisterCollisionEventHandler'.",&tolua_err);
    return 0;
#endif
}

int tolua_Collision_getBoundingBox(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S, 1, "AECollider", 0, &tolua_err))
		goto tolua_lerror;
	else
#endif
	{
		ae::AECollider* self = (ae::AECollider*)tolua_tousertype(tolua_S, 1, 0);
		ae::AERect<int32> box = self->getBoundingBox();
		Rect rect = Rect(box.origin.x, box.origin.y, box.size.w, box.size.h);
		rect_to_luaval(tolua_S, rect);
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror :
	tolua_error(tolua_S, "#ferror in function 'getBoundingBox'.", &tolua_err);
	return 0;
#endif
}

int register_collision_module(lua_State* L)
{
    if (nullptr == L)
        return 0;

	lua_pushstring(L, "AECollider");
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (lua_istable(L, -1))
	{
		tolua_function(L, "registerCollisionEventHandler", tolua_Collision_registerCollisionEventHandler00);
		tolua_function(L, "unregisterCollisionEventHandler", tolua_Collision_unregisterCollisionEventHandler00);
		tolua_function(L, "getBoundingBox", tolua_Collision_getBoundingBox);
	}
	lua_pop(L, 1);

	std::string typeName = typeid(ae::AECollider).name();
	g_luaType[typeName] = "AECollider";
	g_typeCast["AECollider"] = "AECollider";
    
    return 0;
}

int register_all_collision_manual(lua_State* L)
{
    lua_getglobal(L, "_G");
    if (lua_istable(L,-1))
    {
		register_collision_module(L);
    }
    lua_pop(L, 1);

    return 1;
}
