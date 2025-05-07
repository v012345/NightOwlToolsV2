//
//  DecompressWrapper.cpp
//  patcher
//
//  Created by 陈希 on 17/2/8.
//  Copyright © 2017年 wtyqm. All rights reserved.
//

#include "DecompressWrapper.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include "CPFileUtils.h"
#include "cplog.h"
#include <errno.h>

BPDecompress::BPDecompress()
{
    
}

BPDecompress::~BPDecompress()
{
    
}

int BPDecompress::BPWriteData(unzFile &zipfile, const char *destPathName)
{
    if (destPathName == NULL) {
        CP_LOGE("name is null\n");
        return -1;
    }
    
    if (BPCreateDir(destPathName) != 0)
    {
        CP_LOGE("[BPWriteData] create dir failed . path = %s\n",destPathName);
        return -1;
    }
    
    // 删除以.开头的文件
    string s_path = destPathName;
    auto index = s_path.rfind('/');
    string name = s_path.substr(index+1);
    if (name.find('.') == 0) {
//        CP_LOGE("name = %s\n",name.c_str());
        return 0;
    }
    
    FILE *fp = fopen(destPathName, "wb");
    if (fp == NULL) {
		CP_LOGE("open file failed. destPathName = %s errno = %s\n", destPathName, strerror(errno));
        return -1;
    }
    
    char read_buffer[1024] = {0};
    int error = UNZ_OK;
    
    do {
        error = unzReadCurrentFile(zipfile, read_buffer, 1024);
        
        if (error < 0 )
        {
            CP_LOGE("[unzReadCurrentFile] error = %d\n", error);
            return -1;
        }
        
        if (error > 0) {
            fwrite(read_buffer, error, 1, fp);
        }
        
    } while (error > 0);
    
    fclose(fp);
    fp = NULL;
    
    return 0;
}

int BPDecompress::BPCreateDir(const char *path)
{
    string s_path = path;
    
    if (s_path.find("__MACOSX") != string::npos || s_path.find(".DS_Store") != string::npos ) {
        return 0;
    }

    auto last_sep_index = s_path.rfind('/');
    if (last_sep_index != s_path.length()-1) { // this is file
        auto folder_path = s_path.substr(0,last_sep_index);
        if(!CPUtils::CPFileUtils::exist(folder_path.c_str())){
            CPUtils::CPFileUtils::mkdir(folder_path.c_str());
        }
        return 0;
    }
    
    
    if (CPUtils::CPFileUtils::mkdir(path)) {
        return 0;
    }else{
        CP_LOGE("mkdir failed. path = %s\n",path);
        return -1;
    }
    
}

int BPDecompress::BPDecompressZip(string zipFilePath, string strFolder, DecCompress decCompress)
{
    // 1. open zip
    unzFile zipfile = unzOpen(zipFilePath.c_str());
    if (zipfile == NULL)
    {
        CP_LOGE("open zip failed , path = %s",zipFilePath.c_str());
        return -1;
    }
    
    // 2. get global info
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info)!=UNZ_OK){
        unzClose(zipfile);
        CP_LOGE("get global info failed\n");
        return -1;
    }
    if(decCompress != NULL){
        decCompress(0,global_info.number_entry);
    }
    // 3. loop files
    for (uLong i = 0; i<global_info.number_entry; ++i){
        
        unz_file_info64 file_info64;
        char filename[1024]={0};
        
        unzGetCurrentFileInfo64(zipfile, &file_info64, filename, sizeof(filename), NULL, 0, NULL, 0);
        
        const size_t filename_length = strlen(filename);
        char name[1024] = {0};
        if (filename[filename_length-1] == '/')
        {
            // make folder
            sprintf(name, "%s/%s", strFolder.c_str(), filename);
            BPCreateDir(name);
        }else{
            
            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                unzClose(zipfile);
                CP_LOGE("open current file failed\n");
                return -1;
            }
            
            sprintf(name, "%s/%s", strFolder.c_str(), filename);
            
            if (BPWriteData(zipfile, name) < 0) {
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                CP_LOGE("wtite data into file %s failed\n",filename);
                return -1;
            }
            // to set file time
        }
        unzCloseCurrentFile(zipfile);
        
        if ((i+1) < global_info.number_entry && unzGoToNextFile(zipfile) != UNZ_OK) {
            unzClose(zipfile);
            CP_LOGE("go to next failed\n");
            return -1;
        }
        if(decCompress != NULL){
            decCompress(i+1,global_info.number_entry);
        }
    }
    
    unzClose(zipfile);

    return 0;
}
