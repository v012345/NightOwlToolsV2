#include "common/AEUtil.h"
#include "aone/encoding/encoding.h"
#include "AoneClient.h"
#include "aone/channel/channel_dispatcher.h"
#include "ui/UIHelper.h"
#include "platform/CCPlatformConfig.h"
#include "cocostudio/CCComExtensionData.h"
#include "ui/UILayoutComponent.h"
#include "common/AEMD5.h"

#include "time.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "windows.h"
#else
#include <unistd.h> 
#include "sys/time.h"
#endif

#include "scene/GameScene.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "platform/ios/CCGLViewImpl-ios.h"
#endif


extern void setMultipleTouchEnabled_android(bool val);

USING_NS_CC;
NS_AE_BEGIN

float32 AEUtil::s_fscreenwidth = 1136;
float32 AEUtil::s_fscreenheight = 640;
float32 AEUtil::s_fdesignwidth = 1136;
float32 AEUtil::s_fdesignheight = 640;
float32 AEUtil::s_fuiscale = 1;
bool	AEUtil::s_bneedadaption = false;
float32 AEUtil::s_foffsetx = 0;
float32 AEUtil::s_foffsety = 0;
uint32 AEUtil::rand_precision = 100;
bool AEUtil::m_bEnabled = true;

std::random_device AEUtil::s_seedgen;
mtrandom AEUtil::s_randgen(AEUtil::s_seedgen());

AEUtil::AEUtil()
{

}

AEUtil::~AEUtil()
{

}

std::string AEUtil::getAppKey()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	std::string dir = FileUtils::getInstance()->fullPathForFilename("dreamsdk.json");
#else
    std::string dir = FileUtils::getInstance()->fullPathForFilename("aonesdk.json");
#endif
	std::string content = FileUtils::getInstance()->getStringFromFile(dir);

	Json::Value jvalue;
	Json::Reader jreader;

	if (false == jreader.parse(content, jvalue))
		return "";

	return jvalue["appKey"].asCString();
}

void AEUtil::exitGame()
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
	aonesdk::AoneClient::Exit();
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	exit(0);
#else
	ChannelDispatchCenter::inst()->tochannel("{ \"id\" : \"exit\" }");
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void AEUtil::setScreenSize(float width, float hight)
{
	s_fscreenwidth = width;
	s_fscreenheight = hight;
}

void AEUtil::initAdaptionV1()
{
	cocos2d::Size logicSize = cocos2d::Director::getInstance()->getVisibleSize();
	//cocos2d::Size realSize = cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
	cocos2d::Rect safeSize = cocos2d::Director::getInstance()->getSafeAreaRect();
	//安全区域宽高比低于1.7采用UI缩放
	if (safeSize.size.width / safeSize.size.height < 1.7 || safeSize.size.width<logicSize.width || safeSize.size.height< logicSize.height)
		s_bneedadaption = true;
	//计算安全区域缩放比例 
	if (safeSize.size.height < s_fdesignheight)
	{
		s_fuiscale = safeSize.size.height / s_fdesignheight;
	}
	else if (safeSize.size.width < s_fdesignwidth)
	{
		s_fuiscale = safeSize.size.width / s_fdesignwidth;
	}
}

void AEUtil::uiAdaptionV1(Node *rootNode)
{
	cocos2d::Size logicSize = cocos2d::Director::getInstance()->getVisibleSize();
	//cocos2d::Size realSize = cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();

	cocos2d::Rect safeSize = cocos2d::Director::getInstance()->getSafeAreaRect();

	logicSize.width = safeSize.size.width;
	logicSize.height = safeSize.size.height;
	CCLOG("zt AEUtil::uiAdaption safeSize.x = %.2f safeSize.y = %.2f", safeSize.origin.x, safeSize.origin.y);
	CCLOG("zt AEUtil::uiAdaption safeSize.w = %.2f safeSize.h = %.2f", safeSize.size.width, safeSize.size.height);
	cocostudio::ComExtensionData* extData = (cocostudio::ComExtensionData*)(rootNode->getComponent("ComExtensionData"));
	if (extData)
	{
		std::string userdata = extData->getCustomProperty();
		rapidjson::Document doc;
		doc.Parse<rapidjson::kParseDefaultFlags>(userdata.c_str());
		//解析错误
		if (doc.HasParseError())
		{
			rootNode->setContentSize(logicSize);
			cocos2d::ui::Helper::doLayout(rootNode);
		}
		else
		{
			//缩放
			if (s_bneedadaption && doc.HasMember("zoom"))
			{
				if (doc["zoom"].GetBool())
				{
					rootNode->setContentSize(cocos2d::Size(logicSize.width / s_fuiscale, logicSize.height / s_fuiscale));
					rootNode->setScale(s_fuiscale);
					cocos2d::ui::Helper::doLayout_new(rootNode, s_fuiscale);
				}
				else
				{
					rootNode->setContentSize(logicSize);
					rootNode->setScale(1 / s_fuiscale);
					cocos2d::ui::Helper::doLayout(rootNode);
				}
			}
			else
			{
				rootNode->setContentSize(logicSize);
				cocos2d::ui::Helper::doLayout(rootNode);
			}
		}
	}
	else
	{
		rootNode->setContentSize(logicSize);
		cocos2d::ui::Helper::doLayout(rootNode);
	}
}


void AEUtil::initAdaption()
{
	cocos2d::Size logicSize = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Size realSize = cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
	cocos2d::Size safeSize = cocos2d::Director::getInstance()->getSafeAreaSize();
	s_foffsetx = (logicSize.width - logicSize.height / safeSize.height*safeSize.width) / 2;
	//CCLOG("AEUtil::initAdaption:offset x = %d", (int)s_foffsetx);
	//屏幕宽高比低于1.5采用UI缩放
	if (realSize.width / realSize.height < 1.5)
		s_bneedadaption = true;
	if (logicSize.height < s_fdesignheight)
	{
		s_fuiscale = logicSize.height / s_fdesignheight;
		//s_foffsetx = s_fdesignwidth / s_fuiscale / 2 - logicSize.width / 2;
	}
	else if (logicSize.width < s_fdesignwidth)
	{
		s_fuiscale = logicSize.width / s_fdesignwidth;
		//s_foffsety = s_fdesignheight / s_fuiscale / 2 - logicSize.height / 2;
	}
}

void AEUtil::uiAdaption(Node *rootNode)
{
	cocos2d::Size logicSize = cocos2d::Director::getInstance()->getVisibleSize();
	logicSize.width = logicSize.width - s_foffsetx * 2;
	//CCLOG("AEUtil::uiAdaption:logicSize.width = %d logicSize.height = %d", (int)logicSize.width,(int)logicSize.height);
	cocostudio::ComExtensionData* extData = (cocostudio::ComExtensionData*)(rootNode->getComponent("ComExtensionData"));
	if (extData)
	{
		std::string userdata = extData->getCustomProperty();
		rapidjson::Document doc;
		doc.Parse<rapidjson::kParseDefaultFlags>(userdata.c_str());
		//解析错误
		if (doc.HasParseError())
		{
			rootNode->setContentSize(logicSize);
			rootNode->setPositionX(rootNode->getPositionX() + s_foffsetx);
			cocos2d::ui::Helper::doLayout(rootNode);
		}
		else
		{
			//刘海屏坐标偏移
			if (!doc.HasMember("offset") || doc["offset"].GetBool())
			{
				rootNode->setPositionX(rootNode->getPositionX() + s_foffsetx);
			}
			//缩放
			if (s_bneedadaption && doc.HasMember("zoom"))
			{
				if (doc["zoom"].GetBool())
				{
					rootNode->setContentSize(cocos2d::Size(logicSize.width / s_fuiscale, logicSize.height / s_fuiscale));
					rootNode->setScale(s_fuiscale);
					cocos2d::ui::Helper::doLayout_new(rootNode, s_fuiscale);
				}
				else
				{
					rootNode->setContentSize(logicSize);
					rootNode->setScale(1 / s_fuiscale);
					cocos2d::ui::Helper::doLayout(rootNode);
				}
			}
			else
			{
				rootNode->setContentSize(logicSize);
				cocos2d::ui::Helper::doLayout(rootNode);
			}
		}
	}
	else
	{
		rootNode->setContentSize(logicSize);
		rootNode->setPositionX(rootNode->getPositionX() + s_foffsetx);
		cocos2d::ui::Helper::doLayout(rootNode);
	}
}

void AEUtil::addUnicodeScope(int min,int max)
{
    Encoding::unicode::addUnicodeScope(min, max);
}

bool AEUtil::verifyString(std::string str)
{
	return Encoding::utf8::verifyString(str.c_str());
}

int AEUtil::stringLenght(std::string str)
{
	return Encoding::utf8::computeWordLength_ext(str.c_str());
}

int AEUtil::computeWordNumber(std::string str)
{
	return Encoding::utf8::computeWordNumber(str.c_str());
}

int AEUtil::asyncCount()
{
	return Director::getInstance()->getTextureCache()->asyncCount();
}

Color4B AEUtil::getImageColor(Image * image, Vec2 point)
{
	Color4B color = { 0, 0, 0, 0 };
	auto width  = image->getWidth();
	auto height = image->getHeight();
	if (point.x >= 0 && point.x <= width && point.y >= 0 && point.y <= height)
	{
		auto x = point.x;
		auto y = height - point.y;
		auto data = image->getData();
		unsigned int *pixel = (unsigned int *)data;
		pixel = pixel + (int)y * width + (int)x;
		color.r = *pixel & 0xff;
		color.g = (*pixel >> 8) & 0xff;
		color.b = (*pixel >> 16) & 0xff;
		color.a = (*pixel >> 24) & 0xff;
	}
	return color;
}

float AEUtil::SQRT(int val)
{
	return sqrt(val);
}

int AEUtil::OR(int src, int dst)
{
	return src | dst;
}

int AEUtil::AND(int src, int dst)
{
	return src & dst;
}

int AEUtil::XOR(int src, int dst)
{
	return src ^ dst;
}
	
float64 AEUtil::getMS()
{
	// 
	int64 ret_ = getMS64();
	return static_cast<float64>(0xfffffffful & ret_);
}

int64 AEUtil::getMS64()
{
	long l_secs = 0, l_usecs = 0;
	itimeofday(&l_secs, &l_usecs);
	return static_cast<int64>(l_secs)* 1000 + int32(l_usecs / 1000);
}

int64 AEUtil::getMircoSecond()
{
	long l_secs = 0, l_usecs = 0;
	itimeofday(&l_secs, &l_usecs);
	return static_cast<int64>((l_secs)*1000000 + int32(l_usecs));
}

int32 AEUtil::random()
{
	return AEUtil::random(0, 65535);
}

template<typename T>
T AEUtil::random(T min, T max)
{
	if (min > max) swap(min, max);
	//std::uniform_int_distribution<T> dist(min, max);
	return s_randgen.getRandInt(min*rand_precision, max * rand_precision) / (float64)rand_precision;
}

float64 AEUtil::random(float64 min, float64 max)
{
	if (min > max) swap(min, max);
	//std::uniform_real_distribution<float32> dist(min, max);
	return s_randgen.getRandInt(min*rand_precision, max * rand_precision) / (float64)rand_precision;
}

void AEUtil::randomseed(uint32 seed)
{
	s_randgen.reset(seed);
}

void AEUtil::setPrecision(uint32 precision = 100)
{
	rand_precision = precision;
}

void AEUtil::restartGame()
{
	auto director = Director::getInstance();

	auto scene = GameScene::createScene();
	director->replaceScene(scene);
}


void AEUtil::setMultiTouchEnabled(bool bEnabled)
{
	m_bEnabled = bEnabled;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	cocos2d::GLViewImpl::setMultiTouchEnable(bEnabled);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	setMultipleTouchEnabled_android(bEnabled);
#endif
}

bool AEUtil::getMultiTouchEnabled()
{
	return m_bEnabled;
}
	
/* get system time */
void AEUtil::itimeofday(long *sec, long *usec)
{
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
	struct timeval time;
	gettimeofday(&time, NULL);
	if (sec) *sec = time.tv_sec;
	if (usec) *usec = time.tv_usec;
#else
	static long mode = 0, addsec = 0;
	BOOL retval;
	static int64 freq = 1;
	int64 qpc;
	if (mode == 0) {
		retval = QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
		freq = (freq == 0) ? 1 : freq;
		retval = QueryPerformanceCounter((LARGE_INTEGER*)&qpc);
		addsec = (long)time(NULL);
		addsec = addsec - (long)((qpc / freq) & 0x7fffffff);
		mode = 1;
	}
	retval = QueryPerformanceCounter((LARGE_INTEGER*)&qpc);
	retval = retval * 2;
	if (sec) *sec = (long)(qpc / freq) + addsec;
	if (usec) *usec = (long)((qpc % freq) * 1000000 / freq);
#endif 
}

std::string AEUtil::getFileMD5(const std::string& filepath)
{
	return md5_file(filepath);
}

std::string AEUtil::MD5(const std::string content)
{
	return md5(content.c_str(), content.length());
}

float32 AEUtil::getFrameFPS()
{
	return cocos2d::Director::getInstance()->getFrameRate();
}

NS_AE_END
