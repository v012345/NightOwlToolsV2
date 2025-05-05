//
//  BigPatcher.cpp
//  patcher
//
//  Created by 陈希 on 17/2/8.
//  Copyright © 2017年 wtyqm. All rights reserved.
//

#include "BigPatcher.h"
#include "http_download.h"
#include "CPFileUtils.h"
#include "cplog.h"
#include <thread>
#include "unzip.h"
#include "DecompressWrapper.h"

#define BIG_PATCHER_TEMP "BPTemp"
#define BIG_PATCHER_DEFAULT  "BPDefault"

extern "C"
{
    static BigPatcher* bp_default = NULL;
    struct BigPatcher
    {
        BigPatcher()
        {
            m_download = NULL;
            progress_cb = NULL;
            finish_cb = NULL;
            error_cb = NULL;
            finish_notify_complete = false;
            error_notify_complete = false;
            error_code = BPErrorCode_NoErr;
            stage = BPStage_Init;
            inner_code = 0;
            now_download_size = 0.0;
            total_download_size = 0.0;
            now_unzip_num = 0;
            total_unzip_num = 0;
            error_msg = "";
            download_url = "";
            download_path = "";
            temp_download_path = "";
        }
        
        AonePatcher::HttpDownload *m_download;
        bp_progress_cb progress_cb;
        bp_finish_cb finish_cb;
        bp_error_cb error_cb;
        volatile bool finish_notify_complete;
        volatile bool error_notify_complete;
        volatile BPErrorCode error_code;
        volatile BPStage stage;
        volatile int inner_code;
        volatile double now_download_size;
        volatile double total_download_size;
        volatile unsigned long now_unzip_num; //当前已解压的数量
        volatile unsigned long total_unzip_num;//总共需要解压的数量
        string error_msg; ///< 错误信息
        string download_url; ///< 下载文件的URL
        string download_path; ///< 文件解压的地址
        string temp_download_path; ///< 文件下载的临时地址
    };
    
    struct BigPatcher* big_patcher_get_default()
    {
        if (bp_default == NULL)
        {
            bp_default = new BigPatcher();
            bp_default->m_download = new AonePatcher::HttpDownload();
        }
        return bp_default;
    }

    void big_patcher_reset(struct BigPatcher* bp)
    {

        CPUtils::CPFileUtils::remove(bp->temp_download_path.c_str());
        CPUtils::CPFileUtils::rmdir(bp->download_path.c_str());
        
        delete bp_default->m_download;
        delete bp_default;
        bp_default = NULL;
    }
    
    void big_patcher_get_file_name(const string& url,string& file_name)
    {
        if (url.empty())
        {
            CP_LOGE("big patcher get file name failed , url is nil !!");
            file_name = "";
            return;
        }
        
        auto pos = url.rfind('/');
        if(pos == string::npos){
            file_name = url;
        }else{
            file_name = url.substr(pos+1);
        }
    }
    
    bool big_patcher_check_url(struct BigPatcher* bp)
    {
        if (bp->download_url.empty())
        {
            CP_LOGE("big patcher download url is not exist !!!");
            return false;
        }
        bp->m_download->address(bp->download_url);
        return true;
    }
    
    bool big_patcher_check_path(struct BigPatcher* bp)
    {
        string writeable_path = CPUtils::CPFileUtils::getWriteablePath();
        string temp_folder = CPUtils::CPFileUtils::concateFilePath(writeable_path, BIG_PATCHER_TEMP);
        
        // 创建临时文件夹
        if (!CPUtils::CPFileUtils::exist(temp_folder.c_str()) && !CPUtils::CPFileUtils::mkdir(temp_folder.c_str()))
        {
            CP_LOGE("can't create temp folder = %s\n",temp_folder.c_str());
            return false;
        }
        
        string zip_file_name;
        big_patcher_get_file_name(bp->download_url,zip_file_name);
        string zip_download_path = CPUtils::CPFileUtils::concateFilePath(temp_folder, zip_file_name);
        
        bp->m_download->locpath(zip_download_path); //下载路径为临时文件夹路径
        bp->temp_download_path = zip_download_path;
        
        if (bp->download_path.empty())
        {
            // 使用默认的路径
            CP_LOGI("big patcher download path is not exist , use the default path !!");
            string real_folder = CPUtils::CPFileUtils::concateFilePath(writeable_path, BIG_PATCHER_DEFAULT);
            if (!CPUtils::CPFileUtils::exist(real_folder.c_str()) && !CPUtils::CPFileUtils::mkdir(real_folder.c_str()))
            {
                CP_LOGE("can't create default folder = %s",real_folder.c_str());
                return false;
            }
            bp->download_path = real_folder;
        }

        return true;
    }

    void big_patcher_set_url(struct BigPatcher* bp,const char* url)
    {
        bp->download_url = url;
    }
    
    void big_patcher_set_path(struct BigPatcher* bp, const char* path)
    {
        bp->download_path = path;
    }

    void big_pathcher_get_size(struct BigPatcher* bp)
    {
        if (!big_patcher_check_url(bp)) {
            return;
        }
        if (!big_patcher_check_path(bp)) {
            return;
        }
        bp->m_download->queryDownloadInfo();
        bp->now_download_size = bp->m_download->getCurrentSize();
        bp->total_download_size = bp->m_download->getTotalSize();
        bp->stage = BPStage_Query;
    }

    void dec_process_(unsigned long cur,unsigned long total){
        BigPatcher * bp = big_patcher_get_default();
        bp->now_unzip_num = cur;
        bp->total_unzip_num = total;
    }
    int bp_decompress_zip(struct BigPatcher *bp)
    {
        bp->stage = BPStage_Unzip;
        // 解压
        auto *decompress = new BPDecompress();
        CP_LOGI("bp_decompress_zip %s to %s\n",bp->temp_download_path.c_str(),bp->download_path.c_str());

        int result = decompress->BPDecompressZip(bp->temp_download_path,bp->download_path,dec_process_);
        bp->inner_code = result;
        
        if (result == 0)
        {
            CPUtils::CPFileUtils::remove(bp->temp_download_path.c_str());
            bp->stage = BPStage_Finish;
            CP_LOGI("bp_decompress_zip success\n");
        }else
        {
            bp->error_code = BPErrorCode_Unzip;
            bp->error_msg = "bp_decompress_zip " + bp->temp_download_path + " to " + bp->download_path + " failed!!!";
            CP_LOGE("%s\n",  bp->error_msg.c_str());
        }
        
        delete decompress;
        return result;
    }
    
    void bp_downloadCallback_(double cur, double total)
    {
        if (cur == 0 || total == 0) return;
        
        BigPatcher * bp = big_patcher_get_default();
        bp->now_download_size = cur;
        bp->total_download_size = total;
        
        if (cur <= total)
        {
            bp->error_code = BPErrorCode_NoErr;
            bp->stage = BPStage_Downloading;
        }
    }
    
    void * bp_download_(void *argvs)
    {
        auto *bp = (BigPatcher *)argvs;
        
        bp->m_download->address(bp->download_url);
        bp->m_download->notify(bp_downloadCallback_);

        int result  = bp->m_download->download();
        
        if (result != 0) {
            bp->inner_code = result;
            bp->error_code = BPErrorCode_Net;
        }
        
        switch (result) {
            case 1:
                bp->error_msg = "can not open local file.";
                break;
            case 2:
                bp->error_msg = "can not init url.";
                break;
            case 3:
                bp->error_msg = "downloaded file size is zero.";
                break;
            case 4:
                bp->error_msg = "path error.";
                break;
            case 5:
                bp->error_msg = "net work error.";
                break;
            case 6:
                bp->error_msg = "not support break point.";
                break;
            default:
                if (bp->now_download_size > 0 && bp->total_download_size > 0 && bp->total_download_size <= bp->now_download_size)
                {
                    // 解压
                    bp_decompress_zip(bp);
                }
                break;
        }
        return NULL;
    }
    
    void big_patcher_start(struct BigPatcher* bp)
    {
        if (!big_patcher_check_url(bp)) return;
        if (!big_patcher_check_path(bp)) return;

        bp->stage = BPStage_Download_Prepare;
        thread tid(bp_download_,bp);
        tid.detach();
    }
    
    void big_patcher_cancel(struct BigPatcher* bp)
    {
        bp->m_download->interrupt();
    }
    
    void big_patcher_set_callback(struct BigPatcher* bp, bp_progress_cb progress_cb, bp_finish_cb finish_cb, bp_error_cb error_cb)
    {
        if (progress_cb != NULL)
        {
            bp->progress_cb = progress_cb;
        }
        
        if (finish_cb != NULL)
        {
            bp->finish_cb = finish_cb;
        }
        
        if (error_cb != NULL)
        {
            bp->error_cb = error_cb;
        }
    }
    
    void big_patcher_notify(struct BigPatcher* bp)
    {
        if (bp->finish_notify_complete || bp->error_notify_complete) return;
        if (bp->error_code != BPErrorCode_NoErr)
        {
            // error
            if (bp->error_cb != NULL)
            {
                CP_LOGW("big_patcher_notify error_code:%d inner_code:%d error_msg:%s",bp->error_code,bp->inner_code,bp->error_msg.c_str());
                bp->error_cb(bp->error_code,bp->inner_code,bp->error_msg.c_str());
                bp->error_notify_complete = true;
            }
            big_patcher_reset(bp);
            CP_LOGW("big_patcher_notify reset");
        }else{
            switch (bp->stage) {
                case BPStage_Init:
                    break;
                case BPStage_Query:
                    if (bp->progress_cb != NULL){
                        bp->progress_cb(bp->stage, bp->now_download_size, bp->total_download_size);
                    }
                    break;
                case BPStage_Download_Prepare:
                    break;
                case BPStage_Downloading:
                    if (bp->total_download_size > bp->now_download_size && bp->now_download_size > 0) {
                        if (bp->progress_cb != NULL) {
                            bp->progress_cb(bp->stage, bp->now_download_size, bp->total_download_size);
                        }
                    }
                    break;
                case BPStage_Unzip:
                    if (bp->total_unzip_num > bp->now_unzip_num && bp->now_unzip_num > 0) {
                        if (bp->progress_cb != NULL) {
                            bp->progress_cb(bp->stage, bp->now_unzip_num, bp->total_unzip_num);
                        }
                    }
                    break;
                case BPStage_Finish:
                    if (bp->finish_cb != NULL){
                        bp->finish_cb();
                        bp->finish_notify_complete = true;
                    }
                    break;
            }
        }
    }
}
