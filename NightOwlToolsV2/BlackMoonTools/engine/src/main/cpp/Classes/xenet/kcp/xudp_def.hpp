#ifndef _KCP_DEF_HPP_
#define _KCP_DEF_HPP_
	
#include <stdio.h>
#include <stdint.h>
#include <memory>
#include <string>
#include <string.h>
#include "ikcp.h"
	
namespace xudp
{
	
#define ASIO_KCP_CONNECTION_TIMEOUT_TIME 10 * 1000 // default is 10 seconds.
	
enum eEventType
{
	eConnect,
	eDisconnect,
	eRcvMsg,
	eLagNotify,
	eCountOfEventType
};
	
// 
std::string make_kcp_key_connect();
bool is_kcp_key_connect(const char* data, size_t len);

std::string make_kcp_key_connect_response(uint32_t conv);
bool is_kcp_key_connect_response(const char* data, size_t len);
uint32_t get_conv_from_key_connect_response(const char* data);

std::string make_kcp_key_disconnect(uint32_t conv);
bool is_kcp_key_disconnect(const char* data, size_t len);
uint32_t get_conv_from_key_disconnect(const char* data);
	
}

#endif /*_KCP_DEF_HPP_*/