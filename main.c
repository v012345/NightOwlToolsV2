#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
int main(int argc, char const *argv[])
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    luaL_dofile(L, LUA_TEST_SCRIPT);
    return 0;
}
