// 2011-08-29
// aone2_rc4_parser.cpp
// guosh
// aone2协议rc4加密编解码器

#include "aone2_rc4_parser.h"
#include "uuSdkDef.h"
#include <typeinfo>

namespace aone2
{

///////////////////////////////////////////////////////////////////////////////
// class Aone2RC4Parser
///////////////////////////////////////////////////////////////////////////////
Aone2RC4Parser::Aone2RC4Parser(const string& key)
	: m_buffer(NULL)
	, m_cmdLen(0)
	, m_head_len(0)
	, m_buffer_len(0)
	, m_request_seq((uint32)rand() % 1000)
	, m_has_error(false)
{
	m_input_rc4.init(key);
	m_output_rc4.init(key);
}

Aone2RC4Parser::~Aone2RC4Parser()
{
	if (m_buffer)
	{
		delete[] m_buffer;
		m_buffer = NULL;
	}
}

bool Aone2RC4Parser::need_bytes(void** ppBuff, uint32& len)
{
	len = 0;
	assert(ppBuff);
	*ppBuff = NULL;
	if (m_has_error) return false;

	assert(m_head_len <= sizeof(uint32));
	if (m_head_len < sizeof(uint32))
	{
		*ppBuff = (char *)&m_cmdLen + m_head_len;
		len = (uint32)sizeof(uint32)-m_head_len;
		return true;
	}
	if (m_cmdLen < sizeof(Aone2CmdHeader_) + sizeof(uint32) || m_cmdLen > AONE_MSG_MAX_LENGTH)
	{
		uuLOG("Invalid Aone2 message, cmdLen is %d.\n", m_cmdLen);
		m_has_error = true;
		return false;
	}

	if (m_buffer == NULL)
	{
		m_buffer = new byte[m_cmdLen];
		assert(m_buffer && "malloc exception!");
		*(uint32*)m_buffer = m_cmdLen;
		m_buffer_len = sizeof(uint32);
	}

	*ppBuff = m_buffer + m_buffer_len;
	len = m_cmdLen - m_buffer_len;

	return true;
}

bool Aone2RC4Parser::put_bytes(const void* src, uint32 len, bool encrypt)
{
	if (m_has_error) return false;
	if (src == NULL || len == 0) return true;

	if (m_head_len < sizeof(uint32))
	{
		assert(len + m_head_len <= sizeof(uint32) && "call exception!");
		memcpy((char *)&m_cmdLen + m_head_len, src, len);
		if (encrypt) {
			m_input_rc4.update((const byte*)((char *)&m_cmdLen + m_head_len), (byte*)((char *)&m_cmdLen + m_head_len), len); // 对数据解密
		}
		m_head_len += len;
		if (m_head_len == sizeof(uint32) &&
			(m_cmdLen < sizeof(Aone2CmdHeader_) + sizeof(uint32) || m_cmdLen > AONE_MSG_MAX_LENGTH))
		{
			uuLOG("Invalid Aone message, cmdLen is %d.\n", m_cmdLen);
			m_has_error = true;
			return false;
		}
		return true;
	}

	assert(m_buffer);
	assert(m_buffer_len + len <= m_cmdLen && "data size exception!");

	uint8* pos = m_buffer + m_buffer_len;
	if (pos != src) memcpy(pos, src, len);
	m_buffer_len += len;
	if (encrypt) {
		m_input_rc4.update((const byte*)pos, (byte*)pos, len); // 对数据解密
	}
	return true;
}

Aone2BaseMessage* Aone2RC4Parser::get_message()
{
	if (m_has_error) return NULL;
	if (m_head_len < sizeof(uint32)) return NULL;
	if (m_buffer == NULL) return NULL;
	if (m_buffer_len < m_cmdLen) return NULL;

	Aone2CmdHeader_* cmd = (Aone2CmdHeader_*)(m_buffer + sizeof(uint32));
	Aone2BaseMessage* message = create_message(*cmd);
	assert(message);

	if (!message->parse_bytes(m_buffer, m_cmdLen))
	{
		uuLOG("Aone message(Number:%d) parse failed.\n", cmd->m_cmdNum);
		m_has_error = true;
		return NULL;
	}
	delete m_buffer;
	m_buffer = NULL;
	m_cmdLen = 0;
	m_buffer_len = 0;
	m_head_len = 0;
	return message;
}

bool Aone2RC4Parser::has_error()
{
	return m_has_error;
}

bool Aone2RC4Parser::to_buffer(Aone2BaseMessage* msg, byte** ppBuff, uint32& len, bool encrypt)
{
	assert(msg);
	assert(ppBuff);
	*ppBuff = NULL;
	len = 0;

	bool is_request = (msg->cmdheader().m_cmdNum % 2) == 0;
	if (typeid(*msg) == typeid(Aone2NoneRequest) || typeid(*msg) == typeid(Aone2NoneResponse))
	{
		return true; // 不需要发送的消息
	}

	if (is_request && (msg->cmdheader().m_cmdSeq == 0)) msg->cmdheader().m_cmdSeq = ++m_request_seq;  // auto set the sequence of header
	
	bool bl = msg->to_bytes(ppBuff, len);
	if (!bl || *ppBuff == NULL) return false;

	if (is_request)
		uuLOG("<== Aone2 Request: %u.\n", msg->cmdheader().m_cmdNum);
	else
		uuLOG("<== Aone2 Response: %u.\n", msg->cmdheader().m_cmdNum);

	if (encrypt) {
		m_output_rc4.update((const byte*)*ppBuff, (byte*)*ppBuff, len); // 对数据加密
	}
	return true;
}

Aone2BaseMessage* Aone2RC4Parser::create_message(Aone2CmdHeader_& header)
{
	Aone2BaseMessage* msg = XMessageFactory<Aone2BaseMessage, uint32>::instance()->create(header.m_cmdNum);
	if (msg)
	{
		if (header.m_cmdNum % 2 == 0)
			uuLOG("==> Aone2 Request: %u.\n", header.m_cmdNum);
		else
			uuLOG("==> Aone2 Response: %u.\n", header.m_cmdNum);
	}
	else
	{
		if (header.m_cmdNum % 2 == 0)
		{
			uuLOG("==> Unknown Aone2 Request: %u.\n", header.m_cmdNum);
			Aone2UnknownRequest* unknownMsg = new Aone2UnknownRequest;
			unknownMsg->header().m_cmdNum = header.m_cmdNum; //
			return unknownMsg;
		}
		else
		{
			uuLOG("==> Unknown Aone2 Response: %u.\n", header.m_cmdNum);
			Aone2UnknownResponse* unknownMsg = new Aone2UnknownResponse;
			unknownMsg->header().m_cmdNum = header.m_cmdNum; //
			return unknownMsg;
		}
	}
	return msg;
}

byte* Aone2RC4Parser::get_buffer()
{
	return m_buffer;
}

uint32 Aone2RC4Parser::get_buffern_len()
{
	return m_cmdLen;
}

uint32 Aone2RC4Parser::get_buffer_readed_len()
{
	return m_buffer_len;
}

Aone2BaseMessage* Aone2RC4Parser::get_message(byte* buffer, int len)
{
	//m_input_rc4.update(buffer, buffer, len);

	Aone2CmdHeader_* cmd = (Aone2CmdHeader_*)(buffer + sizeof(uint32));
	Aone2BaseMessage* message = create_message(*cmd);
	assert(message);

	if (!message->parse_bytes(buffer, len))
	{
		uuLOG("Aone message(Number:%d) parse failed.\n", cmd->m_cmdNum);
		m_has_error = true;
		return NULL;
	}
	return message;
}

}//namespace aone2

