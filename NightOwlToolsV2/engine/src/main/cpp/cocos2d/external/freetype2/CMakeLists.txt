add_library(cocos_freetype2_static STATIC IMPORTED GLOBAL)

set_target_properties(cocos_freetype2_static PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/android/${ANDROID_ABI}/libfreetype.a
    IMPORTED_SONAME "" # 防止 .so 解析
)

target_include_directories(cocos_freetype2_static INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/android
    ${CMAKE_CURRENT_SOURCE_DIR}/include/android/freetype2
)