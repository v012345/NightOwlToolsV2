#ifndef __AONE_LIVE_STREAM_H__
#define __AONE_LIVE_STREAM_H__

#ifdef __cplusplus
extern "C" {
#endif

enum ALS_OPCode{
	ALS_Init = 0,
	ALS_CaptureStart,
	ALS_CaptureStop,
	ALS_PublishStart,
	ALS_PublishStop,
    ALS_PublishOffLine,
    ALS_PublishDidLine,
    ASL_VolumeIndication,
    ASL_VideoSteamStauts
};

enum ALS_Status{
	ALS_Success = 0,
	ALS_Error,
	ALS_CameraInitFail,
	ALS_MicInitFail,
	ALS_NetError
};

enum ALS_CameraType{
	ALS_CameraFront = 0,
	ALS_CameraBack
};

typedef void (*ALS_CallBack)(int opCode, int status, void* ud);

void ALS_SetParam(const char* key, const char* val);
void ALS_SetCallBack(ALS_CallBack cb, void* ud);
void ALS_StartCapture();
void ALS_StopCapture();
void ALS_StartPublish();
void ALS_StopPublish();
void ALS_Close();
int ALS_GetCameraCount();
int ALS_GetCurCameraType();
bool ALS_IsMicMute();
void ALS_SetMicMute(bool isMute);

// in android: len must assign a num (num is not zero)
void ALS_playSoundEffect(const char * pcmData, int len = 0);

#ifdef __cplusplus
};
#endif

#endif
