# 重新规划了cocos 和 aonesdk 的 c 文件结构
add_subdirectory(external/freetype2) # cocos_freetype2_static
add_subdirectory(external/png) # cocos_png_static
add_subdirectory(external/jpeg) # cocos_jpeg_static
add_subdirectory(external/tiff) # cocos_tiff_static
add_subdirectory(external/webp) # cocos_webp_static
add_subdirectory(external/zlib) # cocos_zlib_static
add_subdirectory(external/curl) # cocos_curl_static
add_subdirectory(external/Box2D) # box2d_static
add_subdirectory(external/recast) # recast_static
add_subdirectory(external/flatbuffers) # cocos_flatbuffers_static
add_subdirectory(external/lua/luajit) # luajit_static
add_subdirectory(external/android-specific/pvmp3dec) # libpvmp3dec
add_subdirectory(external/android-specific/tremolo) # libvorbisidec
add_subdirectory(extensions) # cocos_extension_static
add_subdirectory(cocos/ui) # cocos_ui_static
add_subdirectory(cocos/platform/android) # cocos2dxandroid_static
add_subdirectory(cocos/audio/android) # cocosdenshion_static audioengine_static
add_subdirectory(cocos/editor-support/cocostudio) # cocostudio_static
add_subdirectory(cocos/editor-support/spine) # spine_static
add_subdirectory(cocos/scripting/lua-bindings) # cocos2d_lua_android_static cocos2d_lua_static


add_subdirectory(cocos) # cocos2dx_internal_static cocos2dx_static
