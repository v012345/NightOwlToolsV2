extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "aone_patcher.h"
#include "BigPatcher.h"
}

#include "lpatcher.h"

static int private_progress_callback_ref = LUA_NOREF;
static int private_finish_callback_ref = LUA_NOREF;
static int private_error_callback_ref = LUA_NOREF;

static int private_big_progress_callback_ref = LUA_NOREF;
static int private_big_finish_callback_ref = LUA_NOREF;
static int private_big_error_callback_ref = LUA_NOREF;

static lua_State* private_lua_L = NULL;

static void store_callback(lua_State* L, int index, int* stored)
{
    if (*stored != LUA_NOREF) luaL_unref(L, LUA_REGISTRYINDEX,*stored);
    *stored = LUA_NOREF;

    if (lua_isfunction(L, index)) {
        lua_pushvalue(L, index);
        *stored = luaL_ref(L, LUA_REGISTRYINDEX);
    }
}

void private_progress_cb_l(struct patcher* p, void* ud, uint32 write_bytes, uint32 expect_bytes)
{
	printf("progress.........write_bytes = %d, expect_bytes = %d\n", write_bytes, expect_bytes);
	lua_State* L = private_lua_L;
    if (private_progress_callback_ref != LUA_NOREF) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, private_progress_callback_ref);
        lua_pushinteger(L, write_bytes);
        lua_pushinteger(L, expect_bytes);
        lua_call(L, 2, 0);
    }
}

void private_finish_cb_l(struct patcher* p, void* ud)
{
	lua_State* L = private_lua_L;
    if (private_finish_callback_ref != LUA_NOREF) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, private_finish_callback_ref);
        lua_call(L, 0, 0);
    }
}

void private_error_cb_l(struct patcher* p, void* ud, uint32 code)
{
	lua_State* L = private_lua_L;
    if (private_error_callback_ref != LUA_NOREF) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, private_error_callback_ref);
        lua_pushinteger(L, code);
        lua_call(L, 1, 0);
    }
}

void private_big_progress_cb_l(BPStage stage,double cur, double total)
{
    printf("big_progress.........stage = %d, write_bytes = %f, expect_bytes = %f\n", stage, cur, total);
    lua_State* L = private_lua_L;
    if (private_big_progress_callback_ref != LUA_NOREF) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, private_big_progress_callback_ref);
        lua_pushinteger(L, stage);
        lua_pushnumber(L, cur);
        lua_pushnumber(L, total);
        lua_call(L, 3, 0);
    }
}

void private_big_finish_cb_l()
{
    lua_State* L = private_lua_L;
    if (private_big_finish_callback_ref != LUA_NOREF) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, private_big_finish_callback_ref);
        lua_call(L, 0, 0);
    }
}

void private_big_error_cb_l(BPErrorCode code,int innerCode, const char* msg)
{
    printf("big_error.........code = %d, innerCode = %d, msg = %s\n", code, innerCode, msg);
    lua_State* L = private_lua_L;
    if (private_big_error_callback_ref != LUA_NOREF) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, private_big_error_callback_ref);
        lua_pushinteger(L, code);
        lua_pushinteger(L, innerCode);
        lua_pushstring(L, msg);
        lua_call(L, 3, 0);
    }
}

static int l_callback(lua_State *L)
{
    private_lua_L = L;
    store_callback(L, 1, &private_progress_callback_ref);
    store_callback(L, 2, &private_finish_callback_ref);
    store_callback(L, 3, &private_error_callback_ref);

    patcher_callback(patcher_get_default(), NULL, private_progress_cb_l, private_finish_cb_l, private_error_cb_l);
    return 0;
}

static int l_set_url(lua_State *L)
{
    private_lua_L = L;
    const char* url = luaL_checkstring(L, 1);
    patcher_set_url(patcher_get_default(), url);
    return 0;
}

static int l_set_path(lua_State *L)
{
    private_lua_L = L;
    const char* path = luaL_checkstring(L, 1);
    patcher_set_path(patcher_get_default(), path);
    return 0;
}

static int l_getList(lua_State *L)
{
    private_lua_L = L;
	patcher_get_list(patcher_get_default());
	return 0;
}

static int l_start(lua_State *L)
{
    private_lua_L = L;
    patcher_start(patcher_get_default());
    return 0;
}

static int l_notify(lua_State *L)
{
    private_lua_L = L;
    patcher_notify(patcher_get_default());
    return 0;
}

static int l_free(lua_State *L)
{
    private_lua_L = L;
    patcher_free(patcher_get_default());
    return 0;
}

 static int l_patcher_check(lua_State *L)
 {
     private_lua_L = L;
	 patcher_check(patcher_get_default());
	 return 0;
 }
 
 static int l_remove_useless_files(lua_State *L)
 {
     private_lua_L = L;
	 patcher_remove_useless_files(patcher_get_default());
	 return 0;
 }

static int l_cancel(lua_State *L)
{
    private_lua_L = L;
    patcher_cancel(patcher_get_default());
    return 0;
}

static int l_path(lua_State *L)
{
    private_lua_L = L;
    const char* path = patcher_path(patcher_get_default());
    lua_pushstring(L, path);
    return 1;
}

static int l_get_result(lua_State *L)
{
    private_lua_L = L;
	uint32 inner_code;
    const char* msg = "";
    unsigned int result = patcher_get_result(patcher_get_default(), &inner_code, &msg);
	lua_pushinteger(L, result);
	lua_pushinteger(L, inner_code);
	lua_pushstring(L, msg);
    return 3;
}

static int l_set_package_res_path(lua_State *L)
{
    private_lua_L = L;
    const char* package_res_path = luaL_checkstring(L, 1);
    patcher_set_package_res_path(patcher_get_default(), package_res_path);
    return 0;
}

static int l_big_set_url(lua_State *L)
{
    private_lua_L = L;
    const char* url = luaL_checkstring(L, 1);
    big_patcher_set_url(big_patcher_get_default(), url);
    return 0;
}

static int l_big_set_path(lua_State *L)
{
    private_lua_L = L;
    const char* path = luaL_checkstring(L, 1);
    big_patcher_set_path(big_patcher_get_default(), path);
    return 0;
}

static int l_big_get_size(lua_State *L)
{
    private_lua_L = L;
    big_pathcher_get_size(big_patcher_get_default());
    return 0;
}

static int l_big_start(lua_State *L)
{
    private_lua_L = L;
    big_patcher_start(big_patcher_get_default());
    return 0;
}

static int l_big_cancel(lua_State *L)
{
    private_lua_L = L;
    big_patcher_cancel(big_patcher_get_default());
    return 0;
}

static int l_big_callback(lua_State *L)
{
    private_lua_L = L;
    store_callback(L, 1, &private_big_progress_callback_ref);
    store_callback(L, 2, &private_big_finish_callback_ref);
    store_callback(L, 3, &private_big_error_callback_ref);

    big_patcher_set_callback(big_patcher_get_default(),private_big_progress_cb_l, private_big_finish_cb_l,private_big_error_cb_l);
    return 0;
}

static int l_big_notify(lua_State *L)
{
    private_lua_L = L;
    big_patcher_notify(big_patcher_get_default());
    return 0;
}

static const luaL_Reg R[] =
{
    {"set_url",                 l_set_url},
    {"set_path",                l_set_path},
    {"getList",                 l_getList},
    {"start",                   l_start},
    {"callback",	               l_callback},
    {"notify",                  l_notify},
    {"free",                    l_free},
    {"check",                   l_patcher_check},
    {"cancel",                  l_cancel},
    {"path",                    l_path},
    {"get_result",              l_get_result},
    {"set_package_res_path",    l_set_package_res_path},
    {"remove_useless_files",    l_remove_useless_files},
    {"big_set_url",             l_big_set_url},
    {"big_set_path",            l_big_set_path},
    {"big_get_size",            l_big_get_size},
    {"big_start",               l_big_start},
    {"big_cancel",              l_big_cancel},
    {"big_callback",	            l_big_callback},
    {"big_notify",	            l_big_notify},
    {NULL,	NULL}
};

void luaopen_patcher(lua_State *L)
{
#if (LUA_VERSION_NUM <= 501)
	luaL_register(L, "patcher", R);
	lua_pop(L, 1);
#else
	lua_getglobal(L, "patcher");
	if (lua_isnil(L, -1))
	{
		lua_pop(L, 1);
		lua_newtable(L);
	}
	luaL_setfuncs(L, R, 0);

	lua_setglobal(L, "patcher");
#endif
}