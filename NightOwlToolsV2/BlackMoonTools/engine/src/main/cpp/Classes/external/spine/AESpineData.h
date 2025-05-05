/**************************************************************************
    *  @Copyright (c) 2018, A-one Soft, All rights reserved.

    *  @file			: AESpineData.h
    *  @version	: ver 1.0.0

    *  @author	: andy-zhao
    *  @date		: 2018/2/23
    *  @brief		: 骨骼动画Data
**************************************************************************/
#ifndef _AESpineData_H_
#define _AESpineData_H_

#include "platform/AEStdC.h"
#include "spine/spine-cocos2dx.h"
#include "spine/extension.h"

NS_AE_BEGIN
using namespace spine;

enum AESkeletonType
{
	ST_JSON = 1,
	ST_BINARY = 2,
};

class AESpine;
class AESpineData;
typedef AEShared_ptr<AESpineData> AESpineDataPtr;

class AESpineData
{
	friend class AESpine;
	friend class AESpineCache;
public:
	/**
		创建SpineData。
	*/
	static AESpineDataPtr createSpineData(const std::string& skeletonDataFile, AESkeletonType type, const std::string& atlasFile, float32 scale = 1.0f, bool async = false);

	/**
		销毁SpineData。
	*/
	static void destroySpineData(const std::string& skeletonDataFile);

	AESpineData();
	virtual ~AESpineData();

	/**
		引用计数+1。
	*/
	void retain();

	/**
		引用计数-1，直到为0释放。
	*/
	void release();

	/**
		获取引用计数。
	*/
	uint32 getRefCount() { return m_iRefCount; }

	// AESpineData 缓存。
	static std::map<std::string, AESpineDataPtr> s_mSpineData;

protected:
	// skeleton
	spSkeletonData* m_pSkeletonData;
	// atlas
	spAtlas* m_pAtlas;
	// attachment
	spAttachmentLoader* m_pAttachmentLoader;
	// map key
	std::string m_sSkeletonDataFile;
	// ref count
	uint32 m_iRefCount;

};

NS_AE_END

#endif // _AESpineData_H_