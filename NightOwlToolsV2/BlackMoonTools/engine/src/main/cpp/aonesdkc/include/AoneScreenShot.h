#ifndef __AONE_SCREEN_SHOOT_H__
#define __AONE_SCREEN_SHOOT_H__


typedef void(*AoneScreenShot_CB)(int code, void * info);

class AoneScreenShot{
public:
	AoneScreenShot();
	~AoneScreenShot();
	static void takeScreenShot(const char* path);
	static void callFuncWithStringParam(const char* path);
    
    
    /**
     下载图片

     @param path 图片保存路径
     @param url 下载地址
     @param cb 回调函数: code = 0, 下载成功 ; code = -1, 下载失败
     */
    static void downloadImage(const char * path , const char * url, AoneScreenShot_CB cb);
};

#endif
