#ifndef __CCX_PROTOCOL_PUSH_H__
#define __CCX_PROTOCOL_PUSH_H__

#include "PluginProtocol.h"
#include <map>
#include <string>

namespace aonesdk { namespace plugin {

typedef std::map<std::string, std::string> TUserDeveloperInfo;

typedef enum
{
    kPushSuccess = 0,
    kPushFailed,
	kPushTimeout
} PushResultCode;

class ProtocolPush;
class PushResultListener
{
public:
    virtual void onPushResult(ProtocolPush* pPlugin, PushResultCode code, const char* msg) = 0;
};

class ProtocolPush : public PluginProtocol
{
public:
    ProtocolPush();
    virtual ~ProtocolPush();

    void configDeveloperInfo(TUserDeveloperInfo devInfo);

    void startPush();

    void closePush();

    void setAlias(const char *alias);

    void delAlias(const char *alias);

    void setTags(const char *tags);

    void delTags(const char *tags);

    inline void setPushResultListener(PushResultListener* listener)
    {
        _listener = listener;
    }

    inline PushResultListener* getPushResultListener()
    {
        return _listener;
    }

protected:
    PushResultListener* _listener;
};

}} // namespace aonesdk { namespace plugin {

#endif /* __CCX_PROTOCOL_PUSH_H__ */
