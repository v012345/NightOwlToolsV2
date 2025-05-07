#include "base/ccConfig.h"
#ifndef __behavior_manual_h__
#define __behavior_manual_h__

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

TOLUA_API int register_all_behavior_manual(lua_State* L);

#endif // __behavior_manual_h__
