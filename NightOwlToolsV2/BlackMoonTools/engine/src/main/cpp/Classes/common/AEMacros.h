/************************************************************************** 
    *  @Copyright (c) 2015, A-one Soft, All rights reserved. 
 
    *  @file			: AEMacros.h 
    *  @version	: ver 1.0.0 
 
    *  @author	: andy-zhao 
    *  @date		: 2015/11/28 
    *  @brief		: 宏指令 
**************************************************************************/
#ifndef _AEMacros_H_
#define _AEMacros_H_

#include "platform/AEStdC.h"

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

#endif // _AEMacros_H_