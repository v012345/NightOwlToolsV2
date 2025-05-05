#include "common/AEByteBuffer.h"

NS_AE_BEGIN

const AEByteBuffer AEByteBuffer::ZERO;

AEByteBuffer::AEByteBuffer()
: m_buffer(nullptr)
, m_position(nullptr)
, m_limit(nullptr)
, m_capacity(0)
{

}

AEByteBuffer::AEByteBuffer(uint32 capacity)
{
	m_capacity = capacity;
	m_buffer = new char[m_capacity];
	m_position = m_buffer;
	m_limit = m_buffer + m_capacity;
}

AEByteBuffer::AEByteBuffer(const char* buffer, uint32 capacity)
{
	m_capacity = capacity;
	m_buffer = (char*)buffer;
	m_position = m_buffer;
	m_limit = m_buffer + m_capacity;
}

AEByteBuffer::AEByteBuffer(const char* buffer, uint32 offset, uint32 capacity)
{
	m_capacity = capacity;
	m_buffer = new char[m_capacity];
	memcpy(m_buffer, buffer + offset, m_capacity);
	m_position = m_buffer;
	m_limit = m_buffer + m_capacity;
}

AEByteBuffer::~AEByteBuffer()
{
	if (m_buffer)
	{
		delete[] m_buffer;
		m_buffer = nullptr;
	}
}

void AEByteBuffer::clear()
{
	m_position = m_buffer;
	m_limit = m_buffer + m_capacity;
}

void AEByteBuffer::flip()
{
	m_limit = m_position;
	m_position = m_buffer;
}

void AEByteBuffer::compact()
{
	uint32 pos = getPosition();
	uint32 l = getLimit();
	if (pos > 0)
	{
		for (uint32 i = pos; i < l; i++)
		{
			m_buffer[i - pos] = m_buffer[i];
		}
	}
	m_position = m_buffer + (l - pos);
	m_limit = m_buffer + m_capacity;
}

bool AEByteBuffer::setPosition(uint32 value)
{
	if (value > getLimit()) return false;
	m_position = m_buffer + value;
	return true;
}

bool AEByteBuffer::getBuffer(char* buffer, uint32 length)
{
	return getBuffer(buffer, 0, length);
}

bool AEByteBuffer::getBuffer(char* buffer, uint32 offset, uint32 limit)
{
	if (getPosition() + limit > getLimit())
	{
		memset(buffer + offset, 0, limit);
		return false;
	}
	memcpy(buffer + offset, m_position, limit);
	m_position += limit;
	return true;
}

bool AEByteBuffer::setBuffer(const char* buffer, uint32 offset, uint32 limit)
{
	if (getPosition() + limit > m_capacity) return false;
	memcpy(m_position, buffer + offset, limit);
	m_position += limit;
	return true;
}

bool AEByteBuffer::getBool(bool& value)
{
	if (getPosition() + 1 > getLimit()) return false;
	value = (*(bool*)m_position++ != 0);
	return true;
}

bool AEByteBuffer::setBool(const bool& value)
{
	if (getPosition() + 1 > m_capacity) return false;
	value ? (*(bool*)m_position) = (1 & 0xff) : (*m_position) = (0 & 0xff);
	m_position += 1;
	return true;
}

bool AEByteBuffer::getArrayBool(std::vector<bool>& values)
{
	uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayBool(values, length);
}

bool AEByteBuffer::getArrayBool(std::vector<bool>& values, uint32 lenght)
{
	values.resize(lenght);
	for (uint32 i = 0; i < lenght; i++)
	{
		bool value = false;
		if (!getBool(value)) return false;
		values[i] = value;
	}
	return true;
}

bool AEByteBuffer::setArrayBool(std::vector<bool>& values)
{
	uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (uint32 i = 0; i < values.size(); i++)
	{
		bool value = values[i];
		if (!setBool(value)) return false;
	}
	return true;
}

bool AEByteBuffer::getInt8(int8& value)
{
	if (getPosition() + 1 > getLimit()) return false;
	value = *(int8*)m_position++;
	return true;
}

bool AEByteBuffer::setInt8(const int8& value)
{
	if (getPosition() + 1 > m_capacity) return false;
	(*(int8*)m_position) = value;
	m_position += 1;
	return true;
}

bool AEByteBuffer::getArrayInt8(std::vector<int8>& values)
{
	uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayInt8(values, length);
}

bool AEByteBuffer::getArrayInt8(std::vector<int8>& values, uint32 lenght)
{
	values.resize(lenght);
	for (uint32 i = 0; i < lenght; i++)
	{
		int8 value = 0;
		if (!getInt8(value)) return false;
		values[i] = value;
	}
	return true;
}

bool AEByteBuffer::setArrayInt8(std::vector<int8>& values)
{
	uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (uint32 i = 0; i < values.size(); i++)
	{
		int8 value = values[i];
		if (!setInt8(value)) return false;
	}
	return true;
}

bool AEByteBuffer::getByte(byte& value)
{
	if (getPosition() + 1 > getLimit()) return false;
	value = *(byte*)m_position++;
	return true;
}

bool AEByteBuffer::setByte(const byte& value)
{
	if (getPosition() + 1 > m_capacity) return false;
	(*(byte*)m_position) = value;
	m_position += 1;
	return true;
}

bool AEByteBuffer::getArrayByte(std::vector<byte>& values)
{
	uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayByte(values, length);
}

bool AEByteBuffer::getArrayByte(std::vector<byte>& values, uint32 lenght)
{
	values.resize(lenght);
	for (uint32 i = 0; i < lenght; i++)
	{
		byte value = 0;
		if (!getByte(value)) return false;
		values[i] = value;
	}
	return true;
}

bool AEByteBuffer::setArrayByte(std::vector<byte>& values)
{
	uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (uint32 i = 0; i < values.size(); i++)
	{
		byte value = values[i];
		if (!setByte(value)) return false;
	}
	return true;
}

bool AEByteBuffer::getInt16(int16& value)
{
	if (getPosition() + 2 > getLimit()) return false;
#if AE_BYTE_BUFFER == AE_BYTE_BUFFER_JAVA
	for (byte i = 0; i < 2; i++)
	{
		value |= (((*m_position) & 0xFF) << (8 * (1 - i)));
		m_position++;
	}
#else
	value = *(int16*)m_position;
	m_position += 2;
#endif
	return true;
}

bool AEByteBuffer::setInt16(const int16& value)
{
	if (getPosition() + 2 > m_capacity) return false;
#if AE_BYTE_BUFFER == AE_BYTE_BUFFER_JAVA
	for (byte i = 0; i < 2; i++)
	{
		(*m_position) = (char)((value >> (8 * (1 - i))) & 0xFF);
		m_position++;
	}
#else
	(*(int16*)m_position) = value;
	m_position += 2;
#endif
	return true;
}

bool AEByteBuffer::getArrayInt16(std::vector<int16>& values)
{
	uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayInt16(values, length);
}

bool AEByteBuffer::getArrayInt16(std::vector<int16>& values, uint32 lenght)
{
	values.resize(lenght);
	for (uint32 i = 0; i < lenght; i++)
	{
		int16 value = 0;
		if (!getInt16(value)) return false;
		values[i] = value;
	}
	return true;
}

bool AEByteBuffer::setArrayInt16(std::vector<int16>& values)
{
	uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (uint32 i = 0; i < values.size(); i++)
	{
		int16 value = values[i];
		if (!setInt16(value)) return false;
	}
	return true;
}

bool AEByteBuffer::getUint16(uint16& value)
{
	if (getPosition() + 2 > getLimit()) return false;
#if AE_BYTE_BUFFER == AE_BYTE_BUFFER_JAVA
	for (byte i = 0; i < 2; i++)
	{
		value |= (((*m_position) & 0xFF) << (8 * (1 - i)));
		m_position++;
	}
#else
	value = *(uint16*)m_position;
	m_position += 2;
#endif
	return true;
}

bool AEByteBuffer::setUint16(const uint16& value)
{
	if (getPosition() + 2 > m_capacity) return false;
#if AE_BYTE_BUFFER == AE_BYTE_BUFFER_JAVA
	for (byte i = 0; i < 2; i++)
	{
		(*m_position) = (char)((value >> (8 * (1 - i))) & 0xFF);
		m_position++;
	}
#else
	(*(uint16*)m_position) = value;
	m_position += 2;
#endif
	return true;
}

bool AEByteBuffer::getArrayUint16(std::vector<uint16>& values)
{
	uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayUint16(values, length);
}

bool AEByteBuffer::getArrayUint16(std::vector<uint16>& values, uint32 lenght)
{
	values.resize(lenght);
	for (uint32 i = 0; i < lenght; i++)
	{
		uint16 value = 0;
		if (!getUint16(value)) return false;
		values[i] = value;
	}
	return true;
}

bool AEByteBuffer::setArrayUint16(std::vector<uint16>& values)
{
	uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (uint32 i = 0; i < values.size(); i++)
	{
		uint16 value = values[i];
		if (!setUint16(value)) return false;
	}
	return true;
}

bool AEByteBuffer::getInt32(int32& value)
{
	if (getPosition() + 4 > getLimit()) return false;
#if AE_BYTE_BUFFER == AE_BYTE_BUFFER_JAVA
	for (byte i = 0; i < 4; i++)
	{
		value |= (((*m_position) & 0xFF) << (8 *(3 - i)));
		m_position++;
	}
#else
	value = *(int32*)m_position;
	m_position += 4;
#endif
	return true;
}

bool AEByteBuffer::setInt32(const int32& value)
{
	if (getPosition() + 4 > m_capacity) return false;
#if AE_BYTE_BUFFER == AE_BYTE_BUFFER_JAVA
	for (byte i = 0; i < 4; i++)
	{
		(*m_position) = (char)((value >> (8 * (3 - i))) & 0xFF);
		m_position++;
	}
#else
	(*(int32*)m_position) = value;
	m_position += 4;
#endif
	return true;
}

bool AEByteBuffer::getArrayInt32(std::vector<int32>& values)
{
	uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayInt32(values, length);
}

bool AEByteBuffer::getArrayInt32(std::vector<int32>& values, uint32 lenght)
{
	values.resize(lenght);
	for (uint32 i = 0; i < lenght; i++)
	{
		int32 value = 0;
		if (!getInt32(value)) return false;
		values[i] = value;
	}
	return true;
}

bool AEByteBuffer::setArrayInt32(std::vector<int32>& values)
{
	uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (uint32 i = 0; i < values.size(); i++)
	{
		int32 value = values[i];
		if (!setInt32(value)) return false;
	}
	return true;
}

bool AEByteBuffer::getUint32(uint32& value)
{
	if (getPosition() + 4 > getLimit()) return false;
#if AE_BYTE_BUFFER == AE_BYTE_BUFFER_JAVA
	for (byte i = 0; i < 4; i++)
	{
		value |= (((*m_position) & 0xFF) << (8 * (3 - i)));
		m_position++;
	}
#else
	value = *(uint32*)m_position;
	m_position += 4;
#endif
	return true;
}
bool AEByteBuffer::setUint32(const uint32& value)
{
	if (getPosition() + 4 > m_capacity) return false;
#if AE_BYTE_BUFFER == AE_BYTE_BUFFER_JAVA
	for (byte i = 0; i < 4; i++)
	{
		(*m_position) = (char)((value >> (8 * (3 - i))) & 0xFF);
		m_position++;
	}
#else
	(*(uint32*)m_position) = value;
	m_position += 4;
#endif
	return true;
}

bool AEByteBuffer::getArrayUint32(std::vector<uint32>& values)
{
	uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayUint32(values, length);
}

bool AEByteBuffer::getArrayUint32(std::vector<uint32>& values, uint32 lenght)
{
	values.resize(lenght);
	for (uint32 i = 0; i < lenght; i++)
	{
		uint32 value = 0;
		if (!getUint32(value)) return false;
		values[i] = value;
	}
	return true;
}

bool AEByteBuffer::setArrayUint32(std::vector<uint32>& values)
{
	uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (uint32 i = 0; i < values.size(); i++)
	{
		uint32 value = values[i];
		if (!setUint32(value)) return false;
	}
	return true;
}

bool AEByteBuffer::getInt64(int64& value)
{
	if (getPosition() + 8 > getLimit()) return false;
#if AE_BYTE_BUFFER == AE_BYTE_BUFFER_JAVA
	for (byte i = 0; i < 8; i++)
	{
		value |= (((*m_position) & 0xFF) << (8 *(7 - i)));
		m_position++;
	}
#else
	value = *(int64*)m_position;
	m_position += 8;
#endif
	return true;
}

bool AEByteBuffer::setInt64(const int64 & value)
{
	if (getPosition() + 8 > m_capacity) return false;
#if AE_BYTE_BUFFER == AE_BYTE_BUFFER_JAVA
	for (byte i = 0; i < 8; i++)
	{
		(*m_position) = (char)((value >> (8 * (7 - i))) & 0xFF);
		m_position++;
	}
#else
	(*(int64*)m_position) = value;
	m_position += 8;
#endif
	return true;
}

bool AEByteBuffer::getArrayInt64(std::vector<int64>& values)
{
	uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayInt64(values, length);
}

bool AEByteBuffer::getArrayInt64(std::vector<int64>& values, uint32 lenght)
{
	values.resize(lenght);
	for (uint32 i = 0; i < lenght; i++)
	{
		int64 value = 0;
		if (!getInt64(value)) return false;
		values[i] = value;
	}
	return true;
}

bool AEByteBuffer::setArrayInt64(std::vector<int64>& values)
{
	uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (uint32 i = 0; i < values.size(); i++)
	{
		int64 value = values[i];
		if (!setInt64(value)) return false;
	}
	return true;
}

bool AEByteBuffer::getUint64(uint64& value)
{
	if (getPosition() + 8 > getLimit()) return false;
#if AE_BYTE_BUFFER == AE_BYTE_BUFFER_JAVA
	for (byte i = 0; i < 8; i++)
	{
		value |= (((*m_position) & 0xFF) << (8 *(7 - i)));
		m_position++;
	}
#else
	value = *(uint64*)m_position;
	m_position += 8;
#endif
	return true;
}

bool AEByteBuffer::setUint64(const uint64& value)
{
	if (getPosition() + 4 > m_capacity) return false;
#if AE_BYTE_BUFFER == AE_BYTE_BUFFER_JAVA
	for (byte i = 0; i < 8; i++)
	{
		(*m_position) = (char)((value >> (8 * (7 - i))) & 0xFF);
		m_position++;
	}
#else
	(*(uint64*)m_position) = value;
	m_position += 8;
#endif
	return true;
}

bool AEByteBuffer::getArrayUnt64(std::vector<uint64>& values)
{
	uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayUnt64(values, length);
}

bool AEByteBuffer::getArrayUnt64(std::vector<uint64>& values, uint32 lenght)
{
	values.resize(lenght);
	for (uint32 i = 0; i < lenght; i++)
	{
		uint64 value = 0;
		if (!getUint64(value)) return false;
		values[i] = value;
	}
	return true;
}

bool AEByteBuffer::setArrayUnt64(std::vector<uint64>& values)
{
	uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (uint32 i = 0; i < values.size(); i++)
	{
		uint64 value = values[i];
		if (!setUint64(value)) return false;
	}
	return true;
}

bool AEByteBuffer::getFloat32(float32& value)
{
	if (getPosition() + 4 > getLimit()) return false;
#if AE_BYTE_BUFFER == AE_BYTE_BUFFER_JAVA
	unsigned char* bytes;
	bytes = (unsigned char*)&value;
	bytes[3] = (*m_position++);
	bytes[2] = (*m_position++);
	bytes[1] = (*m_position++);
	bytes[0] = (*m_position++);
#else
	value = *(float32*)m_position;
	m_position += 4;
#endif
	return true;
}

bool AEByteBuffer::setFloat32(const float32& value)
{
	if (getPosition() + 4 > m_capacity) return false;
#if AE_BYTE_BUFFER == AE_BYTE_BUFFER_JAVA
	unsigned char* bytes;
	bytes = (unsigned char*)&value;
	(*m_position++) = bytes[3];
	(*m_position++) = bytes[2];
	(*m_position++) = bytes[1];
	(*m_position++) = bytes[0];
#else
	(*(float32*)m_position) = value;
	m_position += 4;
#endif
	return true;
}

bool AEByteBuffer::getArrayFloat32(std::vector<float32>& values)
{
	uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayFloat32(values, length);
}

bool AEByteBuffer::getArrayFloat32(std::vector<float32>& values, uint32 lenght)
{
	values.resize(lenght);
	for (uint32 i = 0; i < lenght; i++)
	{
		float32 value = 0;
		if (!getFloat32(value)) return false;
		values[i] = value;
	}
	return true;
}

bool AEByteBuffer::setArrayFloat32(std::vector<float32>& values)
{
	uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (uint32 i = 0; i < values.size(); i++)
	{
		float32 value = values[i];
		if (!setFloat32(value)) return false;
	}
	return true;
}

bool AEByteBuffer::getFloat64(float64& value)
{
	if (getPosition() + 8 > getLimit()) return false;
#if AE_BYTE_BUFFER == AE_BYTE_BUFFER_JAVA
	unsigned char* bytes;
	bytes = (unsigned char*)&value;
	bytes[7] = (*m_position++);
	bytes[6] = (*m_position++);
	bytes[5] = (*m_position++);
	bytes[4] = (*m_position++);
	bytes[3] = (*m_position++);
	bytes[2] = (*m_position++);
	bytes[1] = (*m_position++);
	bytes[0] = (*m_position++);
#else
	value = *(float64*)m_position;
	m_position += 8;
#endif
	return true;
}

bool AEByteBuffer::setFloat64(const float64& value)
{
	if (getPosition() + 8 > m_capacity) return false;
#if AE_BYTE_BUFFER == AE_BYTE_BUFFER_JAVA
	unsigned char* bytes;
	bytes = (unsigned char*)&value;
	(*m_position++) = bytes[7];
	(*m_position++) = bytes[6];
	(*m_position++) = bytes[5];
	(*m_position++) = bytes[4];
	(*m_position++) = bytes[3];
	(*m_position++) = bytes[2];
	(*m_position++) = bytes[1];
	(*m_position++) = bytes[0];
#else
	(*(float64*)m_position) = value;
	m_position += 8;
#endif
	return true;
}

bool AEByteBuffer::getArrayFloat64(std::vector<float64>& values)
{
	uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayFloat64(values, length);
}

bool AEByteBuffer::getArrayFloat64(std::vector<float64>& values, uint32 lenght)
{
	values.resize(lenght);
	for (uint32 i = 0; i < lenght; i++)
	{
		float64 value = 0;
		if (!getFloat64(value)) return false;
		values[i] = value;
	}
	return true;
}

bool AEByteBuffer::setArrayFloat64(std::vector<float64>& values)
{
	uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (uint32 i = 0; i < values.size(); i++)
	{
		float64 value = values[i];
		if (!setFloat64(value)) return false;
	}
	return true;
}

bool AEByteBuffer::getString(std::string& value)
{
	if (getPosition() + 4 > getLimit()) return false;
	uint32 length = 0;
	getUint32(length);
	if (length > 0)
	{
		value.append(m_position, length);
		m_position += length;
	}
	else
	{
		value = "";
	}
	return true;
}

bool AEByteBuffer::setString(const std::string& value)
{
	uint32 length = value.length();
	if (getPosition() + 4 + length > m_capacity) return false;
	setUint32(length);
	if (length > 0)
	{
		setBuffer(value.c_str(), 0, length);
	}
	return true;
}

bool AEByteBuffer::getArrayString(std::vector<std::string>& values)
{
	uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayString(values, length);
}

bool AEByteBuffer::getArrayString(std::vector<std::string>& values, uint32 lenght)
{
	values.resize(lenght);
	for (uint32 i = 0; i < lenght; i++)
	{
		std::string value;
		if (!getString(value)) return false;
		values[i] = value;
	}
	return true;

}

bool AEByteBuffer::setArrayString(std::vector<std::string>& values)
{
	uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (uint32 i = 0; i < values.size(); i++)
	{
		std::string value = values[i];
		if (!setString(value)) return false;
	}
	return true;
}

bool& AEByteBuffer::readBool()
{
	bool value = 0;
	getBool(value);
	return value;
}

int8& AEByteBuffer::readInt8()
{
	int8 value = 0;
	getInt8(value);
	return value;
}

byte& AEByteBuffer::readByte()
{
	byte value = 0;
	getByte(value);
	return value;
}

int16& AEByteBuffer::readInt16()
{
	int16 value = 0;
	getInt16(value);
	return value;
}

uint16& AEByteBuffer::readUint16()
{
	uint16 value = 0;
	getUint16(value);
	return (uint16&)value;
}

int32& AEByteBuffer::readInt32()
{
	int32 value = 0;
	getInt32(value);
	return (int32&)value;
}

uint32& AEByteBuffer::readUint32()
{
	uint32 value = 0;
	getUint32(value);
	return (uint32&)value;
}

float32& AEByteBuffer::readFloat32()
{
	float32 value = 0;
	getFloat32(value);
	return (float32&)value;
}

float64& AEByteBuffer::readFloat64()
{
	float64 value = 0;
	getFloat64(value);
	return (float64&)value;
}

std::string AEByteBuffer::readString()
{
	std::string value = "";
	getString(value);
	return value;
}


uint32 AEByteBuffer::sizeOfBool(bool val)
{
	return sizeof(byte);
}

uint32 AEByteBuffer::sizeOfByte(byte val)
{
	return sizeof(byte);
}

uint32 AEByteBuffer::sizeOfInt8(int8 val)
{
	return sizeof(int8);
}

uint32 AEByteBuffer::sizeOfInt16(int16 val)
{
	return sizeof(int16);
}

uint32 AEByteBuffer::sizeOfUint16(uint16 val)
{
	return sizeof(uint16);
}

uint32 AEByteBuffer::sizeOfInt32(int32 val)
{
	return sizeof(int32);
}

uint32 AEByteBuffer::sizeOfUint32(uint32 val)
{
	return sizeof(uint32);
}

uint32 AEByteBuffer::sizeOfInt64(int64 val)
{
	return sizeof(int64);
}

uint32 AEByteBuffer::sizeOfUint64(uint64 val)
{
	return sizeof(uint64);
}

uint32 AEByteBuffer::sizeOfFloat32(float32 val)
{
	return sizeof(float32);
}

uint32 AEByteBuffer::sizeOfFloat64(float64 val)
{
	return sizeof(float64);
}

uint32 AEByteBuffer::sizeOfDouble(double val)
{
	return sizeof(double);
}

uint32 AEByteBuffer::sizeOfString(const std::string& val)
{
	return sizeof(uint32)+val.size();
}

uint32 AEByteBuffer::sizeOfBoolArray(const bool* buf, uint32 len)
{
	return len * sizeof(byte);
}

uint32 AEByteBuffer::sizeOfByteArray(const byte* buf, uint32 len)
{
	return len * sizeof(byte);
}

uint32 AEByteBuffer::sizeOfInt8Array(const int8* buf, uint32 len)
{
	return len * sizeof(int8);
}

uint32 AEByteBuffer::sizeOfInt16Array(const int16* buf, uint32 len)
{
	return len * sizeof(int16);
}

uint32 AEByteBuffer::sizeOfUint16Array(const uint16* buf, uint32 len)
{
	return len * sizeof(uint16);
}

uint32 AEByteBuffer::sizeOfInt32Array(const int32* buf, uint32 len)
{
	return len * sizeof(int32);
}

uint32 AEByteBuffer::sizeOfUint32Array(const uint32* buf, uint32 len)
{
	return len * sizeof(uint32);
}

uint32 AEByteBuffer::sizeOfInt64Array(const int64* buf, uint32 len)
{
	return len * sizeof(int64);
}

uint32 AEByteBuffer::sizeOfUint64Array(const uint64* buf, uint32 len)
{
	return len * sizeof(uint32);
}

uint32 AEByteBuffer::sizeOfFloat32Array(const float32* buf, uint32 len)
{
	return len * sizeof(float32);
}

uint32 AEByteBuffer::sizeOfFloat64Array(const float64* buf, uint32 len)
{
	return len * sizeof(float64);
}

uint32 AEByteBuffer::sizeOfDoubleArray(const double* buf, uint32 len)
{
	return len * sizeof(double);
}

uint32 AEByteBuffer::sizeOfStringArray(const std::string* buf, uint32 len)
{
	uint32 sz = 0;
	for (uint32 i = 0; i < len; i++)
	{
		sz += sizeOfString(buf[i]);
	}
	return sz;
}

uint32 AEByteBuffer::sizeOfBoolVector(const vector<bool>& vals)
{
	uint32 sz = 0;
	uint32 len = vals.size();
	for (uint32 i = 0; i < len; i++)
	{
		sz += sizeOfBool(vals[i]);
	}
	return sz;
}

uint32 AEByteBuffer::sizeOfByteVector(const vector<byte>& vals)
{
	uint32 sz = 0;
	uint32 len = vals.size();
	for (uint32 i = 0; i < len; i++)
	{
		sz += sizeOfByte(vals[i]);
	}
	return sz;
}

uint32 AEByteBuffer::sizeOfInt8Vector(const vector<int8>& vals)
{
	uint32 sz = 0;
	uint32 len = vals.size();
	for (uint32 i = 0; i < len; i++)
	{
		sz += sizeOfInt8(vals[i]);
	}
	return sz;
}

uint32 AEByteBuffer::sizeOfInt16Vector(const vector<int16>& vals)
{
	uint32 sz = 0;
	uint32 len = vals.size();
	for (uint32 i = 0; i < len; i++)
	{
		sz += sizeOfInt16(vals[i]);
	}
	return sz;
}

uint32 AEByteBuffer::sizeOfUint16Vector(const vector<uint16>& vals)
{
	uint32 sz = 0;
	uint32 len = vals.size();
	for (uint32 i = 0; i < len; i++)
	{
		sz += sizeOfUint16(vals[i]);
	}
	return sz;
}

uint32 AEByteBuffer::sizeOfInt32Vector(const vector<int32>& vals)
{
	uint32 sz = 0;
	uint32 len = vals.size();
	for (uint32 i = 0; i < len; i++)
	{
		sz += sizeOfInt32(vals[i]);
	}
	return sz;
}
uint32 AEByteBuffer::sizeOfUint32Vector(const vector<uint32>& vals)
{
	uint32 sz = 0;
	uint32 len = vals.size();
	for (uint32 i = 0; i < len; i++)
	{
		sz += sizeOfUint32(vals[i]);
	}
	return sz;
}

uint32 AEByteBuffer::sizeOfInt64Vector(const vector<int64>& vals)
{
	uint32 sz = 0;
	uint32 len = vals.size();
	for (uint32 i = 0; i < len; i++)
	{
		sz += sizeOfInt64(vals[i]);
	}
	return sz;
}

uint32 AEByteBuffer::sizeOfUint64Vector(const vector<uint64>& vals)
{
	uint32 sz = 0;
	uint32 len = vals.size();
	for (uint32 i = 0; i < len; i++)
	{
		sz += sizeOfUint64(vals[i]);
	}
	return sz;
}

uint32 AEByteBuffer::sizeOfFloat32Vector(const vector<float32>& vals)
{
	uint32 sz = 0;
	uint32 len = vals.size();
	for (uint32 i = 0; i < len; i++)
	{
		sz += sizeOfFloat32(vals[i]);
	}
	return sz;
}

uint32 AEByteBuffer::sizeOfFloat64Vector(const vector<float64>& vals)
{
	uint32 sz = 0;
	uint32 len = vals.size();
	for (uint32 i = 0; i < len; i++)
	{
		sz += sizeOfFloat64(vals[i]);
	}
	return sz;
}

uint32 AEByteBuffer::sizeOfDoubleVector(const vector<double>& vals)
{
	uint32 sz = 0;
	uint32 len = vals.size();
	for (uint32 i = 0; i < len; i++)
	{
		sz += sizeOfDouble(vals[i]);
	}
	return sz;
}

uint32 AEByteBuffer::sizeOfStringVector(const vector<string>& vals)
{
	uint32 sz = 0;
	uint32 len = vals.size();
	for (uint32 i = 0; i < len; i++)
	{
		sz += sizeOfString(vals[i]);
	}
	return sz;
}

NS_AE_END
