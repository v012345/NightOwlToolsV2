/**************************************************************************
    *  @Copyright (c) 2017, A-one Soft, All rights reserved.

    *  @file			: AESpine.h
    *  @version	: ver 1.0.0

    *  @author	: andy-zhao
    *  @date		: 2017/3/20
    *  @brief		: 骨骼动画
**************************************************************************/
#ifndef _AESpineCache_H_
#define _AESpineCache_H_

#include "platform/AEStdC.h"
#include "external/spine/AESpineData.h"
#include <spine/spine-cocos2dx.h>
#include <spine/extension.h>

NS_AE_BEGIN
using namespace spine;

class AESpine;
class AESkeletonAsync
{
	friend class AESpineCache;
public:
	AESkeletonAsync();
	AESkeletonAsync(const std::string& skeletonFile, spAtlas* const atlas, float32 scale, AESkeletonType type, const std::function<void(AESpine*)>& callback);
	virtual ~AESkeletonAsync();

	/**
		初始skeleton。
	*/
	void initSkeleton();

	/**
		初始spine。
	*/
	void initSpine();

	/**
		获取AESpine。
	*/
	AESpine* getSpine() { return m_pSpine; }

protected:
	// spine
	AESpine* m_pSpine;
	// file name
	std::string m_sSkeletonFile;
	// atlas
	spAtlas* m_pAtlas;
	// scale
	float32 m_fScale;
	// type
	AESkeletonType m_eSkeletonType;
	// callback
	std::function<void(AESpine*)> m_pCallback;

};

typedef std::deque<AESpine*> AESpineDeque;

class AESpineCache : public Ref
{
public:
	static AESpineCache* getInstance();

	static void destroyInstance();

	/**
		创建AESpine，通过Thread。
	*/
	AESpine* createWithJsonFileByThread(const std::string& skeletonJsonFile, spAtlas* atlas, float32 scale = 1, const std::function<void(AESpine*)>& callback = nullptr);
	AESpine* createWithJsonFileByThread(const std::string& skeletonJsonFile, const std::string& atlasFile, float32 scale = 1, const std::function<void(AESpine*)>& callback = nullptr, bool async = false);
	AESpine* createWithBinaryFileByThread(const std::string& skeletonBinaryFile, spAtlas* atlas, float32 scale = 1, const std::function<void(AESpine*)>& callback = nullptr);
	AESpine* createWithBinaryFileByThread(const std::string& skeletonBinaryFile, const std::string& atlasFile, float32 scale = 1, const std::function<void(AESpine*)>& callback = nullptr, bool async = false);

	/**
		预加载SpineData数据。
	*/
	void preloadWithJsonFile(const std::string& skeletonDataFile, const std::string& atlasFile, float32 scale = 1.0f, bool async = false);
	void preloadWithBinaryFile(const std::string& skeletonDataFile, const std::string& atlasFile, float32 scale = 1.0f, bool async = false);
	void clearPreload(const std::string& skeletonDataFile);

	/**
		创建AESpine。
	*/
	AESpine* createWithJsonFile(const std::string& skeletonJsonFile, spAtlas* atlas, float32 scale = 1);
	AESpine* createWithJsonFile(const std::string& skeletonJsonFile, const std::string& atlasFile, float32 scale = 1, bool async = false);
	AESpine* createWithBinaryFile(const std::string& skeletonBinaryFile, spAtlas* atlas, float32 scale = 1);
	AESpine* createWithBinaryFile(const std::string& skeletonBinaryFile, const std::string& atlasFile, float32 scale = 1, bool async = false);

	/**
		回收AESpine。
	*/
	void recoverySpine(AESpine* spine);

	/**
		清空AESpine。
	*/
	void clearSpine();

	AESpineCache();
	virtual ~AESpineCache();

private:
	/**
		初始化AESpine。
	*/
	AESpine* initSkeletonWithFile(const std::string& skeletonJsonFile, spAtlas* atlas, AESkeletonType type, float32 scale, const std::function<void(AESpine*)>& callback);

	void initSkeletonAsync();
	void initSkeletonAsyncCallBack(float delta);

	/**
		存入AESpine缓存。
	*/
	void pushSpineCache(AESpine* spine);

	/**
		获取AESpine。
	*/
	AESpine* popSpineCache(const std::string& skeletonBinaryFile);

protected:
	static AESpineCache* s_pSpineCache;

	// thread
	std::thread* m_pLoadingThread;

	// deque
	std::deque<AESkeletonAsync*> m_pAsyncStructQueue;
	std::deque<AESkeletonAsync*> m_pRequestQueue;
	std::deque<AESkeletonAsync*> m_pResponseQueue;

	// lock
	std::mutex m_pRequestMutex;
	std::mutex m_pResponseMutex;

	// sleep
	std::condition_variable m_pSleepCondition;

	// quit
	bool m_bNeedQuit;
	
	// ref
	int m_iAsyncRefCount;

	// AESpine 缓存。
	std::map<std::string, AESpineDeque> m_mSpine;

};

NS_AE_END

#endif // _AESpineCache_H_