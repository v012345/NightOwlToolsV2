#include "scripting/lua-bindings/auto/lua_behavior_auto.hpp"
#include "external/behavior/AEEntityAgent.h"
#include "external/behavior/default/AEBTAction.h"
#include "external/behavior/default/AEBTComposite.h"
#include "external/behavior/default/AEBTCondition.h"
#include "external/behavior/default/AEBTNode.h"
#include "external/behavior/default/AEBTParallel.h"
#include "external/behavior/default/AEBTSelector.h"
#include "external/behavior/default/AEBTSequence.h"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"

int lua_behavior_AEEntityAgent_release(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEEntityAgent* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEEntityAgent",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEEntityAgent*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEEntityAgent_release'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEEntityAgent_release'", nullptr);
            return 0;
        }
        cobj->release();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEEntityAgent:release",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEEntityAgent_release'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEEntityAgent_onEntityAgentListener(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEEntityAgent* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEEntityAgent",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEEntityAgent*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEEntityAgent_onEntityAgentListener'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AEEntityAgent:onEntityAgentListener");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEEntityAgent_onEntityAgentListener'", nullptr);
            return 0;
        }
        int ret = cobj->onEntityAgentListener(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEEntityAgent:onEntityAgentListener",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEEntityAgent_onEntityAgentListener'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEEntityAgent_update(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEEntityAgent* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEEntityAgent",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEEntityAgent*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEEntityAgent_update'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "AEEntityAgent:update");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEEntityAgent_update'", nullptr);
            return 0;
        }
        cobj->update(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEEntityAgent:update",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEEntityAgent_update'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEEntityAgent_setEntityAgentListener(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEEntityAgent* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEEntityAgent",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEEntityAgent*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEEntityAgent_setEntityAgentListener'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::function<int (std::basic_string<char>)> arg0;

        do {
			// Lambda binding for lua is not supported.
			assert(false);
		} while(0)
		;
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEEntityAgent_setEntityAgentListener'", nullptr);
            return 0;
        }
        cobj->setEntityAgentListener(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEEntityAgent:setEntityAgentListener",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEEntityAgent_setEntityAgentListener'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEEntityAgent_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEEntityAgent",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEEntityAgent_create'", nullptr);
            return 0;
        }
        ae::AEEntityAgent* ret = ae::AEEntityAgent::create();
        object_to_luaval<ae::AEEntityAgent>(tolua_S, "AEEntityAgent",(ae::AEEntityAgent*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEEntityAgent:create",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEEntityAgent_create'.",&tolua_err);
#endif
    return 0;
}
int lua_behavior_AEEntityAgent_constructor(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEEntityAgent* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEEntityAgent_constructor'", nullptr);
            return 0;
        }
        cobj = new ae::AEEntityAgent();
        tolua_pushusertype(tolua_S,(void*)cobj,"AEEntityAgent");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEEntityAgent:AEEntityAgent",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEEntityAgent_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_behavior_AEEntityAgent_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (AEEntityAgent)");
    return 0;
}

int lua_register_behavior_AEEntityAgent(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"AEEntityAgent");
    tolua_cclass(tolua_S,"AEEntityAgent","AEEntityAgent","",nullptr);

    tolua_beginmodule(tolua_S,"AEEntityAgent");
        tolua_function(tolua_S,"new",lua_behavior_AEEntityAgent_constructor);
        tolua_function(tolua_S,"release",lua_behavior_AEEntityAgent_release);
        tolua_function(tolua_S,"onEntityAgentListener",lua_behavior_AEEntityAgent_onEntityAgentListener);
        tolua_function(tolua_S,"update",lua_behavior_AEEntityAgent_update);
        tolua_function(tolua_S,"setEntityAgentListener",lua_behavior_AEEntityAgent_setEntityAgentListener);
        tolua_function(tolua_S,"create", lua_behavior_AEEntityAgent_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ae::AEEntityAgent).name();
    g_luaType[typeName] = "AEEntityAgent";
    g_typeCast["AEEntityAgent"] = "AEEntityAgent";
    return 1;
}

int lua_behavior_AEBTNode_getState(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEBTNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEBTNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEBTNode_getState'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTNode_getState'", nullptr);
            return 0;
        }
        int ret = (int)cobj->getState();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTNode:getState",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTNode_getState'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEBTNode_getParent(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEBTNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEBTNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEBTNode_getParent'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTNode_getParent'", nullptr);
            return 0;
        }
        ae::AEBTNode* ret = cobj->getParent();
        object_to_luaval<ae::AEBTNode>(tolua_S, "AEBTNode",(ae::AEBTNode*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTNode:getParent",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTNode_getParent'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEBTNode_setParent(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEBTNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEBTNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEBTNode_setParent'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        ae::AEBTNode* arg0;

        ok &= luaval_to_object<ae::AEBTNode>(tolua_S, 2, "AEBTNode",&arg0, "AEBTNode:setParent");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTNode_setParent'", nullptr);
            return 0;
        }
        cobj->setParent(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTNode:setParent",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTNode_setParent'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEBTNode_update(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEBTNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEBTNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEBTNode_update'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "AEBTNode:update");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTNode_update'", nullptr);
            return 0;
        }
        cobj->update(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTNode:update",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTNode_update'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEBTNode_init(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEBTNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEBTNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEBTNode_init'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTNode_init'", nullptr);
            return 0;
        }
        bool ret = cobj->init();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTNode:init",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTNode_init'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEBTNode_exit(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEBTNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEBTNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEBTNode_exit'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTNode_exit'", nullptr);
            return 0;
        }
        cobj->exit();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTNode:exit",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTNode_exit'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEBTNode_enter(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEBTNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEBTNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEBTNode_enter'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTNode_enter'", nullptr);
            return 0;
        }
        cobj->enter();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTNode:enter",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTNode_enter'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEBTNode_setNodeListener(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEBTNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEBTNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEBTNode_setNodeListener'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::function<bool (std::basic_string<char>, float)> arg0;

        do {
			// Lambda binding for lua is not supported.
			assert(false);
		} while(0)
		;
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTNode_setNodeListener'", nullptr);
            return 0;
        }
        cobj->setNodeListener(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTNode:setNodeListener",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTNode_setNodeListener'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEBTNode_setState(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEBTNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEBTNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEBTNode_setState'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        ae::AEBTNodeStatus arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "AEBTNode:setState");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTNode_setState'", nullptr);
            return 0;
        }
        cobj->setState(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTNode:setState",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTNode_setState'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEBTNode_constructor(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTNode_constructor'", nullptr);
            return 0;
        }
        cobj = new ae::AEBTNode();
        cobj->autorelease();
        int ID =  (int)cobj->_ID ;
        int* luaID =  &cobj->_luaID ;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"AEBTNode");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTNode:AEBTNode",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTNode_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_behavior_AEBTNode_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (AEBTNode)");
    return 0;
}

int lua_register_behavior_AEBTNode(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"AEBTNode");
    tolua_cclass(tolua_S,"AEBTNode","AEBTNode","cc.Ref",nullptr);

    tolua_beginmodule(tolua_S,"AEBTNode");
        tolua_function(tolua_S,"new",lua_behavior_AEBTNode_constructor);
        tolua_function(tolua_S,"getState",lua_behavior_AEBTNode_getState);
        tolua_function(tolua_S,"getParent",lua_behavior_AEBTNode_getParent);
        tolua_function(tolua_S,"setParent",lua_behavior_AEBTNode_setParent);
        tolua_function(tolua_S,"update",lua_behavior_AEBTNode_update);
        tolua_function(tolua_S,"init",lua_behavior_AEBTNode_init);
        tolua_function(tolua_S,"exit",lua_behavior_AEBTNode_exit);
        tolua_function(tolua_S,"enter",lua_behavior_AEBTNode_enter);
        tolua_function(tolua_S,"setNodeListener",lua_behavior_AEBTNode_setNodeListener);
        tolua_function(tolua_S,"setState",lua_behavior_AEBTNode_setState);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ae::AEBTNode).name();
    g_luaType[typeName] = "AEBTNode";
    g_typeCast["AEBTNode"] = "AEBTNode";
    return 1;
}

int lua_behavior_AEBTAction_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEBTAction",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTAction_create'", nullptr);
            return 0;
        }
        ae::AEBTAction* ret = ae::AEBTAction::create();
        object_to_luaval<ae::AEBTAction>(tolua_S, "AEBTAction",(ae::AEBTAction*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEBTAction:create",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTAction_create'.",&tolua_err);
#endif
    return 0;
}
int lua_behavior_AEBTAction_constructor(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTAction* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTAction_constructor'", nullptr);
            return 0;
        }
        cobj = new ae::AEBTAction();
        cobj->autorelease();
        int ID =  (int)cobj->_ID ;
        int* luaID =  &cobj->_luaID ;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"AEBTAction");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTAction:AEBTAction",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTAction_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_behavior_AEBTAction_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (AEBTAction)");
    return 0;
}

int lua_register_behavior_AEBTAction(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"AEBTAction");
    tolua_cclass(tolua_S,"AEBTAction","AEBTAction","AEBTNode",nullptr);

    tolua_beginmodule(tolua_S,"AEBTAction");
        tolua_function(tolua_S,"new",lua_behavior_AEBTAction_constructor);
        tolua_function(tolua_S,"create", lua_behavior_AEBTAction_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ae::AEBTAction).name();
    g_luaType[typeName] = "AEBTAction";
    g_typeCast["AEBTAction"] = "AEBTAction";
    return 1;
}

int lua_behavior_AEBTComposite_removeCondition(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTComposite* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEBTComposite",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEBTComposite*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEBTComposite_removeCondition'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        ae::AEBTNode* arg0;

        ok &= luaval_to_object<ae::AEBTNode>(tolua_S, 2, "AEBTNode",&arg0, "AEBTComposite:removeCondition");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTComposite_removeCondition'", nullptr);
            return 0;
        }
        ae::AEBTNode* ret = cobj->removeCondition(arg0);
        object_to_luaval<ae::AEBTNode>(tolua_S, "AEBTNode",(ae::AEBTNode*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTComposite:removeCondition",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTComposite_removeCondition'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEBTComposite_addChild(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTComposite* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEBTComposite",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEBTComposite*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEBTComposite_addChild'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        ae::AEBTNode* arg0;

        ok &= luaval_to_object<ae::AEBTNode>(tolua_S, 2, "AEBTNode",&arg0, "AEBTComposite:addChild");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTComposite_addChild'", nullptr);
            return 0;
        }
        ae::AEBTNode* ret = cobj->addChild(arg0);
        object_to_luaval<ae::AEBTNode>(tolua_S, "AEBTNode",(ae::AEBTNode*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTComposite:addChild",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTComposite_addChild'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEBTComposite_removeAllChildren(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTComposite* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEBTComposite",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEBTComposite*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEBTComposite_removeAllChildren'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTComposite_removeAllChildren'", nullptr);
            return 0;
        }
        cobj->removeAllChildren();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTComposite:removeAllChildren",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTComposite_removeAllChildren'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEBTComposite_addCondition(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTComposite* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEBTComposite",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEBTComposite*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEBTComposite_addCondition'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        ae::AEBTNode* arg0;

        ok &= luaval_to_object<ae::AEBTNode>(tolua_S, 2, "AEBTNode",&arg0, "AEBTComposite:addCondition");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTComposite_addCondition'", nullptr);
            return 0;
        }
        ae::AEBTNode* ret = cobj->addCondition(arg0);
        object_to_luaval<ae::AEBTNode>(tolua_S, "AEBTNode",(ae::AEBTNode*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTComposite:addCondition",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTComposite_addCondition'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEBTComposite_removeChild(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTComposite* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEBTComposite",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEBTComposite*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEBTComposite_removeChild'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        ae::AEBTNode* arg0;

        ok &= luaval_to_object<ae::AEBTNode>(tolua_S, 2, "AEBTNode",&arg0, "AEBTComposite:removeChild");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTComposite_removeChild'", nullptr);
            return 0;
        }
        ae::AEBTNode* ret = cobj->removeChild(arg0);
        object_to_luaval<ae::AEBTNode>(tolua_S, "AEBTNode",(ae::AEBTNode*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTComposite:removeChild",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTComposite_removeChild'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEBTComposite_findChild(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTComposite* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEBTComposite",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEBTComposite*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEBTComposite_findChild'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned int arg0;

        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "AEBTComposite:findChild");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTComposite_findChild'", nullptr);
            return 0;
        }
        ae::AEBTNode* ret = cobj->findChild(arg0);
        object_to_luaval<ae::AEBTNode>(tolua_S, "AEBTNode",(ae::AEBTNode*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTComposite:findChild",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTComposite_findChild'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEBTComposite_check(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTComposite* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEBTComposite",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEBTComposite*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEBTComposite_check'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTComposite_check'", nullptr);
            return 0;
        }
        bool ret = cobj->check();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTComposite:check",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTComposite_check'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEBTComposite_constructor(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTComposite* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTComposite_constructor'", nullptr);
            return 0;
        }
        cobj = new ae::AEBTComposite();
        cobj->autorelease();
        int ID =  (int)cobj->_ID ;
        int* luaID =  &cobj->_luaID ;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"AEBTComposite");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTComposite:AEBTComposite",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTComposite_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_behavior_AEBTComposite_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (AEBTComposite)");
    return 0;
}

int lua_register_behavior_AEBTComposite(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"AEBTComposite");
    tolua_cclass(tolua_S,"AEBTComposite","AEBTComposite","AEBTNode",nullptr);

    tolua_beginmodule(tolua_S,"AEBTComposite");
        tolua_function(tolua_S,"new",lua_behavior_AEBTComposite_constructor);
        tolua_function(tolua_S,"removeCondition",lua_behavior_AEBTComposite_removeCondition);
        tolua_function(tolua_S,"addChild",lua_behavior_AEBTComposite_addChild);
        tolua_function(tolua_S,"removeAllChildren",lua_behavior_AEBTComposite_removeAllChildren);
        tolua_function(tolua_S,"addCondition",lua_behavior_AEBTComposite_addCondition);
        tolua_function(tolua_S,"removeChild",lua_behavior_AEBTComposite_removeChild);
        tolua_function(tolua_S,"findChild",lua_behavior_AEBTComposite_findChild);
        tolua_function(tolua_S,"check",lua_behavior_AEBTComposite_check);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ae::AEBTComposite).name();
    g_luaType[typeName] = "AEBTComposite";
    g_typeCast["AEBTComposite"] = "AEBTComposite";
    return 1;
}

int lua_behavior_AEBTCondition_check(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTCondition* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEBTCondition",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEBTCondition*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEBTCondition_check'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTCondition_check'", nullptr);
            return 0;
        }
        bool ret = cobj->check();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTCondition:check",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTCondition_check'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEBTCondition_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEBTCondition",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTCondition_create'", nullptr);
            return 0;
        }
        ae::AEBTCondition* ret = ae::AEBTCondition::create();
        object_to_luaval<ae::AEBTCondition>(tolua_S, "AEBTCondition",(ae::AEBTCondition*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEBTCondition:create",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTCondition_create'.",&tolua_err);
#endif
    return 0;
}
int lua_behavior_AEBTCondition_constructor(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTCondition* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTCondition_constructor'", nullptr);
            return 0;
        }
        cobj = new ae::AEBTCondition();
        cobj->autorelease();
        int ID =  (int)cobj->_ID ;
        int* luaID =  &cobj->_luaID ;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"AEBTCondition");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTCondition:AEBTCondition",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTCondition_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_behavior_AEBTCondition_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (AEBTCondition)");
    return 0;
}

int lua_register_behavior_AEBTCondition(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"AEBTCondition");
    tolua_cclass(tolua_S,"AEBTCondition","AEBTCondition","AEBTNode",nullptr);

    tolua_beginmodule(tolua_S,"AEBTCondition");
        tolua_function(tolua_S,"new",lua_behavior_AEBTCondition_constructor);
        tolua_function(tolua_S,"check",lua_behavior_AEBTCondition_check);
        tolua_function(tolua_S,"create", lua_behavior_AEBTCondition_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ae::AEBTCondition).name();
    g_luaType[typeName] = "AEBTCondition";
    g_typeCast["AEBTCondition"] = "AEBTCondition";
    return 1;
}

int lua_behavior_AEBTParallel_updateParallel(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTParallel* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEBTParallel",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEBTParallel*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEBTParallel_updateParallel'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "AEBTParallel:updateParallel");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTParallel_updateParallel'", nullptr);
            return 0;
        }
        cobj->updateParallel(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTParallel:updateParallel",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTParallel_updateParallel'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEBTParallel_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEBTParallel",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTParallel_create'", nullptr);
            return 0;
        }
        ae::AEBTParallel* ret = ae::AEBTParallel::create();
        object_to_luaval<ae::AEBTParallel>(tolua_S, "AEBTParallel",(ae::AEBTParallel*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEBTParallel:create",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTParallel_create'.",&tolua_err);
#endif
    return 0;
}
int lua_behavior_AEBTParallel_constructor(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTParallel* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTParallel_constructor'", nullptr);
            return 0;
        }
        cobj = new ae::AEBTParallel();
        cobj->autorelease();
        int ID =  (int)cobj->_ID ;
        int* luaID =  &cobj->_luaID ;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"AEBTParallel");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTParallel:AEBTParallel",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTParallel_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_behavior_AEBTParallel_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (AEBTParallel)");
    return 0;
}

int lua_register_behavior_AEBTParallel(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"AEBTParallel");
    tolua_cclass(tolua_S,"AEBTParallel","AEBTParallel","AEBTComposite",nullptr);

    tolua_beginmodule(tolua_S,"AEBTParallel");
        tolua_function(tolua_S,"new",lua_behavior_AEBTParallel_constructor);
        tolua_function(tolua_S,"updateParallel",lua_behavior_AEBTParallel_updateParallel);
        tolua_function(tolua_S,"create", lua_behavior_AEBTParallel_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ae::AEBTParallel).name();
    g_luaType[typeName] = "AEBTParallel";
    g_typeCast["AEBTParallel"] = "AEBTParallel";
    return 1;
}

int lua_behavior_AEBTSelector_updateSelector(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTSelector* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEBTSelector",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEBTSelector*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEBTSelector_updateSelector'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "AEBTSelector:updateSelector");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTSelector_updateSelector'", nullptr);
            return 0;
        }
        cobj->updateSelector(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTSelector:updateSelector",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTSelector_updateSelector'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEBTSelector_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEBTSelector",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTSelector_create'", nullptr);
            return 0;
        }
        ae::AEBTSelector* ret = ae::AEBTSelector::create();
        object_to_luaval<ae::AEBTSelector>(tolua_S, "AEBTSelector",(ae::AEBTSelector*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEBTSelector:create",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTSelector_create'.",&tolua_err);
#endif
    return 0;
}
int lua_behavior_AEBTSelector_constructor(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTSelector* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTSelector_constructor'", nullptr);
            return 0;
        }
        cobj = new ae::AEBTSelector();
        cobj->autorelease();
        int ID =  (int)cobj->_ID ;
        int* luaID =  &cobj->_luaID ;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"AEBTSelector");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTSelector:AEBTSelector",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTSelector_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_behavior_AEBTSelector_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (AEBTSelector)");
    return 0;
}

int lua_register_behavior_AEBTSelector(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"AEBTSelector");
    tolua_cclass(tolua_S,"AEBTSelector","AEBTSelector","AEBTComposite",nullptr);

    tolua_beginmodule(tolua_S,"AEBTSelector");
        tolua_function(tolua_S,"new",lua_behavior_AEBTSelector_constructor);
        tolua_function(tolua_S,"updateSelector",lua_behavior_AEBTSelector_updateSelector);
        tolua_function(tolua_S,"create", lua_behavior_AEBTSelector_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ae::AEBTSelector).name();
    g_luaType[typeName] = "AEBTSelector";
    g_typeCast["AEBTSelector"] = "AEBTSelector";
    return 1;
}

int lua_behavior_AEBTSequence_updateSequence(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTSequence* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEBTSequence",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEBTSequence*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_behavior_AEBTSequence_updateSequence'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "AEBTSequence:updateSequence");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTSequence_updateSequence'", nullptr);
            return 0;
        }
        cobj->updateSequence(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTSequence:updateSequence",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTSequence_updateSequence'.",&tolua_err);
#endif

    return 0;
}
int lua_behavior_AEBTSequence_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEBTSequence",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTSequence_create'", nullptr);
            return 0;
        }
        ae::AEBTSequence* ret = ae::AEBTSequence::create();
        object_to_luaval<ae::AEBTSequence>(tolua_S, "AEBTSequence",(ae::AEBTSequence*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEBTSequence:create",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTSequence_create'.",&tolua_err);
#endif
    return 0;
}
int lua_behavior_AEBTSequence_constructor(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEBTSequence* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_behavior_AEBTSequence_constructor'", nullptr);
            return 0;
        }
        cobj = new ae::AEBTSequence();
        cobj->autorelease();
        int ID =  (int)cobj->_ID ;
        int* luaID =  &cobj->_luaID ;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"AEBTSequence");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEBTSequence:AEBTSequence",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_behavior_AEBTSequence_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_behavior_AEBTSequence_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (AEBTSequence)");
    return 0;
}

int lua_register_behavior_AEBTSequence(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"AEBTSequence");
    tolua_cclass(tolua_S,"AEBTSequence","AEBTSequence","AEBTComposite",nullptr);

    tolua_beginmodule(tolua_S,"AEBTSequence");
        tolua_function(tolua_S,"new",lua_behavior_AEBTSequence_constructor);
        tolua_function(tolua_S,"updateSequence",lua_behavior_AEBTSequence_updateSequence);
        tolua_function(tolua_S,"create", lua_behavior_AEBTSequence_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ae::AEBTSequence).name();
    g_luaType[typeName] = "AEBTSequence";
    g_typeCast["AEBTSequence"] = "AEBTSequence";
    return 1;
}
TOLUA_API int register_all_behavior(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,nullptr,0);
	tolua_beginmodule(tolua_S,nullptr);

	lua_register_behavior_AEBTNode(tolua_S);
	lua_register_behavior_AEBTAction(tolua_S);
	lua_register_behavior_AEBTComposite(tolua_S);
	lua_register_behavior_AEBTSequence(tolua_S);
	lua_register_behavior_AEBTCondition(tolua_S);
	lua_register_behavior_AEBTSelector(tolua_S);
	lua_register_behavior_AEEntityAgent(tolua_S);
	lua_register_behavior_AEBTParallel(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

