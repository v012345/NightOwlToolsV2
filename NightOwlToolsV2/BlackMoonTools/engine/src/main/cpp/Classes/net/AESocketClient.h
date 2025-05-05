/**************************************************************************
*  @Copyright (c) 2015, A-one Soft, All rights reserved.

*  @file			: AESocketClient.h
*  @version	: ver 1.0.0

*  @author	: andy-zhao
*  @date		: 2015/11/28
*  @brief		: 客户端socket（多线程）
**************************************************************************/
#ifndef _AESocketClient_H_
#define _AESocketClient_H_

#include "common/AERC4.h"
#include "net/AEMessage.h"
#include "platform/AEStdC.h"
#include <thread>
#include <mutex>
#include <condition_variable>

#if (AE_TARGET_PLATFORM == AE_PLATFORM_WIN32)
#include <Winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "wsock32")
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <unistd.h>
#endif

NS_AE_BEGIN

#if AE_TARGET_PLATFORM != AE_PLATFORM_WIN32
typedef unsigned int SOCKET;
#define INVALID_SOCKET  (SOCKET)(~0)
#endif

typedef enum _SocketState
{
	eSocketState_Wait = 0,
	eSocketState_Connect,
	eSocketState_Disconnect,

} AESocketState;

class AESocketClient
{
public:
	AESocketClient();
	virtual ~AESocketClient();

	/**
		建立socket连接。
	*/	
	bool connect(const std::string& host, const uint16& port);

	/**
		断开socket连接。
	*/
	void disconnect();

	/**
		消息存入发送队列。
	*/ 
	void sendMessage(AEMessage* pMessage);

	/**
		弹出接收队列消息。
	*/ 
	AEMessage* receiveMessage();
    
	/**
		socket连接状态。
	*/ 
	AESocketState getSocketState() { return m_eSocketState; }

private:
	/**
		地址。
	*/ 
	sockaddr_in addr(const std::string& host, const uint16& port);

	/**
		强制关闭Socket连接。
	*/ 
	void shutdown();

	/**
		减少引用计数，关闭Socket连接。
	*/ 
	void close(int32 delay = -1);

	/**
		加密。
	*/
	void encodeBuffer(uint32 input, uint32 output, uint32 length);

	/**
		解密。
	*/
	void decodeBuffer(uint32 input, uint32 output, uint32 length);

public:
	/**
		初始密钥。
	*/
	static std::string s_sKey;
	static void init(const std::string& key);

	/**
		发送线程。
	*/
	static void* threadSend(void* ptr);

	/**
		接收线程。
	*/
	static void* threadReceive(void* ptr);

private:
	// Socket连接
	SOCKET m_pSocket;
	// 连接状态
	AESocketState m_eSocketState;

	// 发送和接收缓冲区。
	// 注意：发送缓冲区满的时候，会断开连接，并提示信号不好。
	AEByteBuffer m_pSendBuffer;
	AEByteBuffer m_pReceiveBuffer;

	// 发送队列
	std::queue<AEMessage*> m_pSendMessageQueue;
	// 接收队列
	std::queue<AEMessage*> m_pReceiveMessageQueue;

	// 发送线程
	std::thread m_pThreadSend;
	std::mutex m_pThreadMutexSend;
	std::condition_variable m_pThreadConditionSend;

	// 接收线程
	std::thread m_pThreadReceive;
	std::mutex m_pThreadMutexReceive;
	std::condition_variable m_pThreadConditionReceive;

	// 发送队列同步锁
	std::mutex m_pThreadMutexQueue;

	// 加密
	AERC4 m_pEncode;
	// 解密
	AERC4 m_pDecode;

};

NS_AE_END

#endif // _AESocketClient_H_