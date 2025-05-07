#include "net/AESocketClient.h"

NS_AE_BEGIN

#define AE_SOCKET_CLIENT_DEBUG true

//连接断开，发送失败
static const int32 SEND_MESSAGE_CONNECT_FAIL = -10001;
//信号较差，发送失败，请检查网络环境
static const int32 SEND_MESSAGE_POOR_SIGNAL = -10002;
//数据过长，发送失败
static const int32 SEND_MESSAGE_PACKET_TOO_BIG = -10003;
//网络异常中断
static const int32 RECV_MESSAGE_CONNECT_TERMINATE = -10004;
//网络超时，自动重连
static const int32 RECV_MESSAGE_AUTO_RECONNECT = -10005;
//网络超时，提示重连
static const int32 RECV_MESSAGE_RECONNECT_HINT = -10006;
//网络超时，强制登录
static const int32 RECV_MESSAGE_FORCE_LOGIN = -10007;
//服务主动关闭连接（服务器维护中）
static const int32 RECV_MESSAGE_SERVER_CLOSE_CONNECTION = -10008;
//数据过长，接受失败
static const int32 RECV_MESSAGE_PACKET_TOO_BIG = -10009;

#if AE_TARGET_PLATFORM == AE_PLATFORM_WIN32
#define Error_Code WSAGetLastError()
#else
#define	 Error_Code errno
#endif

std::string AESocketClient::s_sKey;

void AESocketClient::init(const std::string& key)
{
	s_sKey = key;
}

AESocketClient::AESocketClient()
: m_pSocket(INVALID_SOCKET)
, m_pSendBuffer(1024 * 512)
, m_pReceiveBuffer(1024 * 1024)
{
	m_eSocketState = eSocketState_Wait;

	m_pThreadSend = std::thread(&threadSend, this);
	m_pThreadReceive = std::thread(&threadReceive, this);

#if AE_TARGET_PLATFORM == AE_PLATFORM_WIN32
	WSADATA wsaData;
	WORD version = MAKEWORD(2, 0);
	int32 iRet = WSAStartup(version, &wsaData);
	if (0 == iRet)
	{
		AELOGINFO("WSAStartup success !");
	}
#endif
}

AESocketClient::~AESocketClient()
{
	std::unique_lock<std::mutex> lockSend(m_pThreadMutexSend);
	m_pThreadConditionSend.notify_all();
	m_pThreadSend.join();

	std::unique_lock<std::mutex> lockReceive(m_pThreadMutexReceive);
	m_pThreadConditionReceive.notify_all();
	m_pThreadReceive.join();

	this->shutdown();
	this->close();
#if AE_TARGET_PLATFORM == AE_PLATFORM_WIN32
	WSACleanup();
#endif

	while (!m_pReceiveMessageQueue.empty())
	{
		AEMessage* pMessage = m_pReceiveMessageQueue.front();
		m_pReceiveMessageQueue.pop();
		AESafe_delete(pMessage);
	}

	while (!m_pSendMessageQueue.empty())
	{
		AEMessage* pMessage = m_pSendMessageQueue.front();
		m_pSendMessageQueue.pop();
		AESafe_delete(pMessage);
	}
}

bool AESocketClient::connect(const std::string& host, const uint16& port)
{
	m_pEncode.init(s_sKey);
	m_pDecode.init(s_sKey);

	m_eSocketState = eSocketState_Wait;

	this->close();
	m_pSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == m_pSocket)
	{
		return false;
	}

	sockaddr_in sockAddr = addr(host, port);
	int32 iRetConnect = ::connect(m_pSocket, (sockaddr*)&sockAddr, sizeof(sockAddr));
	if (-1 == iRetConnect)
	{
		AELOGINFO("socket connect error : %d !", Error_Code);
		return false;
	}

	int32 iEnable = 1;
	int32 iRet = setsockopt(m_pSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&iEnable, sizeof(iEnable));
	if (-1 == iRet)
	{
		AELOGINFO("socket connect error : %d !", Error_Code);
		return false;
	}

	m_eSocketState = eSocketState_Connect;

	if (eSocketState_Connect == m_eSocketState)
	{
		std::unique_lock<std::mutex> lock(m_pThreadMutexSend);
		m_pThreadConditionSend.notify_all();
	}

	if (eSocketState_Connect == m_eSocketState)
	{
		std::unique_lock<std::mutex> lock(m_pThreadMutexReceive);
		m_pThreadConditionReceive.notify_all();
	}

	if (AE_SOCKET_CLIENT_DEBUG)
	{
		AELOGINFO("socket connected success !");
	}
	return true;
}

void AESocketClient::disconnect()
{
	this->close();

	m_eSocketState = eSocketState_Disconnect;
}

sockaddr_in AESocketClient::addr(const std::string& host, const uint16& port)
{
	int32 ip = inet_addr(host.c_str());
	sockaddr_in socketAddr;
	memset(&socketAddr, 0, sizeof(socketAddr));
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_port = htons(port);
	socketAddr.sin_addr.s_addr = ip;
	memset(&(socketAddr.sin_zero), 0, sizeof(socketAddr.sin_zero));

	if (AE_SOCKET_CLIENT_DEBUG)
	{
		AELOGINFO("addr host : %s, port : %d !", host.c_str(), port);
	}

	return socketAddr;
}

void AESocketClient::shutdown()
{
	if (INVALID_SOCKET != m_pSocket)
	{
#if AE_TARGET_PLATFORM == AE_PLATFORM_WIN32
		::shutdown(m_pSocket, SD_SEND);
#else
		::shutdown(m_pSocket, SHUT_RD);
#endif
	}
}

void AESocketClient::close(int32 delay /*= -1*/)
{
	if (INVALID_SOCKET != m_pSocket)
	{
		SOCKET sock = m_pSocket;
		m_pSocket = INVALID_SOCKET;

		if (delay >= 0)
		{
			struct linger linger_ = { 1, delay };
			setsockopt(sock, SOL_SOCKET, SO_LINGER, (const char *)&linger_, sizeof(linger_));
		}

#if AE_TARGET_PLATFORM == AE_PLATFORM_IOS
		::shutdown(m_pSocket, SHUT_RD);
#elif AE_TARGET_PLATFORM == AE_PLATFORM_WIN32
		::closesocket(m_pSocket);
#else
		::close(m_pSocket);
#endif
	}
}

void AESocketClient::encodeBuffer(uint32 input, uint32 output, uint32 length)
{
	m_pEncode.update((const byte*)m_pSendBuffer.getBuffer() + input, (byte*)m_pSendBuffer.getBuffer() + output, length);
}

void AESocketClient::decodeBuffer(uint32 input, uint32 output, uint32 length)
{
	m_pDecode.update((const byte*)m_pReceiveBuffer.getBuffer() + input, (byte*)m_pReceiveBuffer.getBuffer() + output, length);
}

void AESocketClient::sendMessage(AEMessage* pMessage)
{
	if (eSocketState_Wait == m_eSocketState)
	{
		AESafe_delete(pMessage);

		// 连接断开，发送失败
		if (AE_SOCKET_CLIENT_DEBUG)
		{
			AELOGINFO("connection is closed, send thread wait !");
		}
		std::unique_lock<std::mutex> lock(m_pThreadMutexQueue);
		m_pReceiveMessageQueue.push(AEMessage::errorMessage(SEND_MESSAGE_CONNECT_FAIL));

		return;
	}

	if (eSocketState_Disconnect == m_eSocketState)
	{
		AESafe_delete(pMessage);

		return;
	}

	std::unique_lock<std::mutex> lock(m_pThreadMutexQueue);
	m_pSendMessageQueue.push(pMessage);

	if (eSocketState_Connect == m_eSocketState)
	{
		std::unique_lock<std::mutex> lock(m_pThreadMutexSend);
		m_pThreadConditionSend.notify_all();
	}
}

AEMessage* AESocketClient::receiveMessage()
{
	AEMessage* pMessage = nullptr;
	std::unique_lock<std::mutex> lock(m_pThreadMutexQueue);

	if (!m_pReceiveMessageQueue.empty())
	{
		pMessage = m_pReceiveMessageQueue.front();
		m_pReceiveMessageQueue.pop();
	}
	return pMessage;
}

void* AESocketClient::threadSend(void* ptr)
{
	AESocketClient* pSocketClient = static_cast<AESocketClient*>(ptr);

	if (eSocketState_Wait == pSocketClient->m_eSocketState)
	{
		std::unique_lock<std::mutex> lock(pSocketClient->m_pThreadMutexSend);
		pSocketClient->m_pThreadConditionSend.wait(lock);
	}

	while (eSocketState_Wait != pSocketClient->m_eSocketState)
	{
		AEByteBuffer& pSendBuffer = pSocketClient->m_pSendBuffer;

		// 发送缓冲区中，遗留消息
		if (pSendBuffer.getPosition() > 0)
		{
			pSendBuffer.flip();
			pSocketClient->encodeBuffer(0, 0, pSendBuffer.getLimit());
				
			int32 iRetSend = ::send(pSocketClient->m_pSocket, (char *)pSendBuffer.getBuffer(), pSendBuffer.getLimit(), 0);
			if (-1 == iRetSend)
			{
				// 信号较差，发送失败，请检查网络环境
				if (AE_SOCKET_CLIENT_DEBUG)
				{
					AELOGINFO("poor signal,  error = %d,  send thread wait !", Error_Code);
				}
				pSocketClient->m_eSocketState = eSocketState_Disconnect;
				std::unique_lock<std::mutex> lock(pSocketClient->m_pThreadMutexQueue);
				pSocketClient->m_pReceiveMessageQueue.push(AEMessage::errorMessage(SEND_MESSAGE_POOR_SIGNAL));
			}
		}

		// 逐条发送
		while (!pSocketClient->m_pSendMessageQueue.empty())
		{
			std::unique_lock<std::mutex> lock(pSocketClient->m_pThreadMutexQueue);
			AEMessage* pMessage = pSocketClient->m_pSendMessageQueue.front();
			pSocketClient->m_pSendMessageQueue.pop();

			AEByteBuffer* pByteBuffer = pMessage->getByteBuffer();
			uint32 iCapacity = pByteBuffer->getCapacity();
			if (iCapacity + pSendBuffer.getPosition() > pSendBuffer.getLimit())
			{
				// 数据过长，发送失败
				if (AE_SOCKET_CLIENT_DEBUG)
				{
					AELOGINFO("packet too big, send thread wait !");
				}
				pSocketClient->m_eSocketState = eSocketState_Disconnect;
				std::unique_lock<std::mutex> lock(pSocketClient->m_pThreadMutexQueue);
				pSocketClient->m_pReceiveMessageQueue.push(AEMessage::errorMessage(SEND_MESSAGE_PACKET_TOO_BIG));
			}

			pSendBuffer.setBuffer(pByteBuffer->getBuffer(), 0, iCapacity);
			pSendBuffer.flip();
			pSocketClient->encodeBuffer(0, 0, pSendBuffer.getLimit());

			int32 iRetSend = ::send(pSocketClient->m_pSocket, (char *)pSendBuffer.getBuffer(), pSendBuffer.getLimit(), 0);
			if (-1 == iRetSend)
			{
				// 信号较差，发送失败，请检查网络环境
				if (AE_SOCKET_CLIENT_DEBUG)
				{
					AELOGINFO("poor signal,  error = %d,  send thread wait !", Error_Code);
				}
				pSocketClient->m_eSocketState = eSocketState_Disconnect;
				std::unique_lock<std::mutex> lock(pSocketClient->m_pThreadMutexQueue);
				pSocketClient->m_pReceiveMessageQueue.push(AEMessage::errorMessage(SEND_MESSAGE_POOR_SIGNAL));
			}

			pSendBuffer.setPosition(pSendBuffer.getPosition() + iRetSend);
			pSendBuffer.compact();

			AESafe_delete(pMessage);
		}

		if (eSocketState_Disconnect == pSocketClient->m_eSocketState || pSocketClient->m_pSendMessageQueue.empty())
		{
			std::unique_lock<std::mutex> lock(pSocketClient->m_pThreadMutexSend);
			pSocketClient->m_pThreadConditionSend.wait(lock);
		}
	}

	if (AE_SOCKET_CLIENT_DEBUG)
	{
		AELOGINFO("threadSend(), send thread stop !");
	}

	return (void*)nullptr;
}

//超时，提示用户重新登录
static const uint16 maxReloginTimes = 180;
//超时，提示选择重连
static const uint16 maxHintTimes = 150;
//超时，自动重连
static const uint16 maxIdleTimes = 120;

void* AESocketClient::threadReceive(void* ptr)
{
	fd_set fdRead;

	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	struct timeval lastRecvTime;
	gettimeofday(&lastRecvTime, nullptr);
	uint16 maxTimes = maxIdleTimes;

	bool decode = false;

	AESocketClient* pSocketClient = static_cast<AESocketClient*>(ptr);

	if (eSocketState_Wait == pSocketClient->m_eSocketState)
	{
		std::unique_lock<std::mutex> lock(pSocketClient->m_pThreadMutexReceive);
		pSocketClient->m_pThreadConditionReceive.wait(lock);
	}

	while (eSocketState_Wait != pSocketClient->m_eSocketState)
	{
		FD_ZERO(&fdRead);
		FD_SET(pSocketClient->m_pSocket, &fdRead);

		tv.tv_sec = 1;
		tv.tv_usec = 0;

		int32 iRetSelect = ::select(pSocketClient->m_pSocket + 1, &fdRead, nullptr, nullptr, &tv);
		if (-1 == iRetSelect)
		{
			if (EINTR != Error_Code)
			{
				// 网络异常中断！
				if (AE_SOCKET_CLIENT_DEBUG)
				{
					AELOGINFO("select error, errno = %d, address = %p, receive thread wait !", Error_Code, socket);
				}
				pSocketClient->m_eSocketState = eSocketState_Disconnect;
				std::unique_lock<std::mutex> lock(pSocketClient->m_pThreadMutexQueue);
				pSocketClient->m_pReceiveMessageQueue.push(AEMessage::errorMessage(RECV_MESSAGE_CONNECT_TERMINATE));
			}
			else
			{
				AELOGINFO(">>>>> 中断信号 <<<<<");
			}
		}
		else if (0 == iRetSelect)
		{
			struct timeval now;
			gettimeofday(&now, nullptr);

			if (now.tv_sec - lastRecvTime.tv_sec > maxReloginTimes && (maxReloginTimes == maxTimes || maxIdleTimes == maxTimes || maxHintTimes == maxTimes))
			{
				lastRecvTime = now;
				// 网络异常，强制登录！
				std::unique_lock<std::mutex> lock(pSocketClient->m_pThreadMutexQueue);
				while (!pSocketClient->m_pReceiveMessageQueue.empty())
				{
					AEMessage* pMessage = pSocketClient->m_pReceiveMessageQueue.front();
					pSocketClient->m_pReceiveMessageQueue.pop();
					AESafe_delete(pMessage);
				}
				pSocketClient->m_pReceiveMessageQueue.push(AEMessage::errorMessage(RECV_MESSAGE_FORCE_LOGIN));
			}
			else if (now.tv_sec - lastRecvTime.tv_sec > maxHintTimes && (maxHintTimes == maxTimes || maxIdleTimes == maxTimes))
			{
				maxTimes = maxReloginTimes;
				// 网络超时，提示重连！
				std::unique_lock<std::mutex> lock(pSocketClient->m_pThreadMutexQueue);
				pSocketClient->m_pReceiveMessageQueue.push(AEMessage::errorMessage(RECV_MESSAGE_RECONNECT_HINT));
			}
			else if (now.tv_sec - lastRecvTime.tv_sec > maxIdleTimes && maxIdleTimes == maxTimes)
			{
				maxTimes = maxHintTimes;
				// 网络超时，自动重连！
				std::unique_lock<std::mutex> lock(pSocketClient->m_pThreadMutexQueue);
				pSocketClient->m_pReceiveMessageQueue.push(AEMessage::errorMessage(RECV_MESSAGE_AUTO_RECONNECT));
			}
		}
		else if (iRetSelect > 0)
		{
			if (FD_ISSET(pSocketClient->m_pSocket, &fdRead))
			{
				int32 iRetRecv = 0;
				AEByteBuffer& pReceiveBuffer = pSocketClient->m_pReceiveBuffer;

				if (pReceiveBuffer.remaining() > 0)
				{
					iRetRecv = ::recv(pSocketClient->m_pSocket, (char*)pReceiveBuffer.getBuffer() + pReceiveBuffer.getPosition(), pReceiveBuffer.remaining(), 0);
				}

				if (-1 == iRetRecv)
				{
					// 网络异常中断！（服务器维护中）
					if (AE_SOCKET_CLIENT_DEBUG)
					{
						AELOGINFO("connect terminate, errno = %d, receive thread wait !", Error_Code);
					}
					pSocketClient->m_eSocketState = eSocketState_Disconnect;
					std::unique_lock<std::mutex> lock(pSocketClient->m_pThreadMutexQueue);
					while (!pSocketClient->m_pReceiveMessageQueue.empty())
					{
						AEMessage* pMessage = pSocketClient->m_pReceiveMessageQueue.front();
						pSocketClient->m_pReceiveMessageQueue.pop();
						AESafe_delete(pMessage);
					}
					pSocketClient->m_pReceiveMessageQueue.push(AEMessage::errorMessage(RECV_MESSAGE_CONNECT_TERMINATE));

				}
				else if (0 == iRetRecv && pReceiveBuffer.remaining() > 0)
				{
					// 服务器关闭连接！
					if (AE_SOCKET_CLIENT_DEBUG)
					{
						AELOGINFO("server closed connection, receive thread wait !");
					}
					pSocketClient->m_eSocketState = eSocketState_Disconnect;
					std::unique_lock<std::mutex> lock(pSocketClient->m_pThreadMutexQueue);
					while (!pSocketClient->m_pReceiveMessageQueue.empty())
					{
						AEMessage* pMessage = pSocketClient->m_pReceiveMessageQueue.front();
						pSocketClient->m_pReceiveMessageQueue.pop();
						AESafe_delete(pMessage);
					}
					pSocketClient->m_pReceiveMessageQueue.push(AEMessage::errorMessage(RECV_MESSAGE_SERVER_CLOSE_CONNECTION));
				}
				else
				{
					gettimeofday(&lastRecvTime, nullptr);
					maxTimes = maxIdleTimes;

					pReceiveBuffer.setPosition(pReceiveBuffer.getPosition() + iRetRecv);
					pReceiveBuffer.flip();

					while (pReceiveBuffer.remaining() > 4)
					{
						uint32 pos = pReceiveBuffer.getPosition();

						if (!decode)
						{
							pSocketClient->decodeBuffer(pos, pos, 4);
							decode = true;
						}
						uint32 len = 0;
						pReceiveBuffer.getUint32(len);

						if (len > pReceiveBuffer.getCapacity())
						{
							// 数据过长，接收失败！
							if (AE_SOCKET_CLIENT_DEBUG)
							{
								pSocketClient->decodeBuffer(pos + 4, pos + 4, 4);
								uint32 protocol = 0;
								pReceiveBuffer.getUint32(protocol);
								AELOGINFO("receive error message size(%d) greate capacity(%d), protocol(%d), receive thread wait !", protocol, len, pReceiveBuffer.getCapacity());
							}
							pSocketClient->m_eSocketState = eSocketState_Disconnect;
							std::unique_lock<std::mutex> lock(pSocketClient->m_pThreadMutexQueue);
							while (!pSocketClient->m_pReceiveMessageQueue.empty())
							{
								AEMessage* pMessage = pSocketClient->m_pReceiveMessageQueue.front();
								pSocketClient->m_pReceiveMessageQueue.pop();
								AESafe_delete(pMessage);
							}
							pSocketClient->m_pReceiveMessageQueue.push(AEMessage::errorMessage(RECV_MESSAGE_PACKET_TOO_BIG));
							break;
						}
						else if (pReceiveBuffer.remaining() >= len - 4)
						{
							pSocketClient->decodeBuffer(pos + 4, pos + 4, len - 4);

							AEMessage* pMessage = new AEMessage(pReceiveBuffer.getBuffer(), pos + 4, len - 4);
							std::unique_lock<std::mutex> lock(pSocketClient->m_pThreadMutexQueue);
							pSocketClient->m_pReceiveMessageQueue.push(pMessage);
							pReceiveBuffer.setPosition(pos + len);
							decode = false;
						}
						else
						{
							pReceiveBuffer.setPosition(pos);
							break;
						}
					}
					pReceiveBuffer.compact();
				}
			}
		}

		if (eSocketState_Disconnect == pSocketClient->m_eSocketState)
		{
			std::unique_lock<std::mutex> lock(pSocketClient->m_pThreadMutexReceive);
			pSocketClient->m_pThreadConditionReceive.wait(lock);
		}
	}

	if (AE_SOCKET_CLIENT_DEBUG)
	{
		AELOGINFO("threadReceive(), receive thread stop !");
	}

	return (void*)nullptr;
}

NS_AE_END
