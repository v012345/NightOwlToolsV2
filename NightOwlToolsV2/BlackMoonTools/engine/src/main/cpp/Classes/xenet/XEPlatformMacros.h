
#ifndef _XEPlatformMacros_H_
#define _XEPlatformMacros_H_

#include "xenet/XEPlatformConfig.h"
#include "xenet/XEPlatformDefine.h"
#include "xenet/XELog.h"
#include <memory>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
NS_XE_BEGIN

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
自定义函数。
*/
#define XE_SYNTHESIZE_READONLY(varType, varName, funName) \
protected: varType varName; \
public: virtual varType get##funName(void) const { return varName; }

#define XE_SYNTHESIZE_READONLY_REF(varType, varName, funName) \
protected: varType varName; \
public: virtual const varType& get##funName(void) const { return varName; }

#define XE_SYNTHESIZE(varType, varName, funName) \
protected: varType varName; \
public: virtual varType get##funName(void) const { return varName; } \
public: virtual void set##funName(varType var) { varName = var; }

#define XE_SYNTHESIZE_REF(varType, varName, funName) \
protected: varType varName; \
public: virtual const varType& get##funName(void) const { return varName; } \
public: virtual void set##funName(const varType& var) { varName = var; }

#define XELOGWRITE(format, ...) XE_LOG(format, ##__VA_ARGS__)

NS_XE_END
#endif // _XEPlatformMacros_H_