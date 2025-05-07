# cocos2dxandroid/CMakeLists.txt

add_library(cocos2dxandroid_static STATIC
    CCApplication-android.cpp
    CCCommon-android.cpp
    CCDevice-android.cpp
    CCGLViewImpl-android.cpp
    CCFileUtils-android.cpp
    javaactivity-android.cpp
    CCEnhanceAPI-android.cpp
    jni/Java_org_cocos2dx_lib_Cocos2dxAccelerometer.cpp
    jni/Java_org_cocos2dx_lib_Cocos2dxBitmap.cpp
    jni/Java_org_cocos2dx_lib_Cocos2dxHelper.cpp
    jni/Java_org_cocos2dx_lib_Cocos2dxRenderer.cpp
    jni/JniHelper.cpp
    jni/TouchesJni.cpp
    jni/test.cpp
)

target_include_directories(cocos2dxandroid_static
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(cocos2dxandroid_static
    PUBLIC
    GLESv1_CM
    GLESv2
    EGL
    log
    android
)
