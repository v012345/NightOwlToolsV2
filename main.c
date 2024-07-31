#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
int luaopen_lfs(lua_State *L);
int luaopen_socket_core(lua_State *L);
int luaopen_mime_core(lua_State *L);
int luaopen_lsqlite3(lua_State *L);
int main(int argc, char const *argv[])
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    luaopen_lfs(L);
    luaL_requiref(L, "sqlite3", luaopen_lsqlite3, 1);
    lua_pop(L, 1);
    luaL_requiref(L, "socket.core", luaopen_socket_core, 0);
    lua_pop(L, 1);
    luaL_requiref(L, "mime.core", luaopen_mime_core, 0);
    lua_pop(L, 1);
    // luaL_dofile(L, LUA_TEST_SCRIPT);
    luaL_dofile(L, "main.lua");
    return 0;
}