#include "scripting/lua-bindings/auto/lua_network_auto.hpp"
#include "net/AEMessage.h"
#include "net/AESocketClient.h"
#include "xenet/XETCPSocket.h"
#include "xenet/XEUDPSocket.h"
#include "xenet/XESelector.h"
#include "xenet/XEMessage.h"
#include "xenet/XEByteBuffer.h"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"

int lua_network_AEMessage_setExtension(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_AEMessage_setExtension'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned int arg0;

        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "AEMessage:setExtension");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AEMessage_setExtension'", nullptr);
            return 0;
        }
        cobj->setExtension(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEMessage:setExtension",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AEMessage_setExtension'.",&tolua_err);
#endif

    return 0;
}
int lua_network_AEMessage_setSequenceID(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_AEMessage_setSequenceID'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned int arg0;

        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "AEMessage:setSequenceID");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AEMessage_setSequenceID'", nullptr);
            return 0;
        }
        cobj->setSequenceID(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEMessage:setSequenceID",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AEMessage_setSequenceID'.",&tolua_err);
#endif

    return 0;
}
int lua_network_AEMessage_getResult(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_AEMessage_getResult'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AEMessage_getResult'", nullptr);
            return 0;
        }
        const int& ret = cobj->getResult();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEMessage:getResult",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AEMessage_getResult'.",&tolua_err);
#endif

    return 0;
}
int lua_network_AEMessage_composite(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_AEMessage_composite'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AEMessage_composite'", nullptr);
            return 0;
        }
        cobj->composite();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEMessage:composite",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AEMessage_composite'.",&tolua_err);
#endif

    return 0;
}
int lua_network_AEMessage_getProtocolID(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_AEMessage_getProtocolID'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AEMessage_getProtocolID'", nullptr);
            return 0;
        }
        const unsigned int& ret = cobj->getProtocolID();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEMessage:getProtocolID",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AEMessage_getProtocolID'.",&tolua_err);
#endif

    return 0;
}
int lua_network_AEMessage_getExtension(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_AEMessage_getExtension'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AEMessage_getExtension'", nullptr);
            return 0;
        }
        const unsigned int& ret = cobj->getExtension();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEMessage:getExtension",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AEMessage_getExtension'.",&tolua_err);
#endif

    return 0;
}
int lua_network_AEMessage_getByteBuffer(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_AEMessage_getByteBuffer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AEMessage_getByteBuffer'", nullptr);
            return 0;
        }
        ae::AEByteBuffer* ret = cobj->getByteBuffer();
        object_to_luaval<ae::AEByteBuffer>(tolua_S, "AEByteBuffer",(ae::AEByteBuffer*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEMessage:getByteBuffer",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AEMessage_getByteBuffer'.",&tolua_err);
#endif

    return 0;
}
int lua_network_AEMessage_setProtocolID(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_AEMessage_setProtocolID'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned int arg0;

        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "AEMessage:setProtocolID");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AEMessage_setProtocolID'", nullptr);
            return 0;
        }
        cobj->setProtocolID(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEMessage:setProtocolID",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AEMessage_setProtocolID'.",&tolua_err);
#endif

    return 0;
}
int lua_network_AEMessage_parse(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_AEMessage_parse'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AEMessage_parse'", nullptr);
            return 0;
        }
        cobj->parse();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEMessage:parse",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AEMessage_parse'.",&tolua_err);
#endif

    return 0;
}
int lua_network_AEMessage_getSequenceID(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_AEMessage_getSequenceID'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AEMessage_getSequenceID'", nullptr);
            return 0;
        }
        const unsigned int& ret = cobj->getSequenceID();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEMessage:getSequenceID",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AEMessage_getSequenceID'.",&tolua_err);
#endif

    return 0;
}
int lua_network_AEMessage_setBodyLenght(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_AEMessage_setBodyLenght'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned int arg0;

        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "AEMessage:setBodyLenght");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AEMessage_setBodyLenght'", nullptr);
            return 0;
        }
        cobj->setBodyLenght(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEMessage:setBodyLenght",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AEMessage_setBodyLenght'.",&tolua_err);
#endif

    return 0;
}
int lua_network_AEMessage_getBodyLenght(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_AEMessage_getBodyLenght'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AEMessage_getBodyLenght'", nullptr);
            return 0;
        }
        const unsigned int& ret = cobj->getBodyLenght();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEMessage:getBodyLenght",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AEMessage_getBodyLenght'.",&tolua_err);
#endif

    return 0;
}
int lua_network_AEMessage_setResult(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_AEMessage_setResult'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "AEMessage:setResult");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AEMessage_setResult'", nullptr);
            return 0;
        }
        cobj->setResult(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AEMessage:setResult",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AEMessage_setResult'.",&tolua_err);
#endif

    return 0;
}
int lua_network_AEMessage_setUserID(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        unsigned int arg0;
        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "AEMessage:setUserID");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AEMessage_setUserID'", nullptr);
            return 0;
        }
        ae::AEMessage::setUserID(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEMessage:setUserID",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AEMessage_setUserID'.",&tolua_err);
#endif
    return 0;
}
int lua_network_AEMessage_receive(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        unsigned int arg0;
        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "AEMessage:receive");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AEMessage_receive'", nullptr);
            return 0;
        }
        ae::AEMessage* ret = ae::AEMessage::receive(arg0);
        object_to_luaval<ae::AEMessage>(tolua_S, "AEMessage",(ae::AEMessage*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEMessage:receive",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AEMessage_receive'.",&tolua_err);
#endif
    return 0;
}
int lua_network_AEMessage_errorMessage(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        int arg0;
        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "AEMessage:errorMessage");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AEMessage_errorMessage'", nullptr);
            return 0;
        }
        ae::AEMessage* ret = ae::AEMessage::errorMessage(arg0);
        object_to_luaval<ae::AEMessage>(tolua_S, "AEMessage",(ae::AEMessage*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEMessage:errorMessage",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AEMessage_errorMessage'.",&tolua_err);
#endif
    return 0;
}
int lua_network_AEMessage_send(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        unsigned int arg0;
        unsigned int arg1;
        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "AEMessage:send");
        ok &= luaval_to_uint32(tolua_S, 3,&arg1, "AEMessage:send");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AEMessage_send'", nullptr);
            return 0;
        }
        ae::AEMessage* ret = ae::AEMessage::send(arg0, arg1);
        object_to_luaval<ae::AEMessage>(tolua_S, "AEMessage",(ae::AEMessage*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEMessage:send",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AEMessage_send'.",&tolua_err);
#endif
    return 0;
}
int lua_network_AEMessage_getUserID(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AEMessage_getUserID'", nullptr);
            return 0;
        }
        unsigned int ret = ae::AEMessage::getUserID();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEMessage:getUserID",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AEMessage_getUserID'.",&tolua_err);
#endif
    return 0;
}
int lua_network_AEMessage_setSessionID(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        unsigned int arg0;
        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "AEMessage:setSessionID");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AEMessage_setSessionID'", nullptr);
            return 0;
        }
        ae::AEMessage::setSessionID(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEMessage:setSessionID",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AEMessage_setSessionID'.",&tolua_err);
#endif
    return 0;
}
int lua_network_AEMessage_getSessionID(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AEMessage_getSessionID'", nullptr);
            return 0;
        }
        unsigned int ret = ae::AEMessage::getSessionID();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AEMessage:getSessionID",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AEMessage_getSessionID'.",&tolua_err);
#endif
    return 0;
}
int lua_network_AEMessage_constructor(lua_State* tolua_S)
{
    int argc = 0;
    ae::AEMessage* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 1) {
            unsigned int arg0;
            ok &= luaval_to_uint32(tolua_S, 2,&arg0, "AEMessage:AEMessage");

            if (!ok) { break; }
            cobj = new ae::AEMessage(arg0);
            tolua_pushusertype(tolua_S,(void*)cobj,"AEMessage");
            tolua_register_gc(tolua_S,lua_gettop(tolua_S));
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 0) {
            cobj = new ae::AEMessage();
            tolua_pushusertype(tolua_S,(void*)cobj,"AEMessage");
            tolua_register_gc(tolua_S,lua_gettop(tolua_S));
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 2) {
            unsigned int arg0;
            ok &= luaval_to_uint32(tolua_S, 2,&arg0, "AEMessage:AEMessage");

            if (!ok) { break; }
            unsigned int arg1;
            ok &= luaval_to_uint32(tolua_S, 3,&arg1, "AEMessage:AEMessage");

            if (!ok) { break; }
            cobj = new ae::AEMessage(arg0, arg1);
            tolua_pushusertype(tolua_S,(void*)cobj,"AEMessage");
            tolua_register_gc(tolua_S,lua_gettop(tolua_S));
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 1) {
            ae::AEByteBuffer* arg0;
            ok &= luaval_to_object<ae::AEByteBuffer>(tolua_S, 2, "AEByteBuffer",&arg0, "AEMessage:AEMessage");

            if (!ok) { break; }
            cobj = new ae::AEMessage(arg0);
            tolua_pushusertype(tolua_S,(void*)cobj,"AEMessage");
            tolua_register_gc(tolua_S,lua_gettop(tolua_S));
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 3) {
            const char* arg0;
            std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "AEMessage:AEMessage"); arg0 = arg0_tmp.c_str();

            if (!ok) { break; }
            unsigned int arg1;
            ok &= luaval_to_uint32(tolua_S, 3,&arg1, "AEMessage:AEMessage");

            if (!ok) { break; }
            unsigned int arg2;
            ok &= luaval_to_uint32(tolua_S, 4,&arg2, "AEMessage:AEMessage");

            if (!ok) { break; }
            cobj = new ae::AEMessage(arg0, arg1, arg2);
            tolua_pushusertype(tolua_S,(void*)cobj,"AEMessage");
            tolua_register_gc(tolua_S,lua_gettop(tolua_S));
            return 1;
        }
    }while(0);
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "AEMessage:AEMessage",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_network_AEMessage_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_network_AEMessage_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (AEMessage)");
    return 0;
}

int lua_register_network_AEMessage(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"AEMessage");
    tolua_cclass(tolua_S,"AEMessage","AEMessage","",nullptr);

    tolua_beginmodule(tolua_S,"AEMessage");
        tolua_function(tolua_S,"new",lua_network_AEMessage_constructor);
        tolua_function(tolua_S,"setExtension",lua_network_AEMessage_setExtension);
        tolua_function(tolua_S,"setSequenceID",lua_network_AEMessage_setSequenceID);
        tolua_function(tolua_S,"getResult",lua_network_AEMessage_getResult);
        tolua_function(tolua_S,"composite",lua_network_AEMessage_composite);
        tolua_function(tolua_S,"getProtocolID",lua_network_AEMessage_getProtocolID);
        tolua_function(tolua_S,"getExtension",lua_network_AEMessage_getExtension);
        tolua_function(tolua_S,"getByteBuffer",lua_network_AEMessage_getByteBuffer);
        tolua_function(tolua_S,"setProtocolID",lua_network_AEMessage_setProtocolID);
        tolua_function(tolua_S,"parse",lua_network_AEMessage_parse);
        tolua_function(tolua_S,"getSequenceID",lua_network_AEMessage_getSequenceID);
        tolua_function(tolua_S,"setBodyLenght",lua_network_AEMessage_setBodyLenght);
        tolua_function(tolua_S,"getBodyLenght",lua_network_AEMessage_getBodyLenght);
        tolua_function(tolua_S,"setResult",lua_network_AEMessage_setResult);
        tolua_function(tolua_S,"setUserID", lua_network_AEMessage_setUserID);
        tolua_function(tolua_S,"receive", lua_network_AEMessage_receive);
        tolua_function(tolua_S,"errorMessage", lua_network_AEMessage_errorMessage);
        tolua_function(tolua_S,"send", lua_network_AEMessage_send);
        tolua_function(tolua_S,"getUserID", lua_network_AEMessage_getUserID);
        tolua_function(tolua_S,"setSessionID", lua_network_AEMessage_setSessionID);
        tolua_function(tolua_S,"getSessionID", lua_network_AEMessage_getSessionID);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ae::AEMessage).name();
    g_luaType[typeName] = "AEMessage";
    g_typeCast["AEMessage"] = "AEMessage";
    return 1;
}

int lua_network_AESocketClient_disconnect(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESocketClient* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AESocketClient",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AESocketClient*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_AESocketClient_disconnect'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AESocketClient_disconnect'", nullptr);
            return 0;
        }
        cobj->disconnect();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESocketClient:disconnect",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AESocketClient_disconnect'.",&tolua_err);
#endif

    return 0;
}
int lua_network_AESocketClient_sendMessage(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESocketClient* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AESocketClient",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AESocketClient*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_AESocketClient_sendMessage'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        ae::AEMessage* arg0;

        ok &= luaval_to_object<ae::AEMessage>(tolua_S, 2, "AEMessage",&arg0, "AESocketClient:sendMessage");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AESocketClient_sendMessage'", nullptr);
            return 0;
        }
        cobj->sendMessage(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESocketClient:sendMessage",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AESocketClient_sendMessage'.",&tolua_err);
#endif

    return 0;
}
int lua_network_AESocketClient_connect(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESocketClient* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AESocketClient",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AESocketClient*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_AESocketClient_connect'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        unsigned short arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AESocketClient:connect");

        ok &= luaval_to_ushort(tolua_S, 3, &arg1, "AESocketClient:connect");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AESocketClient_connect'", nullptr);
            return 0;
        }
        bool ret = cobj->connect(arg0, arg1);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESocketClient:connect",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AESocketClient_connect'.",&tolua_err);
#endif

    return 0;
}
int lua_network_AESocketClient_getSocketState(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESocketClient* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AESocketClient",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AESocketClient*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_AESocketClient_getSocketState'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AESocketClient_getSocketState'", nullptr);
            return 0;
        }
        int ret = (int)cobj->getSocketState();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESocketClient:getSocketState",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AESocketClient_getSocketState'.",&tolua_err);
#endif

    return 0;
}
int lua_network_AESocketClient_receiveMessage(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESocketClient* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"AESocketClient",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ae::AESocketClient*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_AESocketClient_receiveMessage'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AESocketClient_receiveMessage'", nullptr);
            return 0;
        }
        ae::AEMessage* ret = cobj->receiveMessage();
        object_to_luaval<ae::AEMessage>(tolua_S, "AEMessage",(ae::AEMessage*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESocketClient:receiveMessage",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AESocketClient_receiveMessage'.",&tolua_err);
#endif

    return 0;
}
int lua_network_AESocketClient_threadSend(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AESocketClient",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        void* arg0;
        #pragma warning NO CONVERSION TO NATIVE FOR void*
		ok = false;
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AESocketClient_threadSend'", nullptr);
            return 0;
        }
        void* ret = ae::AESocketClient::threadSend(arg0);
        #pragma warning NO CONVERSION FROM NATIVE FOR void*;
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AESocketClient:threadSend",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AESocketClient_threadSend'.",&tolua_err);
#endif
    return 0;
}
int lua_network_AESocketClient_init(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AESocketClient",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "AESocketClient:init");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AESocketClient_init'", nullptr);
            return 0;
        }
        ae::AESocketClient::init(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AESocketClient:init",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AESocketClient_init'.",&tolua_err);
#endif
    return 0;
}
int lua_network_AESocketClient_threadReceive(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"AESocketClient",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        void* arg0;
        #pragma warning NO CONVERSION TO NATIVE FOR void*
		ok = false;
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AESocketClient_threadReceive'", nullptr);
            return 0;
        }
        void* ret = ae::AESocketClient::threadReceive(arg0);
        #pragma warning NO CONVERSION FROM NATIVE FOR void*;
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "AESocketClient:threadReceive",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_AESocketClient_threadReceive'.",&tolua_err);
#endif
    return 0;
}
int lua_network_AESocketClient_constructor(lua_State* tolua_S)
{
    int argc = 0;
    ae::AESocketClient* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_AESocketClient_constructor'", nullptr);
            return 0;
        }
        cobj = new ae::AESocketClient();
        tolua_pushusertype(tolua_S,(void*)cobj,"AESocketClient");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "AESocketClient:AESocketClient",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_network_AESocketClient_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_network_AESocketClient_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (AESocketClient)");
    return 0;
}

int lua_register_network_AESocketClient(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"AESocketClient");
    tolua_cclass(tolua_S,"AESocketClient","AESocketClient","",nullptr);

    tolua_beginmodule(tolua_S,"AESocketClient");
        tolua_function(tolua_S,"new",lua_network_AESocketClient_constructor);
        tolua_function(tolua_S,"disconnect",lua_network_AESocketClient_disconnect);
        tolua_function(tolua_S,"sendMessage",lua_network_AESocketClient_sendMessage);
        tolua_function(tolua_S,"connect",lua_network_AESocketClient_connect);
        tolua_function(tolua_S,"getSocketState",lua_network_AESocketClient_getSocketState);
        tolua_function(tolua_S,"receiveMessage",lua_network_AESocketClient_receiveMessage);
        tolua_function(tolua_S,"threadSend", lua_network_AESocketClient_threadSend);
        tolua_function(tolua_S,"init", lua_network_AESocketClient_init);
        tolua_function(tolua_S,"threadReceive", lua_network_AESocketClient_threadReceive);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ae::AESocketClient).name();
    g_luaType[typeName] = "AESocketClient";
    g_typeCast["AESocketClient"] = "AESocketClient";
    return 1;
}

int lua_network_XEByteBuffer_rewind(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_rewind'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_rewind'", nullptr);
            return 0;
        }
        cobj->rewind();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:rewind",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_rewind'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_readByte(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_readByte'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_readByte'", nullptr);
            return 0;
        }
        unsigned char& ret = cobj->readByte();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:readByte",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_readByte'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_readBool(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_readBool'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_readBool'", nullptr);
            return 0;
        }
        bool& ret = cobj->readBool();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:readBool",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_readBool'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_writeInt16(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_writeInt16'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int32_t arg0;

        ok &= luaval_to_int32(tolua_S, 2,&arg0, "XEByteBuffer:writeInt16");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_writeInt16'", nullptr);
            return 0;
        }
        bool ret = cobj->writeInt16(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:writeInt16",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_writeInt16'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_writeFloat64(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_writeFloat64'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "XEByteBuffer:writeFloat64");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_writeFloat64'", nullptr);
            return 0;
        }
        bool ret = cobj->writeFloat64(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:writeFloat64",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_writeFloat64'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_readInt8(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_readInt8'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_readInt8'", nullptr);
            return 0;
        }
        char& ret = cobj->readInt8();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:readInt8",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_readInt8'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_compact(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_compact'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_compact'", nullptr);
            return 0;
        }
        cobj->compact();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:compact",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_compact'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_readFloat32(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_readFloat32'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_readFloat32'", nullptr);
            return 0;
        }
        float& ret = cobj->readFloat32();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:readFloat32",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_readFloat32'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_writeUint32(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_writeUint32'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned int arg0;

        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "XEByteBuffer:writeUint32");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_writeUint32'", nullptr);
            return 0;
        }
        bool ret = cobj->writeUint32(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:writeUint32",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_writeUint32'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_writeBool(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_writeBool'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0, "XEByteBuffer:writeBool");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_writeBool'", nullptr);
            return 0;
        }
        bool ret = cobj->writeBool(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:writeBool",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_writeBool'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_readUint32(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_readUint32'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_readUint32'", nullptr);
            return 0;
        }
        unsigned int& ret = cobj->readUint32();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:readUint32",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_readUint32'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_readFloat64(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_readFloat64'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_readFloat64'", nullptr);
            return 0;
        }
        double& ret = cobj->readFloat64();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:readFloat64",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_readFloat64'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_readInt32(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_readInt32'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_readInt32'", nullptr);
            return 0;
        }
        int& ret = cobj->readInt32();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:readInt32",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_readInt32'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_writeInt32(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_writeInt32'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "XEByteBuffer:writeInt32");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_writeInt32'", nullptr);
            return 0;
        }
        bool ret = cobj->writeInt32(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:writeInt32",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_writeInt32'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_writeByte(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_writeByte'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        uint16_t arg0;

        ok &= luaval_to_uint16(tolua_S, 2,&arg0, "XEByteBuffer:writeByte");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_writeByte'", nullptr);
            return 0;
        }
        bool ret = cobj->writeByte(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:writeByte",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_writeByte'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_readString(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_readString'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_readString'", nullptr);
            return 0;
        }
        std::string ret = cobj->readString();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:readString",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_readString'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_writeInt8(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_writeInt8'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int32_t arg0;

        ok &= luaval_to_int32(tolua_S, 2,&arg0, "XEByteBuffer:writeInt8");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_writeInt8'", nullptr);
            return 0;
        }
        bool ret = cobj->writeInt8(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:writeInt8",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_writeInt8'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_writeFloat32(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_writeFloat32'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0, "XEByteBuffer:writeFloat32");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_writeFloat32'", nullptr);
            return 0;
        }
        bool ret = cobj->writeFloat32(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:writeFloat32",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_writeFloat32'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_readInt16(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_readInt16'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_readInt16'", nullptr);
            return 0;
        }
        short& ret = cobj->readInt16();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:readInt16",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_readInt16'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_writeUint16(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_writeUint16'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned short arg0;

        ok &= luaval_to_ushort(tolua_S, 2, &arg0, "XEByteBuffer:writeUint16");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_writeUint16'", nullptr);
            return 0;
        }
        bool ret = cobj->writeUint16(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:writeUint16",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_writeUint16'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_writeString(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_writeString'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "XEByteBuffer:writeString");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_writeString'", nullptr);
            return 0;
        }
        bool ret = cobj->writeString(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:writeString",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_writeString'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_writeToFile(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_writeToFile'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "XEByteBuffer:writeToFile");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_writeToFile'", nullptr);
            return 0;
        }
        bool ret = cobj->writeToFile(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:writeToFile",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_writeToFile'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_readToFile(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_readToFile'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "XEByteBuffer:readToFile");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_readToFile'", nullptr);
            return 0;
        }
        bool ret = cobj->readToFile(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:readToFile",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_readToFile'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_clear(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_clear'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_clear'", nullptr);
            return 0;
        }
        cobj->clear();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:clear",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_clear'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_flip(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_flip'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_flip'", nullptr);
            return 0;
        }
        cobj->flip();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:flip",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_flip'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_remaining(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_remaining'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_remaining'", nullptr);
            return 0;
        }
        unsigned int ret = cobj->remaining();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:remaining",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_remaining'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_readUint16(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEByteBuffer",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEByteBuffer*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEByteBuffer_readUint16'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEByteBuffer_readUint16'", nullptr);
            return 0;
        }
        unsigned short& ret = cobj->readUint16();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEByteBuffer:readUint16",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_readUint16'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_constructor(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEByteBuffer* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 1) {
            unsigned int arg0;
            ok &= luaval_to_uint32(tolua_S, 2,&arg0, "XEByteBuffer:XEByteBuffer");

            if (!ok) { break; }
            cobj = new xe::XEByteBuffer(arg0);
            tolua_pushusertype(tolua_S,(void*)cobj,"XEByteBuffer");
            tolua_register_gc(tolua_S,lua_gettop(tolua_S));
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 0) {
            cobj = new xe::XEByteBuffer();
            tolua_pushusertype(tolua_S,(void*)cobj,"XEByteBuffer");
            tolua_register_gc(tolua_S,lua_gettop(tolua_S));
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 2) {
            const char* arg0;
            std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "XEByteBuffer:XEByteBuffer"); arg0 = arg0_tmp.c_str();

            if (!ok) { break; }
            unsigned int arg1;
            ok &= luaval_to_uint32(tolua_S, 3,&arg1, "XEByteBuffer:XEByteBuffer");

            if (!ok) { break; }
            cobj = new xe::XEByteBuffer(arg0, arg1);
            tolua_pushusertype(tolua_S,(void*)cobj,"XEByteBuffer");
            tolua_register_gc(tolua_S,lua_gettop(tolua_S));
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 3) {
            const char* arg0;
            std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "XEByteBuffer:XEByteBuffer"); arg0 = arg0_tmp.c_str();

            if (!ok) { break; }
            unsigned int arg1;
            ok &= luaval_to_uint32(tolua_S, 3,&arg1, "XEByteBuffer:XEByteBuffer");

            if (!ok) { break; }
            unsigned int arg2;
            ok &= luaval_to_uint32(tolua_S, 4,&arg2, "XEByteBuffer:XEByteBuffer");

            if (!ok) { break; }
            cobj = new xe::XEByteBuffer(arg0, arg1, arg2);
            tolua_pushusertype(tolua_S,(void*)cobj,"XEByteBuffer");
            tolua_register_gc(tolua_S,lua_gettop(tolua_S));
            return 1;
        }
    }while(0);
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "XEByteBuffer:XEByteBuffer",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_constructor'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEByteBuffer_createByteBuffer(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertable(tolua_S, 1, "XEByteBuffer", 0, &tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;

	if (argc == 1)
	{
		unsigned int arg0;
		ok &= luaval_to_uint32(tolua_S, 2, &arg0, "XEByteBuffer:createByteBuffer");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_network_XEByteBuffer_createByteBuffer'", nullptr);
			return 0;
		}
		xe::XEByteBuffer* ret = xe::XEByteBuffer::createByteBuffer(arg0);
		object_to_luaval<xe::XEByteBuffer>(tolua_S, "XEByteBuffer", (xe::XEByteBuffer*)ret);
		return 1;
	}

	if (argc == 0) {
		xe::XEByteBuffer* ret = xe::XEByteBuffer::createByteBuffer();
		object_to_luaval<xe::XEByteBuffer>(tolua_S, "XEByteBuffer", (xe::XEByteBuffer*)ret);
		return 1;
	}

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "XEByteBuffer:createByteBuffer", argc, 2);
	return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_network_XEByteBuffer_createByteBuffer'.", &tolua_err);
#endif
	return 0;
}
int lua_network_XEByteBuffer_releaseByteBuffer(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

    argc = lua_gettop(tolua_S) - 1;
    if (argc == 1) {
		xe::XEByteBuffer* arg0;
		ok &= luaval_to_object<xe::XEByteBuffer>(tolua_S, 2, "XEByteBuffer", &arg0, "XEByteBuffer:releaseByteBuffer");

        if (!ok) { 
			tolua_error(tolua_S, "invalid arguments in function 'lua_network_XEByteBuffer_releaseByteBuffer'", nullptr);
			return 0;
		}
        xe::XEByteBuffer::releaseByteBuffer(arg0);
		lua_settop(tolua_S, 1);
        return 1;
    }
    
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "XEByteBuffer:releaseByteBuffer",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEByteBuffer_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_network_XEByteBuffer_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (XEByteBuffer)");
    return 0;
}

int lua_register_network_XEByteBuffer(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"XEByteBuffer");
    tolua_cclass(tolua_S,"XEByteBuffer","XEByteBuffer","",nullptr);

    tolua_beginmodule(tolua_S,"XEByteBuffer");
        tolua_function(tolua_S,"new",lua_network_XEByteBuffer_constructor);
        tolua_function(tolua_S,"rewind",lua_network_XEByteBuffer_rewind);
        tolua_function(tolua_S,"readByte",lua_network_XEByteBuffer_readByte);
        tolua_function(tolua_S,"readBool",lua_network_XEByteBuffer_readBool);
        tolua_function(tolua_S,"writeInt16",lua_network_XEByteBuffer_writeInt16);
        tolua_function(tolua_S,"writeFloat64",lua_network_XEByteBuffer_writeFloat64);
        tolua_function(tolua_S,"readInt8",lua_network_XEByteBuffer_readInt8);
        tolua_function(tolua_S,"compact",lua_network_XEByteBuffer_compact);
        tolua_function(tolua_S,"readFloat32",lua_network_XEByteBuffer_readFloat32);
        tolua_function(tolua_S,"writeUint32",lua_network_XEByteBuffer_writeUint32);
        tolua_function(tolua_S,"writeBool",lua_network_XEByteBuffer_writeBool);
        tolua_function(tolua_S,"readUint32",lua_network_XEByteBuffer_readUint32);
        tolua_function(tolua_S,"readFloat64",lua_network_XEByteBuffer_readFloat64);
        tolua_function(tolua_S,"readInt32",lua_network_XEByteBuffer_readInt32);
        tolua_function(tolua_S,"writeInt32",lua_network_XEByteBuffer_writeInt32);
        tolua_function(tolua_S,"writeByte",lua_network_XEByteBuffer_writeByte);
        tolua_function(tolua_S,"readString",lua_network_XEByteBuffer_readString);
        tolua_function(tolua_S,"writeInt8",lua_network_XEByteBuffer_writeInt8);
        tolua_function(tolua_S,"writeFloat32",lua_network_XEByteBuffer_writeFloat32);
        tolua_function(tolua_S,"readInt16",lua_network_XEByteBuffer_readInt16);
        tolua_function(tolua_S,"writeUint16",lua_network_XEByteBuffer_writeUint16);
        tolua_function(tolua_S,"writeString",lua_network_XEByteBuffer_writeString);
        tolua_function(tolua_S,"writeToFile",lua_network_XEByteBuffer_writeToFile);
        tolua_function(tolua_S,"readToFile",lua_network_XEByteBuffer_readToFile);
        tolua_function(tolua_S,"clear",lua_network_XEByteBuffer_clear);
        tolua_function(tolua_S,"flip",lua_network_XEByteBuffer_flip);
        tolua_function(tolua_S,"remaining",lua_network_XEByteBuffer_remaining);
        tolua_function(tolua_S,"readUint16",lua_network_XEByteBuffer_readUint16);
		tolua_function(tolua_S, "createByteBuffer", lua_network_XEByteBuffer_createByteBuffer);
        tolua_function(tolua_S,"releaseByteBuffer",lua_network_XEByteBuffer_releaseByteBuffer);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(xe::XEByteBuffer).name();
    g_luaType[typeName] = "XEByteBuffer";
    g_typeCast["XEByteBuffer"] = "XEByteBuffer";
    return 1;
}

int lua_network_XEMessage_setExtension(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEMessage_setExtension'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned int arg0;

        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "XEMessage:setExtension");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEMessage_setExtension'", nullptr);
            return 0;
        }
        cobj->setExtension(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEMessage:setExtension",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEMessage_setExtension'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEMessage_setSequenceID(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEMessage_setSequenceID'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned int arg0;

        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "XEMessage:setSequenceID");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEMessage_setSequenceID'", nullptr);
            return 0;
        }
        cobj->setSequenceID(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEMessage:setSequenceID",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEMessage_setSequenceID'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEMessage_getResult(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEMessage_getResult'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEMessage_getResult'", nullptr);
            return 0;
        }
        const int& ret = cobj->getResult();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEMessage:getResult",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEMessage_getResult'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEMessage_composite(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEMessage_composite'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEMessage_composite'", nullptr);
            return 0;
        }
        cobj->composite();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEMessage:composite",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEMessage_composite'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEMessage_getProtocolID(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEMessage_getProtocolID'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEMessage_getProtocolID'", nullptr);
            return 0;
        }
        const unsigned int& ret = cobj->getProtocolID();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEMessage:getProtocolID",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEMessage_getProtocolID'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEMessage_getExtension(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEMessage_getExtension'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEMessage_getExtension'", nullptr);
            return 0;
        }
        const unsigned int& ret = cobj->getExtension();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEMessage:getExtension",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEMessage_getExtension'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEMessage_getByteBuffer(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEMessage_getByteBuffer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEMessage_getByteBuffer'", nullptr);
            return 0;
        }
        xe::XEByteBuffer* ret = cobj->getByteBuffer();
        object_to_luaval<xe::XEByteBuffer>(tolua_S, "XEByteBuffer",(xe::XEByteBuffer*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEMessage:getByteBuffer",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEMessage_getByteBuffer'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEMessage_setProtocolID(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEMessage_setProtocolID'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned int arg0;

        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "XEMessage:setProtocolID");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEMessage_setProtocolID'", nullptr);
            return 0;
        }
        cobj->setProtocolID(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEMessage:setProtocolID",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEMessage_setProtocolID'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEMessage_parse(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEMessage_parse'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEMessage_parse'", nullptr);
            return 0;
        }
        cobj->parse();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEMessage:parse",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEMessage_parse'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEMessage_getSequenceID(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEMessage_getSequenceID'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEMessage_getSequenceID'", nullptr);
            return 0;
        }
        const unsigned int& ret = cobj->getSequenceID();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEMessage:getSequenceID",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEMessage_getSequenceID'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEMessage_setBodyLenght(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEMessage_setBodyLenght'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned int arg0;

        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "XEMessage:setBodyLenght");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEMessage_setBodyLenght'", nullptr);
            return 0;
        }
        cobj->setBodyLenght(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEMessage:setBodyLenght",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEMessage_setBodyLenght'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEMessage_release(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEMessage_release'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEMessage_release'", nullptr);
            return 0;
        }
        cobj->release();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEMessage:release",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEMessage_release'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEMessage_getBodyLenght(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEMessage_getBodyLenght'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEMessage_getBodyLenght'", nullptr);
            return 0;
        }
        const unsigned int& ret = cobj->getBodyLenght();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEMessage:getBodyLenght",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEMessage_getBodyLenght'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEMessage_setResult(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEMessage* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEMessage*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEMessage_setResult'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "XEMessage:setResult");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEMessage_setResult'", nullptr);
            return 0;
        }
        cobj->setResult(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEMessage:setResult",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEMessage_setResult'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEMessage_setUserID(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"XEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        unsigned int arg0;
        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "XEMessage:setUserID");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEMessage_setUserID'", nullptr);
            return 0;
        }
        xe::XEMessage::setUserID(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "XEMessage:setUserID",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEMessage_setUserID'.",&tolua_err);
#endif
    return 0;
}
int lua_network_XEMessage_receive(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"XEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        unsigned int arg0;
        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "XEMessage:receive");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEMessage_receive'", nullptr);
            return 0;
        }
        xe::XEMessage* ret = xe::XEMessage::receive(arg0);
        object_to_luaval<xe::XEMessage>(tolua_S, "XEMessage",(xe::XEMessage*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "XEMessage:receive",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEMessage_receive'.",&tolua_err);
#endif
    return 0;
}
int lua_network_XEMessage_errorMessage(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"XEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S)-1;

    do 
    {
        if (argc == 3)
        {
            int arg0;
            ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "XEMessage:errorMessage");
            if (!ok) { break; }
            unsigned int arg1;
            ok &= luaval_to_uint32(tolua_S, 3,&arg1, "XEMessage:errorMessage");
            if (!ok) { break; }
            unsigned int arg2;
            ok &= luaval_to_uint32(tolua_S, 4,&arg2, "XEMessage:errorMessage");
            if (!ok) { break; }
            xe::XEMessage* ret = xe::XEMessage::errorMessage(arg0, arg1, arg2);
            object_to_luaval<xe::XEMessage>(tolua_S, "XEMessage",(xe::XEMessage*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    do 
    {
        if (argc == 1)
        {
            int arg0;
            ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "XEMessage:errorMessage");
            if (!ok) { break; }
            xe::XEMessage* ret = xe::XEMessage::errorMessage(arg0);
            object_to_luaval<xe::XEMessage>(tolua_S, "XEMessage",(xe::XEMessage*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    do 
    {
        if (argc == 2)
        {
            int arg0;
            ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "XEMessage:errorMessage");
            if (!ok) { break; }
            unsigned int arg1;
            ok &= luaval_to_uint32(tolua_S, 3,&arg1, "XEMessage:errorMessage");
            if (!ok) { break; }
            xe::XEMessage* ret = xe::XEMessage::errorMessage(arg0, arg1);
            object_to_luaval<xe::XEMessage>(tolua_S, "XEMessage",(xe::XEMessage*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d", "XEMessage:errorMessage",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEMessage_errorMessage'.",&tolua_err);
#endif
    return 0;
}
int lua_network_XEMessage_send(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"XEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        unsigned int arg0;
        unsigned int arg1;
        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "XEMessage:send");
        ok &= luaval_to_uint32(tolua_S, 3,&arg1, "XEMessage:send");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEMessage_send'", nullptr);
            return 0;
        }
        xe::XEMessage* ret = xe::XEMessage::send(arg0, arg1);
        object_to_luaval<xe::XEMessage>(tolua_S, "XEMessage",(xe::XEMessage*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "XEMessage:send",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEMessage_send'.",&tolua_err);
#endif
    return 0;
}
int lua_network_XEMessage_getUserID(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"XEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEMessage_getUserID'", nullptr);
            return 0;
        }
        unsigned int ret = xe::XEMessage::getUserID();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "XEMessage:getUserID",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEMessage_getUserID'.",&tolua_err);
#endif
    return 0;
}
int lua_network_XEMessage_setSessionID(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"XEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        unsigned int arg0;
        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "XEMessage:setSessionID");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEMessage_setSessionID'", nullptr);
            return 0;
        }
        xe::XEMessage::setSessionID(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "XEMessage:setSessionID",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEMessage_setSessionID'.",&tolua_err);
#endif
    return 0;
}
int lua_network_XEMessage_getSessionID(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"XEMessage",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEMessage_getSessionID'", nullptr);
            return 0;
        }
        unsigned int ret = xe::XEMessage::getSessionID();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "XEMessage:getSessionID",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEMessage_getSessionID'.",&tolua_err);
#endif
    return 0;
}
int lua_network_XEMessage_constructor(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEMessage* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 1) {
            unsigned int arg0;
            ok &= luaval_to_uint32(tolua_S, 2,&arg0, "XEMessage:XEMessage");

            if (!ok) { break; }
            cobj = new xe::XEMessage(arg0);
            tolua_pushusertype(tolua_S,(void*)cobj,"XEMessage");
            tolua_register_gc(tolua_S,lua_gettop(tolua_S));
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 0) {
            cobj = new xe::XEMessage();
            tolua_pushusertype(tolua_S,(void*)cobj,"XEMessage");
            tolua_register_gc(tolua_S,lua_gettop(tolua_S));
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 2) {
            unsigned int arg0;
            ok &= luaval_to_uint32(tolua_S, 2,&arg0, "XEMessage:XEMessage");

            if (!ok) { break; }
            unsigned int arg1;
            ok &= luaval_to_uint32(tolua_S, 3,&arg1, "XEMessage:XEMessage");

            if (!ok) { break; }
            cobj = new xe::XEMessage(arg0, arg1);
            tolua_pushusertype(tolua_S,(void*)cobj,"XEMessage");
            tolua_register_gc(tolua_S,lua_gettop(tolua_S));
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 1) {
            xe::XEByteBuffer* arg0;
            ok &= luaval_to_object<xe::XEByteBuffer>(tolua_S, 2, "XEByteBuffer",&arg0, "XEMessage:XEMessage");

            if (!ok) { break; }
            cobj = new xe::XEMessage(arg0);
            tolua_pushusertype(tolua_S,(void*)cobj,"XEMessage");
            tolua_register_gc(tolua_S,lua_gettop(tolua_S));
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 3) {
            const char* arg0;
            std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "XEMessage:XEMessage"); arg0 = arg0_tmp.c_str();

            if (!ok) { break; }
            unsigned int arg1;
            ok &= luaval_to_uint32(tolua_S, 3,&arg1, "XEMessage:XEMessage");

            if (!ok) { break; }
            unsigned int arg2;
            ok &= luaval_to_uint32(tolua_S, 4,&arg2, "XEMessage:XEMessage");

            if (!ok) { break; }
            cobj = new xe::XEMessage(arg0, arg1, arg2);
            tolua_pushusertype(tolua_S,(void*)cobj,"XEMessage");
            tolua_register_gc(tolua_S,lua_gettop(tolua_S));
            return 1;
        }
    }while(0);
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "XEMessage:XEMessage",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEMessage_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_network_XEMessage_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (XEMessage)");
    return 0;
}

int lua_register_network_XEMessage(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"XEMessage");
    tolua_cclass(tolua_S,"XEMessage","XEMessage","",nullptr);

    tolua_beginmodule(tolua_S,"XEMessage");
        tolua_function(tolua_S,"new",lua_network_XEMessage_constructor);
        tolua_function(tolua_S,"setExtension",lua_network_XEMessage_setExtension);
        tolua_function(tolua_S,"setSequenceID",lua_network_XEMessage_setSequenceID);
        tolua_function(tolua_S,"getResult",lua_network_XEMessage_getResult);
        tolua_function(tolua_S,"composite",lua_network_XEMessage_composite);
        tolua_function(tolua_S,"getProtocolID",lua_network_XEMessage_getProtocolID);
        tolua_function(tolua_S,"getExtension",lua_network_XEMessage_getExtension);
        tolua_function(tolua_S,"getByteBuffer",lua_network_XEMessage_getByteBuffer);
        tolua_function(tolua_S,"setProtocolID",lua_network_XEMessage_setProtocolID);
        tolua_function(tolua_S,"parse",lua_network_XEMessage_parse);
        tolua_function(tolua_S,"getSequenceID",lua_network_XEMessage_getSequenceID);
        tolua_function(tolua_S,"setBodyLenght",lua_network_XEMessage_setBodyLenght);
        tolua_function(tolua_S,"release",lua_network_XEMessage_release);
        tolua_function(tolua_S,"getBodyLenght",lua_network_XEMessage_getBodyLenght);
        tolua_function(tolua_S,"setResult",lua_network_XEMessage_setResult);
        tolua_function(tolua_S,"setUserID", lua_network_XEMessage_setUserID);
        tolua_function(tolua_S,"receive", lua_network_XEMessage_receive);
        tolua_function(tolua_S,"errorMessage", lua_network_XEMessage_errorMessage);
        tolua_function(tolua_S,"send", lua_network_XEMessage_send);
        tolua_function(tolua_S,"getUserID", lua_network_XEMessage_getUserID);
        tolua_function(tolua_S,"setSessionID", lua_network_XEMessage_setSessionID);
        tolua_function(tolua_S,"getSessionID", lua_network_XEMessage_getSessionID);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(xe::XEMessage).name();
    g_luaType[typeName] = "XEMessage";
    g_typeCast["XEMessage"] = "XEMessage";
    return 1;
}

int lua_network_XESelector_remove_timer(lua_State* tolua_S)
{
    int argc = 0;
    xe::XESelector* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XESelector",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XESelector*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XESelector_remove_timer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "XESelector:remove_timer");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XESelector_remove_timer'", nullptr);
            return 0;
        }
        cobj->remove_timer(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XESelector:remove_timer",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XESelector_remove_timer'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XESelector_stop(lua_State* tolua_S)
{
    int argc = 0;
    xe::XESelector* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XESelector",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XESelector*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XESelector_stop'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XESelector_stop'", nullptr);
            return 0;
        }
        cobj->stop();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XESelector:stop",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XESelector_stop'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XESelector_start(lua_State* tolua_S)
{
    int argc = 0;
    xe::XESelector* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XESelector",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XESelector*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XESelector_start'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XESelector_start'", nullptr);
            return 0;
        }
        bool ret = cobj->start();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XESelector:start",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XESelector_start'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XESelector_getInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"XESelector",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XESelector_getInstance'", nullptr);
            return 0;
        }
        xe::XESelector* ret = xe::XESelector::getInstance();
        object_to_luaval<xe::XESelector>(tolua_S, "XESelector",(xe::XESelector*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "XESelector:getInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XESelector_getInstance'.",&tolua_err);
#endif
    return 0;
}
static int lua_network_XESelector_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (XESelector)");
    return 0;
}

int lua_register_network_XESelector(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"XESelector");
    tolua_cclass(tolua_S,"XESelector","XESelector","",nullptr);

    tolua_beginmodule(tolua_S,"XESelector");
        tolua_function(tolua_S,"remove_timer",lua_network_XESelector_remove_timer);
        tolua_function(tolua_S,"stop",lua_network_XESelector_stop);
        tolua_function(tolua_S,"start",lua_network_XESelector_start);
        tolua_function(tolua_S,"getInstance", lua_network_XESelector_getInstance);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(xe::XESelector).name();
    g_luaType[typeName] = "XESelector";
    g_typeCast["XESelector"] = "XESelector";
    return 1;
}

int lua_network_XETCPSocket_disconnect(lua_State* tolua_S)
{
    int argc = 0;
    xe::XETCPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XETCPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XETCPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XETCPSocket_disconnect'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XETCPSocket_disconnect'", nullptr);
            return 0;
        }
        cobj->disconnect();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XETCPSocket:disconnect",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XETCPSocket_disconnect'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XETCPSocket_setName(lua_State* tolua_S)
{
    int argc = 0;
    xe::XETCPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XETCPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XETCPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XETCPSocket_setName'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "XETCPSocket:setName");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XETCPSocket_setName'", nullptr);
            return 0;
        }
        cobj->setName(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XETCPSocket:setName",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XETCPSocket_setName'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XETCPSocket_wait_close(lua_State* tolua_S)
{
    int argc = 0;
    xe::XETCPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XETCPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XETCPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XETCPSocket_wait_close'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "XETCPSocket:wait_close");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XETCPSocket_wait_close'", nullptr);
            return 0;
        }
        int ret = (int)cobj->wait_close(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XETCPSocket:wait_close",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XETCPSocket_wait_close'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XETCPSocket_getSocketState(lua_State* tolua_S)
{
    int argc = 0;
    xe::XETCPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XETCPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XETCPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XETCPSocket_getSocketState'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XETCPSocket_getSocketState'", nullptr);
            return 0;
        }
        int ret = cobj->getSocketState();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XETCPSocket:getSocketState",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XETCPSocket_getSocketState'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XETCPSocket_getName(lua_State* tolua_S)
{
    int argc = 0;
    xe::XETCPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XETCPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XETCPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XETCPSocket_getName'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XETCPSocket_getName'", nullptr);
            return 0;
        }
        std::string ret = cobj->getName();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XETCPSocket:getName",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XETCPSocket_getName'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XETCPSocket_get_handle(lua_State* tolua_S)
{
    int argc = 0;
    xe::XETCPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XETCPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XETCPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XETCPSocket_get_handle'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XETCPSocket_get_handle'", nullptr);
            return 0;
        }
        int ret = cobj->get_handle();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XETCPSocket:get_handle",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XETCPSocket_get_handle'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XETCPSocket_get_error_info(lua_State* tolua_S)
{
    int argc = 0;
    xe::XETCPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XETCPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XETCPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XETCPSocket_get_error_info'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        unsigned int arg0;
        unsigned int arg1;

        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "XETCPSocket:get_error_info");

        ok &= luaval_to_uint32(tolua_S, 3,&arg1, "XETCPSocket:get_error_info");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XETCPSocket_get_error_info'", nullptr);
            return 0;
        }
        cobj->get_error_info(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XETCPSocket:get_error_info",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XETCPSocket_get_error_info'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XETCPSocket_send_message(lua_State* tolua_S)
{
    int argc = 0;
    xe::XETCPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XETCPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XETCPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XETCPSocket_send_message'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        xe::XEMessage* arg0;

        ok &= luaval_to_object<xe::XEMessage>(tolua_S, 2, "XEMessage",&arg0, "XETCPSocket:send_message");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XETCPSocket_send_message'", nullptr);
            return 0;
        }
        bool ret = cobj->send_message(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    if (argc == 2) 
    {
        xe::XEMessage* arg0;
        unsigned int arg1;

        ok &= luaval_to_object<xe::XEMessage>(tolua_S, 2, "XEMessage",&arg0, "XETCPSocket:send_message");

        ok &= luaval_to_uint32(tolua_S, 3,&arg1, "XETCPSocket:send_message");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XETCPSocket_send_message'", nullptr);
            return 0;
        }
        bool ret = cobj->send_message(arg0, arg1);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XETCPSocket:send_message",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XETCPSocket_send_message'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XETCPSocket_recv_message(lua_State* tolua_S)
{
    int argc = 0;
    xe::XETCPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XETCPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XETCPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XETCPSocket_recv_message'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        xe::XEMessage* arg0;

        ok &= luaval_to_object<xe::XEMessage>(tolua_S, 2, "XEMessage",&arg0, "XETCPSocket:recv_message");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XETCPSocket_recv_message'", nullptr);
            return 0;
        }
        bool ret = cobj->recv_message(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XETCPSocket:recv_message",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XETCPSocket_recv_message'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XETCPSocket_init(lua_State* tolua_S)
{
    int argc = 0;
    xe::XETCPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XETCPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XETCPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XETCPSocket_init'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        xe::XESelector* arg0;
        std::string arg1;

        ok &= luaval_to_object<xe::XESelector>(tolua_S, 2, "XESelector",&arg0, "XETCPSocket:init");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "XETCPSocket:init");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XETCPSocket_init'", nullptr);
            return 0;
        }
        cobj->init(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XETCPSocket:init",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XETCPSocket_init'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XETCPSocket_sendMessage(lua_State* tolua_S)
{
    int argc = 0;
    xe::XETCPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XETCPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XETCPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XETCPSocket_sendMessage'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        xe::XEMessage* arg0;

        ok &= luaval_to_object<xe::XEMessage>(tolua_S, 2, "XEMessage",&arg0, "XETCPSocket:sendMessage");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XETCPSocket_sendMessage'", nullptr);
            return 0;
        }
        cobj->sendMessage(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    if (argc == 2) 
    {
        xe::XEMessage* arg0;
        unsigned int arg1;

        ok &= luaval_to_object<xe::XEMessage>(tolua_S, 2, "XEMessage",&arg0, "XETCPSocket:sendMessage");

        ok &= luaval_to_uint32(tolua_S, 3,&arg1, "XETCPSocket:sendMessage");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XETCPSocket_sendMessage'", nullptr);
            return 0;
        }
        cobj->sendMessage(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XETCPSocket:sendMessage",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XETCPSocket_sendMessage'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XETCPSocket_connect(lua_State* tolua_S)
{
    int argc = 0;
    xe::XETCPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XETCPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XETCPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XETCPSocket_connect'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        unsigned short arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "XETCPSocket:connect");

        ok &= luaval_to_ushort(tolua_S, 3, &arg1, "XETCPSocket:connect");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XETCPSocket_connect'", nullptr);
            return 0;
        }
        int ret = (int)cobj->connect(arg0, arg1);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XETCPSocket:connect",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XETCPSocket_connect'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XETCPSocket_close(lua_State* tolua_S)
{
    int argc = 0;
    xe::XETCPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XETCPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XETCPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XETCPSocket_close'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XETCPSocket_close'", nullptr);
            return 0;
        }
        bool ret = cobj->close();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XETCPSocket:close",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XETCPSocket_close'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XETCPSocket_wait_connect(lua_State* tolua_S)
{
    int argc = 0;
    xe::XETCPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XETCPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XETCPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XETCPSocket_wait_connect'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "XETCPSocket:wait_connect");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XETCPSocket_wait_connect'", nullptr);
            return 0;
        }
        int ret = (int)cobj->wait_connect(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XETCPSocket:wait_connect",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XETCPSocket_wait_connect'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XETCPSocket_receiveMessage(lua_State* tolua_S)
{
    int argc = 0;
    xe::XETCPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XETCPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XETCPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XETCPSocket_receiveMessage'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XETCPSocket_receiveMessage'", nullptr);
            return 0;
        }
        xe::XEMessage* ret = cobj->receiveMessage();
        object_to_luaval<xe::XEMessage>(tolua_S, "XEMessage",(xe::XEMessage*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XETCPSocket:receiveMessage",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XETCPSocket_receiveMessage'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XETCPSocket_is_alive(lua_State* tolua_S)
{
    int argc = 0;
    xe::XETCPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XETCPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XETCPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XETCPSocket_is_alive'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XETCPSocket_is_alive'", nullptr);
            return 0;
        }
        bool ret = cobj->is_alive();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XETCPSocket:is_alive",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XETCPSocket_is_alive'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XETCPSocket_releaseSocket(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"XETCPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        xe::XETCPSocket* arg0;
        ok &= luaval_to_object<xe::XETCPSocket>(tolua_S, 2, "XETCPSocket",&arg0, "XETCPSocket:releaseSocket");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XETCPSocket_releaseSocket'", nullptr);
            return 0;
        }
        xe::XETCPSocket::releaseSocket(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "XETCPSocket:releaseSocket",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XETCPSocket_releaseSocket'.",&tolua_err);
#endif
    return 0;
}
int lua_network_XETCPSocket_createSocket(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"XETCPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        xe::XESelector* arg0;
        std::string arg1;
        ok &= luaval_to_object<xe::XESelector>(tolua_S, 2, "XESelector",&arg0, "XETCPSocket:createSocket");
        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "XETCPSocket:createSocket");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XETCPSocket_createSocket'", nullptr);
            return 0;
        }
        xe::XETCPSocket* ret = xe::XETCPSocket::createSocket(arg0, arg1);
        object_to_luaval<xe::XETCPSocket>(tolua_S, "XETCPSocket",(xe::XETCPSocket*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "XETCPSocket:createSocket",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XETCPSocket_createSocket'.",&tolua_err);
#endif
    return 0;
}
int lua_network_XETCPSocket_constructor(lua_State* tolua_S)
{
    int argc = 0;
    xe::XETCPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XETCPSocket_constructor'", nullptr);
            return 0;
        }
        cobj = new xe::XETCPSocket();
        tolua_pushusertype(tolua_S,(void*)cobj,"XETCPSocket");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XETCPSocket:XETCPSocket",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_network_XETCPSocket_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_network_XETCPSocket_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (XETCPSocket)");
    return 0;
}

int lua_register_network_XETCPSocket(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"XETCPSocket");
    tolua_cclass(tolua_S,"XETCPSocket","XETCPSocket","XESocketBase",nullptr);

    tolua_beginmodule(tolua_S,"XETCPSocket");
        tolua_function(tolua_S,"new",lua_network_XETCPSocket_constructor);
        tolua_function(tolua_S,"disconnect",lua_network_XETCPSocket_disconnect);
        tolua_function(tolua_S,"setName",lua_network_XETCPSocket_setName);
        tolua_function(tolua_S,"wait_close",lua_network_XETCPSocket_wait_close);
        tolua_function(tolua_S,"getSocketState",lua_network_XETCPSocket_getSocketState);
        tolua_function(tolua_S,"getName",lua_network_XETCPSocket_getName);
        tolua_function(tolua_S,"get_handle",lua_network_XETCPSocket_get_handle);
        tolua_function(tolua_S,"get_error_info",lua_network_XETCPSocket_get_error_info);
        tolua_function(tolua_S,"send_message",lua_network_XETCPSocket_send_message);
        tolua_function(tolua_S,"recv_message",lua_network_XETCPSocket_recv_message);
        tolua_function(tolua_S,"init",lua_network_XETCPSocket_init);
        tolua_function(tolua_S,"sendMessage",lua_network_XETCPSocket_sendMessage);
        tolua_function(tolua_S,"connect",lua_network_XETCPSocket_connect);
        tolua_function(tolua_S,"close",lua_network_XETCPSocket_close);
        tolua_function(tolua_S,"wait_connect",lua_network_XETCPSocket_wait_connect);
        tolua_function(tolua_S,"receiveMessage",lua_network_XETCPSocket_receiveMessage);
        tolua_function(tolua_S,"is_alive",lua_network_XETCPSocket_is_alive);
        tolua_function(tolua_S,"releaseSocket", lua_network_XETCPSocket_releaseSocket);
        tolua_function(tolua_S,"createSocket", lua_network_XETCPSocket_createSocket);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(xe::XETCPSocket).name();
    g_luaType[typeName] = "XETCPSocket";
    g_typeCast["XETCPSocket"] = "XETCPSocket";
    return 1;
}

int lua_network_XEUDPSocket_disconnect(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEUDPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEUDPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEUDPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEUDPSocket_disconnect'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEUDPSocket_disconnect'", nullptr);
            return 0;
        }
        cobj->disconnect();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEUDPSocket:disconnect",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEUDPSocket_disconnect'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEUDPSocket_wait_close(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEUDPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEUDPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEUDPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEUDPSocket_wait_close'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "XEUDPSocket:wait_close");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEUDPSocket_wait_close'", nullptr);
            return 0;
        }
        int ret = (int)cobj->wait_close(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEUDPSocket:wait_close",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEUDPSocket_wait_close'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEUDPSocket_getSocketState(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEUDPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEUDPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEUDPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEUDPSocket_getSocketState'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEUDPSocket_getSocketState'", nullptr);
            return 0;
        }
        int ret = cobj->getSocketState();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEUDPSocket:getSocketState",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEUDPSocket_getSocketState'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEUDPSocket_get_handle(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEUDPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEUDPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEUDPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEUDPSocket_get_handle'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEUDPSocket_get_handle'", nullptr);
            return 0;
        }
        int ret = cobj->get_handle();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEUDPSocket:get_handle",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEUDPSocket_get_handle'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEUDPSocket_get_error_info(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEUDPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEUDPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEUDPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEUDPSocket_get_error_info'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        unsigned int arg0;
        unsigned int arg1;

        ok &= luaval_to_uint32(tolua_S, 2,&arg0, "XEUDPSocket:get_error_info");

        ok &= luaval_to_uint32(tolua_S, 3,&arg1, "XEUDPSocket:get_error_info");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEUDPSocket_get_error_info'", nullptr);
            return 0;
        }
        cobj->get_error_info(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEUDPSocket:get_error_info",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEUDPSocket_get_error_info'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEUDPSocket_send_message(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEUDPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEUDPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEUDPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEUDPSocket_send_message'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        xe::XEMessage* arg0;

        ok &= luaval_to_object<xe::XEMessage>(tolua_S, 2, "XEMessage",&arg0, "XEUDPSocket:send_message");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEUDPSocket_send_message'", nullptr);
            return 0;
        }
        bool ret = cobj->send_message(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    if (argc == 2) 
    {
        xe::XEMessage* arg0;
        unsigned int arg1;

        ok &= luaval_to_object<xe::XEMessage>(tolua_S, 2, "XEMessage",&arg0, "XEUDPSocket:send_message");

        ok &= luaval_to_uint32(tolua_S, 3,&arg1, "XEUDPSocket:send_message");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEUDPSocket_send_message'", nullptr);
            return 0;
        }
        bool ret = cobj->send_message(arg0, arg1);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEUDPSocket:send_message",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEUDPSocket_send_message'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEUDPSocket_recv_message(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEUDPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEUDPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEUDPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEUDPSocket_recv_message'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        xe::XEMessage* arg0;

        ok &= luaval_to_object<xe::XEMessage>(tolua_S, 2, "XEMessage",&arg0, "XEUDPSocket:recv_message");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEUDPSocket_recv_message'", nullptr);
            return 0;
        }
        bool ret = cobj->recv_message(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEUDPSocket:recv_message",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEUDPSocket_recv_message'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEUDPSocket_init(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEUDPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEUDPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEUDPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEUDPSocket_init'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        xe::XESelector* arg0;
        std::string arg1;

        ok &= luaval_to_object<xe::XESelector>(tolua_S, 2, "XESelector",&arg0, "XEUDPSocket:init");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "XEUDPSocket:init");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEUDPSocket_init'", nullptr);
            return 0;
        }
        cobj->init(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEUDPSocket:init",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEUDPSocket_init'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEUDPSocket_get_status(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEUDPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEUDPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEUDPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEUDPSocket_get_status'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEUDPSocket_get_status'", nullptr);
            return 0;
        }
        long ret = cobj->get_status();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEUDPSocket:get_status",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEUDPSocket_get_status'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEUDPSocket_sendMessage(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEUDPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEUDPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEUDPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEUDPSocket_sendMessage'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        xe::XEMessage* arg0;

        ok &= luaval_to_object<xe::XEMessage>(tolua_S, 2, "XEMessage",&arg0, "XEUDPSocket:sendMessage");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEUDPSocket_sendMessage'", nullptr);
            return 0;
        }
        cobj->sendMessage(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    if (argc == 2) 
    {
        xe::XEMessage* arg0;
        unsigned int arg1;

        ok &= luaval_to_object<xe::XEMessage>(tolua_S, 2, "XEMessage",&arg0, "XEUDPSocket:sendMessage");

        ok &= luaval_to_uint32(tolua_S, 3,&arg1, "XEUDPSocket:sendMessage");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEUDPSocket_sendMessage'", nullptr);
            return 0;
        }
        cobj->sendMessage(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEUDPSocket:sendMessage",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEUDPSocket_sendMessage'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEUDPSocket_connect(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEUDPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEUDPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEUDPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEUDPSocket_connect'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        unsigned short arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "XEUDPSocket:connect");

        ok &= luaval_to_ushort(tolua_S, 3, &arg1, "XEUDPSocket:connect");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEUDPSocket_connect'", nullptr);
            return 0;
        }
        int ret = (int)cobj->connect(arg0, arg1);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEUDPSocket:connect",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEUDPSocket_connect'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEUDPSocket_close(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEUDPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEUDPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEUDPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEUDPSocket_close'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEUDPSocket_close'", nullptr);
            return 0;
        }
        bool ret = cobj->close();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEUDPSocket:close",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEUDPSocket_close'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEUDPSocket_wait_connect(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEUDPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEUDPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEUDPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEUDPSocket_wait_connect'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "XEUDPSocket:wait_connect");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEUDPSocket_wait_connect'", nullptr);
            return 0;
        }
        int ret = (int)cobj->wait_connect(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEUDPSocket:wait_connect",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEUDPSocket_wait_connect'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEUDPSocket_receiveMessage(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEUDPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEUDPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEUDPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEUDPSocket_receiveMessage'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEUDPSocket_receiveMessage'", nullptr);
            return 0;
        }
        xe::XEMessage* ret = cobj->receiveMessage();
        object_to_luaval<xe::XEMessage>(tolua_S, "XEMessage",(xe::XEMessage*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEUDPSocket:receiveMessage",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEUDPSocket_receiveMessage'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEUDPSocket_is_alive(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEUDPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"XEUDPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (xe::XEUDPSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_network_XEUDPSocket_is_alive'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEUDPSocket_is_alive'", nullptr);
            return 0;
        }
        bool ret = cobj->is_alive();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEUDPSocket:is_alive",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEUDPSocket_is_alive'.",&tolua_err);
#endif

    return 0;
}
int lua_network_XEUDPSocket_releaseSocket(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"XEUDPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        xe::XEUDPSocket* arg0;
        ok &= luaval_to_object<xe::XEUDPSocket>(tolua_S, 2, "XEUDPSocket",&arg0, "XEUDPSocket:releaseSocket");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEUDPSocket_releaseSocket'", nullptr);
            return 0;
        }
        xe::XEUDPSocket::releaseSocket(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "XEUDPSocket:releaseSocket",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEUDPSocket_releaseSocket'.",&tolua_err);
#endif
    return 0;
}
int lua_network_XEUDPSocket_createSocket(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"XEUDPSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        xe::XESelector* arg0;
        std::string arg1;
        ok &= luaval_to_object<xe::XESelector>(tolua_S, 2, "XESelector",&arg0, "XEUDPSocket:createSocket");
        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "XEUDPSocket:createSocket");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEUDPSocket_createSocket'", nullptr);
            return 0;
        }
        xe::XEUDPSocket* ret = xe::XEUDPSocket::createSocket(arg0, arg1);
        object_to_luaval<xe::XEUDPSocket>(tolua_S, "XEUDPSocket",(xe::XEUDPSocket*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "XEUDPSocket:createSocket",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEUDPSocket_createSocket'.",&tolua_err);
#endif
    return 0;
}
int lua_network_XEUDPSocket_constructor(lua_State* tolua_S)
{
    int argc = 0;
    xe::XEUDPSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_network_XEUDPSocket_constructor'", nullptr);
            return 0;
        }
        cobj = new xe::XEUDPSocket();
        tolua_pushusertype(tolua_S,(void*)cobj,"XEUDPSocket");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "XEUDPSocket:XEUDPSocket",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_network_XEUDPSocket_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_network_XEUDPSocket_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (XEUDPSocket)");
    return 0;
}

int lua_register_network_XEUDPSocket(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"XEUDPSocket");
    tolua_cclass(tolua_S,"XEUDPSocket","XEUDPSocket","XESocketBase",nullptr);

    tolua_beginmodule(tolua_S,"XEUDPSocket");
        tolua_function(tolua_S,"new",lua_network_XEUDPSocket_constructor);
        tolua_function(tolua_S,"disconnect",lua_network_XEUDPSocket_disconnect);
        tolua_function(tolua_S,"wait_close",lua_network_XEUDPSocket_wait_close);
        tolua_function(tolua_S,"getSocketState",lua_network_XEUDPSocket_getSocketState);
        tolua_function(tolua_S,"get_handle",lua_network_XEUDPSocket_get_handle);
        tolua_function(tolua_S,"get_error_info",lua_network_XEUDPSocket_get_error_info);
        tolua_function(tolua_S,"send_message",lua_network_XEUDPSocket_send_message);
        tolua_function(tolua_S,"recv_message",lua_network_XEUDPSocket_recv_message);
        tolua_function(tolua_S,"init",lua_network_XEUDPSocket_init);
        tolua_function(tolua_S,"get_status",lua_network_XEUDPSocket_get_status);
        tolua_function(tolua_S,"sendMessage",lua_network_XEUDPSocket_sendMessage);
        tolua_function(tolua_S,"connect",lua_network_XEUDPSocket_connect);
        tolua_function(tolua_S,"close",lua_network_XEUDPSocket_close);
        tolua_function(tolua_S,"wait_connect",lua_network_XEUDPSocket_wait_connect);
        tolua_function(tolua_S,"receiveMessage",lua_network_XEUDPSocket_receiveMessage);
        tolua_function(tolua_S,"is_alive",lua_network_XEUDPSocket_is_alive);
        tolua_function(tolua_S,"releaseSocket", lua_network_XEUDPSocket_releaseSocket);
        tolua_function(tolua_S,"createSocket", lua_network_XEUDPSocket_createSocket);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(xe::XEUDPSocket).name();
    g_luaType[typeName] = "XEUDPSocket";
    g_typeCast["XEUDPSocket"] = "XEUDPSocket";
    return 1;
}
TOLUA_API int register_all_network(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,nullptr,0);
	tolua_beginmodule(tolua_S,nullptr);

	lua_register_network_XESelector(tolua_S);
	lua_register_network_XEUDPSocket(tolua_S);
	lua_register_network_XEByteBuffer(tolua_S);
	lua_register_network_AESocketClient(tolua_S);
	lua_register_network_AEMessage(tolua_S);
	lua_register_network_XEMessage(tolua_S);
	lua_register_network_XETCPSocket(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

