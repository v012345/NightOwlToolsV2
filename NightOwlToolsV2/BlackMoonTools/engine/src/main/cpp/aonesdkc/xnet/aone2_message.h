// 2011-08-29
// aone2_message.h
// guosh
// aone2消息类


#ifndef _AONE2_MESSAGE_H_
#define _AONE2_MESSAGE_H_

#include "xcore_define.h"
#include "xnet_message.h"
#include "aone2_define.h"
#include "aone2_message_body.h"

namespace aone2
{

///////////////////////////////////////////////////////////////////////////////
// class Aone2BaseMessage
///////////////////////////////////////////////////////////////////////////////
class Aone2BaseMessage : public XMessage
{
public:
	DEFINE_MESSAGE_VISITABLE();

	virtual Aone2CmdHeader_& cmdheader() = 0;
	virtual Aone2BaseMessage* clone() = 0;
	virtual uint32 get_number() = 0;
	virtual uint32 get_cseq() = 0;
	virtual void   set_cseq(uint32 cseq) = 0;

	// return -1: error   >=0: bytes of used or need
	virtual bool   parse_bytes(const byte* src, uint32 len) { return  false; }
	virtual bool   to_bytes(byte** ppBuff, uint32& len) { return false; }
};

///////////////////////////////////////////////////////////////////////////////
// class Aone2Message
///////////////////////////////////////////////////////////////////////////////
template <typename HeaderT, typename BodyT, uint32 Number>
class Aone2Message : public Aone2BaseMessage
{
public:
	DEFINE_MESSAGE_VISITABLE();

	enum { NUMBER = Number };

	static string to_str(uint32 num)
	{
		char buf[32];
		sprintf(buf, "%u", num);
		return buf;
	}

	static const string& KEY()
	{
		static string str = "aone_" + to_str(NUMBER);
		return str;
	}

public:
	Aone2Message()
	{
		m_header.m_cmdNum  = NUMBER;
		m_header.m_cmdSeq  = 0;
		m_header.m_reserve = 0;
	}

	Aone2Message(const Aone2Message& src)
	{
		m_header = src.m_header;
		m_body = src.m_body;
	}

	Aone2Message& operator = (const Aone2Message& src)
	{
		if (this != &src)
		{
			m_header = src.m_header;
			m_body = src.m_body;
		}
		return *this;
	}

	virtual const string& unique_key() { return KEY(); }

	virtual Aone2BaseMessage* clone() { return new Aone2Message(*this); }

	virtual Aone2CmdHeader_& cmdheader() { return m_header; }

	virtual uint32 get_number() { return m_header.m_cmdNum; }
	virtual uint32 get_cseq() { return m_header.m_cmdSeq; }
	virtual void   set_cseq(uint32 cseq) { m_header.m_cmdSeq = cseq; }

	HeaderT& header() { return m_header; }

	const HeaderT& header() const { return m_header; }

	BodyT& body() { return m_body; }

	const BodyT& body() const { return m_body; }

	virtual bool parse_bytes(const byte* src, uint32 len)
	{
		assert(src);
		if (len < sizeof(m_header) + sizeof(uint32))
		{
			printf("Invalid Aone2 message, length is %u\n", len);
			return false;
		}
		uint32 cmdLen = *(uint32*)src;
		Aone2CmdHeader_* cmd = (Aone2CmdHeader_*)(src + sizeof(uint32));
		if (cmdLen != len)
		{
			printf("Invalid Aone2 message(cmdNum:%u), cmdlen(%u), recv(%u).\n", m_header.m_cmdNum, cmdLen, len);
			return false;
		}

		if (cmd->m_cmdNum != NUMBER &&
		        NUMBER != AONE2_CMD_UNKNOWN_REQ &&
		        NUMBER != AONE2_CMD_UNKNOWN_RESP)
		{
			printf("Invalid Aone2 message(NUMBER:%u), bad cmdNum(%u).\n", NUMBER, cmd->m_cmdNum);
			return false;
		}

		m_header = *(HeaderT*)cmd;
		uint32 bodyLen = cmdLen - sizeof(HeaderT) - sizeof(uint32);
		Aone2RespHeader_* respHeader = static_cast<Aone2RespHeader_*>(cmd);
		if (m_header.m_cmdNum % 2 == 0 || respHeader->m_result == 0) // 回复时返回码不为0时，不带消息体
		{
			if (m_body.parse_bytes(src + sizeof(uint32)+sizeof(HeaderT), bodyLen) != (int)bodyLen)
			{
				printf("Invalid Aone2 message(cmdNum:%u), parse body failed.\n", m_header.m_cmdNum);
				return false;
			}
		}

		return true;
	}

	virtual bool to_bytes(byte** ppBuff, uint32& len)
	{
		assert(ppBuff);
		*ppBuff = NULL;
		len = 0;

		uint32 maxlen = m_body.calc_length() + sizeof(HeaderT)+sizeof(uint32);
		*ppBuff = new byte[maxlen];
		byte* dst = (byte *)(*ppBuff);
		if (dst == NULL) return false;
		*(uint32*)dst = maxlen;
		memcpy(dst + sizeof(uint32), &m_header, sizeof(HeaderT));

		Aone2RespHeader_* respHeader = (Aone2RespHeader_*)&m_header;
		if (m_header.m_cmdNum % 2 == 0 || respHeader->m_result == 0) // 回复时返回码不为0时，不带消息体
		{
			int bodyLen = m_body.to_bytes(dst + sizeof(HeaderT) + sizeof(uint32), maxlen - sizeof(HeaderT) - sizeof(uint32));
			if (bodyLen != maxlen - sizeof(HeaderT)-sizeof(uint32))
			{
				printf("Aone2 message(cmdNum:%u) body to_bytes() failed.\n", m_header.m_cmdNum);
				return false;
			}
			
			len = maxlen;
			return true;
		}
		
		len = maxlen;
		return true;
	}

protected:
	HeaderT		m_header;
	BodyT		m_body;
};

///////////////////////////////////////////////////////////////////////////////
// class Aone2Request
///////////////////////////////////////////////////////////////////////////////
template <typename BodyT, uint32 Number>
class Aone2Request : public Aone2Message<Aone2ReqHeader_, BodyT, Number>
{
public:
	DEFINE_MESSAGE_VISITABLE();

	Aone2Request()
	{
		Aone2ReqHeader_& header_ = Aone2Message<Aone2ReqHeader_, BodyT, Number>::m_header;
		header_.m_srcId = 0;
		header_.m_session = 0;
	}

	Aone2Request(const Aone2Request& src)
		: Aone2Message<Aone2ReqHeader_, BodyT, Number>(src)
	{
	}

	Aone2Request& operator = (const Aone2Request& src)
	{
		if (this != &src)
		{
			Aone2Message<Aone2ReqHeader_, BodyT, Number>::operator=(src);
		}
		return *this;
	}

	virtual Aone2BaseMessage* clone()
	{
		return new Aone2Request(*this);
	}

	uint32 get_srcId() const
	{
		const Aone2ReqHeader_& header_ = Aone2Message<Aone2ReqHeader_, BodyT, Number>::m_header;
		return header_.m_srcId;
	}

	void set_srcId(uint32 srcId)
	{
		Aone2ReqHeader_& header_ = Aone2Message<Aone2ReqHeader_, BodyT, Number>::m_header;
		header_.m_srcId = srcId;
	}

	uint32 get_session() const
	{
		const Aone2ReqHeader_& header_ = Aone2Message<Aone2ReqHeader_, BodyT, Number>::m_header;
		return header_.m_session;
	}

	void set_session(uint32 session)
	{
		Aone2ReqHeader_& header_ = Aone2Message<Aone2ReqHeader_, BodyT, Number>::m_header;
		header_.m_session = session;
	}
};

///////////////////////////////////////////////////////////////////////////////
// class Aone2Response
///////////////////////////////////////////////////////////////////////////////
template <typename BodyT, uint32 Number>
class Aone2Response : public Aone2Message<Aone2RespHeader_, BodyT, Number>
{
public:
	DEFINE_MESSAGE_VISITABLE();

	Aone2Response()
	{
		Aone2RespHeader_& header_ = Aone2Message<Aone2RespHeader_, BodyT, Number>::m_header;
		header_.m_result = 0;
	}

	Aone2Response(const Aone2Response& src)
		: Aone2Message<Aone2RespHeader_, BodyT, Number>(src)
	{
	}

	Aone2Response& operator = (const Aone2Response& src)
	{
		if (this != &src)
		{
			Aone2Message<Aone2RespHeader_, BodyT, Number>::operator=(src);
		}
		return *this;
	}

	virtual Aone2BaseMessage* clone()
	{
		return new Aone2Response(*this);
	}

	int get_result() const
	{
		const Aone2RespHeader_& header_ = Aone2Message<Aone2RespHeader_, BodyT, Number>::m_header;
		return header_.m_result;
	}

	void set_result(int result)
	{
		Aone2RespHeader_& header_ = Aone2Message<Aone2RespHeader_, BodyT, Number>::m_header;
		header_.m_result = result;
	}
};

typedef Aone2Request  <Aone2BytesMessageBody, AONE2_CMD_UNKNOWN_REQ>  Aone2UnknownRequest;
typedef Aone2Response <Aone2BytesMessageBody, AONE2_CMD_UNKNOWN_RESP> Aone2UnknownResponse;
typedef Aone2Request  <Aone2EmptyMessageBody, AONE2_CMD_UNKNOWN_REQ>  Aone2NoneRequest;
typedef Aone2Response <Aone2EmptyMessageBody, AONE2_CMD_UNKNOWN_RESP> Aone2NoneResponse;

}//namespace aone2

using namespace aone2;

#endif//_AONE2_MESSAGE_H_
