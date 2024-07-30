cmake_minimum_required(VERSION 3.20)
project(liblua)

# 定义是否构建为动态库的选项, 变动的话, 需要删除 build, 重新构建
option(BUILD_SHARED_LIBS "Build liblua as a shared library" OFF)

# 定义是否构建 lua.exe 与 luac.exe
option(BUILD_LUA_AND_LUAC "Build lua.exe and luac.exe or not" OFF)

set(LUA_SRC_CODE
    lapi.c
    lauxlib.c
    lbaselib.c
    lcode.c
    lcorolib.c
    lctype.c
    ldblib.c
    ldebug.c
    ldo.c
    ldump.c
    lfunc.c
    lgc.c
    linit.c
    liolib.c
    llex.c
    lmathlib.c
    lmem.c
    loadlib.c
    lobject.c
    lopcodes.c
    loslib.c
    lparser.c
    lstate.c
    lstring.c
    lstrlib.c
    ltable.c
    ltablib.c
    ltm.c
    lundump.c
    lutf8lib.c
    lvm.c
    lzio.c
)

if(BUILD_SHARED_LIBS)
    message("****** build share lua: liblua.dll ******")
    add_library(${PROJECT_NAME} SHARED ${LUA_SRC_CODE})
    target_compile_definitions(${PROJECT_NAME} PRIVATE LUA_BUILD_AS_DLL)
else()
    message("****** build static lua: liblua.lib ******")
    add_library(${PROJECT_NAME} STATIC ${LUA_SRC_CODE})
endif(BUILD_SHARED_LIBS)

if(BUILD_LUA_AND_LUAC)
    # 创建可执行文件 lua 并链接 liblua
    add_executable(lua lua.c)
    target_link_libraries(lua ${PROJECT_NAME})

    # luac 里有使用 LUAI_FUNC 声明的函数, 所以不能动态库
    add_executable(luac luac.c ${LUA_SRC_CODE})
endif(BUILD_LUA_AND_LUAC)
