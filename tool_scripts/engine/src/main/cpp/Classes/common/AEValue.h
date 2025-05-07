/************************************************************************** 
    *  @Copyright (c) 2015, A-one Soft, All rights reserved. 
 
    *  @file			: AEValue.h 
    *  @version	: ver 1.0.0 
 
    *  @author	: andy-zhao 
    *  @date		: 2015/12/22 
    *  @brief		: 数据类型 
**************************************************************************/
#ifndef _AEValue_H_
#define _AEValue_H_

#include "platform/AEStdC.h"

NS_AE_BEGIN

typedef enum _ValueType
{
	eNullValue = 0,
	eBoolValue,
	eInt8Value,
	eByteValue,
	eInt16Value,
	eUint16Value,
	eInt32Value,
	eUint32Value,
	eInt64Value,
	eUint64Value,
	eFloat32Value,
	eFloat64Value,
	eTimeValue,
	eStringValue,
	eObjectValue

} AEValueType;

typedef union _ValueData
{
	bool m_bool;
	int8 m_int8;
	byte m_byte;
	int16 m_int16;
	uint16 m_uint16;
	int32 m_int32;
	uint32 m_uint32;
	int64 m_int64;
	uint64 m_uint64;
	float32 m_float32;
	float64 m_float64;
	time_t m_time;
	void* m_ptr;

} AEValueData;

class AEValue
{
public:
	AEValue();
	AEValue(const bool& value);
	AEValue(const int8& value);
	AEValue(const byte& value);
	AEValue(const int16& value);
	AEValue(const uint16& value);
	AEValue(const int32& value);
	AEValue(const uint32& value);
	AEValue(const int64& value);
	AEValue(const uint64& value);
	AEValue(const float32& value);
	AEValue(const float64& value);
	AEValue(time_t time);
	AEValue(const std::string& str);
	AEValue(const char* str);
	AEValue(const void* ptr, size_t len);
	AEValue(const AEValue& other);
	virtual ~AEValue();

	AEValue& operator=(const AEValue& other);
	bool operator<(const AEValue &other) const;
	bool operator<=(const AEValue &other) const;
	bool operator>=(const AEValue &other) const;
	bool operator>(const AEValue &other) const;
	bool operator==(const AEValue &other) const;
	bool operator!=(const AEValue &other) const;

	const bool& getBool() const { return m_uData.m_bool; }
	void setBool(const bool& value) { m_uData.m_bool = value; }

	const int8& getInt8() const { return m_uData.m_int8; }
	void setInt8(const int8& value) { m_uData.m_int8 = value; }

	const byte& getByte() const { return m_uData.m_byte; }
	void setByte(const byte& value) { m_uData.m_byte = value; }

	const int16& getInt16() const { return m_uData.m_int16; }
	void setInt16(const int16& value) { m_uData.m_int16 = value; }

	const uint16& getUint16() const { return m_uData.m_uint16; }
	void setUint16(const uint16& value) { m_uData.m_uint16 = value; }

	const int32& getInt32() const { return m_uData.m_int32; }
	void setInt32(const int32& value) { m_uData.m_int32 = value; }

	const uint32& getUint32() const { return m_uData.m_uint32; }
	void setUint32(const uint32& value) { m_uData.m_uint32 = value; }

	const int64& getInt64() const { return m_uData.m_int64; }
	void setInt64(const int64& value) { m_uData.m_int64 = value; }

	const uint64& getUint64() const { return m_uData.m_uint64; }
	void setUint64(const uint64& value) { m_uData.m_uint64 = value; }

	const float32& getFloat32() { return m_uData.m_float32; }
	void setFloat32(const float32& value) { m_uData.m_float32 = value; }

	const float64& getFloat64() const { return m_uData.m_float64; }
	void setFloat64(const float64& value) { m_uData.m_float64 = value; }

	const void* getVoid() const { return m_uData.m_ptr; }
	void setVoid(void* value) { m_uData.m_ptr = value; }

	// 当前值的类型
	AE_SYNTHESIZE_READONLY(AEValueType, m_eType, Type);

protected:
private:
	AEValueData m_uData;
	size_t m_iLen;

};

NS_AE_END

#endif // _AEValue_H_