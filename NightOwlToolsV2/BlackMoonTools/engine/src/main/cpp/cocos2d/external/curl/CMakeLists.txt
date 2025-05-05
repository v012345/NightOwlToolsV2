# ###############################
# cocos_crypto_static
# ###############################
add_library(cocos_crypto_static STATIC IMPORTED GLOBAL)

set_target_properties(cocos_crypto_static PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/android/${ANDROID_ABI}/libcrypto.a
    IMPORTED_SONAME "" # 防止 .so 解析
)

# ###############################
# cocos_ssl_static
# ###############################
add_library(cocos_ssl_static STATIC IMPORTED GLOBAL)

set_target_properties(cocos_ssl_static PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/android/${ANDROID_ABI}/libssl.a
    IMPORTED_SONAME "" # 防止 .so 解析
)

# ###############################
# cocos_curl_static
# ###############################
add_library(cocos_curl_static STATIC IMPORTED GLOBAL)

set_target_properties(cocos_curl_static PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/android/${ANDROID_ABI}/libcurl.a
    IMPORTED_SONAME "" # 防止 .so 解析
)

target_include_directories(cocos_curl_static INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/android
)

target_link_libraries(cocos_curl_static INTERFACE
    cocos_crypto_static
    cocos_ssl_static
)
