//
//  AoneAndroidImagePicker.h
//  TZImagePickerController
//

#include <stdio.h>

typedef enum {
    AonePicker_Cancel = 0,
    AonePicker_Success,
    AonePicker_Fail
}AonePickerStatus;

typedef void (*AONEPICKER_CB)(AonePickerStatus code , void * info);

class AoneAndroidImagePicker
{
    
public:
    
    /**
     开始选择照片

     @param path 图片保存路径
     @param widthPixel 图片像素点的宽度, 传0则截取图片宽度同当前屏幕宽度相同
     @param heightPixel 图片像素点的高度,传0则截取图片高度与宽度相同
     @param cb 回调函数
     */

    void start(const char * path, int widthPixel, int heightPixel, AONEPICKER_CB cb);
	void AonePickerCallback(int code);
	AONEPICKER_CB m_cb;
	static AoneAndroidImagePicker* getInstance();
};
