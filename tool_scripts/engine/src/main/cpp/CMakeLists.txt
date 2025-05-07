# 最低CMake版本
cmake_minimum_required(VERSION 3.10)
project(engine)
set(CMAKE_CXX_STANDARD 11)
add_definitions(-DANDROID)

add_subdirectory(aonesdkc/external/patcher)
add_subdirectory(aonesdkc/external/cputils)
add_subdirectory(aonesdkc)
add_subdirectory(cocos2d)
add_subdirectory(closers)

