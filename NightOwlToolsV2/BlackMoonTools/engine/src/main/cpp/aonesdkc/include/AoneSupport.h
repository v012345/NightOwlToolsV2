//
//  AoneSupport.h
//  uuSdk
//
//  Created by 陈希 on 17/6/5.
//  Copyright © 2017年 Franklin. All rights reserved.
//

#ifndef AoneSupport_h
#define AoneSupport_h


#ifdef __cplusplus
extern "C" {
#endif

// 开启聊天
void AS_ShowConversation();
 
// 开启faq
void AS_ShowFAQs();

// 设置用户名
void AS_SetUserName(const char * userName);

// 设置用户Id
void AS_SetUserIdentifier(const char * userId);

#ifdef __cplusplus
}; // extern "C" end
#endif


#endif /* AoneSupport_h */


