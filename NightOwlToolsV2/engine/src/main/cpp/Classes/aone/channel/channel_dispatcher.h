#ifndef ___channel_dispatch_center_h___
#define ___channel_dispatch_center_h___

#include "lua.h"
#include <map>

class ChannelDispatchCenter
{
protected:
	ChannelDispatchCenter(void);
	~ChannelDispatchCenter(void);

public:
	static ChannelDispatchCenter*
		inst(void);
	static void
		drop(void);


public:

	void togame(std::string str);
	void tochannel(std::string str);
    std::string query(std::string key);


protected:
	bool script(void);
	void cache(std::string& id, std::string& val);

	std::map<std::string, std::string> cache_;

	ChannelDispatchCenter (const ChannelDispatchCenter& );
	void operator= (const ChannelDispatchCenter & );
	
};

int QueryChannelInfo(lua_State *l);

#endif
