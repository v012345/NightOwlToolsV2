#include "scripting/lua-bindings/auto/lua_external_auto.hpp"
#include "external/collision/AEColliderDraw.h"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"

int lua_external_AEColliderRender_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEColliderRender",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        ae::AECollision* arg0;
        ok &= luaval_to_object<ae::AECollision>(tolua_S, 2, "AECollision",&arg0, "AEColliderRender:create");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_external_AEColliderRender_create'", nullptr);
            return 0;
        }
        ae::AEColliderRender* ret = ae::AEColliderRender::create(arg0);
        object_to_luaval<ae::AEColliderRender>(tolua_S, "AEColliderRender",(ae::AEColliderRender*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEColliderRender:create",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_external_AEColliderRender_create'.",&tolua_err);
#endif
    return 0;
}
int lua_external_AEColliderRender_constructor(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEColliderRender* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 1) {
            ae::AECollision* arg0;
            ok &= luaval_to_object<ae::AECollision>(tolua_S, 2, "AECollision",&arg0, "AEColliderRender:AEColliderRender");

            if (!ok) { break; }
            cobj = new ae::AEColliderRender(arg0);
            cobj->autorelease();
            int ID =  (int)cobj->_ID ;
            int* luaID =  &cobj->_luaID ;
            toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"AEColliderRender");
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 0) {
            cobj = new ae::AEColliderRender();
            cobj->autorelease();
            int ID =  (int)cobj->_ID ;
            int* luaID =  &cobj->_luaID ;
            toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"AEColliderRender");
            return 1;
        }
    }while(0);
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "AEColliderRender:AEColliderRender",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_external_AEColliderRender_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_external_AEColliderRender_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (AEColliderRender)");
    return 0;
}

int lua_register_external_AEColliderRender(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"AEColliderRender");
    tolua_cclass(tolua_S,"AEColliderRender","AEColliderRender","cc.Node",nullptr);

    tolua_beginmodule(tolua_S,"AEColliderRender");
        tolua_function(tolua_S,"new",lua_external_AEColliderRender_constructor);
        tolua_function(tolua_S,"create", lua_external_AEColliderRender_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ae::AEColliderRender).name();
    g_luaType[typeName] = "AEColliderRender";
    g_typeCast["AEColliderRender"] = "AEColliderRender";
    return 1;
}
TOLUA_API int register_all_external(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,nullptr,0);
	tolua_beginmodule(tolua_S,nullptr);

	lua_register_external_AEColliderRender(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

