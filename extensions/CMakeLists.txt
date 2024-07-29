cmake_minimum_required(VERSION 3.20)
set(LUA_SRC ${CMAKE_SOURCE_DIR}/lua-5.4.7/src)
add_library(liblfs SHARED luafilesystem/src/lfs.c)
target_include_directories(liblfs PRIVATE ${LUA_SRC})
target_link_libraries(liblfs liblua)

add_library(libluasocket SHARED
    "luasocket/src/mime.c"
    "luasocket/src/compat.c"
    "luasocket/src/auxiliar.c"
    "luasocket/src/buffer.c"
    "luasocket/src/compat.c"
    "luasocket/src/except.c"
    "luasocket/src/inet.c"
    "luasocket/src/io.c"
    "luasocket/src/luasocket.c"
    "luasocket/src/options.c"
    "luasocket/src/select.c"
    "luasocket/src/tcp.c"
    "luasocket/src/timeout.c"
    "luasocket/src/udp.c"
    "luasocket/src/wsocket.c"
)
target_include_directories(libluasocket PRIVATE ${LUA_SRC})
target_link_libraries(libluasocket PRIVATE liblua ws2_32.lib)
