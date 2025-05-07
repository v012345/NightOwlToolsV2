/**************************************************************************
    *  @Copyright (c) 2017, A-one Soft, All rights reserved.

    *  @file			: AESpine.h
    *  @version	: ver 1.0.0

    *  @author	: andy-zhao
    *  @date		: 2017/3/20
    *  @brief		: 骨骼动画
**************************************************************************/
#ifndef _AESpine_H_
#define _AESpine_H_

#include "platform/AEStdC.h"
#include "external/spine/AESpineData.h"
#include <spine/spine-cocos2dx.h>
#include <spine/extension.h>

NS_AE_BEGIN
using namespace spine;

class AESpine : public SkeletonAnimation
{
public:
	/**
		组合Atlas。
	*/
	static spAtlas* appendAtlas(const std::vector<std::string>& atlasFiles);

	/**
		慢动作。
	*/
	static void slowMotion(float32 scale) { s_fScaleTime = scale; }

	// 时间缩放比
	static float s_fScaleTime;

public:
	AESpine();
	AESpine(bool thread);
	virtual ~AESpine();

	bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void update(float32 delta) override;
	void initialize();

	void draw(Renderer* renderer, const Mat4& transform, uint32_t transformFlags) override;

	/**
		设置SpineData。
	*/
	void initSpineData(AESpineDataPtr spineData);

	/**
		异步替换资源。
	*/
	void replaceAtlas(spAtlas* pAtlas);

	/**
		替换皮肤。
	*/
	void replaceSkinForSlotData(AESpine* spine, const char* skinName, const char* attachmentName);

	/**
		重写设置动画。
	*/
	spTrackEntry* setAnimation(int32 trackIndex, const std::string& name, bool loop);

	/**
		改编动画。
	*/
	float32 changeAnimation(int32 trackIndex, const std::string& name, bool loop);

	/**
		获取Box顶点。
	*/
	void getVertices(float32* vertices, uint32* verticesCount, std::string name = "box2d");

	// 当前动画时间
	float32 getDuration() { return m_fDuration; }

	// 拥有数据
	bool isOwnsSkeletonData() { return _ownsSkeletonData; }

	// 加载完成
	bool isInitComplete() { return m_bInitComplete; }

	// 文件名
	AE_SYNTHESIZE(std::string, m_sSkeletonDataFile, SkeletonDataFile);

	// 暂停
	AE_SYNTHESIZE_REF(bool, m_bPause, Pause);

	// 缓存
	AE_SYNTHESIZE_REF(bool, m_bCache, Cache);

	// 重写 获取boundingbox
	virtual cocos2d::Rect getBoundingBox() const override;

	//恢复渲染
	void recoveryGLProgramState();

private:
	// 数据
	AESpineDataPtr m_ptrSpineData;
	// 异步
	bool m_bAsync;
	// 初始化完成
	bool m_bInitComplete;
	// 动画间隔
	float32	m_fDuration;
	// 当前动画数据
	int32 m_iTrackIndex;
	std::string m_sAnimation;
	bool m_bLoop;

};

NS_AE_END

#endif // _AESpine_H_