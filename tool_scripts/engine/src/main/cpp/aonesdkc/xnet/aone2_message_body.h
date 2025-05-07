// 2011-08-17
// aone2_message_body.h
// guosh
// Aone2消息体接口


#ifndef _AONE2_MESSAGE_BODY_H_
#define _AONE2_MESSAGE_BODY_H_

#include "xcore_define.h"


namespace aone2
{

///////////////////////////////////////////////////////////////////////////////
// class Aone2MessageBody
///////////////////////////////////////////////////////////////////////////////
class Aone2MessageBody
{
public:
	virtual ~Aone2MessageBody() {}

	// return -1: error   >=0: bytes of used
	virtual int parse_bytes(const byte* src, uint32 len) { return -1; }
	virtual int to_bytes(byte* dst, uint32 maxlen) { return -1; }
	virtual uint32 calc_length() { return 0; }
};

///////////////////////////////////////////////////////////////////////////////
// class Aone2EmptyMessageBody
///////////////////////////////////////////////////////////////////////////////
class Aone2EmptyMessageBody : public Aone2MessageBody
{
public:
	virtual ~Aone2EmptyMessageBody() {}

	virtual int parse_bytes(const byte* src, uint32 len)
	{
		if (src == NULL) return -1;
		if (len < sizeof(uint32)) return -1;
		uint32 msglen = *(uint32*)src;
		if (msglen < sizeof(uint32) || msglen > len) return -1;
		return msglen;
	}

	virtual int to_bytes(byte* dst, uint32 maxlen)
	{
		if (dst == NULL) return -1;
		if (maxlen < sizeof(uint32)) return -1;
		*(uint32*)dst = sizeof(uint32);
		return sizeof(uint32);
	}

	virtual uint32 calc_length()
	{
		return sizeof(uint32);
	}
};


///////////////////////////////////////////////////////////////////////////////
// class Aone2BytesMessageBody
///////////////////////////////////////////////////////////////////////////////
class Aone2BytesMessageBody : public Aone2MessageBody
{
public:
	Aone2BytesMessageBody()
		: m_data(NULL)
		, m_len(0)
	{
		;
	}

	virtual ~Aone2BytesMessageBody()
	{
		clear();
	}

	Aone2BytesMessageBody(const Aone2BytesMessageBody& body)
		: m_data(NULL)
		, m_len(0)
	{
		_copy(body.m_data, body.m_len);
	}

	Aone2BytesMessageBody& operator = (const Aone2BytesMessageBody& body)
	{
		if (this != &body)
		{
			_copy(body.m_data, body.m_len);
		}
		return *this;
	}

	virtual int parse_bytes(const byte* src, uint32 len)
	{
		if (src == NULL) return -1;
		if (len < sizeof(uint32)) return -1;
		uint32 msglen = *(uint32*)src;
		if (msglen < sizeof(uint32) || msglen > len) return -1;
		_copy(src + sizeof(uint32), msglen - sizeof(uint32));
		return msglen;
	}

	virtual int to_bytes(byte* dst, uint32 maxlen)
	{
		if (dst == NULL) return -1;
		if (maxlen < sizeof(uint32) + m_len) return -1;
		if (m_data == NULL || m_len == 0)
		{
			*(uint32*)dst = sizeof(uint32);
			return sizeof(uint32);
		}
		else
		{
			memcpy(dst + sizeof(uint32), m_data, m_len);
			*(uint32*)dst = sizeof(uint32) + m_len;
			return sizeof(uint32) + m_len;
		}
	}

	virtual uint32 calc_length()
	{
		if (m_data == NULL || m_len == 0)
		{
			return sizeof(uint32);
		}
		return sizeof(uint32) + m_len;
	}

	int length()
	{
		return m_len;
	}

	const byte* get_bytes() const
	{
		return m_data;
	}

	void set_bytes(const byte* src, uint32 len)
	{
		_copy(src, len);
	}

	void clear()
	{
		m_len = 0;
		if (m_data)
		{
			::free(m_data);
			m_data = NULL;
		}
	}

private:
	void _copy(const byte* src, uint32 len)
	{
		clear();

		if ((src != NULL) && (len != 0))
		{
			m_data = (byte*)::malloc(len + 1);
			memcpy(m_data, src, len);
			m_data[len] = '\0';
			m_len = len;
		}
	}

protected:
	byte*     m_data;
	uint32    m_len;
};

}//namespace aone2

using namespace aone2;

#endif//_AONE2_MESSAGE_BODY_H_
