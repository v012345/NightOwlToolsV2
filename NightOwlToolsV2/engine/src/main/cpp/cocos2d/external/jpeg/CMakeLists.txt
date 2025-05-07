add_library(cocos_jpeg_static STATIC IMPORTED GLOBAL)

set_target_properties(cocos_jpeg_static PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/android/${ANDROID_ABI}/libjpeg.a
    IMPORTED_SONAME "" # 防止 .so 解析
)

target_include_directories(cocos_jpeg_static INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/android
)