#include "base/ccConfig.h"
#ifndef __collision_manual_h__
#define __collision_manual_h__

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

TOLUA_API int register_all_collision_manual(lua_State* L);

#endif // __collision_manual_h__
