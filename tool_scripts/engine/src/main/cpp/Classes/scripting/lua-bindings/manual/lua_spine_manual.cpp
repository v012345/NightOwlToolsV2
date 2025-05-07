#include "scripting/lua-bindings/manual/lua_collision_manual.hpp"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"
#include "scripting/lua-bindings/manual/cocos2d/LuaScriptHandlerMgr.h"
#include "scripting/lua-bindings/manual/CCLuaValue.h"
#include "external/spine/AESpineCache.h"
#include "external/spine/AESpine.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"

int executeSpineCallBack(ae::AESpine* pSpine, int handler)
{
	if (nullptr == pSpine || 0 == handler)
        return 0;
    
    LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
    if (nullptr == stack)
        return 0;
    
    lua_State* L = LuaEngine::getInstance()->getLuaStack()->getLuaState();
    if (nullptr == L)
        return 0;
    
    int ret = 0;
	stack->pushObject(pSpine, typeid(ae::AESpine).name());
    ret = stack->executeFunctionByHandler(handler, 1);

	ScriptHandlerMgr::getInstance()->removeObjectHandler((void*)pSpine, ScriptHandlerMgr::HandlerType(10003));

    return ret;
}

int tolua_spine_AESpineCache_createWithJsonFileByThread00(lua_State* tolua_S)
{
	int argc = 0;
	ae::AESpineCache* cobj = nullptr;
	bool ok = true;
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertype(tolua_S, 1, "AESpineCache", 0, &tolua_err)) goto tolua_lerror;
#endif
	cobj = (ae::AESpineCache*)tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_spine_AESpineCache_createWithJsonFileByThread'", nullptr);
		return 0;
	}
#endif
	argc = lua_gettop(tolua_S) - 1;
	do{
		if (argc == 2) {
			std::string arg0;
			ok &= luaval_to_std_string(tolua_S, 2, &arg0, "AESpineCache:createWithJsonFileByThread");

			if (!ok) { break; }
			std::string arg1;
			ok &= luaval_to_std_string(tolua_S, 3, &arg1, "AESpineCache:createWithJsonFileByThread");

			if (!ok) { break; }
			ae::AESpine* ret = cobj->createWithJsonFileByThread(arg0, arg1);
			object_to_luaval<ae::AESpine>(tolua_S, "AESpine", (ae::AESpine*)ret);
			return 1;
		}
	} while (0);
	ok = true;
	do{
		if (argc == 3) {
			std::string arg0;
			ok &= luaval_to_std_string(tolua_S, 2, &arg0, "AESpineCache:createWithJsonFileByThread");

			if (!ok) { break; }
			std::string arg1;
			ok &= luaval_to_std_string(tolua_S, 3, &arg1, "AESpineCache:createWithJsonFileByThread");

			if (!ok) { break; }
			double arg2;
			ok &= luaval_to_number(tolua_S, 4, &arg2, "AESpineCache:createWithJsonFileByThread");

			if (!ok) { break; }
			ae::AESpine* ret = cobj->createWithJsonFileByThread(arg0, arg1, arg2);
			object_to_luaval<ae::AESpine>(tolua_S, "AESpine", (ae::AESpine*)ret);
			return 1;
		}
	} while (0);
	ok = true;
	do{
		if (argc == 4) {
			std::string arg0;
			ok &= luaval_to_std_string(tolua_S, 2, &arg0, "AESpineCache:createWithJsonFileByThread");

			if (!ok) { break; }
			std::string arg1;
			ok &= luaval_to_std_string(tolua_S, 3, &arg1, "AESpineCache:createWithJsonFileByThread");

			if (!ok) { break; }
			double arg2;
			ok &= luaval_to_number(tolua_S, 4, &arg2, "AESpineCache:createWithJsonFileByThread");

			if (!ok) { break; }
			int handler = (toluafix_ref_function(tolua_S, 5, 0));
			std::function<void(ae::AESpine *)> arg3([=](ae::AESpine* pSpine) {
				executeSpineCallBack(pSpine, handler);
			});
			
			if (!ok) { break; }
			ae::AESpine* ret = cobj->createWithJsonFileByThread(arg0, arg1, arg2, arg3);
			object_to_luaval<ae::AESpine>(tolua_S, "AESpine", (ae::AESpine*)ret);
			ScriptHandlerMgr::getInstance()->addObjectHandler((void*)ret, handler, ScriptHandlerMgr::HandlerType(10003));
			return 1;
		}
	} while (0);
	ok = true;
	do{
		if (argc == 5) {
			std::string arg0;
			ok &= luaval_to_std_string(tolua_S, 2, &arg0, "AESpineCache:createWithJsonFileByThread");

			if (!ok) { break; }
			std::string arg1;
			ok &= luaval_to_std_string(tolua_S, 3, &arg1, "AESpineCache:createWithJsonFileByThread");

			if (!ok) { break; }
			double arg2;
			ok &= luaval_to_number(tolua_S, 4, &arg2, "AESpineCache:createWithJsonFileByThread");

			if (!ok) { break; }
			int handler = (toluafix_ref_function(tolua_S, 5, 0));
			std::function<void(ae::AESpine *)> arg3([=](ae::AESpine* pSpine) {
				executeSpineCallBack(pSpine, handler);
			});
			
			if (!ok) { break; }
			bool arg4;
			ok &= luaval_to_boolean(tolua_S, 6, &arg4, "AESpineCache:createWithJsonFileByThread");

			if (!ok) { break; }
			ae::AESpine* ret = cobj->createWithJsonFileByThread(arg0, arg1, arg2, arg3, arg4);
			object_to_luaval<ae::AESpine>(tolua_S, "AESpine", (ae::AESpine*)ret);
			ScriptHandlerMgr::getInstance()->addObjectHandler((void*)ret, handler, ScriptHandlerMgr::HandlerType(10003));
			return 1;
		}
	} while (0);
	ok = true;
	do{
		if (argc == 2) {
			std::string arg0;
			ok &= luaval_to_std_string(tolua_S, 2, &arg0, "AESpineCache:createWithJsonFileByThread");

			if (!ok) { break; }
			spAtlas* arg1 = (spAtlas*)tolua_tousertype(tolua_S, 3, 0);

			if (!ok) { break; }
			ae::AESpine* ret = cobj->createWithJsonFileByThread(arg0, arg1);
			object_to_luaval<ae::AESpine>(tolua_S, "AESpine", (ae::AESpine*)ret);
			return 1;
		}
	} while (0);
	ok = true;
	do{
		if (argc == 3) {
			std::string arg0;
			ok &= luaval_to_std_string(tolua_S, 2, &arg0, "AESpineCache:createWithJsonFileByThread");

			if (!ok) { break; }
			spAtlas* arg1 = (spAtlas*)tolua_tousertype(tolua_S, 3, 0);

			if (!ok) { break; }
			double arg2;
			ok &= luaval_to_number(tolua_S, 4, &arg2, "AESpineCache:createWithJsonFileByThread");

			if (!ok) { break; }
			ae::AESpine* ret = cobj->createWithJsonFileByThread(arg0, arg1, arg2);
			object_to_luaval<ae::AESpine>(tolua_S, "AESpine", (ae::AESpine*)ret);
			return 1;
		}
	} while (0);
	ok = true;
	do{
		if (argc == 4) {
			std::string arg0;
			ok &= luaval_to_std_string(tolua_S, 2, &arg0, "AESpineCache:createWithJsonFileByThread");

			if (!ok) { break; }
			spAtlas* arg1 = (spAtlas*)tolua_tousertype(tolua_S, 3, 0);

			if (!ok) { break; }
			double arg2;
			ok &= luaval_to_number(tolua_S, 4, &arg2, "AESpineCache:createWithJsonFileByThread");

			if (!ok) { break; }
			int handler = (toluafix_ref_function(tolua_S, 5, 0));
			std::function<void(ae::AESpine *)> arg3([=](ae::AESpine* pSpine) {
				executeSpineCallBack(pSpine, handler);
			});

			if (!ok) { break; }
			ae::AESpine* ret = cobj->createWithJsonFileByThread(arg0, arg1, arg2, arg3);
			object_to_luaval<ae::AESpine>(tolua_S, "AESpine", (ae::AESpine*)ret);
			ScriptHandlerMgr::getInstance()->addObjectHandler((void*)ret, handler, ScriptHandlerMgr::HandlerType(10003));
			return 1;
		}
	} while (0);
	ok = true;
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESpineCache:createWithJsonFileByThread", argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_spine_AESpineCache_createWithJsonFileByThread'.", &tolua_err);
#endif

	return 0;
}

int tolua_spine_AESpineCache_createWithBinaryFileByThread00(lua_State* tolua_S)
{
	int argc = 0;
	ae::AESpineCache* cobj = nullptr;
	bool ok = true;
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertype(tolua_S, 1, "AESpineCache", 0, &tolua_err)) goto tolua_lerror;
#endif
	cobj = (ae::AESpineCache*)tolua_tousertype(tolua_S, 1, 0);
#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_spine_AESpineCache_createWithBinaryFileByThread'", nullptr);
		return 0;
	}
#endif
	argc = lua_gettop(tolua_S) - 1;
	do{
		if (argc == 2) {
			std::string arg0;
			ok &= luaval_to_std_string(tolua_S, 2, &arg0, "AESpineCache:createWithBinaryFileByThread");

			if (!ok) { break; }
			std::string arg1;
			ok &= luaval_to_std_string(tolua_S, 3, &arg1, "AESpineCache:createWithBinaryFileByThread");

			if (!ok) { break; }
			ae::AESpine* ret = cobj->createWithBinaryFileByThread(arg0, arg1);
			object_to_luaval<ae::AESpine>(tolua_S, "AESpine", (ae::AESpine*)ret);
			return 1;
		}
	} while (0);
	ok = true;
	do{
		if (argc == 3) {
			std::string arg0;
			ok &= luaval_to_std_string(tolua_S, 2, &arg0, "AESpineCache:createWithBinaryFileByThread");

			if (!ok) { break; }
			std::string arg1;
			ok &= luaval_to_std_string(tolua_S, 3, &arg1, "AESpineCache:createWithBinaryFileByThread");

			if (!ok) { break; }
			double arg2;
			ok &= luaval_to_number(tolua_S, 4, &arg2, "AESpineCache:createWithBinaryFileByThread");

			if (!ok) { break; }
			ae::AESpine* ret = cobj->createWithBinaryFileByThread(arg0, arg1, arg2);
			object_to_luaval<ae::AESpine>(tolua_S, "AESpine", (ae::AESpine*)ret);
			return 1;
		}
	} while (0);
	ok = true;
	do{
		if (argc == 4) {
			std::string arg0;
			ok &= luaval_to_std_string(tolua_S, 2, &arg0, "AESpineCache:createWithBinaryFileByThread");

			if (!ok) { break; }
			std::string arg1;
			ok &= luaval_to_std_string(tolua_S, 3, &arg1, "AESpineCache:createWithBinaryFileByThread");

			if (!ok) { break; }
			double arg2;
			ok &= luaval_to_number(tolua_S, 4, &arg2, "AESpineCache:createWithBinaryFileByThread");

			if (!ok) { break; }
			int handler = (toluafix_ref_function(tolua_S, 5, 0));
			std::function<void(ae::AESpine *)> arg3([=](ae::AESpine* pSpine) {
				executeSpineCallBack(pSpine, handler);
			});

			if (!ok) { break; }
			ae::AESpine* ret = cobj->createWithBinaryFileByThread(arg0, arg1, arg2, arg3);
			object_to_luaval<ae::AESpine>(tolua_S, "AESpine", (ae::AESpine*)ret);
			ScriptHandlerMgr::getInstance()->addObjectHandler((void*)ret, handler, ScriptHandlerMgr::HandlerType(10003));
			return 1;
		}
	} while (0);
	ok = true;
	do{
		if (argc == 5) {
			std::string arg0;
			ok &= luaval_to_std_string(tolua_S, 2, &arg0, "AESpineCache:createWithBinaryFileByThread");

			if (!ok) { break; }
			std::string arg1;
			ok &= luaval_to_std_string(tolua_S, 3, &arg1, "AESpineCache:createWithBinaryFileByThread");

			if (!ok) { break; }
			double arg2;
			ok &= luaval_to_number(tolua_S, 4, &arg2, "AESpineCache:createWithBinaryFileByThread");

			if (!ok) { break; }
			int handler = (toluafix_ref_function(tolua_S, 5, 0));
			std::function<void(ae::AESpine *)> arg3([=](ae::AESpine* pSpine) {
				executeSpineCallBack(pSpine, handler);
			});

			if (!ok) { break; }
			bool arg4;
			ok &= luaval_to_boolean(tolua_S, 6, &arg4, "AESpineCache:createWithBinaryFileByThread");

			if (!ok) { break; }
			ae::AESpine* ret = cobj->createWithBinaryFileByThread(arg0, arg1, arg2, arg3, arg4);
			object_to_luaval<ae::AESpine>(tolua_S, "AESpine", (ae::AESpine*)ret);
			ScriptHandlerMgr::getInstance()->addObjectHandler((void*)ret, handler, ScriptHandlerMgr::HandlerType(10003));
			return 1;
		}
	} while (0);
	ok = true;
	do{
		if (argc == 2) {
			std::string arg0;
			ok &= luaval_to_std_string(tolua_S, 2, &arg0, "AESpineCache:createWithBinaryFileByThread");

			if (!ok) { break; }
			spAtlas* arg1 = (spAtlas*)tolua_tousertype(tolua_S, 3, 0);

			if (!ok) { break; }
			ae::AESpine* ret = cobj->createWithBinaryFileByThread(arg0, arg1);
			object_to_luaval<ae::AESpine>(tolua_S, "AESpine", (ae::AESpine*)ret);
			return 1;
		}
	} while (0);
	ok = true;
	do{
		if (argc == 3) {
			std::string arg0;
			ok &= luaval_to_std_string(tolua_S, 2, &arg0, "AESpineCache:createWithBinaryFileByThread");

			if (!ok) { break; }
			spAtlas* arg1 = (spAtlas*)tolua_tousertype(tolua_S, 3, 0);

			if (!ok) { break; }
			double arg2;
			ok &= luaval_to_number(tolua_S, 4, &arg2, "AESpineCache:createWithBinaryFileByThread");

			if (!ok) { break; }
			ae::AESpine* ret = cobj->createWithBinaryFileByThread(arg0, arg1, arg2);
			object_to_luaval<ae::AESpine>(tolua_S, "AESpine", (ae::AESpine*)ret);
			return 1;
		}
	} while (0);
	ok = true;
	do{
		if (argc == 4) {
			std::string arg0;
			ok &= luaval_to_std_string(tolua_S, 2, &arg0, "AESpineCache:createWithBinaryFileByThread");

			if (!ok) { break; }
			spAtlas* arg1 = (spAtlas*)tolua_tousertype(tolua_S, 3, 0);

			if (!ok) { break; }
			double arg2;
			ok &= luaval_to_number(tolua_S, 4, &arg2, "AESpineCache:createWithBinaryFileByThread");

			if (!ok) { break; }
			int handler = (toluafix_ref_function(tolua_S, 5, 0));
			std::function<void(ae::AESpine *)> arg3([=](ae::AESpine* pSpine) {
				executeSpineCallBack(pSpine, handler);
			});

			if (!ok) { break; }
			ae::AESpine* ret = cobj->createWithBinaryFileByThread(arg0, arg1, arg2, arg3);
			object_to_luaval<ae::AESpine>(tolua_S, "AESpine", (ae::AESpine*)ret);
			ScriptHandlerMgr::getInstance()->addObjectHandler((void*)ret, handler, ScriptHandlerMgr::HandlerType(10003));
			return 1;
		}
	} while (0);
	ok = true;
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESpineCache:createWithBinaryFileByThread", argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_spine_AESpineCache_createWithBinaryFileByThread'.", &tolua_err);
#endif

	return 0;
}

int register_spinecache_module(lua_State* L)
{
    if (nullptr == L)
        return 0;

	lua_pushstring(L, "AESpineCache");
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (lua_istable(L, -1))
	{
		tolua_function(L, "createWithJsonFileByThread", tolua_spine_AESpineCache_createWithJsonFileByThread00);
		tolua_function(L, "createWithBinaryFileByThread", tolua_spine_AESpineCache_createWithBinaryFileByThread00);
	}
	lua_pop(L, 1);

	std::string typeName = typeid(ae::AESpineCache).name();
	g_luaType[typeName] = "AESpineCache";
	g_typeCast["AESpineCache"] = "AESpineCache";
    
    return 0;
}

int register_all_spine_manual(lua_State* L)
{
    lua_getglobal(L, "_G");
    if (lua_istable(L,-1))
    {
		register_spinecache_module(L);
    }
    lua_pop(L, 1);

    return 1;
}
