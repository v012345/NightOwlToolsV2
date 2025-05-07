/**************************************************************************
*  @Copyright (c) 2017, A-one Soft, All rights reserved.

*  @file			: AEBTNode.h
*  @version	: ver 1.0.0

*  @author	: andy-zhao
*  @date		: 2017/4/26
*  @brief		: 自定义行为树。
**************************************************************************/
#ifndef _AEBTNode_H_
#define _AEBTNode_H_

#include "platform/AEStdC.h"

NS_AE_BEGIN

typedef enum AEBTNodeStatus
{
	READIED = 0,
	RUNNING = 1,
	SUCCESS = 2,
	FAILURE = 3,
};

typedef std::function<bool(std::string funcName, float32 delta)> AEBTNodeListener;

class AEBTNode : public Ref
{
public:
	AEBTNode();
	virtual ~AEBTNode();

	/**
		初始化。
	*/
	virtual bool init();

	/**
		刷新。
	*/
	virtual void update(float32 delta);

	/**
		开始执行。
	*/
	virtual void enter();

	/**
		退出执行。
	*/
	virtual void exit();

	/**
		设置行为监听。
	*/
	void setNodeListener(AEBTNodeListener listener);

public:
	// 父节点
	AE_SYNTHESIZE(AEBTNode*, m_pParent, Parent);
	// 节点状态
	AE_SYNTHESIZE_REF(AEBTNodeStatus, m_eState, State);
	// 节点监听
	AEBTNodeListener m_pNodeListener;

};

NS_AE_END

#endif // _AEBTNode_H_