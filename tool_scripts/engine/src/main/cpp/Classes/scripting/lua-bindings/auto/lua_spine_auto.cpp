#include "scripting/lua-bindings/auto/lua_spine_auto.hpp"
#include "external/spine/AESpine.h"
#include "external/spine/AESpineCache.h"
#include "external/spine/AESpineColliderManager.h"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"

int lua_spine_AESpine_replaceSkinForSlotData(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESpine* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AESpine",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AESpine*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_spine_AESpine_replaceSkinForSlotData'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        ae::AESpine* arg0;
        const char* arg1;
        const char* arg2;

        ok &= luaval_to_object<ae::AESpine>(tolua_S, 2, "AESpine",&arg0, "AESpine:replaceSkinForSlotData");

        std::string arg1_tmp; ok &= luaval_to_std_string(tolua_S, 3, &arg1_tmp, "AESpine:replaceSkinForSlotData"); arg1 = arg1_tmp.c_str();

        std::string arg2_tmp; ok &= luaval_to_std_string(tolua_S, 4, &arg2_tmp, "AESpine:replaceSkinForSlotData"); arg2 = arg2_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpine_replaceSkinForSlotData'", nullptr);
            return 0;
        }
        cobj->replaceSkinForSlotData(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESpine:replaceSkinForSlotData",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpine_replaceSkinForSlotData'.",&tolua_err);
#endif

    return 0;
}
int lua_spine_AESpine_recoveryGLProgramState(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESpine* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AESpine",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AESpine*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_spine_AESpine_recoveryGLProgramState'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpine_recoveryGLProgramState'", nullptr);
            return 0;
        }
        cobj->recoveryGLProgramState();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESpine:recoveryGLProgramState",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpine_recoveryGLProgramState'.",&tolua_err);
#endif

    return 0;
}
int lua_spine_AESpine_replaceAtlas(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESpine* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AESpine",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AESpine*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_spine_AESpine_replaceAtlas'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
		spAtlas* arg0 = (spAtlas*)tolua_tousertype(tolua_S, 2, 0);
		if (!arg0)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpine_replaceAtlas'", nullptr);
            return 0;
        }
        cobj->replaceAtlas(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESpine:replaceAtlas",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpine_replaceAtlas'.",&tolua_err);
#endif

    return 0;
}
int lua_spine_AESpine_getPause(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESpine* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AESpine",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AESpine*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_spine_AESpine_getPause'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpine_getPause'", nullptr);
            return 0;
        }
        const bool& ret = cobj->getPause();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESpine:getPause",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpine_getPause'.",&tolua_err);
#endif

    return 0;
}
int lua_spine_AESpine_getDuration(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESpine* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AESpine",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AESpine*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_spine_AESpine_getDuration'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpine_getDuration'", nullptr);
            return 0;
        }
        double ret = cobj->getDuration();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESpine:getDuration",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpine_getDuration'.",&tolua_err);
#endif

    return 0;
}
int lua_spine_AESpine_setPause(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESpine* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AESpine",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AESpine*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_spine_AESpine_setPause'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0, "AESpine:setPause");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpine_setPause'", nullptr);
            return 0;
        }
        cobj->setPause(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESpine:setPause",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpine_setPause'.",&tolua_err);
#endif

    return 0;
}
int lua_spine_AESpine_changeAnimation(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESpine* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AESpine",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AESpine*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_spine_AESpine_changeAnimation'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        int arg0;
        std::string arg1;
        bool arg2;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "AESpine:changeAnimation");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "AESpine:changeAnimation");

        ok &= luaval_to_boolean(tolua_S, 4,&arg2, "AESpine:changeAnimation");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpine_changeAnimation'", nullptr);
            return 0;
        }
        double ret = cobj->changeAnimation(arg0, arg1, arg2);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESpine:changeAnimation",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpine_changeAnimation'.",&tolua_err);
#endif

    return 0;
}
int lua_spine_AESpine_setAnimation(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESpine* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AESpine",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AESpine*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_spine_AESpine_setAnimation'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        int arg0;
        std::string arg1;
        bool arg2;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "AESpine:setAnimation");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "AESpine:setAnimation");

        ok &= luaval_to_boolean(tolua_S, 4,&arg2, "AESpine:setAnimation");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpine_setAnimation'", nullptr);
            return 0;
        }
        spTrackEntry* ret = cobj->setAnimation(arg0, arg1, arg2);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESpine:setAnimation",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpine_setAnimation'.",&tolua_err);
#endif

    return 0;
}
int lua_spine_AESpine_appendAtlas(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AESpine",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::vector<std::string> arg0;
        ok &= luaval_to_std_vector_string(tolua_S, 2, &arg0, "AESpine:appendAtlas");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpine_appendAtlas'", nullptr);
            return 0;
        }
        spAtlas* ret = ae::AESpine::appendAtlas(arg0);
		object_to_luaval<spAtlas>(tolua_S, "spAtlas", (spAtlas*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AESpine:appendAtlas",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpine_appendAtlas'.",&tolua_err);
#endif
    return 0;
}
int lua_spine_AESpine_slowMotion(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AESpine",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        double arg0;
        ok &= luaval_to_number(tolua_S, 2,&arg0, "AESpine:slowMotion");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpine_slowMotion'", nullptr);
            return 0;
        }
        ae::AESpine::slowMotion(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AESpine:slowMotion",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpine_slowMotion'.",&tolua_err);
#endif
    return 0;
}
int lua_spine_AESpine_constructor(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESpine* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 1) {
            bool arg0;
            ok &= luaval_to_boolean(tolua_S, 2,&arg0, "AESpine:AESpine");

            if (!ok) { break; }
            cobj = new ae::AESpine(arg0);
            cobj->autorelease();
            int ID =  (int)cobj->_ID ;
            int* luaID =  &cobj->_luaID ;
            toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"AESpine");
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 0) {
            cobj = new ae::AESpine();
            cobj->autorelease();
            int ID =  (int)cobj->_ID ;
            int* luaID =  &cobj->_luaID ;
            toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"AESpine");
            return 1;
        }
    }while(0);
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "AESpine:AESpine",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpine_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_spine_AESpine_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (AESpine)");
    return 0;
}

int lua_register_spine_AESpine(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"AESpine");
	tolua_usertype(tolua_S,"spAtlas");
    tolua_cclass(tolua_S,"AESpine","AESpine","sp.SkeletonAnimation",nullptr);

    tolua_beginmodule(tolua_S,"AESpine");
        tolua_function(tolua_S,"new",lua_spine_AESpine_constructor);
        tolua_function(tolua_S,"replaceSkinForSlotData",lua_spine_AESpine_replaceSkinForSlotData);
        tolua_function(tolua_S,"recoveryGLProgramState",lua_spine_AESpine_recoveryGLProgramState);
        tolua_function(tolua_S,"replaceAtlas",lua_spine_AESpine_replaceAtlas);
        tolua_function(tolua_S,"getPause",lua_spine_AESpine_getPause);
        tolua_function(tolua_S,"getDuration",lua_spine_AESpine_getDuration);
        tolua_function(tolua_S,"setPause",lua_spine_AESpine_setPause);
        tolua_function(tolua_S,"changeAnimationLua",lua_spine_AESpine_changeAnimation);
        tolua_function(tolua_S,"setAnimationLua",lua_spine_AESpine_setAnimation);
        tolua_function(tolua_S,"appendAtlas", lua_spine_AESpine_appendAtlas);
        tolua_function(tolua_S,"slowMotion", lua_spine_AESpine_slowMotion);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ae::AESpine).name();
    g_luaType[typeName] = "AESpine";
    g_typeCast["AESpine"] = "AESpine";
    return 1;
}

int lua_spine_AESpineCache_preloadWithJsonFile(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESpineCache* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AESpineCache",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AESpineCache*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_spine_AESpineCache_preloadWithJsonFile'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        std::string arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AESpineCache:preloadWithJsonFile");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "AESpineCache:preloadWithJsonFile");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpineCache_preloadWithJsonFile'", nullptr);
            return 0;
        }
        cobj->preloadWithJsonFile(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    if (argc == 3) 
    {
        std::string arg0;
        std::string arg1;
        double arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AESpineCache:preloadWithJsonFile");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "AESpineCache:preloadWithJsonFile");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "AESpineCache:preloadWithJsonFile");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpineCache_preloadWithJsonFile'", nullptr);
            return 0;
        }
        cobj->preloadWithJsonFile(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    if (argc == 4) 
    {
        std::string arg0;
        std::string arg1;
        double arg2;
        bool arg3;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AESpineCache:preloadWithJsonFile");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "AESpineCache:preloadWithJsonFile");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "AESpineCache:preloadWithJsonFile");

        ok &= luaval_to_boolean(tolua_S, 5,&arg3, "AESpineCache:preloadWithJsonFile");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpineCache_preloadWithJsonFile'", nullptr);
            return 0;
        }
        cobj->preloadWithJsonFile(arg0, arg1, arg2, arg3);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESpineCache:preloadWithJsonFile",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpineCache_preloadWithJsonFile'.",&tolua_err);
#endif

    return 0;
}
int lua_spine_AESpineCache_createWithBinaryFile(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESpineCache* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AESpineCache",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (ae::AESpineCache*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_spine_AESpineCache_createWithBinaryFile'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 2) {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AESpineCache:createWithBinaryFile");

            if (!ok) { break; }
            std::string arg1;
            ok &= luaval_to_std_string(tolua_S, 3,&arg1, "AESpineCache:createWithBinaryFile");

            if (!ok) { break; }
            ae::AESpine* ret = cobj->createWithBinaryFile(arg0, arg1);
            object_to_luaval<ae::AESpine>(tolua_S, "AESpine",(ae::AESpine*)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 3) {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AESpineCache:createWithBinaryFile");

            if (!ok) { break; }
            std::string arg1;
            ok &= luaval_to_std_string(tolua_S, 3,&arg1, "AESpineCache:createWithBinaryFile");

            if (!ok) { break; }
            double arg2;
            ok &= luaval_to_number(tolua_S, 4,&arg2, "AESpineCache:createWithBinaryFile");

            if (!ok) { break; }
            ae::AESpine* ret = cobj->createWithBinaryFile(arg0, arg1, arg2);
            object_to_luaval<ae::AESpine>(tolua_S, "AESpine",(ae::AESpine*)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 4) {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AESpineCache:createWithBinaryFile");

            if (!ok) { break; }
            std::string arg1;
            ok &= luaval_to_std_string(tolua_S, 3,&arg1, "AESpineCache:createWithBinaryFile");

            if (!ok) { break; }
            double arg2;
            ok &= luaval_to_number(tolua_S, 4,&arg2, "AESpineCache:createWithBinaryFile");

            if (!ok) { break; }
            bool arg3;
            ok &= luaval_to_boolean(tolua_S, 5,&arg3, "AESpineCache:createWithBinaryFile");

            if (!ok) { break; }
            ae::AESpine* ret = cobj->createWithBinaryFile(arg0, arg1, arg2, arg3);
            object_to_luaval<ae::AESpine>(tolua_S, "AESpine",(ae::AESpine*)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 2) {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AESpineCache:createWithBinaryFile");

            if (!ok) { break; }
			spAtlas* arg1 = (spAtlas*)tolua_tousertype(tolua_S, 3, 0);

            if (!ok) { break; }
            ae::AESpine* ret = cobj->createWithBinaryFile(arg0, arg1);
            object_to_luaval<ae::AESpine>(tolua_S, "AESpine",(ae::AESpine*)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 3) {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AESpineCache:createWithBinaryFile");

            if (!ok) { break; }
			spAtlas* arg1 = (spAtlas*)tolua_tousertype(tolua_S, 3, 0);

            if (!ok) { break; }
            double arg2;
            ok &= luaval_to_number(tolua_S, 4,&arg2, "AESpineCache:createWithBinaryFile");

            if (!ok) { break; }
            ae::AESpine* ret = cobj->createWithBinaryFile(arg0, arg1, arg2);
            object_to_luaval<ae::AESpine>(tolua_S, "AESpine",(ae::AESpine*)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "AESpineCache:createWithBinaryFile",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpineCache_createWithBinaryFile'.",&tolua_err);
#endif

    return 0;
}
int lua_spine_AESpineCache_clearSpine(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESpineCache* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AESpineCache",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AESpineCache*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_spine_AESpineCache_clearSpine'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpineCache_clearSpine'", nullptr);
            return 0;
        }
        cobj->clearSpine();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESpineCache:clearSpine",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpineCache_clearSpine'.",&tolua_err);
#endif

    return 0;
}
int lua_spine_AESpineCache_preloadWithBinaryFile(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESpineCache* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AESpineCache",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AESpineCache*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_spine_AESpineCache_preloadWithBinaryFile'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        std::string arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AESpineCache:preloadWithBinaryFile");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "AESpineCache:preloadWithBinaryFile");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpineCache_preloadWithBinaryFile'", nullptr);
            return 0;
        }
        cobj->preloadWithBinaryFile(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    if (argc == 3) 
    {
        std::string arg0;
        std::string arg1;
        double arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AESpineCache:preloadWithBinaryFile");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "AESpineCache:preloadWithBinaryFile");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "AESpineCache:preloadWithBinaryFile");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpineCache_preloadWithBinaryFile'", nullptr);
            return 0;
        }
        cobj->preloadWithBinaryFile(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    if (argc == 4) 
    {
        std::string arg0;
        std::string arg1;
        double arg2;
        bool arg3;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AESpineCache:preloadWithBinaryFile");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "AESpineCache:preloadWithBinaryFile");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "AESpineCache:preloadWithBinaryFile");

        ok &= luaval_to_boolean(tolua_S, 5,&arg3, "AESpineCache:preloadWithBinaryFile");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpineCache_preloadWithBinaryFile'", nullptr);
            return 0;
        }
        cobj->preloadWithBinaryFile(arg0, arg1, arg2, arg3);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESpineCache:preloadWithBinaryFile",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpineCache_preloadWithBinaryFile'.",&tolua_err);
#endif

    return 0;
}
int lua_spine_AESpineCache_recoverySpine(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESpineCache* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AESpineCache",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AESpineCache*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_spine_AESpineCache_recoverySpine'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        ae::AESpine* arg0;

        ok &= luaval_to_object<ae::AESpine>(tolua_S, 2, "AESpine",&arg0, "AESpineCache:recoverySpine");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpineCache_recoverySpine'", nullptr);
            return 0;
        }
        cobj->recoverySpine(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESpineCache:recoverySpine",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpineCache_recoverySpine'.",&tolua_err);
#endif

    return 0;
}
int lua_spine_AESpineCache_clearPreload(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESpineCache* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AESpineCache",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AESpineCache*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_spine_AESpineCache_clearPreload'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AESpineCache:clearPreload");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpineCache_clearPreload'", nullptr);
            return 0;
        }
        cobj->clearPreload(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESpineCache:clearPreload",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpineCache_clearPreload'.",&tolua_err);
#endif

    return 0;
}
int lua_spine_AESpineCache_createWithJsonFile(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESpineCache* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AESpineCache",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (ae::AESpineCache*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_spine_AESpineCache_createWithJsonFile'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 2) {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AESpineCache:createWithJsonFile");

            if (!ok) { break; }
            std::string arg1;
            ok &= luaval_to_std_string(tolua_S, 3,&arg1, "AESpineCache:createWithJsonFile");

            if (!ok) { break; }
            ae::AESpine* ret = cobj->createWithJsonFile(arg0, arg1);
            object_to_luaval<ae::AESpine>(tolua_S, "AESpine",(ae::AESpine*)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 3) {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AESpineCache:createWithJsonFile");

            if (!ok) { break; }
            std::string arg1;
            ok &= luaval_to_std_string(tolua_S, 3,&arg1, "AESpineCache:createWithJsonFile");

            if (!ok) { break; }
            double arg2;
            ok &= luaval_to_number(tolua_S, 4,&arg2, "AESpineCache:createWithJsonFile");

            if (!ok) { break; }
            ae::AESpine* ret = cobj->createWithJsonFile(arg0, arg1, arg2);
            object_to_luaval<ae::AESpine>(tolua_S, "AESpine",(ae::AESpine*)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 4) {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AESpineCache:createWithJsonFile");

            if (!ok) { break; }
            std::string arg1;
            ok &= luaval_to_std_string(tolua_S, 3,&arg1, "AESpineCache:createWithJsonFile");

            if (!ok) { break; }
            double arg2;
            ok &= luaval_to_number(tolua_S, 4,&arg2, "AESpineCache:createWithJsonFile");

            if (!ok) { break; }
            bool arg3;
            ok &= luaval_to_boolean(tolua_S, 5,&arg3, "AESpineCache:createWithJsonFile");

            if (!ok) { break; }
            ae::AESpine* ret = cobj->createWithJsonFile(arg0, arg1, arg2, arg3);
            object_to_luaval<ae::AESpine>(tolua_S, "AESpine",(ae::AESpine*)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 2) {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AESpineCache:createWithJsonFile");

            if (!ok) { break; }
			spAtlas* arg1 = (spAtlas*)tolua_tousertype(tolua_S, 3, 0);

            if (!ok) { break; }
            ae::AESpine* ret = cobj->createWithJsonFile(arg0, arg1);
            object_to_luaval<ae::AESpine>(tolua_S, "AESpine",(ae::AESpine*)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 3) {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AESpineCache:createWithJsonFile");

            if (!ok) { break; }
			spAtlas* arg1 = (spAtlas*)tolua_tousertype(tolua_S, 3, 0);

            if (!ok) { break; }
            double arg2;
            ok &= luaval_to_number(tolua_S, 4,&arg2, "AESpineCache:createWithJsonFile");

            if (!ok) { break; }
            ae::AESpine* ret = cobj->createWithJsonFile(arg0, arg1, arg2);
            object_to_luaval<ae::AESpine>(tolua_S, "AESpine",(ae::AESpine*)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "AESpineCache:createWithJsonFile",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpineCache_createWithJsonFile'.",&tolua_err);
#endif

    return 0;
}
int lua_spine_AESpineCache_getInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AESpineCache",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpineCache_getInstance'", nullptr);
            return 0;
        }
        ae::AESpineCache* ret = ae::AESpineCache::getInstance();
        object_to_luaval<ae::AESpineCache>(tolua_S, "AESpineCache",(ae::AESpineCache*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AESpineCache:getInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpineCache_getInstance'.",&tolua_err);
#endif
    return 0;
}
static int lua_spine_AESpineCache_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (AESpineCache)");
    return 0;
}

int lua_register_spine_AESpineCache(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"AESpineCache");
    tolua_cclass(tolua_S,"AESpineCache","AESpineCache","cc.Ref",nullptr);

    tolua_beginmodule(tolua_S,"AESpineCache");
        tolua_function(tolua_S,"preloadWithJsonFile",lua_spine_AESpineCache_preloadWithJsonFile);
        tolua_function(tolua_S,"createWithBinaryFile",lua_spine_AESpineCache_createWithBinaryFile);
        tolua_function(tolua_S,"clearSpine",lua_spine_AESpineCache_clearSpine);
        tolua_function(tolua_S,"preloadWithBinaryFile",lua_spine_AESpineCache_preloadWithBinaryFile);
        tolua_function(tolua_S,"recoverySpine",lua_spine_AESpineCache_recoverySpine);
        tolua_function(tolua_S,"clearPreload",lua_spine_AESpineCache_clearPreload);
        tolua_function(tolua_S,"createWithJsonFile",lua_spine_AESpineCache_createWithJsonFile);
        tolua_function(tolua_S,"getInstance", lua_spine_AESpineCache_getInstance);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ae::AESpineCache).name();
    g_luaType[typeName] = "AESpineCache";
    g_typeCast["AESpineCache"] = "AESpineCache";
    return 1;
}

int lua_spine_AESpineColliderManager_getVertices(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESpineColliderManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AESpineColliderManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AESpineColliderManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_spine_AESpineColliderManager_getVertices'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 5) 
    {
        std::string arg0;
        std::string arg1;
        double arg2;
        unsigned int* arg3;
        float* arg4;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AESpineColliderManager:getVertices");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "AESpineColliderManager:getVertices");

        ok &= luaval_to_number(tolua_S, 4,&arg2, "AESpineColliderManager:getVertices");

        #pragma warning NO CONVERSION TO NATIVE FOR unsigned int*
		ok = false;

        #pragma warning NO CONVERSION TO NATIVE FOR float*
		ok = false;
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpineColliderManager_getVertices'", nullptr);
            return 0;
        }
        cobj->getVertices(arg0, arg1, arg2, arg3, arg4);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESpineColliderManager:getVertices",argc, 5);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpineColliderManager_getVertices'.",&tolua_err);
#endif

    return 0;
}

int lua_spine_AESpineColliderManager_writeSpineColliderToFile(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESpineColliderManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AESpineColliderManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AESpineColliderManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_spine_AESpineColliderManager_writeSpineColliderToFile'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        ae::AESpineColliderData* arg0;
        std::string arg1;
        std::string arg2;

        ok &= luaval_to_object<ae::AESpineColliderData>(tolua_S, 2, "AESpineColliderData",&arg0, "AESpineColliderManager:writeSpineColliderToFile");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "AESpineColliderManager:writeSpineColliderToFile");

        ok &= luaval_to_std_string(tolua_S, 4,&arg2, "AESpineColliderManager:writeSpineColliderToFile");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpineColliderManager_writeSpineColliderToFile'", nullptr);
            return 0;
        }
        bool ret = cobj->writeSpineColliderToFile(arg0, arg1, arg2);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESpineColliderManager:writeSpineColliderToFile",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpineColliderManager_writeSpineColliderToFile'.",&tolua_err);
#endif

    return 0;
}
int lua_spine_AESpineColliderManager_readSpineColliderFromFile(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESpineColliderManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AESpineColliderManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AESpineColliderManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_spine_AESpineColliderManager_readSpineColliderFromFile'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AESpineColliderManager:readSpineColliderFromFile");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpineColliderManager_readSpineColliderFromFile'", nullptr);
            return 0;
        }
        cobj->readSpineColliderFromFile(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESpineColliderManager:readSpineColliderFromFile",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpineColliderManager_readSpineColliderFromFile'.",&tolua_err);
#endif

    return 0;
}
int lua_spine_AESpineColliderManager_destroyInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AESpineColliderManager",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpineColliderManager_destroyInstance'", nullptr);
            return 0;
        }
        ae::AESpineColliderManager::destroyInstance();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AESpineColliderManager:destroyInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpineColliderManager_destroyInstance'.",&tolua_err);
#endif
    return 0;
}
int lua_spine_AESpineColliderManager_getInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AESpineColliderManager",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_spine_AESpineColliderManager_getInstance'", nullptr);
            return 0;
        }
        ae::AESpineColliderManager* ret = ae::AESpineColliderManager::getInstance();
        object_to_luaval<ae::AESpineColliderManager>(tolua_S, "AESpineColliderManager",(ae::AESpineColliderManager*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AESpineColliderManager:getInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_spine_AESpineColliderManager_getInstance'.",&tolua_err);
#endif
    return 0;
}
static int lua_spine_AESpineColliderManager_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (AESpineColliderManager)");
    return 0;
}

int lua_register_spine_AESpineColliderManager(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"AESpineColliderManager");
    tolua_cclass(tolua_S,"AESpineColliderManager","AESpineColliderManager","",nullptr);

    tolua_beginmodule(tolua_S,"AESpineColliderManager");
        tolua_function(tolua_S,"getVertices",lua_spine_AESpineColliderManager_getVertices);
        tolua_function(tolua_S,"writeSpineColliderToFile",lua_spine_AESpineColliderManager_writeSpineColliderToFile);
        tolua_function(tolua_S,"readSpineColliderFromFile",lua_spine_AESpineColliderManager_readSpineColliderFromFile);
        tolua_function(tolua_S,"destroyInstance", lua_spine_AESpineColliderManager_destroyInstance);
        tolua_function(tolua_S,"getInstance", lua_spine_AESpineColliderManager_getInstance);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ae::AESpineColliderManager).name();
    g_luaType[typeName] = "AESpineColliderManager";
    g_typeCast["AESpineColliderManager"] = "AESpineColliderManager";
    return 1;
}
TOLUA_API int register_all_spine(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,nullptr,0);
	tolua_beginmodule(tolua_S,nullptr);

	lua_register_spine_AESpineColliderManager(tolua_S);
	lua_register_spine_AESpine(tolua_S);
	lua_register_spine_AESpineCache(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

