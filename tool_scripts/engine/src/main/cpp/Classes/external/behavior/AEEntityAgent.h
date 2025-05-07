/**************************************************************************
*  @Copyright (c) 2017, A-one Soft, All rights reserved.

*  @file			: AEEntityAgent.h
*  @version	: ver 1.0.0

*  @author	: andy-zhao
*  @date		: 2017/4/26
*  @brief		: 腾讯行为树封装。
**************************************************************************/
#ifndef _AEEntityAgent_H_
#define _AEEntityAgent_H_

#include "platform/AEStdC.h"

class EntityAgent;

NS_AE_BEGIN

typedef std::function<int(std::string funcName)> AEEntityAgentListener;
class AEEntityAgent
{
public:
	AEEntityAgent();
	virtual ~AEEntityAgent();

	/**
		创建。
	*/
	static AEEntityAgent* create();

	/**
		释放。
	*/
	void release();

	/**
		更新。
	*/
	void update(float delta);

	/**
		监听。
	*/
	void setEntityAgentListener(AEEntityAgentListener listener);

	/**
		监听触发。
	*/
	int onEntityAgentListener(std::string funcName);

private:
	// 代理
	EntityAgent* m_pEntityAgent;
	// 监听事件
	AEEntityAgentListener m_pEntityAgentListener;

};

NS_AE_END

#endif // _AEEntityAgent_H_