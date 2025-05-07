#include "GameScene.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "behaviac/common/logger/logger.h"
#include "platform/android/CCApplication-android.h"
#endif
#include "behaviac_generated/types/behaviac_types.h"

USING_NS_CC;
USING_NS_AE;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
	auto scene = GameScene::create();
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
	if (!Scene::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    return true;
}

void GameScene::onEnter()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	behaviac::CLogger::SetLoggingLevel(ETagLogLevel::BEHAVIAC_LOG_NONE);
	std::string filePath = FileUtils::getInstance()->fullPathForFilename("src/imports/behaviac_exported/Entity.xml");
	behaviac::Workspace::GetInstance()->SetFilePath(filePath.substr(0, filePath.find("Entity.xml")).c_str());
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	auto amgr = FileUtilsAndroid::getAssetManager();
	behaviac::CFileManager::GetInstance()->SetAssetManager(amgr);
	behaviac::CLogger::SetLoggingLevel(ETagLogLevel::BEHAVIAC_LOG_NONE);
	behaviac::Workspace::GetInstance()->SetFilePath("assets:/src/imports/behaviac_exported");
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	behaviac::CLogger::SetLoggingLevel(ETagLogLevel::BEHAVIAC_LOG_NONE);
    std::string filePath = FileUtils::getInstance()->fullPathForFilename("src/imports/behaviac_exported/Entity.xml");
    behaviac::Workspace::GetInstance()->SetFilePath(filePath.substr(0, filePath.find("Entity.xml")).c_str());
#endif
	behaviac::Workspace::GetInstance()->SetFileFormat(behaviac::Workspace::EFF_xml);
    
	Scene::onEnter();
	// register lua module
	auto engine = LuaEngine::getInstance();

	if (engine->executeScriptFile("main.lua"))
	{
		return;
	}
}

void GameScene::onExit()
{
	behaviac::Workspace::GetInstance()->Cleanup();

	Scene::onExit();
}
