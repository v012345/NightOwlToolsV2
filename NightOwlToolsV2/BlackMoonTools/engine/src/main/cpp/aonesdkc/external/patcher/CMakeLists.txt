add_library(patcher_static STATIC
    src/BigPatcher.cpp
    src/btwo_patcher.cpp
    src/CPFileUtils.cpp
    src/CSVparser.cpp
    src/CurlManagerPool.cpp
    src/DataConfig.cpp
    src/decompress.cpp
    src/DecompressWrapper.cpp
    src/DownloadedMgr.cpp
    src/http_download.cpp
    src/ioapi.c
    src/md5.cpp
    src/PatcherController.cpp
    src/PatcherTask.cpp
    src/PatcherTaskList.cpp
    src/PatchList.cpp
    src/unzip.c
    src/VersionFile.cpp
)
target_compile_definitions(patcher_static PRIVATE UTILS_PLATFORM_ANDROID)

# 给目标 closers 加编译选项
target_compile_options(patcher_static PRIVATE
    -frtti
    -fexceptions
    -fno-builtin
    -fsigned-char
    -Wno-error=format-security
)

target_include_directories(patcher_static
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../cocos2d/external/curl/include/android
    ${CMAKE_CURRENT_SOURCE_DIR}/external/zlib/include/android
    ${CMAKE_CURRENT_SOURCE_DIR}/external/lua/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../cputils/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../cputils/src
)
