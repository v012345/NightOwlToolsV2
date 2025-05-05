/************************************************************************** 
    *  @Copyright (c) 2015, A-one Soft, All rights reserved. 
 
    *  @file			: AEPlatformDefine.h 
    *  @version	: ver 1.0.0 
 
    *  @author	: andy-zhao 
    *  @date		: 2015/11/28 
    *  @brief		: 包含特定头文件 
**************************************************************************/
#ifndef _AEPlatformDefine_H_
#define _AEPlatformDefine_H_

#ifdef __cplusplus
#define NS_AE_BEGIN namespace ae {
#define NS_AE_END }
#define USING_NS_AE using namespace ae
#define NS_AE ::ae
#else
#define NS_AE_BEGIN 
#define NS_AE_END 
#define USING_NS_AE 
#define NS_AE
#endif 

NS_AE_BEGIN

typedef char int8;
typedef unsigned char byte;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;
typedef float float32;
typedef double float64;

typedef uint64 OBJECT_ID;

NS_AE_END

#endif // _AEPlatformDefine_H_