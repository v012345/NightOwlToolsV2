#include "AppDelegate.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "audio/include/SimpleAudioEngine.h"
#include "scripting/lua-bindings/manual/lua_module_register.h"
#include "scripting/lua-bindings/register_custom_function.h"
#include "editor-support/cocostudio/CocoStudio.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "scripting/lua-bindings/SdkToLua.h"
#include "../sdk/SdkMgr.h"
#else
#include "scripting/lua-bindings/AoneClientToLua.h"
#include "AoneClient.h"
#endif
#include "aone_patcher.h"


#include "scene/GameScene.h"
#include "common/AEUtil.h"
#include "net/AESocketClient.h"
#include "external/spine/AESpineCache.h"
#include "external/spine/AESpineColliderManager.h"
#include "aone/filesystem.h"
#include "xenet/XESelector.h"
//#include "xenet/XELog.h"
//#include "../sdkCommon/SdkWarpperIOS.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "scripting/lua-bindings/BuglyLuaAgent.h"
#endif

using namespace CocosDenshion;

USING_NS_AE;
USING_NS_CC;
using namespace std;


AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
	AESpineCache::destroyInstance();
	AESpineColliderManager::destroyInstance();
	// if you use SimpleAudioEngine, it must be end
	SimpleAudioEngine::end();

	// aone sdk exit
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
	aonesdk::AoneClient::Exit();
#endif

	// release cocos instance
	cocostudio::destroyCocosStudio();

	LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
	stack->cleanupXXTEAKeyAndSign();

	ScriptEngineManager::destroyInstance();
	ScriptHandlerMgr::destroyInstance();
	//xe::XELog::stop();
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

void initCallback(int retCode, std::map<std::string, std::string> dataMap)
{
	auto director = Director::getInstance();

	if (retCode == 0)
	{
		// register aone client to lua
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        SdkToLua::init();
#else
        AoneClientToLua::init();
        // aoneskd set language
//        aonesdk::AoneClient::SelectLang("zhcn");
#endif
		// init appkey in socket
		AESocketClient::init(AEUtil::getAppKey());

		// create a scene. it's an autorelease object
		auto scene = GameScene::createScene();
        CCLOG("CC_TARGET_PLATFORM:%d",CC_TARGET_PLATFORM);
        //SdkWarpperIOS::initSDK();
		// run
		director->runWithScene(scene);
	}
	else if (retCode < 0)
	{
		// create a scene. it's an autorelease object
		auto scene = GameScene::createScene();

		// run
		director->runWithScene(scene);
	}
	else
	{

	}
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect("BlackMoon", cocos2d::Rect(0, 0, AEUtil::getScreenWidth(), AEUtil::getScreenHeight()));
#else
//        glview = GLViewImpl::create("BlackMoon");
#endif
        director->setOpenGLView(glview);
    }

	std::string extPath = FileSystem::dlcdir();
	CCLOG("AppDelegate extPath:%s", extPath.c_str());
	FileUtils::getInstance()->addSearchPath(extPath, true);
	FileUtils::getInstance()->addSearchPath(extPath + "/res", true);
	FileUtils::getInstance()->addSearchPath(extPath + "/src", true);
	FileUtils::getInstance()->addSearchPath(extPath + "/src/imports", true);
	FileUtils::getInstance()->addSearchPath(extPath + "/src/imports/table", true);
	FileUtils::getInstance()->addSearchPath("src");
	FileUtils::getInstance()->addSearchPath("res");


	std::string path = patcher_path(patcher_get_default());

	//
	patcher_set_path(patcher_get_default(), FileSystem::dlcdir().c_str());
	patcher_check(patcher_get_default());
    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

	CCTexture2D::PVRImagesHavePremultipliedAlpha(true);

    // Set the design resolution
	cocos2d::Size designResolutionSize = cocos2d::Size(1136, 640);
	cocos2d::Size realSize = cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
	AEUtil::initAdaption();

    //glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    //auto frameSize = glview->getFrameSize();

	//director->setContentScaleFactor(frameSize.height / designResolutionSize.height);

//     // if the frame's height is larger than the height of medium size.
// 	if (frameSize.height > mediumResolutionSize.height)
// 	{
// 		director->setContentScaleFactor(MIN(largeResolutionSize.height / designResolutionSize.height, largeResolutionSize.width / designResolutionSize.width));
// 	}
//     // if the frame's height is larger than the height of small size.
//     else if (frameSize.height > smallResolutionSize.height)
//     {
//         director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
//     }
//     // if the frame's height is smaller than the height of medium size.
//     else
//     {
//         director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
//     }

	// register lua module
	auto engine = LuaEngine::getInstance();
	ScriptEngineManager::getInstance()->setScriptEngine(engine);
	lua_State* L = engine->getLuaStack()->getLuaState();
	lua_module_register(L);

    register_all_packages();

	//LuaStack* stack = engine->getLuaStack();
	//stack->setXXTEAKeyAndSign("2dxLua", strlen("2dxLua"), "XXTEA", strlen("XXTEA"));
	LuaStack* s = engine->getLuaStack();
	s->setXXTEAKeyAndSign("smartspace", strlen("smartspace"), "SMSP", strlen("SMSP"));
	s->addSearchPath("src/");
	// register custom function
	register_custom_function(L);

	// create spine cache
	AESpineCache::getInstance();

	xe::XESelector::getInstance();
	xe::XESelector::getInstance()->start();
	//xe::XELog::start();
	// neon
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	bool enable = MathUtil::isNeon64Enabled();
	if (enable)
	{
		CCLOG("USE 64 NEON !!!");
	}
	else
	{
		enable = MathUtil::isNeon32Enabled();
		if (enable)
		{
			CCLOG("USE 32 NEON !!!");
		}
	}
#endif

	// set aonesdk search path
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	const std::vector<std::string>& paths = FileUtils::getInstance()->getSearchPaths();
	//aonesdk::AoneClient::AddFileSearchPath(paths[0]);
#endif

	// register aone client to lua
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	SdkToLua::init();
#else
	AoneClientToLua::init();
#endif


	// init appkey in socket
	AESocketClient::init(AEUtil::getAppKey());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        BuglyLuaAgent::registerLuaExceptionHandler(engine);
#endif

	// create a scene. it's an autorelease object
	auto scene = GameScene::createScene();
	CCLOG("CC_TARGET_PLATFORM:%d", CC_TARGET_PLATFORM);
	//SdkWarpperIOS::initSDK();
	// run
	director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be paused
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    //进入后台
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("APP_ENTER_BACKGROUND_EVENT");
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    //回到游戏
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("APP_ENTER_FOREGROUND_EVENT");
}
