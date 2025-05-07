/************************************************************************** 
    *  @Copyright (c) 2015, A-one Soft, All rights reserved. 
 
    *  @file     	: XEByteBuffer.h 
    *  @version	: ver 1.0.0 
 
    *  @author	: andy-zhao 
    *  @date		: 2015/06/26 
    *  @brief		:  
**************************************************************************/
#ifndef _XEByteBuffer_H_
#define _XEByteBuffer_H_

#include "xenet/XEStdC.h"
#include <vector>

NS_XE_BEGIN

class XEByteBuffer
{
public:
	XEByteBuffer();
	XEByteBuffer(uint32 capacity);
	XEByteBuffer(const char* buffer, uint32 capacity);
	XEByteBuffer(const char* buffer, uint32 offset, uint32 capacity);
	virtual ~XEByteBuffer();

	bool writeToFile(std::string& file);
	bool readToFile(std::string& file);

	static XEByteBuffer* createByteBuffer();
	static XEByteBuffer* createByteBuffer(uint32 capacity);
	static void releaseByteBuffer(XEByteBuffer* pBuff);

	/**
		clear方法将缓冲区清空，一般是在重新写缓冲区时调用。
	*/
	void clear();

	/**
		反转缓冲区。首先将限制设置为当前位置，然后将位置设置为 0。
		如果已定义了标记，则丢弃该标记。
		常与compact方法一起使用。
		通常情况下，在准备从缓冲区中读取数据时调用flip方法。
	*/
	void flip();

	/**
		
	*/
	void compact();

	/**
		limit保持不变，重读缓冲区的所有数据。
	*/
	void rewind() { m_position = m_buffer; }

	/**
		缓冲区剩余空间。
	*/
	uint32 remaining() { return m_limit - m_position; }

	/**
		缓冲区可用空间。
	*/
	uint32 getLimit() { return m_limit - m_buffer; }

	/**
		缓冲区最大空间。
	*/
	uint32 getCapacity() { return m_capacity; }

	/**
		获取游标的位置。
	*/
	uint32 getPosition() { return m_position - m_buffer; }

	/**
		设置游标的位置。
	*/
	bool setPosition(uint32 value);

	/**
		获取缓冲区数据。
	*/
	const char* getBuffer() { return m_buffer; }
	bool getBuffer(char* buffer, uint32 length);
	bool getBuffer(char* buffer, uint32 offset, uint32 limit);

	/**
		设置缓冲区数据。
	*/
	bool setBuffer(const char* buffer, uint32 offset, uint32 limit);

	/**
		读写不同类型缓冲区数据。
	*/
	bool getBool(bool& value);
	bool setBool(const bool& value);
	bool getArrayBool(std::vector<bool>& values);
	bool getArrayBool(std::vector<bool>& values, uint32 lenght);
	bool setArrayBool(std::vector<bool>& values);
	
	bool getInt8(int8& value);
	bool setInt8(const int8& value);
	bool getArrayInt8(std::vector<int8>& values);
	bool getArrayInt8(std::vector<int8>& values, uint32 lenght);
	bool setArrayInt8(std::vector<int8>& values);
	
	bool getByte(byte& value);
	bool setByte(const byte& value);
	bool getArrayByte(std::vector<byte>& values);
	bool getArrayByte(std::vector<byte>& values, uint32 lenght);
	bool setArrayByte(std::vector<byte>& values);
	
	bool getInt16(int16& value);
	bool setInt16(const int16& value);
	bool getArrayInt16(std::vector<int16>& values);
	bool getArrayInt16(std::vector<int16>& values, uint32 lenght);
	bool setArrayInt16(std::vector<int16>& values);
	
	bool getUint16(uint16& value);
	bool setUint16(const uint16& value);
	bool getArrayUint16(std::vector<uint16>& values);
	bool getArrayUint16(std::vector<uint16>& values, uint32 lenght);
	bool setArrayUint16(std::vector<uint16>& values);
	
	bool getInt32(int32& value);
	bool setInt32(const int32& value);
	bool getArrayInt32(std::vector<int32>& values);
	bool getArrayInt32(std::vector<int32>& values, uint32 lenght);
	bool setArrayInt32(std::vector<int32>& values);
	
	bool getUint32(uint32& value);
	bool setUint32(const uint32& value);
	bool getArrayUint32(std::vector<uint32>& values);
	bool getArrayUint32(std::vector<uint32>& values, uint32 lenght);
	bool setArrayUint32(std::vector<uint32>& values);
	
	bool getInt64(int64& value);
	bool setInt64(const int64 & value);
	bool getArrayInt64(std::vector<int64>& values);
	bool getArrayInt64(std::vector<int64>& values, uint32 lenght);
	bool setArrayInt64(std::vector<int64>& values);

	bool getUint64(uint64& value);
	bool setUint64(const uint64& value);
	bool getArrayUnt64(std::vector<uint64>& values);
	bool getArrayUnt64(std::vector<uint64>& values, uint32 lenght);
	bool setArrayUnt64(std::vector<uint64>& values);

	bool getFloat32(float32& value);
	bool setFloat32(const float32& value);
	bool getArrayFloat32(std::vector<float32>& values);
	bool getArrayFloat32(std::vector<float32>& values, uint32 lenght);
	bool setArrayFloat32(std::vector<float32>& values);

	bool getFloat64(float64& value);
	bool setFloat64(const float64& value);
	bool getArrayFloat64(std::vector<float64>& values);
	bool getArrayFloat64(std::vector<float64>& values, uint32 lenght);
	bool setArrayFloat64(std::vector<float64>& values);

	bool getString(std::string& value);
	bool setString(const std::string& value);
	bool getArrayString(std::vector<std::string>& values);
	bool getArrayString(std::vector<std::string>& values, uint32 lenght);
	bool setArrayString(std::vector<std::string>& values);

	bool writeBool(const bool& value) { return setBool(value); }
	bool& readBool();

	bool writeInt8(const int8& value) { return setInt8(value); }
	int8& readInt8();

	bool writeByte(const byte& value) { return setByte(value); }
	byte& readByte();

	bool writeInt16(const int16& value) { return setInt16(value); }
	int16& readInt16();

	bool writeUint16(const uint16& value) { return setUint16(value); }
	uint16& readUint16();

	bool writeInt32(const int32& value) { return setInt32(value); }
	int32& readInt32();

	bool writeUint32(const uint32& value) { return setUint32(value); }
	uint32& readUint32();

	bool writeFloat32(const float32& value) { return setFloat32(value); }
	float32& readFloat32();

	bool writeFloat64(const float64& value) { return setFloat64(value); }
	float64& readFloat64();

	bool writeString(const std::string& value) { return setString(value); }
	std::string readString();

	// 空缓冲区
	static const XEByteBuffer ZERO;

protected:
	// 缓冲区数据
	char* m_buffer;
	// 代表对缓冲区进行读写时，当前游标的位置。
	char* m_position;
	// 所有对Buffer读写操作都会以limit变量的值作为上限。
	char* m_limit;
	// 代表缓冲区的最大容量（一般新建一个缓冲区的时候，limit的值和capacity的值默认是相等的）。
	uint32 m_capacity;

private:
};

NS_XE_END

#endif // _XEByteBuffer_H_