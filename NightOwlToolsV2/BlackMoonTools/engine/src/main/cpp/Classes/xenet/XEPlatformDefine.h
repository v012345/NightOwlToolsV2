
#ifndef _XEPlatformDefine_H_
#define _XEPlatformDefine_H_

//#include "base/CCConsole.h"

#ifdef __cplusplus
#define NS_XE_BEGIN namespace xe {
#define NS_XE_END }
#define USING_NS_XE using namespace xe
#define NS_XE ::xe
#else
#define NS_XE_BEGIN 
#define NS_XE_END 
#define USING_NS_XE 
#define NS_XE
#endif 

NS_XE_BEGIN

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

#if XE_TARGET_PLATFORM == XE_PLATFORM_WIN32
typedef uint32					ssize_t;
typedef volatile long			atomic_t;
typedef int						socklen_t;
typedef char*					ae_socket_buf_type;
typedef const char*				ae_socket_buf_type_const;
#else
typedef void*					ae_socket_buf_type;
typedef const void*				ae_socket_buf_type_const;
typedef long					atomic_t;
typedef unsigned char			BYTE;
typedef unsigned short			WORD;
typedef unsigned long			DWORD;
typedef int						SOCKET;
typedef int						BOOL;
typedef int						HANDLE;
#define TRUE					(1)
#define FALSE					(0)
#define INVALID_SOCKET			(SOCKET)(~0)
#define INVALID_HANDLE_VALUE	(HANDLE)(-1)
#define SOCKET_ERROR			(-1)
#define __stdcall
#define I64D "%ld"
#define I64U "%lu"
#endif // !windows

NS_XE_END

#endif // _XEPlatformDefine_H_