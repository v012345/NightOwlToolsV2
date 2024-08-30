#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include <windows.h>
#include <direct.h>

typedef struct ExecLuaCodeParam
{
    char *code;
    char *msg;
    HANDLE handle;
} ExecLuaCodeParam;

int luaopen_lfs(lua_State *L);
int luaopen_socket_core(lua_State *L);
int luaopen_mime_core(lua_State *L);
int luaopen_lsqlite3(lua_State *L);
static int luaopen_Common(lua_State *L);
DWORD G_X = 0;
int main(int argc, char const *argv[])
{
    if (argc >= 2 && _chdir(argv[1]) != 0)
    {
        perror("Failed to change directory");
        return 1;
    }
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    luaopen_lfs(L);
    luaopen_Common(L);
    luaL_requiref(L, "sqlite3", luaopen_lsqlite3, 1);
    lua_pop(L, 1);
    luaL_requiref(L, "socket.core", luaopen_socket_core, 0);
    lua_pop(L, 1);
    luaL_requiref(L, "mime.core", luaopen_mime_core, 0);
    lua_pop(L, 1);
    // luaL_dofile(L, LUA_TEST_SCRIPT);
    luaL_dofile(L, "main.lua");
    lua_close(L);
    return 0;
}

static int lua_CreateThread(lua_State *L);
static int lua_WaitForSingleObject(lua_State *L);
static int lua_GetExitCodeThread(lua_State *L);
static int lua_CloseHandle(lua_State *L);
static int lua_GetConsoleScreenBufferInfo(lua_State *L);
static int luaopen_Common(lua_State *L)
{
    luaL_Reg lua_reg[] = {
        {"CreateThread", lua_CreateThread},                             //
        {"WaitForSingleObject", lua_WaitForSingleObject},               //
        {"GetExitCodeThread", lua_GetExitCodeThread},                   //
        {"CloseHandle", lua_CloseHandle},                               //
        {"GetConsoleScreenBufferInfo", lua_GetConsoleScreenBufferInfo}, //
        {NULL, NULL},
    };
    lua_newtable(L);
    luaL_setfuncs(L, lua_reg, 0);
    lua_setglobal(L, "Common");
    return 1;
}

static int lua_GetConsoleScreenBufferInfo(lua_State *L)
{
    int consoleWidth;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    // 获取控制台输出的句柄
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    lua_pushinteger(L, consoleWidth);
    return 1;
}

static DWORD WINAPI ExecLuaCode(LPVOID lpParam);

static int lua_CreateThread(lua_State *L)
{
    const char *code = lua_tostring(L, 1);
    ExecLuaCodeParam *pExecLuaCodeParam = NULL;
    pExecLuaCodeParam = realloc(pExecLuaCodeParam, sizeof(ExecLuaCodeParam));
    pExecLuaCodeParam->code = strdup(code);
    HANDLE hThread;
    DWORD dwThreadId;
    // 创建一个线程来执行耗时任务
    hThread = CreateThread(
        NULL,                      // 默认安全属性
        0,                         // 默认堆栈大小
        ExecLuaCode,               // 线程函数
        (LPVOID)pExecLuaCodeParam, // 传递的参数 (字符串)
        0,                         // 默认创建标志
        &dwThreadId                // 获取线程ID
    );
    pExecLuaCodeParam->handle = hThread;
    ExecLuaCodeParam **ppExecLuaCodeParam = lua_newuserdata(L, sizeof(ExecLuaCodeParam *));
    *ppExecLuaCodeParam = pExecLuaCodeParam;
    return 1;
}

static int lua_CloseHandle(lua_State *L)
{
    ExecLuaCodeParam **pExecLuaCodeParam = (ExecLuaCodeParam **)lua_touserdata(L, 1);
    CloseHandle((*pExecLuaCodeParam)->handle);
    free((void *)(*pExecLuaCodeParam)->code);
    free((void *)(*pExecLuaCodeParam)->msg);
    return 0;
}
static int lua_WaitForSingleObject(lua_State *L)
{
    ExecLuaCodeParam **ppExecLuaCodeParam = (ExecLuaCodeParam **)lua_touserdata(L, 1);
    // 检查子线程是否已经结束
    DWORD result = WaitForSingleObject((*ppExecLuaCodeParam)->handle, 0);
    lua_pushboolean(L, result == WAIT_OBJECT_0);
    return 1;
}
static int lua_GetExitCodeThread(lua_State *L)
{
    ExecLuaCodeParam **pExecLuaCodeParam = (ExecLuaCodeParam **)lua_touserdata(L, 1);
    // 获取子线程的返回值
    DWORD exitCode;
    GetExitCodeThread((*pExecLuaCodeParam)->handle, &exitCode);
    lua_pushstring(L, (*pExecLuaCodeParam)->msg);
    return 1;
}

// 线程函数
static DWORD WINAPI ExecLuaCode(LPVOID lpParam)
{
    ExecLuaCodeParam *pExecLuaCodeParam = (ExecLuaCodeParam *)lpParam;
    const char *code = pExecLuaCodeParam->code;
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    luaopen_lfs(L);
    luaL_requiref(L, "sqlite3", luaopen_lsqlite3, 1);
    lua_pop(L, 1);
    luaL_requiref(L, "socket.core", luaopen_socket_core, 0);
    lua_pop(L, 1);
    luaL_requiref(L, "mime.core", luaopen_mime_core, 0);
    lua_pop(L, 1);

    const char *result = NULL;
    if (luaL_dostring(L, code) == LUA_OK)
    {
        // if (lua_isstring(L, -1))
        // {
        result = lua_tostring(L, -1);
        // }
    }
    else
    {
        const char *error = lua_tostring(L, -1);
        printf("Error: %s\n", error);
    }
    pExecLuaCodeParam->msg = strdup(result); // 复制返回的字符串，以便主线程使用

    return 1; // 返回字符串的指针作为线程退出码
}