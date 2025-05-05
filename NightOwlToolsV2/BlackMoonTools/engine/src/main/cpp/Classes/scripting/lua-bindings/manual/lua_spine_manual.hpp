#include "base/ccConfig.h"
#ifndef __spine_manual_h__
#define __spine_manual_h__

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

TOLUA_API int register_all_spine_manual(lua_State* L);

#endif // __spine_manual_h__
