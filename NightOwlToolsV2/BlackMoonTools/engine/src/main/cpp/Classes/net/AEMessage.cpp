#include "net/AEMessage.h"

NS_AE_BEGIN

uint32 AEMessage::s_iSequenceID = 0;
uint32 AEMessage::s_iUserID = 0;
uint32 AEMessage::s_iSessionID = 0;

AEMessage::AEMessage()
: m_iProtocolID(0)
, m_iSequenceID(0)
, m_iExtension(0)
, m_iBodyLenght(0)
, m_iResult(0)
{

}

AEMessage::AEMessage(uint32 capacity)
: m_iProtocolID(0)
, m_iSequenceID(0)
, m_iExtension(0)
, m_iBodyLenght(0)
, m_iResult(0)
{
	m_pByteBuffer = new AEByteBuffer(capacity);
}

AEMessage::AEMessage(uint32 capacity, uint32 protocolId)
: m_iProtocolID(protocolId)
, m_iSequenceID(0)
, m_iExtension(0)
, m_iBodyLenght(0)
, m_iResult(0)
{
	m_pByteBuffer = new AEByteBuffer(capacity);
}

AEMessage::AEMessage(AEByteBuffer* pByteBuffer)
: m_iProtocolID(0)
, m_iSequenceID(0)
, m_iExtension(0)
, m_iBodyLenght(0)
, m_iResult(0)
{
	m_pByteBuffer = pByteBuffer;
}

AEMessage::AEMessage(const char* buffer, uint32 offset, uint32 capacity)
: m_iProtocolID(0)
, m_iSequenceID(0)
, m_iExtension(0)
, m_iBodyLenght(0)
, m_iResult(0)
{
	m_pByteBuffer = new AEByteBuffer(buffer, offset, capacity);
}

AEMessage::~AEMessage()
{
	AESafe_delete(m_pByteBuffer);
}

AEMessage* AEMessage::send(uint32 capacity, uint32 protocolId)
{
	AEMessage* pMessage = new AEMessage(s_iSendHeaderLenght + capacity, protocolId);
	pMessage->setSequenceID(++s_iSequenceID);
	pMessage->setBodyLenght(capacity);
	pMessage->composite();
	return pMessage;
}

AEMessage* AEMessage::receive(uint32 capacity)
{
	return new AEMessage(capacity);
}

AEMessage* AEMessage::errorMessage(int32 errorCode)
{
	AEMessage* pMessage = AEMessage::receive(20);
	pMessage->m_pByteBuffer->writeUint32(1); // protocol
	pMessage->m_pByteBuffer->writeUint32(0); // sequence
	pMessage->m_pByteBuffer->writeUint32(0); // extension
	pMessage->m_pByteBuffer->writeInt32(errorCode); // result
	pMessage->m_pByteBuffer->flip();
	return pMessage;
}

void AEMessage::composite() const
{
	m_pByteBuffer->writeUint32(m_pByteBuffer->getCapacity());
	m_pByteBuffer->writeUint32(m_iProtocolID);
	m_pByteBuffer->writeUint32(m_iSequenceID);
	m_pByteBuffer->writeUint32(m_iExtension);
	m_pByteBuffer->writeUint32(s_iUserID);
	m_pByteBuffer->writeUint32(s_iSessionID);
}

void AEMessage::parse()
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

NS_AE_END
