/**************************************************************************
    *  @Copyright (c) 2017, A-one Soft, All rights reserved.

    *  @file			: AEUtil.h
    *  @version	: ver 1.0.0

    *  @author	: andy-zhao
    *  @date		: 2017/3/2
    *  @brief		: 通用工具
**************************************************************************/
#ifndef _AEUtil_H_
#define _AEUtil_H_

#include "platform/AEStdC.h"
#include "AERandom.h"
NS_AE_BEGIN

class AEUtil
{
public:
	AEUtil();
	virtual ~AEUtil();

	/**
		获取appKey。
	*/
	static std::string getAppKey();

	/**
		退出游戏。
	*/
	static void exitGame();

	/**
		设置屏幕适配。
	*/
	static void setScreenSize(float32 width, float32 hight);
	static int32 getScreenWidth() { return s_fscreenwidth; }
	static int32 getScreenHeight() { return s_fscreenheight; }
	static void initAdaption();
	static void uiAdaption(Node *rootNode);

	static float32 s_fscreenwidth;
	static float32 s_fscreenheight;
	static float32 s_fdesignwidth;
	static float32 s_fdesignheight;
	static float32 s_fuiscale;
	static bool	   s_bneedadaption;
	static float32 s_foffsetx;
	static float32 s_foffsety;




	/**
	设置屏幕适配新版本。
	*/
	static void initAdaptionV1();
	static void uiAdaptionV1(Node *rootNode);
	/**
	设置屏幕适配新版本。
	*/



	/**
	 * 添加字符集范围
	 * */
    static void addUnicodeScope(int min, int max);
	static bool verifyString(std::string str);
	static int32 stringLenght(std::string str);
	static int32 computeWordNumber(std::string str);

	/*
		获取当前异步加载数量。
	*/
	static int32 asyncCount();

	/*
		获取点在纹理的色值。
	*/
	static Color4B getImageColor(Image* image, Vec2 point);

	/**
		开发平方。
	*/
	static float32 SQRT(int val);

	/**
		与。
	*/
	static int32 OR(int32 src, int32 dst);

	/**
		或。
	*/
	static int32 AND(int32 src, int32 dst);

	/**
		异或。
	*/
	static int32 XOR(int32 src, int32 dst);

	/**
		系统时间。
	*/
	static float64 getMS();
	static int64 getMS64();

	/**
		随机数。
	*/
	static int32 random();

	template<typename T>
	static T random(T min, T max);

	/**
		随机数，范围。
	*/
	static float64 random(float64 min, float64 max);

	/**
		随机种子。
	*/
	static void randomseed(uint32 seed);

	static std::random_device s_seedgen;
	static mtrandom s_randgen;

	/**
		随机数精度
	*/
	static void setPrecision(uint32 precision);
	
	/** 
		重启游戏
	*/
	static void restartGame();

	/**
		设置多点触摸
	*/
	static void setMultiTouchEnabled(bool bEnabled);
	static bool getMultiTouchEnabled();

	/* get system time */
	static void itimeofday(long *sec, long *usec);
	static int64 getMircoSecond();

	/**
		获取文件MD5值，失败则返回空字符串
	*/
	static std::string getFileMD5(const std::string& filepath);

	/************************************************************************/
	/* 获取字符串的md5值                                                                     */
	/************************************************************************/
	static std::string MD5(const std::string content);

	/**
		获取当前FPS
	*/
	static float32 getFrameFPS();
	
protected:
private:
	
	static uint32 rand_precision;
	static bool m_bEnabled;
};

NS_AE_END

#endif // _AEUtil_H_