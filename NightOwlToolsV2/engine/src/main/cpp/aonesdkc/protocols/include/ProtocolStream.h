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
#ifndef __CCX_PROTOCOL_STREAM_H__
#define __CCX_PROTOCOL_STREAM_H__

#include "PluginProtocol.h"
#include <map>
#include <string>

namespace aonesdk { namespace plugin {

typedef std::map<std::string, std::string> TStreamDeveloperInfo;

typedef enum
{
    SCT_Front = 0,
	SCT_Back
} StreamCameraType;

typedef enum
{
	SEC_Success = 0,
    SEC_VideoStop,
    SEC_VideoResume
} StreamErrorCode;

class ProtocolStream;
class StreamActionListener
{
public:
	virtual void onInitCameraFail(ProtocolStream* pPlugin) = 0;
	virtual void onInitMicrophoneFail(ProtocolStream* pPlugin) = 0;
	virtual void onCaptureStart(ProtocolStream* pPlugin) = 0;
	virtual void onCaptureStop(ProtocolStream* pPlugin) = 0;
	virtual void onPublishStart(ProtocolStream* pPlugin, void * info) = 0;
	virtual void onPublishStop(ProtocolStream* pPlugin, StreamErrorCode code) = 0;
    virtual void onPublishDidOffLine(ProtocolStream* pPlugin, StreamErrorCode code, void* userInfo) =0;
    virtual void onPublishDidLine(ProtocolStream* pPlugin, StreamErrorCode code ,void* userInfo) = 0;
    virtual void onVolumeIndication(ProtocolStream * pPlugin, void * info) = 0;
    virtual void onVideoStreamStatus(ProtocolStream * pPlugin, StreamErrorCode code , void * info) = 0;
};

class ProtocolStream : public PluginProtocol
{
public:
	ProtocolStream();
	virtual ~ProtocolStream();

    /**
    @brief config the application info
    @param devInfo This parameter is the info of aplication,
           different plugin have different format
    @warning Must invoke this interface before other interfaces.
             And invoked only once.
    */
	void configDeveloperInfo(TStreamDeveloperInfo devInfo);

	/**
	@param devInfo, keys : 
		param must set before create and can't change before close
			serverAddr
			streamName
			userId
			password

		param may be set any time
			camera[front/back]
			beauty[0 - 100] defaule is 0
			addFilter[filterNormal/overlayLogo/overlaySubtitle/filterSkinSmooth]
            removeFilter[filterNormal/overlayLogo/overlaySubtitle/filterSkinSmooth]
            videoQuality[smooth/hd]
            screen[undefined/portraitHalf/portraitFull/landscapeFull]
     
			x,y,w,h -- default to full screen
			and other specific params
	@warning set params before create, capture and publish
	*/

	void setStreamParams(TStreamDeveloperInfo devInfo);

	void startCapture();

	void stopCapture();

	void startPublish();

	void stopPublish();

	void close();

	int getCameraCount();

	StreamCameraType getCurCameraType();

	bool isMicMute();

	void setMicMute(bool isMute);

    void playSoundEffect(const char* pcmDataPath, int len = 0);
    
	inline void setActionListener(StreamActionListener* listener)
    {
        _listener = listener;
    }

	inline StreamActionListener* getActionListener()
    {
        return _listener;
    }

protected:
	StreamActionListener* _listener;
};

}} // namespace aonesdk { namespace plugin {

#endif /* __CCX_PROTOCOL_STREAM_H__ */
