#ifndef ___cocos_engine_h___
#define ___cocos_engine_h___

#include <cocos2d.h>
#include <SimpleAudioEngine.h>
USING_NS_CC;
using namespace CocosDenshion;

#include "scripting/lua-bindings/manual/CCLuaEngine.h"
extern "C"{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
};

#endif
