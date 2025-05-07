/**************************************************************************
*  @Copyright (c) 2015, A-one Soft, All rights reserved.

*  @file			: AEPlatformMacros.h
*  @version	: ver 1.0.0

*  @author	: andy-zhao
*  @date		: 2015/11/28
*  @brief		: 客户端与服务器通用宏指令
**************************************************************************/
#ifndef _AEPlatformMacros_H_
#define _AEPlatformMacros_H_

#include "platform/AEPlatformConfig.h"
#include "platform/AEPlatformDefine.h"
#include <memory>

/**
指针删除并置空。
*/
#define  AESafe_delete(ptr) \
{ \
	if (nullptr != ptr) \
	{ \
		delete ptr; \
		ptr = nullptr; \
	} \
}

/**
条件跳出。
*/
#define AEBreak_if(cond) if ((cond)) break;

/**
断言并打印信息。
*/
#include <assert.h>
#define AEASSERT(cond, msg) \
if (!(cond)) \
{ \
	if (!strlen(msg)) \
	{ \
		cocos2d::log("Assert failed: %s", msg); \
	} \
	assert(cond); \
}

/**
智能指针。
*/
#define AEShared_ptr std::shared_ptr
#define AEWeak_ptr std::weak_ptr

/**
自定义函数。
*/
#define AE_SYNTHESIZE_READONLY(varType, varName, funName) \
protected: varType varName; \
public: virtual varType get##funName(void) const { return varName; }

#define AE_SYNTHESIZE_READONLY_REF(varType, varName, funName) \
protected: varType varName; \
public: virtual const varType& get##funName(void) const { return varName; }

#define AE_SYNTHESIZE(varType, varName, funName) \
protected: varType varName; \
public: virtual varType get##funName(void) const { return varName; } \
public: virtual void set##funName(varType var) { varName = var; }

#define AE_SYNTHESIZE_REF(varType, varName, funName) \
protected: varType varName; \
public: virtual const varType& get##funName(void) const { return varName; } \
public: virtual void set##funName(const varType& var) { varName = var; }

/**
打印输出。
*/
#define AELOGINFO(format, ...) CCLOG(format, ##__VA_ARGS__)
#define AELOGWARN(format, ...) CCLOGWARN(format, ##__VA_ARGS__)
#define AELOGERROR(format, ...) CCLOGERROR(format, ##__VA_ARGS__)

#endif // _AEPlatformMacros_H_