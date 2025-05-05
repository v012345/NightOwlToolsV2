// 2011-09-14
// xnet_sock_transfer.cpp
// guosh
// TCP数据传送

#include "xnet_sock_transfer.h"
#include "uuSdkDef.h"

namespace aonesdk_xnet {

///////////////////////////////////////////////////////////////////////////////
// class XSockTransfer
///////////////////////////////////////////////////////////////////////////////
XSockTransfer::XSockTransfer(SOCKET sock, const string& key)
	: m_sock(sock)
	, m_parser(key)
{
	assert(sock != INVALID_SOCKET);

	char buf[800];
	sprintf(buf, "local:%s, remote:%s, ptr:0X%X", 
			m_sock.local_addr().to_str().c_str(), m_sock.remote_addr().to_str().c_str(), *(uint32*)this);
	m_description = buf;

	uuLOG("XSockTransfer(%s) constructed. key is %s\n", description().c_str(), key.c_str());
}

XSockTransfer::~XSockTransfer()
{
	this->close();
	uuLOG("XSockTransfer(%s) destructed.\n", description().c_str());
}


int XSockTransfer::get_handle() const
{
	return m_sock.get_handle();
}

bool XSockTransfer::is_open() const
{
	return m_sock.is_open();
}

bool XSockTransfer::close()
{
	if (is_open())
	{
		m_sock.shutdown();
		m_sock.close();
		uuLOG("XSockTransfer(%s) closed by local.\n", m_description.c_str());
	}

	// 未发送的消息释放空间
	while (!m_buffs.empty())
	{
		delete[] m_buffs.front();
		m_buffs.pop_front();
	}
	m_buffs.clear();
	m_lens.clear();

	return true;
}

bool XSockTransfer::can_recv(int timeout_ms)
{
	return m_sock.can_recv(timeout_ms);
}

bool XSockTransfer::can_send(int timeout_ms)
{
	return m_sock.can_send(timeout_ms);
}

string XSockTransfer::description()
{
	return m_description;
}

bool XSockTransfer::write(Aone2BaseMessage* msg)
{
	if (!m_sock.is_open()) return false;

	if (msg)
	{
		byte* pBuff = NULL;
		uint32 len = 0;
		if (!m_parser.to_buffer(msg, &pBuff, len)) return false;
		if (pBuff != NULL && len != 0)
		{
			m_buffs.push_back(pBuff);
			m_lens.push_back(len);
		}
	}

	while (m_sock.can_send(0))
	{
		if (m_buffs.empty()) return true;
		byte* pBuff = m_buffs.front();
		uint32& len = m_lens.front();
		int ret = m_sock.send(pBuff, (int)len);
		if (ret < 0) return false;
		uuLOG("XSockTransfer(%s) need send %d bytes, send return %d.\n", m_description.c_str(), len, ret);
		assert((uint32)ret <= len);
		len -= ret;
		if (len == 0)
		{
            delete[] m_buffs.front();
			m_buffs.pop_front();
			m_lens.pop_front();
		}
	}
	
	return true;
}

bool XSockTransfer::read(Aone2BaseMessage** msg, int timeout_ms)
{
	if (msg == NULL) return true;
	*msg = NULL;

	if (m_parser.has_error()) return false;
	*msg = m_parser.get_message();
	if (*msg)
	{
		return true;
	}
	if (m_parser.has_error()) return false;
	if (!m_sock.is_open()) return false;

	while (m_sock.can_recv(timeout_ms))
	{
		char* need_bytes_buf = NULL;
		uint32 need_bytes_sz = 0;
		if (!m_parser.need_bytes((void **)&need_bytes_buf, need_bytes_sz)) return false;
		assert(need_bytes_buf != NULL && need_bytes_sz > 0);

		int recv_sz = m_sock.recv(need_bytes_buf, need_bytes_sz);
		uuLOG("XSockTransfer(%s) need recv %d bytes, recv return %d.\n", m_description.c_str(), need_bytes_sz, recv_sz);
		if (0 == recv_sz) continue;
		if (recv_sz < 0)
		{
			uuLOG("XSockTransfer(%s) socket recv error or closed.\n", m_description.c_str());
			m_sock.close();
			return false;
		}

		if (!m_parser.put_bytes(need_bytes_buf, recv_sz))
		{
			uuLOG("XSockTransfer(%s) parser message error.\n", m_description.c_str());
			return false;
		}

		*msg = m_parser.get_message();
		if (*msg)
		{
			return true;
		}
		if (m_parser.has_error()) return false;
	}

	return true;
}

bool XSockTransfer::writeBytes(byte* buff, uint32 len)
{
	if (!m_sock.is_open()) return false;
	
	if (buff == NULL || len <= 0) {
		return false;
	}

	m_buffs.push_back(buff);
	m_lens.push_back(len);

	while (m_sock.can_send(0))
	{
		if (m_buffs.empty()) return true;
		byte* pBuff = m_buffs.front();
		uint32& len = m_lens.front();
		int ret = m_sock.send(pBuff, (int)len);
		if (ret < 0) return false;
		uuLOG("XSockTransfer(%s) need send %d bytes, send return %d.\n", m_description.c_str(), len, ret);
		assert((uint32)ret <= len);
		len -= ret;
		if (len == 0)
		{
			m_buffs.pop_front();
			m_lens.pop_front();
		}
	}

	return true;
}

bool XSockTransfer::readBytes(byte** msg, uint32 &len, int timeout_ms)
//bool read(Aone2BaseMessage** msg, int timeout_ms)
{
	if (msg == NULL) return false;
	*msg = NULL;
	len = 0;
	if (m_parser.has_error()) return false;
	//*msg = m_parser.get_message();
	*msg = m_parser.get_buffer();
	if (*msg)
	{
		len = m_parser.get_buffern_len();
		return true;
	}
	if (m_parser.has_error()) return false;

	if (!m_sock.is_open()) return false;

	while (m_sock.can_recv(timeout_ms))
	{
		char* need_bytes_buf = NULL;
		uint32 need_bytes_sz = 0;
		if (!m_parser.need_bytes((void **)&need_bytes_buf, need_bytes_sz)) return false;
		assert(need_bytes_buf != NULL && need_bytes_sz > 0);

		int recv_sz = m_sock.recv(need_bytes_buf, need_bytes_sz);
		uuLOG("XSockTransfer(%s) need recv %d bytes, recv return %d.\n", m_description.c_str(), need_bytes_sz, recv_sz);
		if (0 == recv_sz) continue;
		if (0 > recv_sz)
		{
			uuLOG("XSockTransfer(%s) socket recv error or closed.\n", m_description.c_str());
			m_sock.close();
			return false;
		}

		if (!m_parser.put_bytes(need_bytes_buf, recv_sz))
		{
			uuLOG("XSockTransfer(%s) parser message error.\n", m_description.c_str());
			return false;
		}

		//*msg = m_parser.get_message();
		*msg = m_parser.get_buffer();
		if (*msg && m_parser.get_buffern_len() == m_parser.get_buffer_readed_len())
		{
			len = m_parser.get_buffern_len();
			return true;
		}
		if (m_parser.has_error()) return false;
	}

    if (*msg == NULL || m_parser.get_buffern_len() != m_parser.get_buffer_readed_len())
    {
        uuLOG("receive incomplete");
        return false;
    }
	return true;
}

bool XSockTransfer::readHttpBytes( byte** msg, uint32 &len, int timeout_ms /*= -1*/ )
{
	if (msg == NULL) return false;
	*msg = NULL;
	len = 0;
	if (!m_sock.is_open()) return false;

	uint32 bytes_len = 100;	// no need receive all data for http server
	*msg = new byte[bytes_len];
	uint32 need_bytes_sz = 100;
	int recv_sz = 0;
	while (m_sock.can_recv(timeout_ms))
	{
		int recv_len = m_sock.recv(*msg + recv_sz, need_bytes_sz);
		uuLOG("XSockTransfer(%s) need recv %d bytes, recv return %d.\n", m_description.c_str(), need_bytes_sz, recv_len);
		if (0 == recv_len) continue;
		if (-1 == recv_len)
		{
			uuLOG("XSockTransfer(%s) socket recv error or closed.\n", m_description.c_str());
			m_sock.close();
			return false;
		}
		recv_sz += recv_len;
		if (bytes_len - recv_sz < need_bytes_sz) {
			need_bytes_sz = bytes_len - recv_sz;
		}

		if (recv_sz >= bytes_len) {
			len = bytes_len;
			return true;
		}
	}
	len = recv_sz < bytes_len ? recv_sz : bytes_len;
	return true;
}

}//namespace xnet

