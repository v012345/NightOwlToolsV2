#include "external/spine/AESpineCache.h"
#include "external/spine/AESpine.h"

extern int g_iAtlasAsync;

NS_AE_BEGIN

AESkeletonAsync::AESkeletonAsync()
: m_pSpine(nullptr)
, m_pAtlas(nullptr)
{
	m_fScale = 1.0f;
	m_eSkeletonType = AESkeletonType::ST_JSON;
	m_pCallback = nullptr;
}

AESkeletonAsync::AESkeletonAsync(const std::string& skeletonFile, spAtlas* const atlas, float32 scale, AESkeletonType type, const std::function<void(AESpine*)>& callback)
: m_pAtlas(atlas)
{
	m_pSpine = new AESpine(true);
	m_sSkeletonFile = skeletonFile;
	m_fScale = scale;
	m_eSkeletonType = type;
	m_pCallback = callback;
}

AESkeletonAsync::~AESkeletonAsync()
{

}

void AESkeletonAsync::initSkeleton()
{
	if (AESkeletonType::ST_JSON == m_eSkeletonType)
	{
		m_pSpine->initWithJsonFile(m_sSkeletonFile, m_pAtlas, m_fScale, true);
		m_pSpine->setSkeletonDataFile(m_sSkeletonFile);
	}
	else if (AESkeletonType::ST_BINARY == m_eSkeletonType)
	{
		m_pSpine->initWithBinaryFile(m_sSkeletonFile, m_pAtlas, m_fScale, true);
		m_pSpine->setSkeletonDataFile(m_sSkeletonFile);
	}
}

void AESkeletonAsync::initSpine()
{
	m_pSpine->initialize();

	if (m_pSpine->isRunning() && nullptr != m_pCallback)
	{
		m_pCallback(m_pSpine);
	}
}

AESpineCache* AESpineCache::s_pSpineCache = nullptr;

AESpineCache* AESpineCache::getInstance()
{
	if (nullptr == s_pSpineCache)
	{
		s_pSpineCache = new (std::nothrow) AESpineCache();
	}
	return s_pSpineCache;
}

void AESpineCache::destroyInstance()
{
	AESafe_delete(s_pSpineCache);
}

AESpine* AESpineCache::createWithJsonFileByThread(const std::string& skeletonJsonFile, spAtlas* atlas, float32 scale, const std::function<void(AESpine*)>& callback)
{
	AESpine* spine = popSpineCache(skeletonJsonFile);
	if (nullptr != spine)
	{
		spine->replaceAtlas(atlas);
		spine->autorelease();
		callback(spine);

		return spine;
	}

	return initSkeletonWithFile(skeletonJsonFile, atlas, AESkeletonType::ST_JSON, scale, callback);
}

AESpine* AESpineCache::createWithJsonFileByThread(const std::string& skeletonJsonFile, const std::string& atlasFile, float32 scale, const std::function<void(AESpine*)>& callback, bool async)
{
	std::map<std::string, AESpineDataPtr>::iterator iterData = AESpineData::s_mSpineData.find(skeletonJsonFile);
	if (AESpineData::s_mSpineData.end() != iterData)
	{
		AESpineDataPtr ptrSpineData = iterData->second;
		ptrSpineData->retain();

		auto spine = new AESpine();
		spine->initSpineData(ptrSpineData);
		spine->autorelease();
		callback(spine);

		return spine;
	}

	g_iAtlasAsync = (int)async;
	spAtlas* atlas = spAtlas_createFromFile(atlasFile.c_str(), 0);
	AEASSERT(atlas, "Error reading atlas file.");

	return initSkeletonWithFile(skeletonJsonFile, atlas, AESkeletonType::ST_JSON, scale, callback);
}

AESpine* AESpineCache::createWithBinaryFileByThread(const std::string& skeletonBinaryFile, spAtlas* atlas, float32 scale, const std::function<void(AESpine*)>& callback)
{
	AESpine* spine = popSpineCache(skeletonBinaryFile);
	if (nullptr != spine)
	{
		spine->replaceAtlas(atlas);
		spine->autorelease();
		callback(spine);

		return spine;
	}

	return initSkeletonWithFile(skeletonBinaryFile, atlas, AESkeletonType::ST_BINARY, scale, callback);
}

AESpine* AESpineCache::createWithBinaryFileByThread(const std::string& skeletonBinaryFile, const std::string& atlasFile, float32 scale, const std::function<void(AESpine*)>& callback, bool async)
{
	std::map<std::string, AESpineDataPtr>::iterator iterData = AESpineData::s_mSpineData.find(skeletonBinaryFile);
	if (AESpineData::s_mSpineData.end() != iterData)
	{
		AESpineDataPtr ptrSpineData = iterData->second;
		ptrSpineData->retain();

		auto spine = new AESpine();
		spine->initSpineData(ptrSpineData);
		spine->autorelease();
		callback(spine);

		return spine;
	}

	g_iAtlasAsync = (int)async;
	spAtlas* atlas = spAtlas_createFromFile(atlasFile.c_str(), 0);
	AEASSERT(atlas, "Error reading atlas file.");

	return initSkeletonWithFile(skeletonBinaryFile, atlas, AESkeletonType::ST_BINARY, scale, callback);
}

void AESpineCache::preloadWithJsonFile(const std::string& skeletonDataFile, const std::string& atlasFile, float32 scale, bool async)
{
	AESpineData::createSpineData(skeletonDataFile, AESkeletonType::ST_JSON, atlasFile, scale, async);
}

void AESpineCache::preloadWithBinaryFile(const std::string& skeletonDataFile, const std::string& atlasFile, float32 scale, bool async)
{
	AESpineData::createSpineData(skeletonDataFile, AESkeletonType::ST_BINARY, atlasFile, scale, async);
}

void AESpineCache::clearPreload(const std::string& skeletonDataFile)
{
	AESpineData::destroySpineData(skeletonDataFile);
}

AESpine* AESpineCache::createWithJsonFile(const std::string& skeletonJsonFile, spAtlas* atlas, float32 scale)
{
	AESpine* spine = popSpineCache(skeletonJsonFile);
	if (nullptr != spine)
	{
		spine->setScale(scale);
		spine->replaceAtlas(atlas);
		spine->autorelease();

		return spine;
	}

	spine = new AESpine();
	spine->initWithJsonFile(skeletonJsonFile, atlas, scale);
	spine->setSkeletonDataFile(skeletonJsonFile);
	spine->autorelease();

	return spine;
}

AESpine* AESpineCache::createWithJsonFile(const std::string& skeletonJsonFile, const std::string& atlasFile, float32 scale, bool async)
{
	auto ret = new AESpine();
	AESpineDataPtr ptrSpineData = AESpineData::createSpineData(skeletonJsonFile, AESkeletonType::ST_JSON, atlasFile, scale, async);
	ret->initSpineData(ptrSpineData);
	ret->autorelease();

	return ret;
}

AESpine* AESpineCache::createWithBinaryFile(const std::string& skeletonBinaryFile, spAtlas* atlas, float32 scale)
{
	AESpine* spine = popSpineCache(skeletonBinaryFile);
	if (nullptr != spine)
	{
		spine->setScale(scale);
		spine->replaceAtlas(atlas);
		spine->autorelease();

		return spine;
	}

	spine = new AESpine();
	spine->initWithBinaryFile(skeletonBinaryFile, atlas, scale);
	spine->setSkeletonDataFile(skeletonBinaryFile);
	spine->autorelease();

	return spine;
}

AESpine* AESpineCache::createWithBinaryFile(const std::string& skeletonBinaryFile, const std::string& atlasFile, float32 scale, bool async)
{
	auto ret = new AESpine();
	AESpineDataPtr ptrSpineData = AESpineData::createSpineData(skeletonBinaryFile, AESkeletonType::ST_BINARY, atlasFile, scale, async);
	ret->initSpineData(ptrSpineData);
	ret->autorelease();

	return ret;
}

void AESpineCache::recoverySpine(AESpine* spine)
{
	if (spine->isInitComplete() && spine->isOwnsSkeletonData())
	{
		pushSpineCache(spine);
	}

	if (1 == spine->getReferenceCount())
	{
		spine->retain();
	}

	spine->setCache(true);
	spine->removeFromParent();
}

void AESpineCache::clearSpine()
{
	std::map<std::string, AESpineDeque>::iterator iterData = m_mSpine.begin();
	for (; iterData != m_mSpine.end(); ++iterData)
	{
		AESpineDeque& spineDeque = iterData->second;
		while (!spineDeque.empty())
		{
			AESpine* spine = spineDeque.front();
			spineDeque.pop_front();
			spine->release();
		}
	}
	m_mSpine.clear();

	for (int32 i = 0; i < m_pAsyncStructQueue.size(); i++)
	{
		AESkeletonAsync* pSkeletonAsync = m_pAsyncStructQueue.at(i);
		pSkeletonAsync->getSpine()->setCache(false);
	}
}

AESpineCache::AESpineCache()
: m_pLoadingThread(nullptr)
, m_bNeedQuit(false)
, m_iAsyncRefCount(0)
{

}

AESpineCache::~AESpineCache()
{

}

AESpine* AESpineCache::initSkeletonWithFile(const std::string& skeletonJsonFile, spAtlas* atlas, AESkeletonType type, float32 scale, const std::function<void(AESpine*)>& callback)
{
	if (nullptr == m_pLoadingThread)
	{
		// create a new thread to load images
		m_pLoadingThread = new (std::nothrow) std::thread(&AESpineCache::initSkeletonAsync, this);
		m_bNeedQuit = false;
	}

	if (0 == m_iAsyncRefCount)
	{
		Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(AESpineCache::initSkeletonAsyncCallBack), this, 0, false);
	}

	++m_iAsyncRefCount;

	// generate async struct
	AESkeletonAsync* pSkeletonAsync = new (std::nothrow) AESkeletonAsync(skeletonJsonFile, atlas, scale, type, callback);

	// add async struct into queue
	m_pAsyncStructQueue.push_back(pSkeletonAsync);
	m_pRequestMutex.lock();
	m_pRequestQueue.push_back(pSkeletonAsync);
	m_pRequestMutex.unlock();

	m_pSleepCondition.notify_one();

	return pSkeletonAsync->getSpine();
}

void AESpineCache::initSkeletonAsync()
{
	AESkeletonAsync* pSkeletonAsync = nullptr;
	std::mutex signalMutex;
	std::unique_lock<std::mutex> signal(signalMutex);
	while (!m_bNeedQuit)
	{
		// pop an AsyncStruct from request queue
		m_pRequestMutex.lock();
		if (!m_pRequestQueue.empty())
		{
			pSkeletonAsync = m_pRequestQueue.front();
			m_pRequestQueue.pop_front();
		}
		m_pRequestMutex.unlock();

		if (nullptr == pSkeletonAsync)
		{
			m_pSleepCondition.wait(signal);
			continue;
		}

		// init skeleton
		pSkeletonAsync->initSkeleton();

		// push the asyncStruct to response queue
		m_pResponseMutex.lock();
		m_pResponseQueue.push_back(pSkeletonAsync);
		m_pResponseMutex.unlock();

		pSkeletonAsync = nullptr;
	}
}

void AESpineCache::initSkeletonAsyncCallBack(float delta)
{
	auto now = std::chrono::steady_clock::now();
	float animationInterval = Director::getInstance()->getAnimationInterval();
	AESkeletonAsync* pSkeletonAsync = nullptr;
	while (true)
	{
		m_pResponseMutex.lock();
		if (!m_pResponseQueue.empty())
		{
			m_pAsyncStructQueue.pop_front();
			pSkeletonAsync = m_pResponseQueue.front();
			m_pResponseQueue.pop_front();
		}
		m_pResponseMutex.unlock();

		if (nullptr == pSkeletonAsync)
		{
			break;
		}

		pSkeletonAsync->initSpine();

		AESpine* spine = pSkeletonAsync->getSpine();
		if (spine->getCache())
		{
			pushSpineCache(spine);
		}
		else
		{
			spine->autorelease();
		}

		delete pSkeletonAsync;
		pSkeletonAsync = nullptr;
		--m_iAsyncRefCount;

		float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - now).count() / 1000000.0f;
		deltaTime = MAX(0, deltaTime);

		if (deltaTime > animationInterval / 10)
		{
			break;
		}
	}

	if (0 == m_iAsyncRefCount)
	{
		Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(AESpineCache::initSkeletonAsyncCallBack), this);
	}
}

void AESpineCache::pushSpineCache(AESpine* spine)
{
	AEASSERT(spine->isInitComplete(), "");

	std::string skeletonDataFile = spine->getSkeletonDataFile();
	std::map<std::string, AESpineDeque>::iterator iterData = m_mSpine.find(skeletonDataFile);
	if (m_mSpine.end() != iterData)
	{
		AESpineDeque& qeque = iterData->second;
		qeque.push_back(spine);
	}
	else
	{
		AESpineDeque qeque;
		qeque.push_back(spine);
		m_mSpine.insert(std::map<std::string, AESpineDeque>::value_type(skeletonDataFile, qeque));
	}
}

AESpine* AESpineCache::popSpineCache(const std::string& skeletonBinaryFile)
{
	std::map<std::string, AESpineDeque>::iterator iterData = m_mSpine.find(skeletonBinaryFile);
	if (m_mSpine.end() != iterData)
	{
		AESpineDeque& qeque = iterData->second;
		AESpine* spine = qeque.front();
		qeque.pop_front();

		if (qeque.empty())
		{
			m_mSpine.erase(iterData);
		}

		return spine;
	}

	return nullptr;
}

NS_AE_END
