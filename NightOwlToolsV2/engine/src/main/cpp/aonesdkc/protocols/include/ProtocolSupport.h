//
//  ProtocolSupport.h
//  PluginProtocol
//
//  Created by 陈希 on 17/6/5.
//  Copyright © 2017年 zhangbin. All rights reserved.
//

#ifndef ProtocolSupport_h
#define ProtocolSupport_h

#include <stdio.h>
#include "PluginProtocol.h"
#include <map>
#include <string>

namespace aonesdk { namespace plugin {
   
typedef std::map<std::string, std::string> TSupportDeveloperInfo;
    
class ProtocolSupport : public PluginProtocol {
    
public:
    ProtocolSupport();
    virtual ~ProtocolSupport();
    
    /**
     @brief config the application info
     @param devInfo This parameter is the info of aplication,
     different plugin have different format
     @warning Must invoke this interface before other interfaces.
     And invoked only once.
     */
    void configDeveloperInfo(TSupportDeveloperInfo devInfo);
    
    void showConversation();
    
    void showFAQS();
    
    void setUserName(const char * userName);
    
    void setUserIdentifier(const char * userId);
    
};
    
} // nameplace plugin end
} // nameplace aonesdk end

#endif /* ProtocolSupport_h */
