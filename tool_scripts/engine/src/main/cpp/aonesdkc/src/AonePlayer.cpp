#include "AonePlayer.h"
#include "uuSdkManager.h"
#include "uuSdkConfig.h"
#include "ProtocolPlayer.h"

extern "C"
{
	static AP_CallBack _apCB = NULL;
	static void* _cbUd = NULL;

	class AonePlayerListner : public PlayerActionListener
	{
		virtual void onPlayerStart(ProtocolPlayer* pPlugin)
		{
			if (_apCB != NULL)
				_apCB(AP_OPCode::AP_Start, AP_Status::AP_Success, _cbUd);
		}

		virtual void onPlayerStop(ProtocolPlayer* pPlugin, PlayerErrorCode code)
		{
			if (_apCB != NULL)
				_apCB(AP_OPCode::AP_Stop, code, _cbUd);
		}
        virtual void onPlayerError(ProtocolPlayer* pPlugin, PlayerErrorCode code,void * uid)
        {
            if (_apCB != NULL)
                _cbUd =uid;
                _apCB(AP_OPCode::AP_DidOff, code,_cbUd);
        }
        virtual void onPlayerDidline(ProtocolPlayer* pPlugin, PlayerErrorCode code,void * uid)
        {
            if (_apCB != NULL)
                _cbUd =uid;
            _apCB(AP_OPCode::AP_DidLine, code,_cbUd);
        }
	};

	static AonePlayerListner _protocolListener;

	static ProtocolPlayer* _GetPlayerProtocol()
	{
		vector<string> allPlayerSdkNames = uuSdkConfig::getInstance()->getAllSdkNamesByType("player");
		if (allPlayerSdkNames.size() == 0) return NULL;
		if (allPlayerSdkNames.size() > 1)
		{
			uuLOG("only first player sdk is valid");
		}
		
		uuSdkWrapper* wrap = uuSdkManager::getInstance()->getSdk(allPlayerSdkNames[0]);
		if (wrap != NULL)
			return (ProtocolPlayer*)wrap->getSdk();

		return NULL;
	}

	void AP_SetParam(const char* key, const char* val)
	{
		map<string, string> params;
		params.insert(make_pair(key, val));

		ProtocolPlayer* p = _GetPlayerProtocol();
		if (p != NULL)
			p->setStreamParams(params);
	}

	void AP_SetCallBack(AP_CallBack cb, void* ud)
	{
		ProtocolPlayer* p = _GetPlayerProtocol();
		if (p != NULL)
		{
			p->setActionListener(&_protocolListener);
			_apCB = cb;
			_cbUd = ud;
		}
	}

	void AP_StartPlay()
	{
		ProtocolPlayer* p = _GetPlayerProtocol();
		if (p != NULL)
			p->start();
	}

	void AP_StopPlay()
	{
		ProtocolPlayer* p = _GetPlayerProtocol();
		if (p != NULL)
			p->stop();
	}

	void AP_Close()
	{
		ProtocolPlayer* p = _GetPlayerProtocol();
		if (p != NULL)
			p->close();
	}

	void AP_PlaySoundEffect(const char * pcmData, int len)
	{
		ProtocolPlayer* p = _GetPlayerProtocol();
		if (p != NULL)
			p->playSoundEffect(pcmData, len);
	}
}

