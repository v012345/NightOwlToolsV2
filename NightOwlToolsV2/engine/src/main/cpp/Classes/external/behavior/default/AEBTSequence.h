/**************************************************************************
*  @Copyright (c) 2017, A-one Soft, All rights reserved.

*  @file			: AEBTSequence.h
*  @version	: ver 1.0.0

*  @author	: andy-zhao
*  @date		: 2017/4/26
*  @brief		: 自定义行为树。
**************************************************************************/
#ifndef _AEBTSequence_H_
#define _AEBTSequence_H_

#include "platform/AEStdC.h"
#include "external/behavior/default/AEBTComposite.h"

NS_AE_BEGIN

class AEBTSequence : public AEBTComposite
{
public:
	AEBTSequence();
	virtual ~AEBTSequence();

	/**
		创建。
	*/
	static AEBTSequence* create();

	/**
		刷新。
	*/
	virtual void update(float32 delta) override;

	/**
		刷新Selector。
	*/
	void updateSequence(float32 delta);

	/**
		开始执行。
	*/
	virtual void enter() override;

	/**
		退出执行。
	*/
	virtual void exit() override;

private:
	// 顺序节点。
	int32 m_iSequenceIndex;

};

NS_AE_END

#endif // _AEBTSequence_H_