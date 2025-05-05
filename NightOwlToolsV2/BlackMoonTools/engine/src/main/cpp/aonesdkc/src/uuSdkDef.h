#ifndef __UUSDKDEF_HH__
#define __UUSDKDEF_HH__

#include "xcore_define.h"

typedef signed char			i8;
typedef unsigned char		u8;
typedef signed short		i16;
typedef unsigned short		u16;
typedef signed long			i32;
typedef unsigned long		u32;

typedef signed char         int8;
//typedef unsigned char       uint8;
typedef signed short        int16;
//typedef unsigned short      uint16;
typedef signed int          int32;
//typedef unsigned long int   uint32;
//typedef signed long long    int64;
//typedef unsigned long long  uint64;
typedef unsigned char		byte;
typedef unsigned char       boolean;


#include <stdio.h>
#include <cplog.h>

// TODO
//#ifdef DEBUG
#define uuLOGN(fmt, ...) NSLog((@"@%s @%d " fmt),__func__, __LINE__,##__VA_ARGS__)
//#define uuLOGN(fmt, ...)
//#else
//#define uuLOGN(fmt, ...)
//#endif

// TODO
#ifndef uuLOG

//#ifdef DEBUG
#define uuLOG CP_LOGI
//#else
//#define uuLOG
//#endif

#endif

#ifndef uuAssert
//#define uuAssert CCAssert
#define uuAssert
#endif

#ifndef uuMessageBox
#define uuMessageBox
#endif

#ifndef uuObject
class uuObject {
public:
	virtual ~uuObject() {;}
};

#endif

/*
#ifndef uuFileUtils
#include "platform/CCFileUtils.h"
#define uuFileUtils aonesdk::FileUtils
#endif
*/

#ifndef uuRETURN_RET_IF_COND
#define uuRETURN_RET_IF_COND(cond, ret) \
  if (cond) { \
  return	ret; \
  }

#endif

#endif // __UUSDKDEF_HH__
