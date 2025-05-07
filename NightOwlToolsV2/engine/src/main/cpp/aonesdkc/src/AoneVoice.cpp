#include "AoneVoice.h"
#include "ProtocolVoice.h"
#include "uuSdkManager.h"
#include "uuSdkConfig.h"

extern "C"
{
	static AVEC_Callback _avecCB = NULL;
	static void* _cbUd = NULL;

	class AoneVoiceListener : public VoiceActionListener
	{
		virtual void onVoiceEvent(ProtocolVoice* pPlugin, VocieEventType type, VoiceErrorCode errCode)
		{
			if (_avecCB != NULL)
			{
				_avecCB(type, errCode, _cbUd);
			}
		}
	};

	static AoneVoiceListener _protocolListener;

	static ProtocolVoice* _GetVoiceProtocol()
	{
		vector<string> allVoiceSdkNames = uuSdkConfig::getInstance()->getAllSdkNamesByType("voice");
		if (allVoiceSdkNames.size() == 0) return NULL;
		if (allVoiceSdkNames.size() > 1)
		{
			uuLOG("only first voice sdk is valid");
		}
		uuSdkWrapper* wrap = uuSdkManager::getInstance()->getSdk(allVoiceSdkNames[0]);
		if (wrap != NULL)
		{
			return (ProtocolVoice*)wrap->getSdk();
		}
		return NULL;
	}

	void AVEC_SetCallback(AVEC_Callback cb, void* ud)
	{
		ProtocolVoice* p = _GetVoiceProtocol();
		if (p != NULL)
		{
			p->setActionListener(&_protocolListener);
			_avecCB = cb;
			_cbUd = ud;
		}
	}

	void AVEC_SetVoiceParams(const char* key, const char* value)
	{
		map<string, string> params;
		params.insert(make_pair(key, value));

		ProtocolVoice* p = _GetVoiceProtocol();
		if (p != NULL)
			p->setVoiceParams(params);
	}

	void AVEC_Start(const char* roomId)
	{
		ProtocolVoice* p = _GetVoiceProtocol();
		if (p != NULL)
			p->start(roomId);
	}

	void AVEC_Stop()
	{
		ProtocolVoice* p = _GetVoiceProtocol();
		if (p != NULL)
			p->stop();
	}

	bool AVEC_IsMicMute()
	{
		ProtocolVoice* p = _GetVoiceProtocol();
		if (p != NULL)
			return p->isMicMute();

		return false;
	}

	void AVEC_SetMicMute(bool mute)
	{
		ProtocolVoice* p = _GetVoiceProtocol();
		if (p != NULL)
			p->setMicMute(mute);
	}

	int AVEC_GetVolume()
	{
		ProtocolVoice* p = _GetVoiceProtocol();
		if (p != NULL)
			return p->getVolume();

		return 0;
	}

	void AVEC_SetVolume(int volume)
	{
		ProtocolVoice* p = _GetVoiceProtocol();
		if (p != NULL)
			p->setVolume(volume);
	}
}

