add_library(luajit_static STATIC IMPORTED GLOBAL)

set_target_properties(luajit_static PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/android/${ANDROID_ABI}/libluajit.a
    IMPORTED_SONAME "" # 防止 .so 解析
)

target_include_directories(luajit_static INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)