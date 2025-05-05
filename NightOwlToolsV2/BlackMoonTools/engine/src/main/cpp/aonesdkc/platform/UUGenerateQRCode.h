//
//  UUGenerateQRCode.h
//  uuSdk
//
//  Created by 陈希 on 17/8/23.
//  Copyright © 2017年 Franklin. All rights reserved.
//

#ifndef UUGenerateQRCode_h
#define UUGenerateQRCode_h

#include <stdio.h>
#include <string>

using namespace std;

const char * generateQRCode(string srcImagePath, string content, int x, int y, int width, int height);

#endif /* UUGenerateQRCode_h */
