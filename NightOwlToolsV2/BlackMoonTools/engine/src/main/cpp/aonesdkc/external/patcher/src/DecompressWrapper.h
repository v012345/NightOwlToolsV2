//
//  DecompressWrapper.h
//  patcher
//
//  Created by 陈希 on 17/2/8.
//  Copyright © 2017年 wtyqm. All rights reserved.
//

#ifndef DecompressWrapper_h
#define DecompressWrapper_h

#include <stdio.h>
#include "unzip.h"
#include "zlib.h"
#include <string>

using namespace std;

class BPDecompress {
    
public:
    BPDecompress();
    ~BPDecompress();

    typedef void (*DecCompress)(unsigned long cur, unsigned long total);

    /**
     解压文件
     @param zipFilePath zip文件路径
     @param strFolder 解压后的文件路径
     @param decCompress 解压进度回调
     @return 0 解压成功， -1 解压失败
     */
    int BPDecompressZip(string zipFilePath, string strFolder, DecCompress decCompress);
    
private:
    int BPWriteData(unzFile &zipfile, const char *destPathName);
    int BPCreateDir(const char * path);
};

#endif /* DecompressWrapper_h */
