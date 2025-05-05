// 2011-08-29
// aone2_message.h
// guosh
// aone2消息定义

#ifndef _AONE2_DEFINE_H_
#define _AONE2_DEFINE_H_

#include "xcore_define.h"

namespace aone2
{

///////////////////////////////////////////////////////////////////////////////
// 命令号定义
///////////////////////////////////////////////////////////////////////////////
#define AONE2_CMD_UNKNOWN_REQ                  0       // 用于匹配未知请求命令
#define AONE2_CMD_UNKNOWN_RESP                 1       // 用于匹配未知回复命令
#define AONE2_CMD_BASE                         1000    // 起始号


///////////////////////////////////////////////////////////////////////////////
// 相关宏和结构定义
///////////////////////////////////////////////////////////////////////////////

#define AONE_MSG_MAX_LENGTH 1024*1024*2  // 最大消息大小定义

struct Aone2CmdHeader_
{
	uint32 m_cmdNum;  // 消息编号
	uint32 m_cmdSeq;  // 消息序列号(递增)
	uint32 m_reserve; // 保留字段
};

struct Aone2ReqHeader_ : public Aone2CmdHeader_
{
	uint32 m_srcId;   // 操作源ID
	uint32 m_session; // 会话ID
};

struct Aone2RespHeader_ : public Aone2CmdHeader_
{
	int32  m_result; // 回复结果码
};

}//namespace aone2

using namespace aone2;

#endif//_AONE2_DEFINE_H_
