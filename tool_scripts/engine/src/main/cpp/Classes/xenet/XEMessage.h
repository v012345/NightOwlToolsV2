/**************************************************************************
	*  @Copyright (c) 2015, A-one Soft, All rights reserved.

	*  @file			: XEMessage.h
	*  @version	: ver 1.0.0

	*  @author	: andy-zhao
	*  @date		: 2015/12/22
	*  @brief		: 消息体
**************************************************************************/
#ifndef _XEMessage_H_
#define _XEMessage_H_

#include "xenet/XEByteBuffer.h"

NS_XE_BEGIN
	
	
struct Aone2CmdHeader_
{
	uint32 m_cmdNum;  // 消息编号
	uint32 m_cmdSeq;  // 消息序列号
	uint32 m_reserve;  // 保留字段
};
	
class XEMessage
{
	// 发送消息，头长度
	static const int s_iSendHeaderLenght = 24;
	// 接收消息，头长度
	static const int s_iReceiveHeaderLenght = 20;

public:
	XEMessage();
	XEMessage(uint32 capacity);
	XEMessage(uint32 capacity, uint32 protocolId);
	XEMessage(XEByteBuffer* pByteBuffer);
	XEMessage(const char* buffer, uint32 offset, uint32 capacity);
	virtual ~XEMessage();

	/**
		构造发送时的消息。
	*/
	static XEMessage* send(uint32 capacity, uint32 protocolId);

	/**
		构造接收时的消息。
	*/
	static XEMessage* receive(uint32 capacity);

	/**
		XESocketClient错误信息。
	*/
	static XEMessage* errorMessage(int32 errorCode, uint32 protocolId = 1);
	static XEMessage* errorMessage(int32 errorCode, uint32 protocolId, uint32 msg_seq);
	
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
		复制一条消息
	*/
	XEMessage* clone() const;
	
	/**
		解析消息头。
	*/ 
	void parse();

	/**
		删除
	*/
	void release();

	// 消息体数据缓存
	XE_SYNTHESIZE_READONLY(XEByteBuffer*, m_pByteBuffer, ByteBuffer);
	// 消息号
	XE_SYNTHESIZE_REF(uint32, m_iProtocolID, ProtocolID);
	// 消息队列号
	XE_SYNTHESIZE_REF(uint32, m_iSequenceID, SequenceID);
	// 消息备用
	XE_SYNTHESIZE_REF(uint32, m_iExtension, Extension);
	// 消息体长度
	XE_SYNTHESIZE_REF(uint32, m_iBodyLenght, BodyLenght);
	// 消息结果
	XE_SYNTHESIZE_REF(int32, m_iResult, Result);

protected:
private:
	//消息队列号
	static uint32 s_iSequenceID;
	// 消息用户ID
	static uint32 s_iUserID;
	// 消息会话ID
	static uint32 s_iSessionID;

};

NS_XE_END

#endif // _XEMessage_H_