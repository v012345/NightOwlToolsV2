#ifndef __AONE_VOICE_H_
#define __AONE_VOICE_H_

#ifdef __cplusplus
extern "C" {
#endif

enum AVEC_Type
{
	AVEC_Init = 0,
	AVEC_StartVoice,
	AVEC_StopVoice
};

enum AVEC_Code
{
	AVEC_Success = 0,
	AVEC_Failed,
};

typedef void (*AVEC_Callback)(int type, int code, void* ud);

void AVEC_SetCallback(AVEC_Callback cb, void* ud);
void AVEC_SetVoiceParams(const char* key, const char* value);
void AVEC_Start(const char* roomId);
void AVEC_Stop();
bool AVEC_IsMicMute();
void AVEC_SetMicMute(bool mute);
int AVEC_GetVolume();
void AVEC_SetVolume(int volume);

#ifdef __cplusplus
};
#endif

#endif // __AONE_VOICE_H_