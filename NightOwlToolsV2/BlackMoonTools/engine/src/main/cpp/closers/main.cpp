#ifdef USE_FAKE_STACK_CHK

extern "C"
{

    void __stack_chk_fail(void)
    {
        // printf("fake __stack_chk_fail triggered\n");
        // abort(); // 或者不abort，看你需求
    }

    void __stack_chk_fail_local(void)
    {
        __stack_chk_fail();
    }
}

#endif

#include <memory>
#include <jni.h>
#include <android/log.h>

#include "AppDelegate.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "channel_inc.h"
#define LOG_TAG "main"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

AAssetManager *amgr = NULL;
namespace
{
    std::unique_ptr<AppDelegate> appDelegate;
}

void cocos_android_app_init(JNIEnv *env)
{
    LOGD("cocos_android_app_init");
    appDelegate.reset(new AppDelegate());
}

void setMultipleTouchEnabled_android(bool val)
{
    JniMethodInfo jmi;
    if (JniHelper::getStaticMethodInfo(jmi, "org.cocos2dx.lib/Cocos2dxGLSurfaceView", "setMultipleTouchEnabled", "(Z)V"))
    {
        jmi.env->CallStaticVoidMethod(jmi.classID, jmi.methodID, (jboolean)val);
    }
    else
    {
        LOGD(" -- err : call setMultipleTouchEnabled error");
    }
}

extern "C"
{
    void Java_channel_adapter_GamePipe_ToGame(JNIEnv *env, jobject thiz, jstring params)
    {
        const char *params_ = env->GetStringUTFChars(params, 0);
        ChannelDispatchCenter::inst()->togame(params_);
        env->ReleaseStringUTFChars(params, params_);
    }
    void Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesBegin(JNIEnv *env, jobject thiz, jint id, jfloat x, jfloat y);
    void Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeRender(JNIEnv *env);
    // 让链接器别优化掉这些 JNI 方法
    void Java_making_cmake_happy(void)
    {
        volatile void *p1 = (void *)Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesBegin;
        volatile void *p2 = (void *)Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeRender;
        (void)p1;
        (void)p2;
    }
}
