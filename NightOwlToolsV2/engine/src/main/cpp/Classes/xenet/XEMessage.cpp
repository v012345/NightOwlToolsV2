#include "xenet/XEMessage.h"
#include <iostream>

NS_XE_BEGIN

uint32 XEMessage::s_iSequenceID = 0;
uint32 XEMessage::s_iUserID = 0;
uint32 XEMessage::s_iSessionID = 0;

XEMessage::XEMessage()
: m_iProtocolID(0)
, m_iSequenceID(0)
, m_iExtension(0)
, m_iBodyLenght(0)
, m_iResult(0)
{

}

XEMessage::XEMessage(uint32 capacity)
: m_iProtocolID(0)
, m_iSequenceID(0)
, m_iExtension(0)
, m_iBodyLenght(0)
, m_iResult(0)
{
	m_pByteBuffer = new XEByteBuffer(capacity);
}

XEMessage::XEMessage(uint32 capacity, uint32 protocolId)
: m_iProtocolID(protocolId)
, m_iSequenceID(0)
, m_iExtension(0)
, m_iBodyLenght(0)
, m_iResult(0)
{
	m_pByteBuffer = new XEByteBuffer(capacity);
}

XEMessage::XEMessage(XEByteBuffer* pByteBuffer)
: m_iProtocolID(0)
, m_iSequenceID(0)
, m_iExtension(0)
, m_iBodyLenght(0)
, m_iResult(0)
{
	m_pByteBuffer = pByteBuffer;
}

XEMessage::XEMessage(const char* buffer, uint32 offset, uint32 capacity)
: m_iProtocolID(0)
, m_iSequenceID(0)
, m_iExtension(0)
, m_iBodyLenght(0)
, m_iResult(0)
{
	//std::cout << "XEMessage::XEMessage: >>>>>>>>>>>>>>>>>>>>" << endl;
	m_pByteBuffer = new XEByteBuffer(buffer, offset, capacity);
}

XEMessage::~XEMessage()
{
	AESafe_delete(m_pByteBuffer);
}

XEMessage* XEMessage::send(uint32 capacity, uint32 protocolId)
{
	XEMessage* pMessage = new XEMessage(s_iSendHeaderLenght + capacity, protocolId);
	pMessage->setSequenceID(++s_iSequenceID);
	pMessage->setBodyLenght(capacity);
	pMessage->composite();
	return pMessage;
}

XEMessage* XEMessage::receive(uint32 capacity)
{
	return new XEMessage(capacity);
}

XEMessage* XEMessage::errorMessage(int32 errorCode, uint32 protocolId/* = 1*/)
{
	std::cout << "XEMessage::errorMessage: >>>>>>>>>>>>>>>>>>>>errorCode:" << errorCode << " protocolId:" << protocolId << endl;
	XEMessage* pMessage = XEMessage::receive(20);
	pMessage->m_pByteBuffer->writeUint32(protocolId); // protocol
	pMessage->m_pByteBuffer->writeUint32(0); // sequence
	pMessage->m_pByteBuffer->writeUint32(0); // extension
	pMessage->m_pByteBuffer->writeInt32(errorCode); // result
	pMessage->m_pByteBuffer->flip();
	
	return pMessage;
}
	
XEMessage* XEMessage::errorMessage(int32 errorCode, uint32 protocolId, uint32 msg_seq)
{
	std::cout << "XEMessage::errorMessage: >>>>>>>>>>>>>>>>>>>>errorCode:" << errorCode << " protocolId:" << protocolId << endl;
	XEMessage* pMessage = XEMessage::receive(20);
	pMessage->m_pByteBuffer->writeUint32(protocolId); // protocol
	pMessage->m_pByteBuffer->writeUint32(msg_seq); // sequence
	pMessage->m_pByteBuffer->writeUint32(0); // extension
	pMessage->m_pByteBuffer->writeInt32(errorCode); // result
	pMessage->m_pByteBuffer->flip();
	return pMessage;
}

void XEMessage::composite() const
{
	m_pByteBuffer->writeUint32(m_pByteBuffer->getCapacity());
	m_pByteBuffer->writeUint32(m_iProtocolID);
	m_pByteBuffer->writeUint32(m_iSequenceID);
	m_pByteBuffer->writeUint32(m_iExtension);
	m_pByteBuffer->writeUint32(s_iUserID);
	m_pByteBuffer->writeUint32(s_iSessionID);
}

XEMessage* XEMessage::clone() const
{
	if (NULL == m_pByteBuffer)
		return NULL;
	
	uint32 capcity_size = m_pByteBuffer->getCapacity();
	XEMessage* p_ret = new XEMessage(capcity_size);
	
	if (0 != m_iProtocolID)
	{
		p_ret->m_iBodyLenght = m_iBodyLenght;
		p_ret->m_iProtocolID = m_iProtocolID;
		p_ret->m_iSequenceID = m_iSequenceID;
		p_ret->m_iExtension = m_iExtension;
		if (0 != m_iProtocolID % 2)
		{
			p_ret->m_iResult = m_iResult;
		}
	}
	// bytes 
	XEByteBuffer* p_new_buffer = p_ret->getByteBuffer(),
				* p_old_buffer = m_pByteBuffer;
	memcpy((void*)p_new_buffer->getBuffer(), p_old_buffer->getBuffer(), capcity_size);
	
	return p_ret;
}

void XEMessage::parse()
{
	m_iProtocolID = m_pByteBuffer->readUint32();
	m_iSequenceID = m_pByteBuffer->readUint32();
	m_iExtension = m_pByteBuffer->readUint32();

	if (0 == m_iProtocolID % 2)
	{
		m_pByteBuffer->readUint32();
		m_pByteBuffer->readUint32();
	}
	else
	{
		m_iResult = m_pByteBuffer->readInt32();
	}
}

void XEMessage::release()
{
	delete this;
}
NS_XE_END
