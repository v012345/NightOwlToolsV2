# ##########################
# cocos2d_lua_android_static
# ##########################
add_library(cocos2d_lua_android_static STATIC
    manual/platform/android/CCLuaJavaBridge.cpp
    manual/platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxLuaJavaBridge.cpp
)

# Link system libraries
target_link_libraries(cocos2d_lua_android_static
    PUBLIC
    GLESv2
    log
    android
)

target_include_directories(cocos2d_lua_android_static PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/manual
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../external/lua/tolua
    ${CMAKE_CURRENT_SOURCE_DIR}/manual/platform/android
    ${CMAKE_CURRENT_SOURCE_DIR}/manual/platform/android/jni
)
target_link_libraries(cocos2d_lua_android_static PRIVATE
    luajit_static
)

# ##########################
# cocos2d_lua_static
# ##########################
add_library(cocos2d_lua_static STATIC
    manual/CCLuaBridge.cpp
    manual/CCLuaEngine.cpp
    manual/CCLuaStack.cpp
    manual/CCLuaValue.cpp
    manual/Cocos2dxLuaLoader.cpp
    manual/LuaBasicConversions.cpp
    manual/lua_module_register.cpp
    auto/lua_cocos2dx_auto.cpp
    auto/lua_cocos2dx_experimental_auto.cpp
    manual/cocos2d/lua_cocos2dx_deprecated.cpp
    manual/cocos2d/lua_cocos2dx_experimental_manual.cpp
    manual/cocos2d/lua_cocos2dx_manual.cpp
    manual/cocos2d/LuaOpengl.cpp
    manual/cocos2d/LuaScriptHandlerMgr.cpp
    manual/tolua_fix.cpp
    ../../../external/lua/tolua/tolua_event.c
    ../../../external/lua/tolua/tolua_is.c
    ../../../external/lua/tolua/tolua_map.c
    ../../../external/lua/tolua/tolua_push.c
    ../../../external/lua/tolua/tolua_to.c
    ../../../external/xxtea/xxtea.cpp
    auto/lua_cocos2dx_audioengine_auto.cpp
    manual/audioengine/lua_cocos2dx_audioengine_manual.cpp
    manual/CCComponentLua.cpp
    manual/cocosdenshion/lua_cocos2dx_cocosdenshion_manual.cpp
    auto/lua_cocos2dx_cocosdenshion_auto.cpp
    manual/cocostudio/lua_cocos2dx_coco_studio_manual.cpp
    manual/cocostudio/CustomGUIReader.cpp
    manual/cocostudio/lua_cocos2dx_csloader_manual.cpp
    auto/lua_cocos2dx_csloader_auto.cpp
    auto/lua_cocos2dx_studio_auto.cpp
    manual/cocostudio/lua-cocos-studio-conversions.cpp
    manual/spine/lua_cocos2dx_spine_manual.cpp
    manual/spine/LuaSkeletonAnimation.cpp
    auto/lua_cocos2dx_spine_auto.cpp

    manual/ui/lua_cocos2dx_experimental_webview_manual.cpp
    manual/ui/lua_cocos2dx_experimental_video_manual.cpp
    manual/ui/lua_cocos2dx_ui_manual.cpp
    auto/lua_cocos2dx_experimental_video_auto.cpp
    auto/lua_cocos2dx_ui_auto.cpp
    auto/lua_cocos2dx_experimental_webview_auto.cpp

    manual/extension/lua_cocos2dx_extension_manual.cpp
    auto/lua_cocos2dx_extension_auto.cpp
)

target_include_directories(cocos2d_lua_static

    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../external/lua/tolua
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../external/lua/luajit/include
    ${CMAKE_CURRENT_SOURCE_DIR}/auto
    ${CMAKE_CURRENT_SOURCE_DIR}/manual
    ${CMAKE_CURRENT_SOURCE_DIR}/manual/manual/cocos2d
    ${CMAKE_CURRENT_SOURCE_DIR}/manual/manual/cocosdenshion
    ${CMAKE_CURRENT_SOURCE_DIR}/manual/manual/audioengine
    ${CMAKE_CURRENT_SOURCE_DIR}/manual/manual/extension
    ${CMAKE_CURRENT_SOURCE_DIR}/manual/manual/cocostudio
    ${CMAKE_CURRENT_SOURCE_DIR}/manual/manual/spine
    ${CMAKE_CURRENT_SOURCE_DIR}/manual/manual/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../external
    ${CMAKE_CURRENT_SOURCE_DIR}/../../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../..

    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../2d
    ${CMAKE_CURRENT_SOURCE_DIR}/../../editor-support/cocostudio
    ${CMAKE_CURRENT_SOURCE_DIR}/../../editor-support/cocostudio/ActionTimeline
    ${CMAKE_CURRENT_SOURCE_DIR}/../../editor-support/spine
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ui
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../extensions
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../external/xxtea
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../external/lua
)

target_link_libraries(cocos2d_lua_static PRIVATE
    cocos2dx_static
    -Wl,--whole-archive cocos2d_lua_android_static -Wl,--no-whole-archive
)