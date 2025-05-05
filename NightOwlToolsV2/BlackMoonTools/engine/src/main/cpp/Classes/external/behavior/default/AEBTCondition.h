/**************************************************************************
*  @Copyright (c) 2017, A-one Soft, All rights reserved.

*  @file			: AEBTCondition.h
*  @version	: ver 1.0.0

*  @author	: andy-zhao
*  @date		: 2017/4/26
*  @brief		: 自定义行为树。
**************************************************************************/
#ifndef _AEBTCondition_H_
#define _AEBTCondition_H_

#include "platform/AEStdC.h"
#include "external/behavior/default/AEBTNode.h"

NS_AE_BEGIN

class AEBTCondition : public AEBTNode
{
public:
	AEBTCondition();
	virtual ~AEBTCondition();

	/**
		创建。
	*/
	static AEBTCondition* create();

	/**
		刷新。
	*/
	virtual void update(float32 delta) override;

	/**
		条件检测。
	*/
	virtual bool check();

	/**
		开始执行。
	*/
	virtual void enter() override;

	/**
		退出执行。
	*/
	virtual void exit() override;

};

NS_AE_END

#endif // _AEBTCondition_H_