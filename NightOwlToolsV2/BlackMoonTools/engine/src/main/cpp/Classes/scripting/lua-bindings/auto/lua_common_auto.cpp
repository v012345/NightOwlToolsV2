#include "scripting/lua-bindings/auto/lua_common_auto.hpp"
#include "include/json_inc.h"
#include "common/AEByteBuffer.h"
#include "common/AEUtil.h"
#include "common/AEVibrator.h"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"

int lua_common_AEByteBuffer_rewind(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_rewind'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_rewind'", nullptr);
            return 0;
        }
        cobj->rewind();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:rewind",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_rewind'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_readByte(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_readByte'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_readByte'", nullptr);
            return 0;
        }
        unsigned char& ret = cobj->readByte();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:readByte",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_readByte'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_readBool(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_readBool'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_readBool'", nullptr);
            return 0;
        }
        bool& ret = cobj->readBool();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:readBool",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_readBool'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_writeInt16(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_writeInt16'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int32_t arg0;

        ok &= luaval_to_int32(tolua_S, 2,&arg0, "AEByteBuffer:writeInt16");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_writeInt16'", nullptr);
            return 0;
        }
        bool ret = cobj->writeInt16(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:writeInt16",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_writeInt16'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_writeFloat64(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_writeFloat64'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "AEByteBuffer:writeFloat64");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_writeFloat64'", nullptr);
            return 0;
        }
        bool ret = cobj->writeFloat64(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:writeFloat64",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_writeFloat64'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_readInt8(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_readInt8'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_readInt8'", nullptr);
            return 0;
        }
        char& ret = cobj->readInt8();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:readInt8",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_readInt8'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_compact(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_compact'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_compact'", nullptr);
            return 0;
        }
        cobj->compact();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:compact",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_compact'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_readFloat32(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_readFloat32'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_readFloat32'", nullptr);
            return 0;
        }
        float& ret = cobj->readFloat32();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:readFloat32",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_readFloat32'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_writeUint32(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_writeUint32'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned int arg0;

        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "AEByteBuffer:writeUint32");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_writeUint32'", nullptr);
            return 0;
        }
        bool ret = cobj->writeUint32(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:writeUint32",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_writeUint32'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_writeBool(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_writeBool'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0, "AEByteBuffer:writeBool");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_writeBool'", nullptr);
            return 0;
        }
        bool ret = cobj->writeBool(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:writeBool",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_writeBool'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_readUint32(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_readUint32'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_readUint32'", nullptr);
            return 0;
        }
        unsigned int& ret = cobj->readUint32();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:readUint32",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_readUint32'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_readFloat64(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_readFloat64'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_readFloat64'", nullptr);
            return 0;
        }
        double& ret = cobj->readFloat64();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:readFloat64",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_readFloat64'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_readInt32(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_readInt32'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_readInt32'", nullptr);
            return 0;
        }
        int& ret = cobj->readInt32();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:readInt32",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_readInt32'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_writeInt32(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_writeInt32'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "AEByteBuffer:writeInt32");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_writeInt32'", nullptr);
            return 0;
        }
        bool ret = cobj->writeInt32(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:writeInt32",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_writeInt32'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_writeByte(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_writeByte'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        uint16_t arg0;

        ok &= luaval_to_uint16(tolua_S, 2,&arg0, "AEByteBuffer:writeByte");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_writeByte'", nullptr);
            return 0;
        }
        bool ret = cobj->writeByte(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:writeByte",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_writeByte'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_readString(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_readString'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_readString'", nullptr);
            return 0;
        }
        std::string ret = cobj->readString();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:readString",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_readString'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_writeInt8(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_writeInt8'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int32_t arg0;

        ok &= luaval_to_int32(tolua_S, 2,&arg0, "AEByteBuffer:writeInt8");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_writeInt8'", nullptr);
            return 0;
        }
        bool ret = cobj->writeInt8(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:writeInt8",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_writeInt8'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_writeFloat32(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_writeFloat32'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "AEByteBuffer:writeFloat32");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_writeFloat32'", nullptr);
            return 0;
        }
        bool ret = cobj->writeFloat32(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:writeFloat32",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_writeFloat32'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_readInt16(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_readInt16'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_readInt16'", nullptr);
            return 0;
        }
        short& ret = cobj->readInt16();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:readInt16",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_readInt16'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_writeUint16(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_writeUint16'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned short arg0;

        ok &= luaval_to_ushort(tolua_S, 2, &arg0, "AEByteBuffer:writeUint16");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_writeUint16'", nullptr);
            return 0;
        }
        bool ret = cobj->writeUint16(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:writeUint16",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_writeUint16'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_writeString(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_writeString'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AEByteBuffer:writeString");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_writeString'", nullptr);
            return 0;
        }
        bool ret = cobj->writeString(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:writeString",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_writeString'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_clear(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_clear'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_clear'", nullptr);
            return 0;
        }
        cobj->clear();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:clear",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_clear'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_flip(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_flip'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_flip'", nullptr);
            return 0;
        }
        cobj->flip();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:flip",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_flip'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_remaining(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_remaining'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_remaining'", nullptr);
            return 0;
        }
        unsigned int ret = cobj->remaining();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:remaining",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_remaining'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_readUint16(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_common_AEByteBuffer_readUint16'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEByteBuffer_readUint16'", nullptr);
            return 0;
        }
        unsigned short& ret = cobj->readUint16();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEByteBuffer:readUint16",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_readUint16'.",&tolua_err);
#endif

    return 0;
}
int lua_common_AEByteBuffer_constructor(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEByteBuffer* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 1) {
            unsigned int arg0;
            ok &= luaval_to_uint32(tolua_S, 2,&arg0, "AEByteBuffer:AEByteBuffer");

            if (!ok) { break; }
            cobj = new ae::AEByteBuffer(arg0);
            tolua_pushusertype(tolua_S,(void*)cobj,"AEByteBuffer");
            tolua_register_gc(tolua_S,lua_gettop(tolua_S));
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 0) {
            cobj = new ae::AEByteBuffer();
            tolua_pushusertype(tolua_S,(void*)cobj,"AEByteBuffer");
            tolua_register_gc(tolua_S,lua_gettop(tolua_S));
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 2) {
            const char* arg0;
            std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "AEByteBuffer:AEByteBuffer"); arg0 = arg0_tmp.c_str();

            if (!ok) { break; }
            unsigned int arg1;
            ok &= luaval_to_uint32(tolua_S, 3,&arg1, "AEByteBuffer:AEByteBuffer");

            if (!ok) { break; }
            cobj = new ae::AEByteBuffer(arg0, arg1);
            tolua_pushusertype(tolua_S,(void*)cobj,"AEByteBuffer");
            tolua_register_gc(tolua_S,lua_gettop(tolua_S));
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 3) {
            const char* arg0;
            std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "AEByteBuffer:AEByteBuffer"); arg0 = arg0_tmp.c_str();

            if (!ok) { break; }
            unsigned int arg1;
            ok &= luaval_to_uint32(tolua_S, 3,&arg1, "AEByteBuffer:AEByteBuffer");

            if (!ok) { break; }
            unsigned int arg2;
            ok &= luaval_to_uint32(tolua_S, 4,&arg2, "AEByteBuffer:AEByteBuffer");

            if (!ok) { break; }
            cobj = new ae::AEByteBuffer(arg0, arg1, arg2);
            tolua_pushusertype(tolua_S,(void*)cobj,"AEByteBuffer");
            tolua_register_gc(tolua_S,lua_gettop(tolua_S));
            return 1;
        }
    }while(0);
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "AEByteBuffer:AEByteBuffer",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEByteBuffer_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_common_AEByteBuffer_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (AEByteBuffer)");
    return 0;
}

int lua_register_common_AEByteBuffer(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"AEByteBuffer");
    tolua_cclass(tolua_S,"AEByteBuffer","AEByteBuffer","",nullptr);

    tolua_beginmodule(tolua_S,"AEByteBuffer");
        tolua_function(tolua_S,"new",lua_common_AEByteBuffer_constructor);
        tolua_function(tolua_S,"rewind",lua_common_AEByteBuffer_rewind);
        tolua_function(tolua_S,"readByte",lua_common_AEByteBuffer_readByte);
        tolua_function(tolua_S,"readBool",lua_common_AEByteBuffer_readBool);
        tolua_function(tolua_S,"writeInt16",lua_common_AEByteBuffer_writeInt16);
        tolua_function(tolua_S,"writeFloat64",lua_common_AEByteBuffer_writeFloat64);
        tolua_function(tolua_S,"readInt8",lua_common_AEByteBuffer_readInt8);
        tolua_function(tolua_S,"compact",lua_common_AEByteBuffer_compact);
        tolua_function(tolua_S,"readFloat32",lua_common_AEByteBuffer_readFloat32);
        tolua_function(tolua_S,"writeUint32",lua_common_AEByteBuffer_writeUint32);
        tolua_function(tolua_S,"writeBool",lua_common_AEByteBuffer_writeBool);
        tolua_function(tolua_S,"readUint32",lua_common_AEByteBuffer_readUint32);
        tolua_function(tolua_S,"readFloat64",lua_common_AEByteBuffer_readFloat64);
        tolua_function(tolua_S,"readInt32",lua_common_AEByteBuffer_readInt32);
        tolua_function(tolua_S,"writeInt32",lua_common_AEByteBuffer_writeInt32);
        tolua_function(tolua_S,"writeByte",lua_common_AEByteBuffer_writeByte);
        tolua_function(tolua_S,"readString",lua_common_AEByteBuffer_readString);
        tolua_function(tolua_S,"writeInt8",lua_common_AEByteBuffer_writeInt8);
        tolua_function(tolua_S,"writeFloat32",lua_common_AEByteBuffer_writeFloat32);
        tolua_function(tolua_S,"readInt16",lua_common_AEByteBuffer_readInt16);
        tolua_function(tolua_S,"writeUint16",lua_common_AEByteBuffer_writeUint16);
        tolua_function(tolua_S,"writeString",lua_common_AEByteBuffer_writeString);
        tolua_function(tolua_S,"clear",lua_common_AEByteBuffer_clear);
        tolua_function(tolua_S,"flip",lua_common_AEByteBuffer_flip);
        tolua_function(tolua_S,"remaining",lua_common_AEByteBuffer_remaining);
        tolua_function(tolua_S,"readUint16",lua_common_AEByteBuffer_readUint16);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ae::AEByteBuffer).name();
    g_luaType[typeName] = "AEByteBuffer";
    g_typeCast["AEByteBuffer"] = "AEByteBuffer";
    return 1;
}

int lua_common_AEUtil_random(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S)-1;

    do 
    {
        if (argc == 2)
        {
            double arg0;
            ok &= luaval_to_number(tolua_S, 2,&arg0, "AEUtil:random");
            if (!ok) { break; }
            double arg1;
            ok &= luaval_to_number(tolua_S, 3,&arg1, "AEUtil:random");
            if (!ok) { break; }
            double ret = ae::AEUtil::random(arg0, arg1);
            tolua_pushnumber(tolua_S,(lua_Number)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    do 
    {
        if (argc == 0)
        {
            int ret = ae::AEUtil::random();
            tolua_pushnumber(tolua_S,(lua_Number)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d", "AEUtil:random",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_random'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEUtil_getFrameFPS(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_getFrameFPS'", nullptr);
            return 0;
        }
        double ret = ae::AEUtil::getFrameFPS();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:getFrameFPS",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_getFrameFPS'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEUtil_getMS(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_getMS'", nullptr);
            return 0;
        }
        double ret = ae::AEUtil::getMS();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:getMS",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_getMS'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEUtil_getFileMD5(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AEUtil:getFileMD5");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_getFileMD5'", nullptr);
            return 0;
        }
        std::string ret = ae::AEUtil::getFileMD5(arg0);
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:getFileMD5",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_getFileMD5'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEUtil_itimeofday(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        long* arg0;
        long* arg1;
        #pragma warning NO CONVERSION TO NATIVE FOR long*
		ok = false;
        #pragma warning NO CONVERSION TO NATIVE FOR long*
		ok = false;
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_itimeofday'", nullptr);
            return 0;
        }
        ae::AEUtil::itimeofday(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:itimeofday",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_itimeofday'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEUtil_getMS64(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_getMS64'", nullptr);
            return 0;
        }
        long long ret = ae::AEUtil::getMS64();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:getMS64",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_getMS64'.",&tolua_err);
#endif
    return 0;
}

int lua_common_AEUtil_getMircoSecond(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_getMircoSecond'", nullptr);
            return 0;
        }
        long long ret = ae::AEUtil::getMircoSecond();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:getMircoSecond",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_getMircoSecond'.",&tolua_err);
#endif
    return 0;
}

int lua_common_AEUtil_setPrecision(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        unsigned int arg0;
        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "AEUtil:setPrecision");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_setPrecision'", nullptr);
            return 0;
        }
        ae::AEUtil::setPrecision(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:setPrecision",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_setPrecision'.",&tolua_err);
#endif
    return 0;
}

int lua_common_AEUtil_addUnicodeScope(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        int arg0;
        int arg1;
        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "AEUtil:addUnicodeScope");
        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "AEUtil:addUnicodeScope");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_addUnicodeScope'", nullptr);
            return 0;
        }
        ae::AEUtil::addUnicodeScope(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:addUnicodeScope",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_addUnicodeScope'.",&tolua_err);
#endif
    return 0;
}

int lua_common_AEUtil_verifyString(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AEUtil:verifyString");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_verifyString'", nullptr);
            return 0;
        }
        bool ret = ae::AEUtil::verifyString(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:verifyString",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_verifyString'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEUtil_exitGame(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_exitGame'", nullptr);
            return 0;
        }
        ae::AEUtil::exitGame();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:exitGame",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_exitGame'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEUtil_initAdaption(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_initAdaption'", nullptr);
            return 0;
        }
        ae::AEUtil::initAdaption();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:initAdaption",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_initAdaption'.",&tolua_err);
#endif
    return 0;
}

int lua_common_AEUtil_initAdaptionV1(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertable(tolua_S, 1, "AEUtil", 0, &tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;

	if (argc == 0)
	{
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_common_AEUtil_initAdaptionV1'", nullptr);
			return 0;
		}
		ae::AEUtil::initAdaptionV1();
		lua_settop(tolua_S, 1);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:initAdaptionV1", argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_common_AEUtil_initAdaptionV1'.", &tolua_err);
#endif
	return 0;
}

int lua_common_AEUtil_XOR(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        int arg0;
        int arg1;
        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "AEUtil:XOR");
        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "AEUtil:XOR");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_XOR'", nullptr);
            return 0;
        }
        int ret = ae::AEUtil::XOR(arg0, arg1);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:XOR",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_XOR'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEUtil_restartGame(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_restartGame'", nullptr);
            return 0;
        }
        ae::AEUtil::restartGame();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:restartGame",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_restartGame'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEUtil_MD5(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AEUtil:MD5");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_MD5'", nullptr);
            return 0;
        }
        std::string ret = ae::AEUtil::MD5(arg0);
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:MD5",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_MD5'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEUtil_AND(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        int arg0;
        int arg1;
        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "AEUtil:AND");
        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "AEUtil:AND");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_AND'", nullptr);
            return 0;
        }
        int ret = ae::AEUtil::AND(arg0, arg1);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:AND",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_AND'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEUtil_randomseed(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        unsigned int arg0;
        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "AEUtil:randomseed");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_randomseed'", nullptr);
            return 0;
        }
        ae::AEUtil::randomseed(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:randomseed",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_randomseed'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEUtil_setMultiTouchEnabled(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        bool arg0;
        ok &= luaval_to_boolean(tolua_S, 2,&arg0, "AEUtil:setMultiTouchEnabled");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_setMultiTouchEnabled'", nullptr);
            return 0;
        }
        ae::AEUtil::setMultiTouchEnabled(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:setMultiTouchEnabled",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_setMultiTouchEnabled'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEUtil_getMultiTouchEnabled(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_getMultiTouchEnabled'", nullptr);
            return 0;
        }
        bool ret = ae::AEUtil::getMultiTouchEnabled();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:getMultiTouchEnabled",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_getMultiTouchEnabled'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEUtil_SQRT(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        int arg0;
        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "AEUtil:SQRT");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_SQRT'", nullptr);
            return 0;
        }
        double ret = ae::AEUtil::SQRT(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:SQRT",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_SQRT'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEUtil_uiAdaption(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        cocos2d::Node* arg0;
        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0, "AEUtil:uiAdaption");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_uiAdaption'", nullptr);
            return 0;
        }
        ae::AEUtil::uiAdaption(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:uiAdaption",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_uiAdaption'.",&tolua_err);
#endif
    return 0;
}

int lua_common_AEUtil_uiAdaptionV1(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertable(tolua_S, 1, "AEUtil", 0, &tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;

	if (argc == 1)
	{
		cocos2d::Node* arg0;
		ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node", &arg0, "AEUtil:uiAdaptionV1");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_common_AEUtil_uiAdaptionV1'", nullptr);
			return 0;
		}
		ae::AEUtil::uiAdaptionV1(arg0);
		lua_settop(tolua_S, 1);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:uiAdaptionV1", argc, 1);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_common_AEUtil_uiAdaptionV1'.", &tolua_err);
#endif
	return 0;
}

int lua_common_AEUtil_asyncCount(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_asyncCount'", nullptr);
            return 0;
        }
        int ret = ae::AEUtil::asyncCount();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:asyncCount",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_asyncCount'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEUtil_getImageColor(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        cocos2d::Image* arg0;
        cocos2d::Vec2 arg1;
        ok &= luaval_to_object<cocos2d::Image>(tolua_S, 2, "cc.Image",&arg0, "AEUtil:getImageColor");
        ok &= luaval_to_vec2(tolua_S, 3, &arg1, "AEUtil:getImageColor");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_getImageColor'", nullptr);
            return 0;
        }
        cocos2d::Color4B ret = ae::AEUtil::getImageColor(arg0, arg1);
        color4b_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:getImageColor",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_getImageColor'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEUtil_getAppKey(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_getAppKey'", nullptr);
            return 0;
        }
        std::string ret = ae::AEUtil::getAppKey();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:getAppKey",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_getAppKey'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEUtil_OR(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        int arg0;
        int arg1;
        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "AEUtil:OR");
        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "AEUtil:OR");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_OR'", nullptr);
            return 0;
        }
        int ret = ae::AEUtil::OR(arg0, arg1);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:OR",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_OR'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEUtil_stringLenght(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AEUtil:stringLenght");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_stringLenght'", nullptr);
            return 0;
        }
        int ret = ae::AEUtil::stringLenght(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:stringLenght",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_stringLenght'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEUtil_computeWordNumber(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertable(tolua_S, 1, "AEUtil", 0, &tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;

	if (argc == 1)
	{
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, "AEUtil:computeWordNumber");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_common_AEUtil_computeWordNumber'", nullptr);
			return 0;
		}
		int ret = ae::AEUtil::computeWordNumber(arg0);
		tolua_pushnumber(tolua_S, (lua_Number)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEUtil:computeWordNumber", argc, 1);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_common_AEUtil_computeWordNumber'.", &tolua_err);
#endif
	return 0;
}
int lua_common_AEUtil_constructor(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEUtil_constructor'", nullptr);
            return 0;
        }
        cobj = new ae::AEUtil();
        tolua_pushusertype(tolua_S,(void*)cobj,"AEUtil");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEUtil:AEUtil",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEUtil_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_common_AEUtil_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (AEUtil)");
    return 0;
}

int lua_register_common_AEUtil(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"AEUtil");
    tolua_cclass(tolua_S,"AEUtil","AEUtil","",nullptr);

    tolua_beginmodule(tolua_S,"AEUtil");
        tolua_function(tolua_S,"new",lua_common_AEUtil_constructor);
        tolua_function(tolua_S,"random", lua_common_AEUtil_random);
        tolua_function(tolua_S,"getFrameFPS", lua_common_AEUtil_getFrameFPS);
        tolua_function(tolua_S,"getMS", lua_common_AEUtil_getMS);
        tolua_function(tolua_S,"getFileMD5", lua_common_AEUtil_getFileMD5);
        tolua_function(tolua_S,"itimeofday", lua_common_AEUtil_itimeofday);
        tolua_function(tolua_S,"getMS64", lua_common_AEUtil_getMS64);
        tolua_function(tolua_S,"getMircoSecond", lua_common_AEUtil_getMircoSecond);
        tolua_function(tolua_S,"setPrecision", lua_common_AEUtil_setPrecision);
        tolua_function(tolua_S,"addUnicodeScope", lua_common_AEUtil_addUnicodeScope);
        tolua_function(tolua_S,"verifyString", lua_common_AEUtil_verifyString);
        tolua_function(tolua_S,"exitGame", lua_common_AEUtil_exitGame);
        tolua_function(tolua_S,"initAdaption", lua_common_AEUtil_initAdaption);
        tolua_function(tolua_S,"initAdaptionV1", lua_common_AEUtil_initAdaptionV1);
        tolua_function(tolua_S,"XOR", lua_common_AEUtil_XOR);
        tolua_function(tolua_S,"restartGame", lua_common_AEUtil_restartGame);
        tolua_function(tolua_S,"MD5", lua_common_AEUtil_MD5);
        tolua_function(tolua_S,"AND", lua_common_AEUtil_AND);
        tolua_function(tolua_S,"randomseed", lua_common_AEUtil_randomseed);
        tolua_function(tolua_S,"setMultiTouchEnabled", lua_common_AEUtil_setMultiTouchEnabled);
        tolua_function(tolua_S,"getMultiTouchEnabled", lua_common_AEUtil_getMultiTouchEnabled);
        tolua_function(tolua_S,"SQRT", lua_common_AEUtil_SQRT);
        tolua_function(tolua_S,"uiAdaption", lua_common_AEUtil_uiAdaption);
        tolua_function(tolua_S,"uiAdaptionV1", lua_common_AEUtil_uiAdaptionV1);
        tolua_function(tolua_S,"asyncCount", lua_common_AEUtil_asyncCount);
        tolua_function(tolua_S,"getImageColor", lua_common_AEUtil_getImageColor);
        tolua_function(tolua_S,"getAppKey", lua_common_AEUtil_getAppKey);
        tolua_function(tolua_S,"OR", lua_common_AEUtil_OR);
        tolua_function(tolua_S,"stringLenght", lua_common_AEUtil_stringLenght);
        tolua_function(tolua_S,"computeWordNumber", lua_common_AEUtil_computeWordNumber);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ae::AEUtil).name();
    g_luaType[typeName] = "AEUtil";
    g_typeCast["AEUtil"] = "AEUtil";
    return 1;
}

int lua_common_AEVibrator_vibrateOnce(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEVibrator",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEVibrator_vibrateOnce'", nullptr);
            return 0;
        }
        ae::AEVibrator::vibrateOnce();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEVibrator:vibrateOnce",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEVibrator_vibrateOnce'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEVibrator_cancelVibrate(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEVibrator",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEVibrator_cancelVibrate'", nullptr);
            return 0;
        }
        ae::AEVibrator::cancelVibrate();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEVibrator:cancelVibrate",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEVibrator_cancelVibrate'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEVibrator_vibrateWithCadence(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEVibrator",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 4)
    {
        int arg0;
        int arg1;
        bool arg2;
        bool arg3;
        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "AEVibrator:vibrateWithCadence");
        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "AEVibrator:vibrateWithCadence");
        ok &= luaval_to_boolean(tolua_S, 4,&arg2, "AEVibrator:vibrateWithCadence");
        ok &= luaval_to_boolean(tolua_S, 5,&arg3, "AEVibrator:vibrateWithCadence");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEVibrator_vibrateWithCadence'", nullptr);
            return 0;
        }
        ae::AEVibrator::vibrateWithCadence(arg0, arg1, arg2, arg3);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEVibrator:vibrateWithCadence",argc, 4);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEVibrator_vibrateWithCadence'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEVibrator_vibrate(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEVibrator",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        int arg0;
        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "AEVibrator:vibrate");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEVibrator_vibrate'", nullptr);
            return 0;
        }
        ae::AEVibrator::vibrate(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEVibrator:vibrate",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEVibrator_vibrate'.",&tolua_err);
#endif
    return 0;
}
int lua_common_AEVibrator_constructor(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEVibrator* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_common_AEVibrator_constructor'", nullptr);
            return 0;
        }
        cobj = new ae::AEVibrator();
        tolua_pushusertype(tolua_S,(void*)cobj,"AEVibrator");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEVibrator:AEVibrator",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_common_AEVibrator_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_common_AEVibrator_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (AEVibrator)");
    return 0;
}

int lua_register_common_AEVibrator(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"AEVibrator");
    tolua_cclass(tolua_S,"AEVibrator","AEVibrator","",nullptr);

    tolua_beginmodule(tolua_S,"AEVibrator");
        tolua_function(tolua_S,"new",lua_common_AEVibrator_constructor);
        tolua_function(tolua_S,"vibrateOnce", lua_common_AEVibrator_vibrateOnce);
        tolua_function(tolua_S,"cancelVibrate", lua_common_AEVibrator_cancelVibrate);
        tolua_function(tolua_S,"vibrateWithCadence", lua_common_AEVibrator_vibrateWithCadence);
        tolua_function(tolua_S,"vibrate", lua_common_AEVibrator_vibrate);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ae::AEVibrator).name();
    g_luaType[typeName] = "AEVibrator";
    g_typeCast["AEVibrator"] = "AEVibrator";
    return 1;
}
TOLUA_API int register_all_common(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,nullptr,0);
	tolua_beginmodule(tolua_S,nullptr);

	lua_register_common_AEByteBuffer(tolua_S);
	lua_register_common_AEUtil(tolua_S);
	lua_register_common_AEVibrator(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

