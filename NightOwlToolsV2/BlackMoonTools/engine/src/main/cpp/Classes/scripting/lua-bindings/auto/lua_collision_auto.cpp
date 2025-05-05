#include "scripting/lua-bindings/auto/lua_collision_auto.hpp"
#include "external/collision/AECollider.h"
#include "external/collision/AECollision.h"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"

int lua_collision_AECollision_update(lua_State* tolua_S)
{
    int argc = 0;
    ae::AECollision* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AECollision",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AECollision*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_collision_AECollision_update'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "AECollision:update");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_collision_AECollision_update'", nullptr);
            return 0;
        }
        cobj->update(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AECollision:update",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_collision_AECollision_update'.",&tolua_err);
#endif

    return 0;
}
int lua_collision_AECollision_release(lua_State* tolua_S)
{
    int argc = 0;
    ae::AECollision* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AECollision",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AECollision*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_collision_AECollision_release'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_collision_AECollision_release'", nullptr);
            return 0;
        }
        cobj->release();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AECollision:release",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_collision_AECollision_release'.",&tolua_err);
#endif

    return 0;
}
int lua_collision_AECollision_getInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AECollision",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_collision_AECollision_getInstance'", nullptr);
            return 0;
        }
        ae::AECollision* ret = ae::AECollision::getInstance();
        object_to_luaval<ae::AECollision>(tolua_S, "AECollision",(ae::AECollision*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AECollision:getInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_collision_AECollision_getInstance'.",&tolua_err);
#endif
    return 0;
}
int lua_collision_AECollision_constructor(lua_State* tolua_S)
{
    int argc = 0;
    ae::AECollision* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_collision_AECollision_constructor'", nullptr);
            return 0;
        }
        cobj = new ae::AECollision();
        tolua_pushusertype(tolua_S,(void*)cobj,"AECollision");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AECollision:AECollision",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_collision_AECollision_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_collision_AECollision_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (AECollision)");
    return 0;
}

int lua_register_collision_AECollision(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"AECollision");
    tolua_cclass(tolua_S,"AECollision","AECollision","b2ContactListener",nullptr);

    tolua_beginmodule(tolua_S,"AECollision");
        tolua_function(tolua_S,"new",lua_collision_AECollision_constructor);
        tolua_function(tolua_S,"update",lua_collision_AECollision_update);
        tolua_function(tolua_S,"release",lua_collision_AECollision_release);
        tolua_function(tolua_S,"getInstance", lua_collision_AECollision_getInstance);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ae::AECollision).name();
    g_luaType[typeName] = "AECollision";
    g_typeCast["AECollision"] = "AECollision";
    return 1;
}

int lua_collision_AEColliderData_getContactPointY(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEColliderData* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEColliderData",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEColliderData*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_collision_AEColliderData_getContactPointY'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_collision_AEColliderData_getContactPointY'", nullptr);
            return 0;
        }
        int ret = cobj->getContactPointY();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEColliderData:getContactPointY",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_collision_AEColliderData_getContactPointY'.",&tolua_err);
#endif

    return 0;
}
int lua_collision_AEColliderData_getCollider(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEColliderData* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEColliderData",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEColliderData*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_collision_AEColliderData_getCollider'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_collision_AEColliderData_getCollider'", nullptr);
            return 0;
        }
        ae::AECollider* ret = cobj->getCollider();
        object_to_luaval<ae::AECollider>(tolua_S, "AECollider",(ae::AECollider*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEColliderData:getCollider",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_collision_AEColliderData_getCollider'.",&tolua_err);
#endif

    return 0;
}
int lua_collision_AEColliderData_getContactPointX(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEColliderData* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEColliderData",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEColliderData*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_collision_AEColliderData_getContactPointX'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_collision_AEColliderData_getContactPointX'", nullptr);
            return 0;
        }
        int ret = cobj->getContactPointX();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEColliderData:getContactPointX",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_collision_AEColliderData_getContactPointX'.",&tolua_err);
#endif

    return 0;
}
int lua_collision_AEColliderData_constructor(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEColliderData* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 2) {
            ae::AECollider* arg0;
            ok &= luaval_to_object<ae::AECollider>(tolua_S, 2, "AECollider",&arg0, "AEColliderData:AEColliderData");

            if (!ok) { break; }
            ae::AECollider* arg1;
            ok &= luaval_to_object<ae::AECollider>(tolua_S, 3, "AECollider",&arg1, "AEColliderData:AEColliderData");

            if (!ok) { break; }
            cobj = new ae::AEColliderData(arg0, arg1);
            cobj->autorelease();
            int ID =  (int)cobj->_ID ;
            int* luaID =  &cobj->_luaID ;
            toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"AEColliderData");
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 0) {
            cobj = new ae::AEColliderData();
            cobj->autorelease();
            int ID =  (int)cobj->_ID ;
            int* luaID =  &cobj->_luaID ;
            toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"AEColliderData");
            return 1;
        }
    }while(0);
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "AEColliderData:AEColliderData",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_collision_AEColliderData_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_collision_AEColliderData_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (AEColliderData)");
    return 0;
}

int lua_register_collision_AEColliderData(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"AEColliderData");
    tolua_cclass(tolua_S,"AEColliderData","AEColliderData","cc.Ref",nullptr);

    tolua_beginmodule(tolua_S,"AEColliderData");
        tolua_function(tolua_S,"new",lua_collision_AEColliderData_constructor);
        tolua_function(tolua_S,"getContactPointY",lua_collision_AEColliderData_getContactPointY);
        tolua_function(tolua_S,"getCollider",lua_collision_AEColliderData_getCollider);
        tolua_function(tolua_S,"getContactPointX",lua_collision_AEColliderData_getContactPointX);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ae::AEColliderData).name();
    g_luaType[typeName] = "AEColliderData";
    g_typeCast["AEColliderData"] = "AEColliderData";
    return 1;
}

int lua_collision_AECollider_updateVerticesSync(lua_State* tolua_S)
{
    int argc = 0;
    ae::AECollider* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AECollider",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AECollider*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_collision_AECollider_updateVerticesSync'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 7) 
    {
        std::string arg0;
        std::string arg1;
        double arg2;
        double arg3;
        double arg4;
        double arg5;
        double arg6;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AECollider:updateVerticesSync");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "AECollider:updateVerticesSync");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "AECollider:updateVerticesSync");

        ok &= luaval_to_number(tolua_S, 5,&arg3, "AECollider:updateVerticesSync");

        ok &= luaval_to_number(tolua_S, 6,&arg4, "AECollider:updateVerticesSync");

        ok &= luaval_to_number(tolua_S, 7,&arg5, "AECollider:updateVerticesSync");

        ok &= luaval_to_number(tolua_S, 8,&arg6, "AECollider:updateVerticesSync");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_collision_AECollider_updateVerticesSync'", nullptr);
            return 0;
        }
        cobj->updateVerticesSync(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AECollider:updateVerticesSync",argc, 7);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_collision_AECollider_updateVerticesSync'.",&tolua_err);
#endif

    return 0;
}
int lua_collision_AECollider_getType(lua_State* tolua_S)
{
    int argc = 0;
    ae::AECollider* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AECollider",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AECollider*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_collision_AECollider_getType'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_collision_AECollider_getType'", nullptr);
            return 0;
        }
        const unsigned int& ret = cobj->getType();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AECollider:getType",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_collision_AECollider_getType'.",&tolua_err);
#endif

    return 0;
}
int lua_collision_AECollider_update(lua_State* tolua_S)
{
    int argc = 0;
    ae::AECollider* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AECollider",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AECollider*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_collision_AECollider_update'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "AECollider:update");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_collision_AECollider_update'", nullptr);
            return 0;
        }
        cobj->update(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AECollider:update",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_collision_AECollider_update'.",&tolua_err);
#endif

    return 0;
}
int lua_collision_AECollider_updateVertices(lua_State* tolua_S)
{
    int argc = 0;
    ae::AECollider* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AECollider",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AECollider*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_collision_AECollider_updateVertices'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        ae::AESpine* arg0;
        double arg1;

        ok &= luaval_to_object<ae::AESpine>(tolua_S, 2, "AESpine",&arg0, "AECollider:updateVertices");

        ok &= luaval_to_number(tolua_S, 3,&arg1, "AECollider:updateVertices");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_collision_AECollider_updateVertices'", nullptr);
            return 0;
        }
        cobj->updateVertices(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AECollider:updateVertices",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_collision_AECollider_updateVertices'.",&tolua_err);
#endif

    return 0;
}
int lua_collision_AECollider_setTag(lua_State* tolua_S)
{
    int argc = 0;
    ae::AECollider* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AECollider",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AECollider*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_collision_AECollider_setTag'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "AECollider:setTag");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_collision_AECollider_setTag'", nullptr);
            return 0;
        }
        cobj->setTag(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AECollider:setTag",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_collision_AECollider_setTag'.",&tolua_err);
#endif

    return 0;
}
int lua_collision_AECollider_release(lua_State* tolua_S)
{
    int argc = 0;
    ae::AECollider* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AECollider",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AECollider*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_collision_AECollider_release'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_collision_AECollider_release'", nullptr);
            return 0;
        }
        cobj->release();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AECollider:release",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_collision_AECollider_release'.",&tolua_err);
#endif

    return 0;
}
int lua_collision_AECollider_getTag(lua_State* tolua_S)
{
    int argc = 0;
    ae::AECollider* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AECollider",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AECollider*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_collision_AECollider_getTag'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_collision_AECollider_getTag'", nullptr);
            return 0;
        }
        const int& ret = cobj->getTag();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AECollider:getTag",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_collision_AECollider_getTag'.",&tolua_err);
#endif

    return 0;
}
int lua_collision_AECollider_setType(lua_State* tolua_S)
{
    int argc = 0;
    ae::AECollider* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AECollider",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AECollider*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_collision_AECollider_setType'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned int arg0;

        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "AECollider:setType");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_collision_AECollider_setType'", nullptr);
            return 0;
        }
        cobj->setType(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AECollider:setType",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_collision_AECollider_setType'.",&tolua_err);
#endif

    return 0;
}
int lua_collision_AECollider_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AECollider",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 5)
    {
        ae::AECollision* arg0;
        unsigned int arg1;
        int arg2;
        unsigned short arg3;
        unsigned short arg4;
        ok &= luaval_to_object<ae::AECollision>(tolua_S, 2, "AECollision",&arg0, "AECollider:create");
        ok &= luaval_to_uint32(tolua_S, 3,&arg1, "AECollider:create");
        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "AECollider:create");
        ok &= luaval_to_ushort(tolua_S, 5, &arg3, "AECollider:create");
        ok &= luaval_to_ushort(tolua_S, 6, &arg4, "AECollider:create");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_collision_AECollider_create'", nullptr);
            return 0;
        }
        ae::AECollider* ret = ae::AECollider::create(arg0, arg1, arg2, arg3, arg4);
        object_to_luaval<ae::AECollider>(tolua_S, "AECollider",(ae::AECollider*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AECollider:create",argc, 5);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_collision_AECollider_create'.",&tolua_err);
#endif
    return 0;
}
int lua_collision_AECollider_constructor(lua_State* tolua_S)
{
    int argc = 0;
    ae::AECollider* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 1) {
            ae::AECollision* arg0;
            ok &= luaval_to_object<ae::AECollision>(tolua_S, 2, "AECollision",&arg0, "AECollider:AECollider");

            if (!ok) { break; }
            cobj = new ae::AECollider(arg0);
            tolua_pushusertype(tolua_S,(void*)cobj,"AECollider");
            tolua_register_gc(tolua_S,lua_gettop(tolua_S));
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 0) {
            cobj = new ae::AECollider();
            tolua_pushusertype(tolua_S,(void*)cobj,"AECollider");
            tolua_register_gc(tolua_S,lua_gettop(tolua_S));
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 5) {
            ae::AECollision* arg0;
            ok &= luaval_to_object<ae::AECollision>(tolua_S, 2, "AECollision",&arg0, "AECollider:AECollider");

            if (!ok) { break; }
            unsigned int arg1;
            ok &= luaval_to_uint32(tolua_S, 3,&arg1, "AECollider:AECollider");

            if (!ok) { break; }
            int arg2;
            ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "AECollider:AECollider");

            if (!ok) { break; }
            unsigned short arg3;
            ok &= luaval_to_ushort(tolua_S, 5, &arg3, "AECollider:AECollider");

            if (!ok) { break; }
            unsigned short arg4;
            ok &= luaval_to_ushort(tolua_S, 6, &arg4, "AECollider:AECollider");

            if (!ok) { break; }
            cobj = new ae::AECollider(arg0, arg1, arg2, arg3, arg4);
            tolua_pushusertype(tolua_S,(void*)cobj,"AECollider");
            tolua_register_gc(tolua_S,lua_gettop(tolua_S));
            return 1;
        }
    }while(0);
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "AECollider:AECollider",argc, 5);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_collision_AECollider_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_collision_AECollider_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (AECollider)");
    return 0;
}

int lua_register_collision_AECollider(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"AECollider");
    tolua_cclass(tolua_S,"AECollider","AECollider","",nullptr);

    tolua_beginmodule(tolua_S,"AECollider");
        tolua_function(tolua_S,"new",lua_collision_AECollider_constructor);
        tolua_function(tolua_S,"updateVerticesSync",lua_collision_AECollider_updateVerticesSync);
        tolua_function(tolua_S,"getType",lua_collision_AECollider_getType);
        tolua_function(tolua_S,"update",lua_collision_AECollider_update);
        tolua_function(tolua_S,"updateVertices",lua_collision_AECollider_updateVertices);
        tolua_function(tolua_S,"setTag",lua_collision_AECollider_setTag);
        tolua_function(tolua_S,"release",lua_collision_AECollider_release);
        tolua_function(tolua_S,"getTag",lua_collision_AECollider_getTag);
        tolua_function(tolua_S,"setType",lua_collision_AECollider_setType);
        tolua_function(tolua_S,"create", lua_collision_AECollider_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ae::AECollider).name();
    g_luaType[typeName] = "AECollider";
    g_typeCast["AECollider"] = "AECollider";
    return 1;
}
TOLUA_API int register_all_collision(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,nullptr,0);
	tolua_beginmodule(tolua_S,nullptr);

	lua_register_collision_AEColliderData(tolua_S);
	lua_register_collision_AECollision(tolua_S);
	lua_register_collision_AECollider(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

