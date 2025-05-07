/**************************************************************************
*  @Copyright (c) 2017, A-one Soft, All rights reserved.

*  @file			: AEBTComposite.h
*  @version	: ver 1.0.0

*  @author	: andy-zhao
*  @date		: 2017/4/26
*  @brief		: 自定义行为树。
**************************************************************************/
#ifndef _AEBTComposite_H_
#define _AEBTComposite_H_

#include "platform/AEStdC.h"
#include "external/behavior/default/AEBTNode.h"

NS_AE_BEGIN
typedef std::vector<AEBTNode*> AEBTNodeVector;

class AEBTComposite : public AEBTNode
{
public:
	AEBTComposite();
	virtual ~AEBTComposite();

	/**
		节点前置条件检测。
	*/
	virtual bool check();

	/**
		退出执行。
	*/
	virtual void exit() override;

	/**
		添加前置条件节点。
	*/
	AEBTNode* addCondition(AEBTNode* condition);

	/**
		删除前置条件节点。
	*/
	AEBTNode* removeCondition(AEBTNode* condition);

	/**
		添加子节点。
	*/
	AEBTNode* addChild(AEBTNode* child);

	/**
		删除子节点。
	*/
	AEBTNode* removeChild(AEBTNode* child);

	/**
		删除全部子节点。
	*/
	void removeAllChildren();

	/**
		查找子节点。
	*/
	AEBTNode* findChild(uint32 childIndex);

public:
	// 前置条件
	AEBTNodeVector m_pCondition;
	// 子节点
	AEBTNodeVector m_pChildren;

};

NS_AE_END

#endif // _AEBTComposite_H_