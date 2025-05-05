// 2011-08-29
// aone2_rc4_parser.h
// guosh
// aone2协议rc4加密编解码器


#ifndef _AONE2_RC4_CODEC_H_
#define _AONE2_RC4_CODEC_H_

#include "xcore_define.h"
#include "xcore_rc4.h"
#include "aone2_message.h"

namespace aone2
{

////////////////////////////////////////////////////////////////////////////////
// class Aone2RC4Parser
////////////////////////////////////////////////////////////////////////////////
class Aone2RC4Parser
{
public:
	Aone2RC4Parser(const string& key);

	~Aone2RC4Parser();

	bool need_bytes(void** ppBuff, uint32& len);

	bool put_bytes(const void* src, uint32 len, bool encrypt = true);

	Aone2BaseMessage* get_message();

	bool has_error();

	bool to_buffer(Aone2BaseMessage* msg, byte** ppBuff, uint32& len, bool encrypt = true);

	byte* get_buffer();
	uint32 get_buffern_len();
	uint32 get_buffer_readed_len();

	Aone2BaseMessage* get_message(byte* buffer, int len);

private:
	Aone2BaseMessage* create_message(Aone2CmdHeader_& header);

private:
	byte*               m_buffer;
	uint32               m_cmdLen;
	uint32               m_head_len;
	uint32				 m_buffer_len;
	uint32               m_request_seq;
	bool                 m_has_error;
	RC4                  m_input_rc4;  // 协议输入解密
	RC4                  m_output_rc4; // 协议输出加密
};

}//namespace aone2

using namespace aone2;

#endif//_AONE2_RC4_CODEC_H_
