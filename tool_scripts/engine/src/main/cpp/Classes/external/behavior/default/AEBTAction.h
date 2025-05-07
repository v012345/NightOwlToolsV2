/**************************************************************************
*  @Copyright (c) 2017, A-one Soft, All rights reserved.

*  @file			: AEBTAction.h
*  @version	: ver 1.0.0

*  @author	: andy-zhao
*  @date		: 2017/4/26
*  @brief		: 自定义行为树。
**************************************************************************/
#ifndef _AEBTAction_H_
#define _AEBTAction_H_

#include "platform/AEStdC.h"
#include "external/behavior/default/AEBTNode.h"

NS_AE_BEGIN

class AEBTAction : public AEBTNode
{
public:
	AEBTAction();
	virtual ~AEBTAction();

	/**
		创建。
	*/
	static AEBTAction* create();

	/**
		刷新。
	*/
	virtual void update(float32 delta) override;

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

#endif // _AEBTAction_H_