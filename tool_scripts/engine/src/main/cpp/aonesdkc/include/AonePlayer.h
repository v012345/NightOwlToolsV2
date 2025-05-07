#ifndef __AONE_PLAYER_H__
#define __AONE_PLAYER_H__

#ifdef __cplusplus
extern "C" {
#endif

enum AP_OPCode{
	AP_Start = 0,
	AP_Stop,
    AP_DidOff,
    AP_DidLine
};

enum AP_Status{
	AP_Success = 0,
	AP_Error,
	AP_NetError
};

typedef void (*AP_CallBack)(int opCode, int status, void* ud);

void AP_SetParam(const char* key, const char* val);
void AP_SetCallBack(AP_CallBack cb, void* ud);
void AP_StartPlay();
void AP_StopPlay();
void AP_Close();

// in android: len must assign a num (num is not zero)
void AP_PlaySoundEffect(const char * pcmData, int len = 0);

#ifdef __cplusplus
};
#endif

#endif
