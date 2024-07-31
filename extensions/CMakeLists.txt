cmake_minimum_required(VERSION 3.20)
set(LUA_SRC ${CMAKE_SOURCE_DIR}/lua-5.4.7/src)
set(lfs_src luafilesystem/src/lfs.c)
set(lsqlite3_src
    lsqlite3_v096/sqlite3.c
    lsqlite3_v096/lsqlite3.c)
set(luasocket_src
    luasocket/src/mime.c
    luasocket/src/compat.c
    luasocket/src/auxiliar.c
    luasocket/src/buffer.c
    luasocket/src/compat.c
    luasocket/src/except.c
    luasocket/src/inet.c
    luasocket/src/io.c
    luasocket/src/luasocket.c
    luasocket/src/options.c
    luasocket/src/select.c
    luasocket/src/tcp.c
    luasocket/src/timeout.c
    luasocket/src/udp.c
    luasocket/src/wsocket.c)

if(BUILD_SHARED_LIBS)
    message("****** build share extensions ******")
    add_library(liblfs SHARED ${lfs_src})
    add_library(liblsqlite3 SHARED ${lsqlite3_src})
    add_library(libluasocket SHARED ${luasocket_src})

    target_compile_definitions(liblfs PRIVATE LUA_BUILD_AS_DLL)
    target_compile_definitions(liblsqlite3 PRIVATE LUA_BUILD_AS_DLL)
    target_compile_definitions(libluasocket PRIVATE LUA_BUILD_AS_DLL)

else()
    message("****** build static extensions ******")
    add_library(liblfs STATIC ${lfs_src})
    add_library(liblsqlite3 STATIC ${lsqlite3_src})
    add_library(libluasocket STATIC ${luasocket_src})
endif(BUILD_SHARED_LIBS)

target_include_directories(liblfs PRIVATE ${LUA_SRC})
target_link_libraries(liblfs PRIVATE liblua)

target_include_directories(liblsqlite3 PRIVATE ${LUA_SRC})
target_link_libraries(liblsqlite3 PRIVATE liblua)

target_include_directories(libluasocket PRIVATE ${LUA_SRC})
target_link_libraries(libluasocket PRIVATE liblua ws2_32.lib)
