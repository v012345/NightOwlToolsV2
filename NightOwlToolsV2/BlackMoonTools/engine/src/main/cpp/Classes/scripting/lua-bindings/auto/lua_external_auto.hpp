#include "base/ccConfig.h"
#ifndef __external_h__
#define __external_h__

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

int register_all_external(lua_State* tolua_S);




#endif // __external_h__
