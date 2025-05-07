#ifndef __UUNETASYNC_ANDROID_HH__
#define __UUNETASYNC_ANDROID_HH__

#include "aonesdk.h"
#include <string>
#include <map>
using namespace std;

namespace aonesdk {

class uuNetAsync {
public:
	static void send_recv(AoneSDK* sdk, NET_CB cb, const string ip, int port, const string key, unsigned char* request, int requestLen);
	static void http_send_recv(AoneSDK* sdk, HTTP_CB cb, const string ip, int port, const string key, const char* request, int requestLen);

public:
	static AoneSDK* mSdk;
	static NET_CB mCb;
	static AoneSDK* mHttpSdk;
	static HTTP_CB mHttpCb;
	static map<string, string> mParams;

	static vector<AoneSDK*> mSdks;
	static vector<NET_CB> mCbs;
	static int g_netNumber;

	static vector<AoneSDK*> mHttpSdks;
	static vector<HTTP_CB> mHttpCbs;
	static int g_httpNumber;

	static map<int, map<string, string> > mParamsMap;
};
	

} // aonesdk
#endif // __UUNETASYNC_ANDROID_HH__