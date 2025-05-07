/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#ifndef __CCX_PROTOCOL_EXCEPTION_H__
#define __CCX_PROTOCOL_EXCEPTION_H__

#include "PluginProtocol.h"
#include <map>
#include <string>

namespace aonesdk { namespace plugin {
    
    typedef std::map<std::string, std::string> TExceptionDeveloperInfo;
    
    typedef enum
    {
        kCheckSucceed=0,
        kCheckFailed
    
    } ExceptionActionResultCode;
    
    
    class ProtocolException;
    class ExceptionActionListener
    {
    public:
        virtual void onActionResult(ProtocolException* pPlugin, ExceptionActionResultCode code, const char* msg) = 0;
    };
    
    
    class ProtocolException : public PluginProtocol
    {
    public:
        ProtocolException();
        virtual ~ProtocolException();
        
    public:
        void configDeveloperInfo(TExceptionDeveloperInfo devInfo);
        
        
        //上报异常日志
        //errorCode 错误码
        //msg 错误描述信息
        void reportException(const char* errorCode, const char* msg);
        
        //设置用户标识
        void setUserId(const char* userId);
        
        void log(int level,const char* message);
        
    public:
        inline void setActionListener(ExceptionActionListener* listener)
        {
            _listener = listener;
        }
        
        inline ExceptionActionListener* getActionListener()
        {
            return _listener;
        }
        
    protected:
        ExceptionActionListener* _listener;
    };
    
}} // namespace aonesdk { namespace plugin {

#endif /* __CCX_PROTOCOL_EXCEPTION_H__ */
