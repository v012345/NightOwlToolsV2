//
//  BigPatcher.h
//  patcher
//
//  Created by 陈希 on 17/2/8.
//  Copyright © 2017年 wtyqm. All rights reserved.
//

#ifndef BigPatcher_h
#define BigPatcher_h

#include <stdio.h>

typedef unsigned int uint32;

extern "C"
{
    enum BPStage{
        BPStage_Init,
        BPStage_Query,
        BPStage_Download_Prepare,
        BPStage_Downloading,
        BPStage_Unzip,
        BPStage_Finish
    };
    
    enum BPErrorCode{
        BPErrorCode_NoErr,
        BPErrorCode_Net,
        BPErrorCode_Unzip
    };

    typedef void (*bp_progress_cb)(BPStage stage,double cur, double total);
    typedef void (*bp_finish_cb)();
    typedef void (*bp_error_cb)(BPErrorCode code,int innerCode, const char* msg);

    struct BigPatcher* big_patcher_get_default();
    
    /// 设置URL，下载的文件名为url的最后一个'/'之后的字符串
    void big_patcher_set_url(struct BigPatcher* bp,const char* url);
    
    /// 文件解压的位置, 默认位置为:`Library/Application Support/BPDefault/`
    void big_patcher_set_path(struct BigPatcher* bp,const char* path);

    /// 检查文件大小和是否支持分段下载
    void big_pathcher_get_size(struct BigPatcher* bp);
    
    void big_patcher_start(struct BigPatcher* bp);
    void big_patcher_cancel(struct BigPatcher* bp);

    void big_patcher_set_callback(struct BigPatcher* bp, bp_progress_cb progress_cb, bp_finish_cb finish_cb, bp_error_cb error_cb);
    void big_patcher_notify(struct BigPatcher* bp);
}


#endif /* BigPatcher_h */
