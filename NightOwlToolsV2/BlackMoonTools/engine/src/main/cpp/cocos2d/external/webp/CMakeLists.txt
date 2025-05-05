add_library(cocos_webp_static STATIC IMPORTED GLOBAL)

set_target_properties(cocos_webp_static PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/android/${ANDROID_ABI}/libwebp.a
    IMPORTED_SONAME "" # 防止 .so 解析
)

target_include_directories(cocos_webp_static INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/android
)

add_library(cpufeatures STATIC ${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c)
target_include_directories(cpufeatures PUBLIC ${ANDROID_NDK}/sources/android/cpufeatures)

target_link_libraries(cocos_webp_static INTERFACE
    -Wl,--whole-archive cpufeatures -Wl,--no-whole-archive
)

if(ANDROID_ABI STREQUAL "armeabi-v7a")
    target_compile_definitions(cocos_webp_static INTERFACE -DHAVE_NEON=1)
endif()