#include "xenet/XEValue.h"

NS_XE_BEGIN

XEValue::XEValue()
{

}

XEValue::XEValue(const bool& value)
: m_eType(eBoolValue)
, m_iLen(sizeof(bool))
{
	m_uData.m_bool = value;
}

XEValue::XEValue(const int8& value)
: m_eType(eInt8Value)
, m_iLen(sizeof(int8))
{
	m_uData.m_int8 = value;
}

XEValue::XEValue(const byte& value)
: m_eType(eByteValue)
, m_iLen(sizeof(byte))
{
	m_uData.m_byte = value;
}

XEValue::XEValue(const int16& value)
: m_eType(eInt16Value)
, m_iLen(sizeof(int16))
{
	m_uData.m_int16 = value;
}

XEValue::XEValue(const uint16& value)
: m_eType(eUint16Value)
, m_iLen(sizeof(uint16))
{
	m_uData.m_uint16 = value;
}

XEValue::XEValue(const int32& value)
: m_eType(eInt32Value)
, m_iLen(sizeof(int32))
{
	m_uData.m_int32 = value;
}

XEValue::XEValue(const uint32& value)
: m_eType(eUint32Value)
, m_iLen(sizeof(uint32))
{
	m_uData.m_uint32 = value;
}

XEValue::XEValue(const int64& value)
: m_eType(eInt64Value)
, m_iLen(sizeof(int64))
{
	m_uData.m_int64 = value;
}

XEValue::XEValue(const uint64& value)
: m_eType(eUint64Value)
, m_iLen(sizeof(uint64))
{
	m_uData.m_uint64 = value;
}

XEValue::XEValue(const float32& value)
: m_eType(eFloat32Value)
, m_iLen(sizeof(float32))
{
	m_uData.m_float32 = value;
}

XEValue::XEValue(const float64& value)
: m_eType(eFloat64Value)
, m_iLen(sizeof(float64))
{
	m_uData.m_float64 = value;
}

XEValue::XEValue(time_t time)
: m_eType(eTimeValue)
, m_iLen(sizeof(time_t))
{
	m_uData.m_time = time;
}

XEValue::XEValue(const std::string& str)
: m_eType(eStringValue)
, m_iLen(str.length())
{
	m_uData.m_ptr = malloc(m_iLen + 1);
	memcpy(m_uData.m_ptr, str.c_str(), m_iLen + 1);
}

XEValue::XEValue(const char* str)
: m_eType(eStringValue)
, m_iLen(0)
{
	m_iLen = strlen(str);
	m_uData.m_ptr = malloc(m_iLen + 1);
	memcpy(m_uData.m_ptr, str, m_iLen + 1);
}

XEValue::XEValue(const void* ptr, size_t len)
: m_eType(eObjectValue)
, m_iLen(len)
{
	m_uData.m_ptr = malloc(m_iLen + 1);
	memcpy(m_uData.m_ptr, ptr, m_iLen);
}

XEValue::XEValue(const XEValue& other)
: m_eType(other.m_eType)
, m_iLen(other.m_iLen)
{
	switch (m_eType)
	{
	case eBoolValue:
		m_uData.m_bool = other.m_uData.m_bool;
		break;
	case eInt8Value:
		m_uData.m_int8 = other.m_uData.m_int8;
		break;
	case eByteValue:
		m_uData.m_byte = other.m_uData.m_byte;
		break;
	case eInt16Value:
		m_uData.m_int16 = other.m_uData.m_int16;
		break;
	case eUint16Value:
		m_uData.m_uint16 = other.m_uData.m_uint16;
		break;
	case eInt32Value:
		m_uData.m_int32 = other.m_uData.m_int32;
		break;
	case eUint32Value:
		m_uData.m_uint32 = other.m_uData.m_uint32;
		break;
	case eInt64Value:
		m_uData.m_int64 = other.m_uData.m_int64;
		break;
	case eUint64Value:
		m_uData.m_uint64 = other.m_uData.m_uint64;
		break;
	case eFloat32Value:
		m_uData.m_float32 = other.m_uData.m_float32;
		break;
	case eFloat64Value:
		m_uData.m_float64 = other.m_uData.m_float64;
		break;
	case eTimeValue:
		m_uData.m_time = other.m_uData.m_time;
		break;
	case eStringValue:
		m_uData.m_ptr = malloc(m_iLen + 1);
		memcpy(m_uData.m_ptr, other.m_uData.m_ptr, m_iLen + 1);
		break;
	case eObjectValue:
		m_uData.m_ptr = malloc(m_iLen);
		memcpy(m_uData.m_ptr, other.m_uData.m_ptr, m_iLen);
		break;
	default:
		break;
	}
}

XEValue::~XEValue()
{
	if (eStringValue == m_eType || eObjectValue == m_eType)
	{
		free(m_uData.m_ptr);
	}
	m_eType = eNullValue;
	m_iLen = 0;
}

XEValue& XEValue::operator=(const XEValue& other)
{
	if (&other == this) return *this;

	if (eStringValue == m_eType || eObjectValue == m_eType)
	{
		free(m_uData.m_ptr);
	}
	m_eType = other.m_eType;
	m_iLen = other.m_iLen;

	switch (m_eType)
	{
	case eBoolValue:
		m_uData.m_bool = other.m_uData.m_bool;
		break;
	case eInt8Value:
		m_uData.m_int8 = other.m_uData.m_int8;
		break;
	case eByteValue:
		m_uData.m_byte = other.m_uData.m_byte;
		break;
	case eInt16Value:
		m_uData.m_int16 = other.m_uData.m_int16;
		break;
	case eUint16Value:
		m_uData.m_uint16 = other.m_uData.m_uint16;
		break;
	case eInt32Value:
		m_uData.m_int32 = other.m_uData.m_int32;
		break;
	case eUint32Value:
		m_uData.m_uint32 = other.m_uData.m_uint32;
		break;
	case eInt64Value:
		m_uData.m_int64 = other.m_uData.m_int64;
		break;
	case eUint64Value:
		m_uData.m_uint64 = other.m_uData.m_uint64;
		break;
	case eFloat32Value:
		m_uData.m_float32 = other.m_uData.m_float32;
		break;
	case eFloat64Value:
		m_uData.m_float64 = other.m_uData.m_float64;
		break;
	case eTimeValue:
		m_uData.m_time = other.m_uData.m_time;
		break;
	case eStringValue:
		m_uData.m_ptr = malloc(m_iLen + 1);
		memcpy(m_uData.m_ptr, other.m_uData.m_ptr, m_iLen + 1);
		break;
	case eObjectValue:
		m_uData.m_ptr = malloc(m_iLen);
		memcpy(m_uData.m_ptr, other.m_uData.m_ptr, m_iLen);
		break;
	default:
		break;
	}
	return *this;
}

bool XEValue::operator<(const XEValue &other) const
{
	if (m_eType != other.m_eType) return false;

	switch (m_eType)
	{
	case eBoolValue:
		return false;
	case eInt8Value:
		return m_uData.m_int8 < other.m_uData.m_int8;
	case eByteValue:
		return m_uData.m_byte < other.m_uData.m_byte;
	case eInt16Value:
		return m_uData.m_int16 < other.m_uData.m_int16;
	case eUint16Value:
		return m_uData.m_uint16 < other.m_uData.m_uint16;
	case eInt32Value:
		return m_uData.m_int32 < other.m_uData.m_int32;
	case eUint32Value:
		return m_uData.m_uint32 < other.m_uData.m_uint32;
	case eInt64Value:
		return m_uData.m_int64 < other.m_uData.m_int64;
	case eUint64Value:
		return m_uData.m_uint64 < other.m_uData.m_uint64;
	case eFloat32Value:
		return m_uData.m_float32 < other.m_uData.m_float32;
	case eFloat64Value:
		return m_uData.m_float64 < other.m_uData.m_float64;
	case eTimeValue:
		return m_uData.m_time < other.m_uData.m_time;
	case eStringValue:
		return false;
	case eObjectValue:
		return false;
	default:
		break;
	}
	return false;
}

bool XEValue::operator<=(const XEValue &other) const
{
	return !(other > *this);
}

bool XEValue::operator>=(const XEValue &other) const
{
	return !(*this < other);
}

bool XEValue::operator>(const XEValue &other) const
{
	return other < *this;
}

bool XEValue::operator==(const XEValue &other) const
{
	if (m_eType != other.m_eType) return false;

	switch (m_eType)
	{
	case eBoolValue:
		return m_uData.m_bool == other.m_uData.m_bool;
	case eInt8Value:
		return m_uData.m_int8 == other.m_uData.m_int8;
	case eByteValue:
		return m_uData.m_byte == other.m_uData.m_byte;
	case eInt16Value:
		return m_uData.m_int16 == other.m_uData.m_int16;
	case eUint16Value:
		return m_uData.m_uint16 == other.m_uData.m_uint16;
	case eInt32Value:
		return m_uData.m_int32 == other.m_uData.m_int32;
	case eUint32Value:
		return m_uData.m_uint32 == other.m_uData.m_uint32;
	case eInt64Value:
		return m_uData.m_int64 == other.m_uData.m_int64;
	case eUint64Value:
		return m_uData.m_uint64 == other.m_uData.m_uint64;
	case eFloat32Value:
		return m_uData.m_float32 == other.m_uData.m_float32;
	case eFloat64Value:
		return m_uData.m_float64 == other.m_uData.m_float64;
	case eTimeValue:
		return m_uData.m_time == other.m_uData.m_time;
	case eStringValue:
		return strcmp((char*)m_uData.m_ptr, (char*)other.m_uData.m_ptr) <= 0;
	case eObjectValue:
		return m_uData.m_ptr == other.m_uData.m_ptr;
	default:
		break;
	}
	return false;
}

bool XEValue::operator!=(const XEValue &other) const
{
	return !(other == *this);
}

NS_XE_END
