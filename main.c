#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include <windows.h>

int luaopen_lfs(lua_State *L);
int luaopen_socket_core(lua_State *L);
int luaopen_mime_core(lua_State *L);
int luaopen_lsqlite3(lua_State *L);
static int luaopen_Common(lua_State *L);
DWORD G_X = 0;
int main(int argc, char const *argv[])
{
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
    luaL_dofile(L, LUA_TEST_SCRIPT);
    // luaL_dofile(L, "main.lua");
    lua_close(L);
    return 0;
}

static int lua_CreateThread(lua_State *L);
static int lua_WaitForSingleObject(lua_State *L);
static int lua_GetExitCodeThread(lua_State *L);
static int lua_CloseHandle(lua_State *L);
static int luaopen_Common(lua_State *L)
{
    luaL_Reg lua_reg[] = {
        {"CreateThread", lua_CreateThread},               //
        {"WaitForSingleObject", lua_WaitForSingleObject}, //
        {"GetExitCodeThread", lua_GetExitCodeThread},     //
        {"CloseHandle", lua_CloseHandle},                 //
        {NULL, NULL},
    };
    lua_newtable(L);
    luaL_setfuncs(L, lua_reg, 0);
    lua_setglobal(L, "Common");
    return 1;
}

static DWORD WINAPI ExecLuaCode(LPVOID lpParam);
static int lua_CreateThread(lua_State *L)
{
    const char *code = lua_tostring(L, 1);
    char *code_copy = strdup(code);
    HANDLE hThread;
    DWORD dwThreadId;
    // 创建一个线程来执行耗时任务
    hThread = CreateThread(
        NULL,              // 默认安全属性
        0,                 // 默认堆栈大小
        ExecLuaCode,       // 线程函数
        (LPVOID)code_copy, // 传递的参数 (字符串)
        0,                 // 默认创建标志
        &dwThreadId        // 获取线程ID
    );
    HANDLE *phThread = lua_newuserdata(L, sizeof(HANDLE));
    *phThread = hThread;
    return 1;
    // if (hThread == NULL)
    // {
    //     printf("CreateThread failed. Error: %d\n", GetLastError());
    //     return 1;
    // }
}

static int lua_CloseHandle(lua_State *L)
{
    // 关闭线程句柄
    HANDLE *phThread = (HANDLE *)lua_touserdata(L, 1);
    CloseHandle(*phThread);
    return 0;
}
static int lua_WaitForSingleObject(lua_State *L)
{
    HANDLE *phThread = (HANDLE *)lua_touserdata(L, 1);
    // 检查子线程是否已经结束
    DWORD result = WaitForSingleObject(*phThread, 0);
    lua_pushboolean(L, result == WAIT_OBJECT_0);
    return 1;
}
static int lua_GetExitCodeThread(lua_State *L)
{
    HANDLE *phThread = (HANDLE *)lua_touserdata(L, 1);
    // 获取子线程的返回值
    DWORD exitCode;
    if (GetExitCodeThread(*phThread, &exitCode))
    {
        char *pExitCode = (char *)exitCode; // 将 DWORD 转换回字符串指针
        if (pExitCode != NULL)
        {
            lua_pushstring(L, pExitCode);
            // 使用完字符串后释放内存
            free(pExitCode);
        }
        else
        {
            printf("Thread returned NULL.\n");
            lua_pushnil(L);
        }
    }
    else
    {
        printf("Failed to get thread exit code. Error: %d\n", GetLastError());
        lua_pushnil(L);
    }

    return 1;
}

// 线程函数
static DWORD WINAPI ExecLuaCode(LPVOID lpParam)
{
    const char *code = (const char *)lpParam;
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
        if (lua_isstring(L, -1))
        {
            result = lua_tostring(L, -1);
            printf("Returned string from Lua: %s\n", result);
        }
        else
        {
            printf("No string returned.\n");
        }
    }
    else
    {
        const char *error = lua_tostring(L, -1);
        printf("Error: %s\n", error);
    }

    free((void *)code);

    char *r = strdup(result); // 复制返回的字符串，以便主线程使用
    int a = sizeof(char *);
    int b = sizeof(DWORD);
    int c = sizeof(DWORD_PTR);
    int d = sizeof(unsigned);
    // G_X = (uintptr_t)r;
    return (DWORD_PTR)r; // 返回字符串的指针作为线程退出码
}