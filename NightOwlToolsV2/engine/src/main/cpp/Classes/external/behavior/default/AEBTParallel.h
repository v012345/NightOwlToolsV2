/**************************************************************************
*  @Copyright (c) 2017, A-one Soft, All rights reserved.

*  @file			: AEBTNode.h
*  @version	: ver 1.0.0

*  @author	: andy-zhao
*  @date		: 2017/4/26
*  @brief		: 自定义行为树。
**************************************************************************/
#ifndef _AEBTParallel_H_
#define _AEBTParallel_H_

#include "platform/AEStdC.h"
#include "external/behavior/default/AEBTComposite.h"

NS_AE_BEGIN

class AEBTParallel : public AEBTComposite
{
public:
	AEBTParallel();
	virtual ~AEBTParallel();

	/**
		创建。
	*/
	static AEBTParallel* create();

	/**
		刷新。
	*/
	virtual void update(float32 delta) override;

	/**
		刷新Parallel。
	*/
	void updateParallel(float32 delta);

	/**
		开始执行。
	*/
	virtual void enter() override;

	/**
		退出执行。
	*/
	virtual void exit() override;

private:
	// 并行节点。
	std::list<uint32> m_pParallelIndex;

};

NS_AE_END

#endif // _AEBTParallel_H_