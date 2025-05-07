#include "stdc++.h"
#include "cocos_engine.h"
#include "json_inc.h"

#include "channel_dispatch_lua.h"
#include "channel_dispatcher.h"

using namespace std;
static ChannelDispatchCenter* ins = nullptr;
void ChannelDispatchCenter::togame(std::string str)
{
	Json::Value j;
	Json::Reader r;

	if (false == r.parse(str, j))
		return;

	if (script()){
		call_luafunc(str.c_str());
	}
	else {
		string id = j["id"].asCString();
		cache(id, str);
	}
}

void ChannelDispatchCenter::tochannel(std::string str)
{
	call_channel(str.c_str());
}

bool ChannelDispatchCenter::script(void)
{
	return ScriptEngineManager::getInstance()->getScriptEngine() != nullptr;
}

void ChannelDispatchCenter::cache(std::string& id, std::string& val)
{
	cache_[id] = val;
}

std::string ChannelDispatchCenter::query(std::string key)
{
	if (cache_.find(key) == cache_.end())
		return "";

	return cache_[key];
}


ChannelDispatchCenter* ChannelDispatchCenter::inst(void)
{
	if (ins == nullptr)
		ins = new ChannelDispatchCenter();
	return ins;
}

void ChannelDispatchCenter::drop(void)
{
	if (ins != nullptr)
		delete ins;
	ins = nullptr;
}

ChannelDispatchCenter::ChannelDispatchCenter(void)
{
	cache_.clear();
}

ChannelDispatchCenter::~ChannelDispatchCenter(void)
{
	cache_.clear();
}

int QueryChannelInfo(lua_State *l)
{
	/*
	Json::Value v;
	Json::Value c;
	c["id"] = 2;
	c["name"] = "hehe";
	v["channel"] = c;
	const char* id = lua_tostring(l, -1);
	string out = v.toStyledString();
	lua_pushstring(l, out.c_str());
	*/

	const char* id = lua_tostring(l, -1);
	log(" -- Query Channel Info id = %s", id);

	string out;
	if (ins == nullptr) {
		lua_pushnil(l);
	} else {
        string out = ins->query(id);
        if (out == "") {
            lua_pushnil(l);
        }
        else
        {
            lua_pushstring(l, out.c_str());
        }
	}

	return 1;
}
