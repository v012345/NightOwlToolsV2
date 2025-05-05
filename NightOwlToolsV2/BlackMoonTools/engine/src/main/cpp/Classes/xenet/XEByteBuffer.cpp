#include "xenet/XEByteBuffer.h"
#include <sys/stat.h>
#include "XERC4.h"

#define XERC4_CODE  "24920b8f1db741409fb53e57da71e60a"

NS_XE_BEGIN

const XEByteBuffer XEByteBuffer::ZERO;

XEByteBuffer::XEByteBuffer()
: m_buffer(nullptr)
, m_position(nullptr)
, m_limit(nullptr)
, m_capacity(0)
{

}

XEByteBuffer::XEByteBuffer(xe::uint32 capacity)
{
	m_capacity = capacity;
	m_buffer = new char[m_capacity];
	m_position = m_buffer;
	m_limit = m_buffer + m_capacity;
}

XEByteBuffer::XEByteBuffer(const char* buffer, xe::uint32 capacity)
{
	m_capacity = capacity;
	m_buffer = (char*)buffer;
	m_position = m_buffer;
	m_limit = m_buffer + m_capacity;
}

XEByteBuffer::XEByteBuffer(const char* buffer, xe::uint32 offset, xe::uint32 capacity)
{
	m_capacity = capacity;
	m_buffer = new char[m_capacity];
	memcpy(m_buffer, buffer + offset, m_capacity);
	m_position = m_buffer;
	m_limit = m_buffer + m_capacity;
}

XEByteBuffer::~XEByteBuffer()
{
	if (m_buffer)
	{
		delete[] m_buffer;
		m_buffer = nullptr;
	}
}

XEByteBuffer* XEByteBuffer::createByteBuffer(uint32 capacity)
{
	return new XEByteBuffer(capacity);
}

XEByteBuffer* XEByteBuffer::createByteBuffer()
{
	return new XEByteBuffer();
}

void XEByteBuffer::releaseByteBuffer(XEByteBuffer* pBuff)
{
	if (pBuff)
	{
		delete pBuff;
	}
}

bool XEByteBuffer::writeToFile(std::string& file)
{
	size_t size = getPosition();
	const char* mode = "wb";

	CCASSERT(!file.empty() && size != 0, "Invalid parameters.");

	auto fileutils = FileUtils::getInstance();
	do
	{
		// Read the file from hardware
		FILE *fp = fopen(fileutils->getSuitableFOpen(file).c_str(), mode);
		CC_BREAK_IF(!fp);

		xe::XERC4 rc4;
		rc4.init(XERC4_CODE);
		rc4.update((const xe::byte*)m_buffer, (xe::byte*)m_buffer, size);
		fwrite(m_buffer, size, 1, fp);

		fclose(fp);

	} while (0);

	return true;
}

bool XEByteBuffer::readToFile(std::string& filename)
{
	if (filename.empty())
		return false;

	auto fs = FileUtils::getInstance();

	std::string fullPath = fs->fullPathForFilename(filename);
	if (fullPath.empty())
		return false;

	FILE *fp = fopen(fs->getSuitableFOpen(fullPath).c_str(), "rb");
	if (!fp)
		return false;

#if defined(_MSC_VER)
	auto descriptor = _fileno(fp);
#else
	auto descriptor = fileno(fp);
#endif
	struct stat statBuf;
	if (fstat(descriptor, &statBuf) == -1) {
		fclose(fp);
		return false;
	}
	size_t size = statBuf.st_size;

	m_capacity = size;
	if (m_buffer)
	{
		delete[] m_buffer;
	}
	m_buffer = new char[m_capacity];
	m_position = m_buffer;
	m_limit = m_buffer + m_capacity;

	size_t readsize = fread(m_buffer, 1, size, fp);
	fclose(fp);

	if (readsize < size) {
		return false;
	}

	xe::XERC4 rc4;
	rc4.init(XERC4_CODE);
	rc4.update((const xe::byte*)m_buffer, (xe::byte*)m_buffer, size);

	return true;
}

void XEByteBuffer::clear()
{
	m_position = m_buffer;
	m_limit = m_buffer + m_capacity;
}

void XEByteBuffer::flip()
{
	m_limit = m_position;
	m_position = m_buffer;
}

void XEByteBuffer::compact()
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

bool XEByteBuffer::setPosition(xe::uint32 value)
{
	if (value > getLimit()) return false;
	m_position = m_buffer + value;
	return true;
}

bool XEByteBuffer::getBuffer(char* buffer, xe::uint32 length)
{
	return getBuffer(buffer, 0, length);
}

bool XEByteBuffer::getBuffer(char* buffer, xe::uint32 offset, xe::uint32 limit)
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

bool XEByteBuffer::setBuffer(const char* buffer, xe::uint32 offset, xe::uint32 limit)
{
	if (getPosition() + limit > m_capacity) return false;
	memcpy(m_position, buffer + offset, limit);
	m_position += limit;
	return true;
}

bool XEByteBuffer::getBool(bool& value)
{
	if (getPosition() + 1 > getLimit()) return false;
	value = (*(bool*)m_position++ != 0);
	return true;
}

bool XEByteBuffer::setBool(const bool& value)
{
	if (getPosition() + 1 > m_capacity) return false;
	value ? (*(bool*)m_position) = (1 & 0xff) : (*m_position) = (0 & 0xff);
	m_position += 1;
	return true;
}

bool XEByteBuffer::getArrayBool(std::vector<bool>& values)
{
	xe::uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayBool(values, length);
}

bool XEByteBuffer::getArrayBool(std::vector<bool>& values, xe::uint32 lenght)
{
	values.resize(lenght);
	for (xe::uint32 i = 0; i < lenght; i++)
	{
		bool value = false;
		if (!getBool(value)) return false;
		values[i] = value;
	}
	return true;
}

bool XEByteBuffer::setArrayBool(std::vector<bool>& values)
{
	xe::uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (xe::uint32 i = 0; i < values.size(); i++)
	{
		bool value = values[i];
		if (!setBool(value)) return false;
	}
	return true;
}

bool XEByteBuffer::getInt8(xe::int8& value)
{
	if (getPosition() + 1 > getLimit()) return false;
	value = *(xe::int8*)m_position++;
	return true;
}

bool XEByteBuffer::setInt8(const xe::int8& value)
{
	if (getPosition() + 1 > m_capacity) return false;
	(*(xe::int8*)m_position) = value;
	m_position += 1;
	return true;
}

bool XEByteBuffer::getArrayInt8(std::vector<xe::int8>& values)
{
	uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayInt8(values, length);
}

bool XEByteBuffer::getArrayInt8(std::vector<xe::int8>& values, xe::uint32 lenght)
{
	values.resize(lenght);
	for (xe::uint32 i = 0; i < lenght; i++)
	{
		xe::int8 value = 0;
		if (!getInt8(value)) return false;
		values[i] = value;
	}
	return true;
}

bool XEByteBuffer::setArrayInt8(std::vector<xe::int8>& values)
{
	xe::uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (xe::uint32 i = 0; i < values.size(); i++)
	{
		xe::int8 value = values[i];
		if (!setInt8(value)) return false;
	}
	return true;
}

bool XEByteBuffer::getByte(xe::byte& value)
{
	if (getPosition() + 1 > getLimit()) return false;
	value = *(xe::byte*)m_position++;
	return true;
}

bool XEByteBuffer::setByte(const xe::byte& value)
{
	if (getPosition() + 1 > m_capacity) return false;
	(*(xe::byte*)m_position) = value;
	m_position += 1;
	return true;
}

bool XEByteBuffer::getArrayByte(std::vector<xe::byte>& values)
{
	xe::uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayByte(values, length);
}

bool XEByteBuffer::getArrayByte(std::vector<xe::byte>& values, xe::uint32 lenght)
{
	values.resize(lenght);
	for (xe::uint32 i = 0; i < lenght; i++)
	{
		xe::byte value = 0;
		if (!getByte(value)) return false;
		values[i] = value;
	}
	return true;
}

bool XEByteBuffer::setArrayByte(std::vector<xe::byte>& values)
{
	xe::uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (xe::uint32 i = 0; i < values.size(); i++)
	{
		xe::byte value = values[i];
		if (!setByte(value)) return false;
	}
	return true;
}

bool XEByteBuffer::getInt16(xe::int16& value)
{
	if (getPosition() + 2 > getLimit()) return false;
#if XE_BYTE_BUFFER == XE_BYTE_BUFFER_JAVA
	for (xe::byte i = 0; i < 2; i++)
	{
		value |= (((*m_position) & 0xFF) << (8 * (1 - i)));
		m_position++;
	}
#else
	value = *(xe::int16*)m_position;
	m_position += 2;
#endif
	return true;
}

bool XEByteBuffer::setInt16(const xe::int16& value)
{
	if (getPosition() + 2 > m_capacity) return false;
#if XE_BYTE_BUFFER == XE_BYTE_BUFFER_JAVA
	for (byte i = 0; i < 2; i++)
	{
		(*m_position) = (char)((value >> (8 * (1 - i))) & 0xFF);
		m_position++;
	}
#else
	(*(xe::int16*)m_position) = value;
	m_position += 2;
#endif
	return true;
}

bool XEByteBuffer::getArrayInt16(std::vector<xe::int16>& values)
{
	xe::uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayInt16(values, length);
}

bool XEByteBuffer::getArrayInt16(std::vector<int16>& values, xe::uint32 lenght)
{
	values.resize(lenght);
	for (xe::uint32 i = 0; i < lenght; i++)
	{
		int16 value = 0;
		if (!getInt16(value)) return false;
		values[i] = value;
	}
	return true;
}

bool XEByteBuffer::setArrayInt16(std::vector<xe::int16>& values)
{
	xe::uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (xe::uint32 i = 0; i < values.size(); i++)
	{
		xe::int16 value = values[i];
		if (!setInt16(value)) return false;
	}
	return true;
}

bool XEByteBuffer::getUint16(xe::uint16& value)
{
	if (getPosition() + 2 > getLimit()) return false;
#if XE_BYTE_BUFFER == XE_BYTE_BUFFER_JAVA
	for (byte i = 0; i < 2; i++)
	{
		value |= (((*m_position) & 0xFF) << (8 * (1 - i)));
		m_position++;
	}
#else
	value = *(xe::uint16*)m_position;
	m_position += 2;
#endif
	return true;
}

bool XEByteBuffer::setUint16(const xe::uint16& value)
{
	if (getPosition() + 2 > m_capacity) return false;
#if XE_BYTE_BUFFER == XE_BYTE_BUFFER_JAVA
	for (byte i = 0; i < 2; i++)
	{
		(*m_position) = (char)((value >> (8 * (1 - i))) & 0xFF);
		m_position++;
	}
#else
	(*(xe::uint16*)m_position) = value;
	m_position += 2;
#endif
	return true;
}

bool XEByteBuffer::getArrayUint16(std::vector<xe::uint16>& values)
{
	xe::uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayUint16(values, length);
}

bool XEByteBuffer::getArrayUint16(std::vector<xe::uint16>& values, xe::uint32 lenght)
{
	values.resize(lenght);
	for (xe::uint32 i = 0; i < lenght; i++)
	{
		xe::uint16 value = 0;
		if (!getUint16(value)) return false;
		values[i] = value;
	}
	return true;
}

bool XEByteBuffer::setArrayUint16(std::vector<xe::uint16>& values)
{
	xe::uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (xe::uint32 i = 0; i < values.size(); i++)
	{
		xe::uint16 value = values[i];
		if (!setUint16(value)) return false;
	}
	return true;
}

bool XEByteBuffer::getInt32(xe::int32& value)
{
	if (getPosition() + 4 > getLimit()) return false;
#if XE_BYTE_BUFFER == XE_BYTE_BUFFER_JAVA
	for (byte i = 0; i < 4; i++)
	{
		value |= (((*m_position) & 0xFF) << (8 *(3 - i)));
		m_position++;
	}
#else
	value = *(xe::int32*)m_position;
	m_position += 4;
#endif
	return true;
}

bool XEByteBuffer::setInt32(const xe::int32& value)
{
	if (getPosition() + 4 > m_capacity) return false;
#if XE_BYTE_BUFFER == XE_BYTE_BUFFER_JAVA
	for (byte i = 0; i < 4; i++)
	{
		(*m_position) = (char)((value >> (8 * (3 - i))) & 0xFF);
		m_position++;
	}
#else
	(*(xe::int32*)m_position) = value;
	m_position += 4;
#endif
	return true;
}

bool XEByteBuffer::getArrayInt32(std::vector<xe::int32>& values)
{
	xe::uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayInt32(values, length);
}

bool XEByteBuffer::getArrayInt32(std::vector<xe::int32>& values, xe::uint32 lenght)
{
	values.resize(lenght);
	for (xe::uint32 i = 0; i < lenght; i++)
	{
		xe::int32 value = 0;
		if (!getInt32(value)) return false;
		values[i] = value;
	}
	return true;
}

bool XEByteBuffer::setArrayInt32(std::vector<xe::int32>& values)
{
	xe::uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (xe::uint32 i = 0; i < values.size(); i++)
	{
		xe::int32 value = values[i];
		if (!setInt32(value)) return false;
	}
	return true;
}

bool XEByteBuffer::getUint32(xe::uint32& value)
{
	if (getPosition() + 4 > getLimit()) return false;
#if XE_BYTE_BUFFER == XE_BYTE_BUFFER_JAVA
	for (byte i = 0; i < 4; i++)
	{
		value |= (((*m_position) & 0xFF) << (8 * (3 - i)));
		m_position++;
	}
#else
	value = *(xe::uint32*)m_position;
	m_position += 4;
#endif
	return true;
}
bool XEByteBuffer::setUint32(const xe::uint32& value)
{
	if (getPosition() + 4 > m_capacity) return false;
#if XE_BYTE_BUFFER == XE_BYTE_BUFFER_JAVA
	for (byte i = 0; i < 4; i++)
	{
		(*m_position) = (char)((value >> (8 * (3 - i))) & 0xFF);
		m_position++;
	}
#else
	(*(xe::uint32*)m_position) = value;
	m_position += 4;
#endif
	return true;
}

bool XEByteBuffer::getArrayUint32(std::vector<xe::uint32>& values)
{
	xe::uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayUint32(values, length);
}

bool XEByteBuffer::getArrayUint32(std::vector<xe::uint32>& values, xe::uint32 lenght)
{
	values.resize(lenght);
	for (xe::uint32 i = 0; i < lenght; i++)
	{
		xe::uint32 value = 0;
		if (!getUint32(value)) return false;
		values[i] = value;
	}
	return true;
}

bool XEByteBuffer::setArrayUint32(std::vector<xe::uint32>& values)
{
	xe::uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (xe::uint32 i = 0; i < values.size(); i++)
	{
		xe::uint32 value = values[i];
		if (!setUint32(value)) return false;
	}
	return true;
}

bool XEByteBuffer::getInt64(int64& value)
{
	if (getPosition() + 8 > getLimit()) return false;
#if XE_BYTE_BUFFER == XE_BYTE_BUFFER_JAVA
	for (byte i = 0; i < 8; i++)
	{
		value |= (((*m_position) & 0xFF) << (8 *(7 - i)));
		m_position++;
	}
#else
	value = *(xe::int64*)m_position;
	m_position += 8;
#endif
	return true;
}

bool XEByteBuffer::setInt64(const xe::int64 & value)
{
	if (getPosition() + 8 > m_capacity) return false;
#if XE_BYTE_BUFFER == XE_BYTE_BUFFER_JAVA
	for (byte i = 0; i < 8; i++)
	{
		(*m_position) = (char)((value >> (8 * (7 - i))) & 0xFF);
		m_position++;
	}
#else
	(*(xe::int64*)m_position) = value;
	m_position += 8;
#endif
	return true;
}

bool XEByteBuffer::getArrayInt64(std::vector<xe::int64>& values)
{
	xe::uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayInt64(values, length);
}

bool XEByteBuffer::getArrayInt64(std::vector<xe::int64>& values, xe::uint32 lenght)
{
	values.resize(lenght);
	for (xe::uint32 i = 0; i < lenght; i++)
	{
		xe::int64 value = 0;
		if (!getInt64(value)) return false;
		values[i] = value;
	}
	return true;
}

bool XEByteBuffer::setArrayInt64(std::vector<xe::int64>& values)
{
	xe::uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (xe::uint32 i = 0; i < values.size(); i++)
	{
		xe::int64 value = values[i];
		if (!setInt64(value)) return false;
	}
	return true;
}

bool XEByteBuffer::getUint64(xe::uint64& value)
{
	if (getPosition() + 8 > getLimit()) return false;
#if XE_BYTE_BUFFER == XE_BYTE_BUFFER_JAVA
	for (byte i = 0; i < 8; i++)
	{
		value |= (((*m_position) & 0xFF) << (8 *(7 - i)));
		m_position++;
	}
#else
	value = *(xe::uint64*)m_position;
	m_position += 8;
#endif
	return true;
}

bool XEByteBuffer::setUint64(const xe::uint64& value)
{
	if (getPosition() + 4 > m_capacity) return false;
#if XE_BYTE_BUFFER == XE_BYTE_BUFFER_JAVA
	for (byte i = 0; i < 8; i++)
	{
		(*m_position) = (char)((value >> (8 * (7 - i))) & 0xFF);
		m_position++;
	}
#else
	(*(xe::uint64*)m_position) = value;
	m_position += 8;
#endif
	return true;
}

bool XEByteBuffer::getArrayUnt64(std::vector<xe::uint64>& values)
{
	xe::uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayUnt64(values, length);
}

bool XEByteBuffer::getArrayUnt64(std::vector<xe::uint64>& values, xe::uint32 lenght)
{
	values.resize(lenght);
	for (xe::uint32 i = 0; i < lenght; i++)
	{
		xe::uint64 value = 0;
		if (!getUint64(value)) return false;
		values[i] = value;
	}
	return true;
}

bool XEByteBuffer::setArrayUnt64(std::vector<xe::uint64>& values)
{
	xe::uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (xe::uint32 i = 0; i < values.size(); i++)
	{
		xe::uint64 value = values[i];
		if (!setUint64(value)) return false;
	}
	return true;
}

bool XEByteBuffer::getFloat32(xe::float32& value)
{
	if (getPosition() + 4 > getLimit()) return false;
#if XE_BYTE_BUFFER == XE_BYTE_BUFFER_JAVA
	unsigned char* bytes;
	bytes = (unsigned char*)&value;
	bytes[3] = (*m_position++);
	bytes[2] = (*m_position++);
	bytes[1] = (*m_position++);
	bytes[0] = (*m_position++);
#else
	value = *(xe::float32*)m_position;
	m_position += 4;
#endif
	return true;
}

bool XEByteBuffer::setFloat32(const xe::float32& value)
{
	if (getPosition() + 4 > m_capacity) return false;
#if XE_BYTE_BUFFER == XE_BYTE_BUFFER_JAVA
	unsigned char* bytes;
	bytes = (unsigned char*)&value;
	(*m_position++) = bytes[3];
	(*m_position++) = bytes[2];
	(*m_position++) = bytes[1];
	(*m_position++) = bytes[0];
#else
	(*(xe::float32*)m_position) = value;
	m_position += 4;
#endif
	return true;
}

bool XEByteBuffer::getArrayFloat32(std::vector<xe::float32>& values)
{
	xe::uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayFloat32(values, length);
}

bool XEByteBuffer::getArrayFloat32(std::vector<xe::float32>& values, xe::uint32 lenght)
{
	values.resize(lenght);
	for (xe::uint32 i = 0; i < lenght; i++)
	{
		xe::float32 value = 0;
		if (!getFloat32(value)) return false;
		values[i] = value;
	}
	return true;
}

bool XEByteBuffer::setArrayFloat32(std::vector<xe::float32>& values)
{
	xe::uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (xe::uint32 i = 0; i < values.size(); i++)
	{
		xe::float32 value = values[i];
		if (!setFloat32(value)) return false;
	}
	return true;
}

bool XEByteBuffer::getFloat64(xe::float64& value)
{
	if (getPosition() + 8 > getLimit()) return false;
#if XE_BYTE_BUFFER == XE_BYTE_BUFFER_JAVA
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
	value = *(xe::float64*)m_position;
	m_position += 8;
#endif
	return true;
}

bool XEByteBuffer::setFloat64(const float64& value)
{
	if (getPosition() + 8 > m_capacity) return false;
#if XE_BYTE_BUFFER == XE_BYTE_BUFFER_JAVA
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
	(*(xe::float64*)m_position) = value;
	m_position += 8;
#endif
	return true;
}

bool XEByteBuffer::getArrayFloat64(std::vector<xe::float64>& values)
{
	xe::uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayFloat64(values, length);
}

bool XEByteBuffer::getArrayFloat64(std::vector<xe::float64>& values, xe::uint32 lenght)
{
	values.resize(lenght);
	for (xe::uint32 i = 0; i < lenght; i++)
	{
		xe::float64 value = 0;
		if (!getFloat64(value)) return false;
		values[i] = value;
	}
	return true;
}

bool XEByteBuffer::setArrayFloat64(std::vector<xe::float64>& values)
{
	xe::uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (xe::uint32 i = 0; i < values.size(); i++)
	{
		xe::float64 value = values[i];
		if (!setFloat64(value)) return false;
	}
	return true;
}

bool XEByteBuffer::getString(std::string& value)
{
	if (getPosition() + 4 > getLimit()) return false;
	xe::uint32 length = 0;
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

bool XEByteBuffer::setString(const std::string& value)
{
	xe::uint32 length = value.length();
	if (getPosition() + 4 + length > m_capacity) return false;
	setUint32(length);
	if (length > 0)
	{
		setBuffer(value.c_str(), 0, length);
	}
	return true;
}

bool XEByteBuffer::getArrayString(std::vector<std::string>& values)
{
	xe::uint32 length = 0;
	if (!getUint32(length)) return false;
	return getArrayString(values, length);
}

bool XEByteBuffer::getArrayString(std::vector<std::string>& values, xe::uint32 lenght)
{
	values.resize(lenght);
	for (xe::uint32 i = 0; i < lenght; i++)
	{
		std::string value;
		if (!getString(value)) return false;
		values[i] = value;
	}
	return true;

}

bool XEByteBuffer::setArrayString(std::vector<std::string>& values)
{
	xe::uint32 length = values.size();
	if (!setUint32(length)) return false;
	for (xe::uint32 i = 0; i < values.size(); i++)
	{
		std::string value = values[i];
		if (!setString(value)) return false;
	}
	return true;
}

bool& XEByteBuffer::readBool()
{
	bool value = 0;
	getBool(value);
	return value;
}

xe::int8& XEByteBuffer::readInt8()
{
	xe::int8 value = 0;
	getInt8(value);
	return value;
}

xe::byte& XEByteBuffer::readByte()
{
	xe::byte value = 0;
	getByte(value);
	return value;
}

xe::int16& XEByteBuffer::readInt16()
{
	xe::int16 value = 0;
	getInt16(value);
	return value;
}

xe::uint16& XEByteBuffer::readUint16()
{
	xe::uint16 value = 0;
	getUint16(value);
	return (xe::uint16&)value;
}

int32& XEByteBuffer::readInt32()
{
	xe::int32 value = 0;
	getInt32(value);
	return (xe::int32&)value;
}

uint32& XEByteBuffer::readUint32()
{
	xe::uint32 value = 0;
	getUint32(value);
	return (xe::uint32&)value;
}

xe::float32& XEByteBuffer::readFloat32()
{
	xe::float32 value = 0;
	getFloat32(value);
	return (xe::float32&)value;
}

xe::float64& XEByteBuffer::readFloat64()
{
	xe::float64 value = 0;
	getFloat64(value);
	return (xe::float64&)value;
}

std::string XEByteBuffer::readString()
{
	std::string value = "";
	getString(value);
	return value;
}

NS_XE_END
