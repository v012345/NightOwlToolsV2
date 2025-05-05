add_library(bugly_native_prebuilt SHARED IMPORTED GLOBAL)

set_target_properties(bugly_native_prebuilt PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/external/bugly/BuglySDK/Android/libs/${ANDROID_ABI}/libBugly.so
    IMPORTED_SONAME ""
)

# 如果有 .mm 文件需要特殊处理，可以加
set_source_files_properties(
    external/bugly/CocosPlugin/bugly/CrashReport.mm
    PROPERTIES LANGUAGE CXX
)

add_library(cputils_static STATIC
    src/cputils_android.cpp
    src/cplog.cpp
    src/cpfile.cpp
    src/zf_log.c
    src/cJSON.c
    src/base/CCConsole.cpp
    src/base/CCData.cpp
    src/base/ccUtils.cpp
    src/platform/CCFileUtils.cpp
    src/platform/android/CCFileUtils-android.cpp
    external/bugly/CocosPlugin/bugly/CrashReport.mm
)

target_compile_definitions(cputils_static PRIVATE USE_BUGLY)

target_compile_options(cputils_static PRIVATE
    -frtti
    -fexceptions
    -Wno-error=format-security
    $<$<COMPILE_LANGUAGE:C>:-std=c99 -DZF_LOG_USE_ANDROID_LOG>
)

target_include_directories(cputils_static
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/external/zflog
    ${CMAKE_CURRENT_SOURCE_DIR}/external/bugly/CocosPlugin/bugly
)

# 链接 bugly_native_prebuilt 库
target_link_libraries(cputils_static PRIVATE
    bugly_native_prebuilt
)