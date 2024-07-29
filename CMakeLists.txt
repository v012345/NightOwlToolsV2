cmake_minimum_required(VERSION 3.20)
project(NightOwlToolsV2)

# 构建好的可执行文件放到一块, 不然动态库要手动复制到可执行文件的目录下
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory("lua-5.4.7/src")
add_subdirectory("extensions")

add_executable(${PROJECT_NAME}
    main.c
)
target_link_libraries(${PROJECT_NAME}
    liblua
    liblfs
    libluasocket
)

target_include_directories(${PROJECT_NAME} PRIVATE
    lua-5.4.7/src
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    LUA_TEST_SCRIPT="${CMAKE_CURRENT_SOURCE_DIR}/test.lua"
)