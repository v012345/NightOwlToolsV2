#include "AoneLiveStream.h"
#include "uuSdkManager.h"
#include "ProtocolStream.h"
#include "uuSdkConfig.h"

extern "C"
{
	static ALS_CallBack _alsCB = NULL;
	static void * _cbUd = NULL;

	class AoneStreamListener : public StreamActionListener
	{
		virtual void onInitCameraFail(ProtocolStream* pPlugin)
		{
			if (_alsCB != NULL)
			{
				_alsCB(ALS_Init, ALS_CameraInitFail, _cbUd);
			}
		}

		virtual void onInitMicrophoneFail(ProtocolStream* pPlugin)
		{
			if (_alsCB != NULL)
			{
				_alsCB(ALS_Init, ALS_MicInitFail, _cbUd);
			}
		}

		virtual void onCaptureStart(ProtocolStream* pPlugin)
		{
			if (_alsCB != NULL)
			{
				_alsCB(ALS_CaptureStart, ALS_Success, _cbUd);
			}
		}

		virtual void onCaptureStop(ProtocolStream* pPlugin)
		{
			if (_alsCB != NULL)
			{
				_alsCB(ALS_CaptureStop, ALS_Success, _cbUd);
			}
		}

		virtual void onPublishStart(ProtocolStream* pPlugin, void * info)
		{
			if (_alsCB != NULL)
			{
				_alsCB(ALS_PublishStart, ALS_Success, info);
			}
		}

		virtual void onPublishStop(ProtocolStream* pPlugin, StreamErrorCode code)
		{
			if (_alsCB != NULL)
			{
				_alsCB(ALS_PublishStop, code, _cbUd);
			}
		}
        
        virtual void onPublishDidOffLine(ProtocolStream* pPlugin, StreamErrorCode code ,void* userInfo)
        {
            if (_alsCB != NULL)
            {
                _cbUd = userInfo;
                _alsCB(ALS_PublishOffLine, code, _cbUd);
            }
        }
        
        virtual void onPublishDidLine(ProtocolStream* pPlugin, StreamErrorCode code ,void* userInfo)
        {
            if (_alsCB != NULL)
            {
                _cbUd = userInfo;
                _alsCB(ALS_PublishDidLine, code, _cbUd);
            }
        }

        virtual void onVolumeIndication(ProtocolStream * pPlugin, void * info)
        {
            if (_alsCB != NULL)
            {
                _alsCB(ASL_VolumeIndication, SEC_Success, info);
            }
        }
        
        virtual void onVideoStreamStatus(ProtocolStream * pPlugin, StreamErrorCode code , void * info)
        {
            if (_alsCB != NULL)
            {
                _alsCB(ASL_VideoSteamStauts, code , info);
            }
        }
	};

	static AoneStreamListener _protocolListener;

	static ProtocolStream* _GetStreamProtocol()
    
	{
		vector<string> allStreamSdkNames = uuSdkConfig::getInstance()->getAllSdkNamesByType("stream");
		if (allStreamSdkNames.size() == 0) return NULL;
		if (allStreamSdkNames.size() > 1)
		{
			uuLOG("only first stream sdk is valid");
		}
		uuSdkWrapper* wrap = uuSdkManager::getInstance()->getSdk(allStreamSdkNames[0]);
		if (wrap != NULL)
		{
			return (ProtocolStream*)wrap->getSdk();
		}
		return NULL;
	}

	void ALS_SetCallBack(ALS_CallBack cb, void* ud)
	{
		ProtocolStream* p = _GetStreamProtocol();
		if (p != NULL)
		{
			p->setActionListener(&_protocolListener);
			_alsCB = cb;
			_cbUd = ud;
		}
	}

	void ALS_SetParam(const char* key, const char* val)
	{
		map<string, string> params;
		params.insert(make_pair(key, val));

		ProtocolStream* p = _GetStreamProtocol();
		if (p != NULL)
			p->setStreamParams(params);
	}

	void ALS_StartCapture()
	{
		ProtocolStream* p = _GetStreamProtocol();
		if (p != NULL)
			p->startCapture();
	}

	void ALS_StopCapture()
	{
		ProtocolStream* p = _GetStreamProtocol();
		if (p != NULL)
			p->stopCapture();
	}

	void ALS_StartPublish()
	{
		ProtocolStream* p = _GetStreamProtocol();
		if (p != NULL)
			p->startPublish();
	}

	void ALS_StopPublish()
	{
		ProtocolStream* p = _GetStreamProtocol();
		if (p != NULL)
			p->stopPublish();
	}

	void ALS_Close()
	{
		ProtocolStream* p = _GetStreamProtocol();
		if (p != NULL)
			p->close();
	}

	int ALS_GetCameraCount()
	{
		ProtocolStream* p = _GetStreamProtocol();
		if (p != NULL)
			return p->getCameraCount();

		return 0;
	}

	int ALS_GetCurCameraType()
	{
		ProtocolStream* p = _GetStreamProtocol();
		if (p != NULL)
			return p->getCurCameraType();

		return 0;
	}

	bool ALS_IsMicMute()
	{
		ProtocolStream* p = _GetStreamProtocol();
		if (p != NULL)
			return p->isMicMute();

		return false;
	}

	void ALS_SetMicMute(bool isMute)
	{
		ProtocolStream* p = _GetStreamProtocol();
		if (p != NULL)
			p->setMicMute(isMute);
	}
    
    void ALS_playSoundEffect(const char * pcmDataPath, int len)
    {
        ProtocolStream *p = _GetStreamProtocol();
        if (p != NULL) {
            p->playSoundEffect(pcmDataPath, len);
        }
    }
}

