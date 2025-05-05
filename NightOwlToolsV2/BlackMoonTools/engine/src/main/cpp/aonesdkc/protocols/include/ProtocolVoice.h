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
#ifndef __CCX_PROTOCOL_VOICE_H__
#define __CCX_PROTOCOL_VOICE_H__

#include "PluginProtocol.h"
#include <map>
#include <string>

namespace aonesdk { namespace plugin {

typedef std::map<std::string, std::string> TVoiceDeveloperInfo;

typedef enum
{
	VEC_Success = 0,
} VoiceErrorCode;

typedef enum
{
	VET_INIT,
	VET_START,
	VET_STOP
}VocieEventType;

class ProtocolVoice;
class VoiceActionListener
{
public:
	virtual void onVoiceEvent(ProtocolVoice* pPlugin, VocieEventType type, VoiceErrorCode errCode) = 0;
};

class ProtocolVoice : public PluginProtocol
{
public:
	ProtocolVoice();
	virtual ~ProtocolVoice();

    /**
    @brief config the application info
    @param devInfo This parameter is the info of aplication,
           different plugin have different format
    @warning Must invoke this interface before other interfaces.
             And invoked only once.
    */
	void configDeveloperInfo(TVoiceDeveloperInfo devInfo);

	/**
	@param devInfo, keys : 
		param must set before create and can't change before close
			userName : must use number
			password
			roomId : must use number
			ipAddr
			port : must use number

	@warning set params before init
	*/
	void setVoiceParams(TVoiceDeveloperInfo devInfo);

	void start(const char* roomId);

	void stop();

	bool isMicMute();

	void setMicMute(bool mute);

	int getVolume();

	void setVolume(int volume);

	inline void setActionListener(VoiceActionListener* listener)
    {
        _listener = listener;
    }

	inline VoiceActionListener* getActionListener()
    {
        return _listener;
    }

protected:
	VoiceActionListener* _listener;
};

}} // namespace aonesdk { namespace plugin {

#endif /* __CCX_PROTOCOL_VOICE_H__ */
