/****************************************************************************
Copyright (c) 2013 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#ifndef __CCX_PROTOCOL_PLAYER_H__
#define __CCX_PROTOCOL_PLAYER_H__

#include "PluginProtocol.h"
#include <map>
#include <string>

namespace aonesdk { namespace plugin {

typedef std::map<std::string, std::string> TPlayerDeveloperInfo;

typedef enum
{
	PEC_Success = 0,
} PlayerErrorCode;

class ProtocolPlayer;
class PlayerActionListener
{
public:
	virtual void onPlayerStart(ProtocolPlayer* pPlugin) = 0;
	virtual void onPlayerStop(ProtocolPlayer* pPlugin, PlayerErrorCode code) = 0;
    virtual void onPlayerError(ProtocolPlayer* pPlugin,PlayerErrorCode code,void* uid)=0;
    virtual void onPlayerDidline(ProtocolPlayer* pPlugin,PlayerErrorCode code,void* uid)=0;
};

class ProtocolPlayer : public PluginProtocol
{
public:
	ProtocolPlayer();
	virtual ~ProtocolPlayer();

    /**
    @brief config the application info
    @param devInfo This parameter is the info of aplication,
           different plugin have different format
    @warning Must invoke this interface before other interfaces.
             And invoked only once.
    */
	void configDeveloperInfo(TPlayerDeveloperInfo devInfo);

	/**
	@param devInfo, keys : 
		param must set before create and can't change before close
			serverAddr
			streamName

		param may be set any time
			x,y,w,h - default to full screen
            hideVideo [yes/no]- hide the video ,default is no
			and other specific params
	@warning set params before capture and publish
	*/
	void setStreamParams(TPlayerDeveloperInfo devInfo);

	void start();

	void stop();

	void close();

	void playSoundEffect(const char* pcmDataPath, int len = 0);

	inline void setActionListener(PlayerActionListener* listener)
    {
        _listener = listener;
    }

	inline PlayerActionListener* getActionListener()
    {
        return _listener;
    }

protected:
	PlayerActionListener* _listener;
};

}} // namespace aonesdk { namespace plugin {

#endif /* __CCX_PROTOCOL_PLAYER_H__ */
