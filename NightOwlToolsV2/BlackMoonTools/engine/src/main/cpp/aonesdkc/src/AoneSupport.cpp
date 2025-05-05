//
//  AoneSupport.cpp
//  uuSdk
//
//  Created by 陈希 on 17/6/5.
//  Copyright © 2017年 Franklin. All rights reserved.
//

#include "AoneSupport.h"
#include "uuSdkManager.h"
#include "uuSdkConfig.h"
#include "ProtocolSupport.h"
#include <vector>
#include <string>

extern "C" {
    
    static ProtocolSupport* _GetSuppportProtocol()
    {
        vector<string> allVoiceSdkNames = uuSdkConfig::getInstance()->getAllSdkNamesByType("support");
        if (allVoiceSdkNames.size() == 0)return NULL;
        if (allVoiceSdkNames.size() > 1)
        {
            uuLOG("only first voice sdk is valid");
        }
        
        uuSdkWrapper* wrap = uuSdkManager::getInstance()->getSdk(allVoiceSdkNames[0]);
        if (wrap != NULL)
        {
            return (ProtocolSupport *)wrap -> getSdk();
        }
        
        return NULL;
    }
    
    
    void AS_ShowConversation()
    {
        ProtocolSupport * p = _GetSuppportProtocol();
        if (p != NULL) {
            p -> showConversation();
        }
    }
    
    void AS_ShowFAQs()
    {
        ProtocolSupport * p = _GetSuppportProtocol();
        if (p != NULL) {
            p -> showFAQS();
        }
    }
    
    void AS_SetUserName(const char * userName)
    {
        ProtocolSupport * p = _GetSuppportProtocol();
        if (p != NULL) {
            p -> setUserName(userName);
        }
    }
    
    void AS_SetUserIdentifier(const char * userId)
    {
        ProtocolSupport * p = _GetSuppportProtocol();
        if (p != NULL) {
            p -> setUserIdentifier(userId);
        }
    }
    
}

