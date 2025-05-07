add_library(cocos_png_static STATIC IMPORTED GLOBAL)

set_target_properties(cocos_png_static PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/android/${ANDROID_ABI}/libpng.a
    IMPORTED_SONAME "" # 防止 .so 解析
)

target_include_directories(cocos_png_static INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/android
)