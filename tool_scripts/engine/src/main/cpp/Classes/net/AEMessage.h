/**************************************************************************
	*  @Copyright (c) 2015, A-one Soft, All rights reserved.

	*  @file			: AEValue.h
	*  @version	: ver 1.0.0

	*  @author	: andy-zhao
	*  @date		: 2015/12/22
	*  @brief		: 数据类型
**************************************************************************/
#ifndef _AEMessage_H_
#define _AEMessage_H_

#include "common/AEByteBuffer.h"

NS_AE_BEGIN

class AEMessage
{
	// 发送消息，头长度
	static const int s_iSendHeaderLenght = 24;
	// 接收消息，头长度
	static const int s_iReceiveHeaderLenght = 20;

public:
	AEMessage();
	AEMessage(uint32 capacity);
	AEMessage(uint32 capacity, uint32 protocolId);
	AEMessage(AEByteBuffer* pByteBuffer);
	AEMessage(const char* buffer, uint32 offset, uint32 capacity);
	virtual ~AEMessage();

	/**
		构造发送时的消息。
	*/
	static AEMessage* send(uint32 capacity, uint32 protocolId);

	/**
		构造接收时的消息。
	*/
	static AEMessage* receive(uint32 capacity);

	/**
		AESocketClient错误信息。
	*/
	static AEMessage* errorMessage(int32 errorCode);

	/**
		用户ID。
	*/
	static uint32 getUserID() { return s_iUserID; };
	static void setUserID(uint32 userId) { s_iUserID = userId; };

	/**
		会话ID。
	*/
	static uint32 getSessionID() { return s_iSessionID; };
	static void setSessionID(uint32 sessionId) { s_iSessionID = sessionId; };

	/**
		合成消息头。
	*/ 
	void composite() const;
	
	/**
		解析消息头。
	*/ 
	void parse();

	// 消息体数据缓存
	AE_SYNTHESIZE_READONLY(AEByteBuffer*, m_pByteBuffer, ByteBuffer);
	// 消息号
	AE_SYNTHESIZE_REF(uint32, m_iProtocolID, ProtocolID);
	// 消息队列号
	AE_SYNTHESIZE_REF(uint32, m_iSequenceID, SequenceID);
	// 消息备用
	AE_SYNTHESIZE_REF(uint32, m_iExtension, Extension);
	// 消息体长度
	AE_SYNTHESIZE_REF(uint32, m_iBodyLenght, BodyLenght);
	// 消息结果
	AE_SYNTHESIZE_REF(int32, m_iResult, Result);

protected:
private:
	//消息队列号
	static uint32 s_iSequenceID;
	// 消息用户ID
	static uint32 s_iUserID;
	// 消息会话ID
	static uint32 s_iSessionID;

};

NS_AE_END

#endif // _AEMessage_H_