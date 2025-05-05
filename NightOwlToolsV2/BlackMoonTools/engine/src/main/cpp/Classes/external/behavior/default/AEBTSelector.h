/**************************************************************************
*  @Copyright (c) 2017, A-one Soft, All rights reserved.

*  @file			: AEBTSelector.h
*  @version	: ver 1.0.0

*  @author	: andy-zhao
*  @date		: 2017/4/26
*  @brief		: 自定义行为树。
**************************************************************************/
#ifndef _AEBTSelector_H_
#define _AEBTSelector_H_

#include "platform/AEStdC.h"
#include "external/behavior/default/AEBTComposite.h"

NS_AE_BEGIN

class AEBTSelector : public AEBTComposite
{
public:
	AEBTSelector();
	virtual ~AEBTSelector();

	/**
		创建。
	*/
	static AEBTSelector* create();

	/**
		刷新。
	*/
	virtual void update(float32 delta) override;

	/**
		刷新Selector。
	*/
	void updateSelector(float32 delta);

	/**
		开始执行。
	*/
	virtual void enter() override;

	/**
		退出执行。
	*/
	virtual void exit() override;

private:
	// 选择节点。
	int32 m_iSelectorIndex;

};

NS_AE_END

#endif // _AEBTSelector_H_