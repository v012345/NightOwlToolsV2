#include "scripting/lua-bindings/register_custom_function.h"

#include "scripting/lua-bindings/auto/lua_behavior_auto.hpp"
#include "scripting/lua-bindings/auto/lua_collision_auto.hpp"
#include "scripting/lua-bindings/auto/lua_common_auto.hpp"
#include "scripting/lua-bindings/auto/lua_external_auto.hpp"
#include "scripting/lua-bindings/auto/lua_network_auto.hpp"
#include "scripting/lua-bindings/auto/lua_spine_auto.hpp"

#include "scripting/lua-bindings/manual/lua_behavior_manual.hpp"
#include "scripting/lua-bindings/manual/lua_collision_manual.hpp"
#include "scripting/lua-bindings/manual/lua_spine_manual.hpp"
#include "aone/lpatcher.h"

int register_custom_function(lua_State* L)
{
    // Don't change the module register order unless you know what your are doing
	register_all_behavior(L);
	register_all_collision(L);
	register_all_common(L);
	register_all_external(L);
	register_all_network(L);
	register_all_spine(L);
	register_all_behavior_manual(L);
	register_all_collision_manual(L);
	register_all_spine_manual(L);
	luaopen_patcher(L);

    return 1;
}

