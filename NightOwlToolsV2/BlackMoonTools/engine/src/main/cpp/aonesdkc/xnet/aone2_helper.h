// date: 2011-04-25
// file: aone2_helper.h
// auth: guosh
// desc: aone2消息编解码辅助类


#ifndef _AONE2_HELPER_H_
#define _AONE2_HELPER_H_

#include "xcore_define.h"

namespace aone2
{

/////////////////////////////////////////////////////////////////////
// class Aone2Decoder
/////////////////////////////////////////////////////////////////////
class Aone2Decoder
{
public:
	Aone2Decoder(const byte* buff, uint32 length);
	Aone2Decoder(const Aone2Decoder& other);
	Aone2Decoder& operator=(const Aone2Decoder& other);

	void attach(const byte* buff, uint32 length);

	const byte*  position() const { return m_pos; }
	const byte*  start() const { return m_buff; }
	const byte*  end() const { return m_end; }

	uint32 remain() const { return (uint32)(m_end - m_pos); }
	bool   eof() const { return m_pos >= m_end; }
	void   reset() { m_pos = m_buff; }
	bool   set_pos(const byte* pos);

public:
	bool get_bool(bool& val);
	bool get_byte(byte& val);
	bool get_int8(int8& val);
	bool get_uint8(uint8& val);
	bool get_int16(int16& val);
	bool get_uint16(uint16& val);
	bool get_int32(int32& val);
	bool get_uint32(uint32& val);
	bool get_int64(int64& val);
	bool get_uint64(uint64& val);
	bool get_float32(float32& val);
	bool get_float64(float64& val);
	bool get_double(double& val);
	bool get_string(string& val);

	bool get_bool_array(bool* buf, uint32 len);
	bool get_byte_array(byte* buf, uint32 len);
	bool get_int8_array(int8* buf, uint32 len);
	bool get_uint8_array(uint8* buf, uint32 len);
	bool get_int16_array(int16* buf, uint32 len);
	bool get_uint16_array(uint16* buf, uint32 len);
	bool get_int32_array(int32* buf, uint32 len);
	bool get_uint32_array(uint32* buf, uint32 len);
	bool get_int64_array(int64* buf, uint32 len);
	bool get_uint64_array(uint64* buf, uint32 len);
	bool get_float32_array(float32* buf, uint32 len);
	bool get_float64_array(float64* buf, uint32 len);
	bool get_double_array(double* buf, uint32 len);
	bool get_string_array(string* buf, uint32 len);

	bool get_bool_vector(vector<bool>& val);
	bool get_byte_vector(vector<byte>& val);
	bool get_int8_vector(vector<int8>& val);
	bool get_uint8_vector(vector<uint8>& val);
	bool get_int16_vector(vector<int16>& val);
	bool get_uint16_vector(vector<uint16>& val);
	bool get_int32_vector(vector<int32>& val);
	bool get_uint32_vector(vector<uint32>& val);
	bool get_int64_vector(vector<int64>& val);
	bool get_uint64_vector(vector<uint64>& val);
	bool get_float32_vector(vector<float32>& val);
	bool get_float64_vector(vector<float64>& val);
	bool get_double_vector(vector<double>& val);
	bool get_string_vector(vector<string>& val);

protected:
	const byte*    m_buff;
	const byte*    m_pos;
	const byte*    m_end;
};

/////////////////////////////////////////////////////////////////////
// class Aone2Coder
/////////////////////////////////////////////////////////////////////
class Aone2Coder
{
public:
	Aone2Coder(byte* buff, uint32 length);
	Aone2Coder(const Aone2Coder& other);
	Aone2Coder& operator=(const Aone2Coder& other);

	void attach(byte* buff, uint32 length);

	byte*  position() const { return m_pos; }
	byte*  start() const { return m_buff; }
	byte*  end() const { return m_end; }

	uint32 remain() const { return (uint32)(m_end - m_pos); }
	bool   eof() const { return m_pos >= m_end; }
	void   reset() { m_pos = m_buff; }
	bool   set_pos(byte* pos);

public:
	bool set_bool(bool val);
	bool set_byte(byte val);
	bool set_int8(int8 val);
	bool set_uint8(uint8 val);
	bool set_int16(int16 val);
	bool set_uint16(uint16 val);
	bool set_int32(int32 val);
	bool set_uint32(uint32 val);
	bool set_int64(int64 val);
	bool set_uint64(uint64 val);
	bool set_float32(float32 val);
	bool set_float64(float64 val);
	bool set_double(double val);
	bool set_string(const string& val);

	bool set_bool_array(const bool* buf, uint32 len);
	bool set_byte_array(const byte* buf, uint32 len);
	bool set_int8_array(const int8* buf, uint32 len);
	bool set_uint8_array(const uint8* buf, uint32 len);
	bool set_int16_array(const int16* buf, uint32 len);
	bool set_uint16_array(const uint16* buf, uint32 len);
	bool set_int32_array(const int32* buf, uint32 len);
	bool set_uint32_array(const uint32* buf, uint32 len);
	bool set_int64_array(const int64* buf, uint32 len);
	bool set_uint64_array(const uint64* buf, uint32 len);
	bool set_float32_array(const float32* buf, uint32 len);
	bool set_float64_array(const float64* buf, uint32 len);
	bool set_double_array(const double* buf, uint32 len);
	bool set_string_array(const string* buf, uint32 len);

	bool set_bool_vector(const vector<bool>& vals);
	bool set_byte_vector(const vector<byte>& vals);
	bool set_int8_vector(const vector<int8>& vals);
	bool set_uint8_vector(const vector<uint8>& vals);
	bool set_int16_vector(const vector<int16>& vals);
	bool set_uint16_vector(const vector<uint16>& vals);
	bool set_int32_vector(const vector<int32>& vals);
	bool set_uint32_vector(const vector<uint32>& vals);
	bool set_int64_vector(const vector<int64>& vals);
	bool set_uint64_vector(const vector<uint64>& vals);
	bool set_float32_vector(const vector<float32>& vals);
	bool set_float64_vector(const vector<float64>& vals);
	bool set_double_vector(const vector<double>& vals);
	bool set_string_vector(const vector<string>& vals);

public:
	static uint32 size_bool(bool val);
	static uint32 size_byte(byte val);
	static uint32 size_int8(int8 val);
	static uint32 size_uint8(uint8 val);
	static uint32 size_int16(int16 val);
	static uint32 size_uint16(uint16 val);
	static uint32 size_int32(int32 val);
	static uint32 size_uint32(uint32 val);
	static uint32 size_int64(int64 val);
	static uint32 size_uint64(uint64 val);
	static uint32 size_float32(float32 val);
	static uint32 size_float64(float64 val);
	static uint32 size_double(double val);
	static uint32 size_string(const string& val);

	static uint32 size_bool_array(const bool* buf, uint32 len);
	static uint32 size_byte_array(const byte* buf, uint32 len);
	static uint32 size_int8_array(const int8* buf, uint32 len);
	static uint32 size_uint8_array(const uint8* buf, uint32 len);
	static uint32 size_int16_array(const int16* buf, uint32 len);
	static uint32 size_uint16_array(const uint16* buf, uint32 len);
	static uint32 size_int32_array(const int32* buf, uint32 len);
	static uint32 size_uint32_array(const uint32* buf, uint32 len);
	static uint32 size_int64_array(const int64* buf, uint32 len);
	static uint32 size_uint64_array(const uint64* buf, uint32 len);
	static uint32 size_float32_array(const float32* buf, uint32 len);
	static uint32 size_float64_array(const float64* buf, uint32 len);
	static uint32 size_double_array(const double* buf, uint32 len);
	static uint32 size_string_array(const string* buf, uint32 len);

	static uint32 size_bool_vector(const vector<bool>& vals);
	static uint32 size_byte_vector(const vector<byte>& vals);
	static uint32 size_int8_vector(const vector<int8>& vals);
	static uint32 size_uint8_vector(const vector<uint8>& vals);
	static uint32 size_int16_vector(const vector<int16>& vals);
	static uint32 size_uint16_vector(const vector<uint16>& vals);
	static uint32 size_int32_vector(const vector<int32>& vals);
	static uint32 size_uint32_vector(const vector<uint32>& vals);
	static uint32 size_int64_vector(const vector<int64>& vals);
	static uint32 size_uint64_vector(const vector<uint64>& vals);
	static uint32 size_float32_vector(const vector<float32>& vals);
	static uint32 size_float64_vector(const vector<float64>& vals);
	static uint32 size_double_vector(const vector<double>& vals);
	static uint32 size_string_vector(const vector<string>& vals);

protected:
	byte*    m_buff;
	byte*    m_pos;
	byte*    m_end;
};

}//namespace aone2

using namespace aone2;

#endif//_AONE2_HELPER_H_
