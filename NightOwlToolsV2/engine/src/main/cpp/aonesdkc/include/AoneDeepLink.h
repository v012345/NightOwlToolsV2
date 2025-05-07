#ifndef __AONE_DEEP_LINK_H__
#define __AONE_DEEP_LINK_H__
#include "string"

class AoneDeepLink{
public:
	AoneDeepLink();
	~AoneDeepLink();
	static std::string getDeepLinkData();
};

#endif