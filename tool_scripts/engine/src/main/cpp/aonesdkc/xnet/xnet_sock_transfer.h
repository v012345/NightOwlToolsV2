// 2011-09-14
// xnet_sock_transfer.h
// guosh
// TCP数据传送


#ifndef _XNET_SOCK_TRANSFER_H_
#define _XNET_SOCK_TRANSFER_H_

#include "xcore_define.h"
#include "xnet_socket.h"
#include "xnet_message.h"
#include "aone2_message.h"
#include "aone2_rc4_parser.h"

namespace aonesdk_xnet {
	
///////////////////////////////////////////////////////////////////////////////
// class XSockTransfer
///////////////////////////////////////////////////////////////////////////////
class XSockTransfer
{
public:
	XSockTransfer(SOCKET sock, const string& key);

	~XSockTransfer();

	int get_handle() const;

	bool is_open() const;

	bool close();

	bool can_recv(int timeout_ms);

	bool can_send(int timeout_ms);

	string description();

	// msg == NULL时尝试把缓存的数据发送出去
	bool write(Aone2BaseMessage* msg);

	bool read(Aone2BaseMessage** msg, int timeout_ms = -1);

public:
	bool writeBytes(byte* buff, uint32 len);
    
	bool readBytes(byte** msg, uint32 &len, int timeout_ms = -1);
	bool readHttpBytes(byte** msg, uint32 &len, int timeout_ms = -1);

private:
	XSocket              m_sock;
	Aone2RC4Parser       m_parser;
	string               m_description;
	list<byte*>          m_buffs;
	list<uint32>         m_lens;
};

}//namespace xnet

using namespace aonesdk_xnet;

#endif//_XNET_SOCK_TRANSFER_H_
