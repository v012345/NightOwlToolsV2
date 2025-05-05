// date: 2011-04-25
// file: aone2_helper.cpp
// auth: guosh
// desc: aone2消息编解码辅助类


#include "aone2_helper.h"

namespace aone2
{

/////////////////////////////////////////////////////////////////////
// class Aone2Decoder
/////////////////////////////////////////////////////////////////////
Aone2Decoder::Aone2Decoder(const byte* buff, uint32 length)
	: m_buff(buff)
	, m_pos(buff)
	, m_end(buff + length)
{
	assert(m_buff);
}

Aone2Decoder::Aone2Decoder(const Aone2Decoder& other)
	: m_buff(other.m_buff)
	, m_pos(other.m_pos)
	, m_end(other.m_end)
{
	// empty
}

Aone2Decoder& Aone2Decoder::operator=(const Aone2Decoder& other)
{
	if (&other != this)
	{
		m_buff = other.m_buff;
		m_pos = other.m_pos;
		m_end = other.m_end;
	}
	return *this;
}

void Aone2Decoder::attach(const byte* buff, uint32 length)
{
	assert(buff);
	m_buff = buff;
	m_pos = m_buff;
	m_end = m_buff + length;
}

bool Aone2Decoder::set_pos(const byte* pos)
{
	assert(pos);
	assert(pos >= m_buff && pos <= m_end);
	if (pos < m_buff) return false;
	if (pos > m_end) return false;
	m_pos = pos;
	return true;
}

bool Aone2Decoder::get_bool(bool& val)
{
	if (remain() < sizeof(byte)) return false;
	val = (0 != *m_pos);
	m_pos += sizeof(byte);
	return true;
}

bool Aone2Decoder::get_byte(byte& val)
{
	if (remain() < sizeof(byte)) return false;
	val = *(byte*)m_pos;
	m_pos += sizeof(byte);
	return true;
}

bool Aone2Decoder::get_int8(int8& val)
{
	if (remain() < sizeof(int8)) return false;
	val = *(int8*)m_pos;
	m_pos += sizeof(int8);
	return true;
}

bool Aone2Decoder::get_uint8(uint8& val)
{
	if (remain() < sizeof(uint8)) return false;
	val = *(uint8*)m_pos;
	m_pos += sizeof(uint8);
	return true;
}

bool Aone2Decoder::get_int16(int16& val)
{
	if (remain() < sizeof(int16)) return false;
	memcpy(&val, m_pos, sizeof(int16));
	m_pos += sizeof(int16);
	return true;
}

bool Aone2Decoder::get_uint16(uint16& val)
{
	if (remain() < sizeof(uint16)) return false;
	memcpy(&val, m_pos, sizeof(uint16));
	m_pos += sizeof(uint16);
	return true;
}

bool Aone2Decoder::get_int32(int32& val)
{
	if (remain() < sizeof(int32)) return false;
	memcpy(&val, m_pos, sizeof(int32));
	m_pos += sizeof(int32);
	return true;
}

bool Aone2Decoder::get_uint32(uint32& val)
{
	if (remain() < sizeof(uint32)) return false;
	memcpy(&val, m_pos, sizeof(uint32));
	m_pos += sizeof(uint32);
	return true;
}

bool Aone2Decoder::get_int64(int64& val)
{
	if (remain() < sizeof(int64)) return false;
	memcpy(&val, m_pos, sizeof(int64));
	m_pos += sizeof(int64);
	return true;
}

bool Aone2Decoder::get_uint64(uint64& val)
{
	if (remain() < sizeof(uint64)) return false;
	memcpy(&val, m_pos, sizeof(uint64));
	m_pos += sizeof(uint64);
	return true;
}

bool Aone2Decoder::get_float32(float32& val)
{
	if (remain() < sizeof(float32)) return false;
	memcpy(&val, m_pos, sizeof(float32));
	m_pos += sizeof(float32);
	return true;
}

bool Aone2Decoder::get_float64(float64& val)
{
	if (remain() < sizeof(float64)) return false;
	memcpy(&val, m_pos, sizeof(float64));
	m_pos += sizeof(float64);
	return true;
}

bool Aone2Decoder::get_double(double& val)
{
	if (remain() < sizeof(double)) return false;
	memcpy(&val, m_pos, sizeof(double));
	m_pos += sizeof(double);
	return true;
}

bool Aone2Decoder::get_string(string& val)
{
	uint32 len = 0;
	if (!get_uint32(len))return false;
	if (remain() < len) return false;
	val.assign((const char*)m_pos, len);
	m_pos += len;
	return true;
}

bool Aone2Decoder::get_bool_array(bool* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(byte)) return false;
	for (uint32 i = 0; i < len; i++)
	{
		buf[i] = (0 != *m_pos++);
	}
	return true;
}

bool Aone2Decoder::get_byte_array(byte* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(byte)) return false;
	if (len > 0) memcpy(buf, m_pos, len * sizeof(byte));
	m_pos += len * sizeof(byte);
	return true;
}

bool Aone2Decoder::get_int8_array(int8* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(int8)) return false;
	if (len > 0) memcpy(buf, m_pos, len * sizeof(int8));
	m_pos += len * sizeof(int8);
	return true;
}

bool Aone2Decoder::get_uint8_array(uint8* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(uint8)) return false;
	if (len > 0) memcpy(buf, m_pos, len * sizeof(uint8));
	m_pos += len * sizeof(uint8);
	return true;
}

bool Aone2Decoder::get_int16_array(int16* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(int16)) return false;
	if (len > 0) memcpy(buf, m_pos, len * sizeof(int16));
	m_pos += len * sizeof(int16);
	return true;
}
bool Aone2Decoder::get_uint16_array(uint16* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(uint16)) return false;
	if (len > 0) memcpy(buf, m_pos, len * sizeof(uint16));
	m_pos += len * sizeof(uint16);
	return true;
}

bool Aone2Decoder::get_int32_array(int32* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(int32)) return false;
	if (len > 0) memcpy(buf, m_pos, len * sizeof(int32));
	m_pos += len * sizeof(int32);
	return true;
}

bool Aone2Decoder::get_uint32_array(uint32* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(uint32)) return false;
	if (len > 0) memcpy(buf, m_pos, len * sizeof(uint32));
	m_pos += len * sizeof(uint32);
	return true;
}

bool Aone2Decoder::get_int64_array(int64* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(int64)) return false;
	if (len > 0) memcpy(buf, m_pos, len * sizeof(int64));
	m_pos += len * sizeof(int64);
	return true;
}

bool Aone2Decoder::get_uint64_array(uint64* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(uint64)) return false;
	if (len > 0) memcpy(buf, m_pos, len * sizeof(uint64));
	m_pos += len * sizeof(uint64);
	return true;
}

bool Aone2Decoder::get_float32_array(float32* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(float32)) return false;
	if (len > 0) memcpy(buf, m_pos, len * sizeof(float32));
	m_pos += len * sizeof(float32);
	return true;
}

bool Aone2Decoder::get_float64_array(float64* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(float64)) return false;
	if (len > 0) memcpy(buf, m_pos, len * sizeof(float64));
	m_pos += len * sizeof(float64);
	return true;
}

bool Aone2Decoder::get_double_array(double* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(double)) return false;
	if (len > 0) memcpy(buf, m_pos, len * sizeof(double));
	m_pos += len * sizeof(double);
	return true;
}

bool Aone2Decoder::get_string_array(string* buf, uint32 len)
{
	if (buf == NULL) return false;
	for (uint32 i = 0; i < len; i++)
	{
		if (!get_string(buf[i])) return false;
	}
	return true;
}

bool Aone2Decoder::get_bool_vector(vector<bool>& val)
{
	uint32 len = 0;
	if (!get_uint32(len)) return false;
	if (remain() < len * sizeof(byte)) return false;
	for (uint16 i = 0; i < len; i++)
	{
		val.push_back(0 != *m_pos++);
	}
	return true;
}

bool Aone2Decoder::get_byte_vector(vector<byte>& val)
{
	uint32 len = 0;
	if (!get_uint32(len)) return false;
	if (remain() < len * sizeof(byte)) return false;
	val.insert(val.end(), m_pos, m_pos + len * sizeof(byte));
	m_pos += len * sizeof(byte);
	return true;
}

bool Aone2Decoder::get_int8_vector(vector<int8>& val)
{
	uint32 len = 0;
	if (!get_uint32(len)) return false;
	if (remain() < len * sizeof(int8)) return false;
	val.insert(val.end(), (const int8*)m_pos, ((const int8*)m_pos) + len);
	m_pos += len * sizeof(int8);
	return true;
}

bool Aone2Decoder::get_uint8_vector(vector<uint8>& val)
{
	uint32 len = 0;
	if (!get_uint32(len)) return false;
	if (remain() < len * sizeof(uint8)) return false;
	val.insert(val.end(), (const uint8*)m_pos, ((const uint8*)m_pos) + len);
	m_pos += len * sizeof(uint8);
	return true;
}

bool Aone2Decoder::get_int16_vector(vector<int16>& val)
{
	uint32 len = 0;
	if (!get_uint32(len)) return false;
	if (remain() < len * sizeof(int16)) return false;
	val.insert(val.end(), (const int16*)m_pos, ((const int16*)m_pos) + len);
	m_pos += len * sizeof(int16);
	return true;
}

bool Aone2Decoder::get_uint16_vector(vector<uint16>& val)
{
	uint32 len = 0;
	if (!get_uint32(len)) return false;
	if (remain() < len * sizeof(uint16)) return false;
	val.insert(val.end(), (const uint16*)m_pos, ((const uint16*)m_pos) + len);
	m_pos += len * sizeof(uint16);
	return true;
}

bool Aone2Decoder::get_int32_vector(vector<int32>& val)
{
	uint32 len = 0;
	if (!get_uint32(len)) return false;
	if (remain() < len * sizeof(int32)) return false;
	val.insert(val.end(), (const int32*)m_pos, ((const int32*)m_pos) + len);
	m_pos += len * sizeof(int32);
	return true;
}

bool Aone2Decoder::get_uint32_vector(vector<uint32>& val)
{
	uint32 len = 0;
	if (!get_uint32(len)) return false;
	if (remain() < len * sizeof(uint32)) return false;
	val.insert(val.end(), (const uint32*)m_pos, ((const uint32*)m_pos) + len);
	m_pos += len * sizeof(uint32);
	return true;
}

bool Aone2Decoder::get_int64_vector(vector<int64>& val)
{
	uint32 len = 0;
	if (!get_uint32(len)) return false;
	if (remain() < len * sizeof(int64)) return false;
	val.insert(val.end(), (const int64*)m_pos, ((const int64*)m_pos) + len);
	m_pos += len * sizeof(int64);
	return true;
}

bool Aone2Decoder::get_uint64_vector(vector<uint64>& val)
{
	uint32 len = 0;
	if (!get_uint32(len)) return false;
	if (remain() < len * sizeof(uint64)) return false;
	val.insert(val.end(), (const uint64*)m_pos, ((const uint64*)m_pos) + len);
	m_pos += len * sizeof(uint64);
	return true;
}

bool Aone2Decoder::get_float32_vector(vector<float32>& val)
{
	uint32 len = 0;
	if (!get_uint32(len)) return false;
	if (remain() < len * sizeof(float32)) return false;
	val.insert(val.end(), (const float32*)m_pos, ((const float32*)m_pos) + len);
	m_pos += len * sizeof(float32);
	return true;
}

bool Aone2Decoder::get_float64_vector(vector<float64>& val)
{
	uint32 len = 0;
	if (!get_uint32(len)) return false;
	if (remain() < len * sizeof(float64)) return false;
	val.insert(val.end(), (const float64*)m_pos, ((const float64*)m_pos) + len);
	m_pos += len * sizeof(float64);
	return true;
}

bool Aone2Decoder::get_double_vector(vector<double>& val)
{
	uint32 len = 0;
	if (!get_uint32(len)) return false;
	if (remain() < len * sizeof(double)) return false;
	val.insert(val.end(), (const double*)m_pos, ((const double*)m_pos) + len);
	m_pos += len * sizeof(double);
	return true;
}

bool Aone2Decoder::get_string_vector(vector<string>& val)
{
	uint32 len = 0;
	if (!get_uint32(len)) return false;
	for (uint32 i = 0; i < len; i++)
	{
		val.push_back(string());
		if (!get_string(val.back())) return false;
	}
	return true;
}



/////////////////////////////////////////////////////////////////////
// class Aone2Coder
/////////////////////////////////////////////////////////////////////
Aone2Coder::Aone2Coder(byte* buff, uint32 length)
	: m_buff(buff)
	, m_pos(buff)
	, m_end(buff + length)
{
	assert(m_buff);
}

Aone2Coder::Aone2Coder(const Aone2Coder& other)
	: m_buff(other.m_buff)
	, m_pos(other.m_pos)
	, m_end(other.m_end)
{
	// empty
}

Aone2Coder& Aone2Coder::operator=(const Aone2Coder& other)
{
	if (&other != this)
	{
		m_buff = other.m_buff;
		m_pos = other.m_pos;
		m_end = other.m_end;
	}
	return *this;
}

void Aone2Coder::attach(byte* buff, uint32 length)
{
	assert(buff);
	m_buff = buff;
	m_pos = m_buff;
	m_end = m_buff + length;
}

bool Aone2Coder::set_pos(byte* pos)
{
	assert(pos);
	assert(pos >= m_buff && pos <= m_end);
	if (pos < m_buff) return false;
	if (pos > m_end) return false;
	m_pos = pos;
	return true;
}

bool Aone2Coder::set_bool(bool val)
{
	if (remain() < sizeof(byte)) return false;
	*m_pos = val ? 1 : 0;
	m_pos += sizeof(byte);
	return true;
}

bool Aone2Coder::set_byte(byte val)
{
	if (remain() < sizeof(byte)) return false;
	memcpy(m_pos, &val, sizeof(byte));
	m_pos += sizeof(byte);
	return true;
}

bool Aone2Coder::set_int8(int8 val)
{
	if (remain() < sizeof(int8)) return false;
	memcpy(m_pos, &val, sizeof(int8));
	m_pos += sizeof(int8);
	return true;
}

bool Aone2Coder::set_uint8(uint8 val)
{
	if (remain() < sizeof(uint8)) return false;
	memcpy(m_pos, &val, sizeof(uint8));
	m_pos += sizeof(uint8);
	return true;
}

bool Aone2Coder::set_int16(int16 val)
{
	if (remain() < sizeof(int16)) return false;
	memcpy(m_pos, &val, sizeof(int16));
	m_pos += sizeof(int16);
	return true;
}

bool Aone2Coder::set_uint16(uint16 val)
{
	if (remain() < sizeof(uint16)) return false;
	memcpy(m_pos, &val, sizeof(uint16));
	m_pos += sizeof(uint16);
	return true;
}

bool Aone2Coder::set_int32(int32 val)
{
	if (remain() < sizeof(int32)) return false;
	memcpy(m_pos, &val, sizeof(int32));
	m_pos += sizeof(int32);
	return true;
}

bool Aone2Coder::set_uint32(uint32 val)
{
	if (remain() < sizeof(uint32)) return false;
	memcpy(m_pos, &val, sizeof(uint32));
	m_pos += sizeof(uint32);
	return true;
}

bool Aone2Coder::set_int64(int64 val)
{
	if (remain() < sizeof(int64)) return false;
	memcpy(m_pos, &val, sizeof(int64));
	m_pos += sizeof(int64);
	return true;
}

bool Aone2Coder::set_uint64(uint64 val)
{
	if (remain() < sizeof(uint64)) return false;
	memcpy(m_pos, &val, sizeof(uint64));
	m_pos += sizeof(uint64);
	return true;
}

bool Aone2Coder::set_float32(float32 val)
{
	if (remain() < sizeof(float32)) return false;
	memcpy(m_pos, &val, sizeof(float32));
	m_pos += sizeof(float32);
	return true;
}

bool Aone2Coder::set_float64(float64 val)
{
	if (remain() < sizeof(float64)) return false;
	memcpy(m_pos, &val, sizeof(float64));
	m_pos += sizeof(float64);
	return true;
}

bool Aone2Coder::set_double(double val)
{
	if (remain() < sizeof(double)) return false;
	memcpy(m_pos, &val, sizeof(double));
	m_pos += sizeof(double);
	return true;
}

bool Aone2Coder::set_string(const string& val)
{
	if (!set_uint32(val.size())) return false;
	if (remain() < val.size()) return false;
	if (!val.empty()) memcpy(m_pos, val.c_str(), val.size());
	m_pos += val.size();
	return true;
}

bool Aone2Coder::set_bool_array(const bool* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(byte)) return false;
	for (uint32 i = 0; i < len; i++)
	{
		*m_pos++ = m_buff[i] ? 1 : 0;
	}
	return true;
}

bool Aone2Coder::set_byte_array(const byte* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(byte)) return false;
	if (len > 0) memcpy(m_pos, buf, len * sizeof(byte));
	m_pos += len * sizeof(byte);
	return true;
}

bool Aone2Coder::set_int8_array(const int8* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(int8)) return false;
	if (len > 0) memcpy(m_pos, buf, len * sizeof(int8));
	m_pos += len * sizeof(int8);
	return true;
}

bool Aone2Coder::set_uint8_array(const uint8* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(uint8)) return false;
	if (len > 0) memcpy(m_pos, buf, len * sizeof(uint8));
	m_pos += len * sizeof(uint8);
	return true;
}

bool Aone2Coder::set_int16_array(const int16* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(int16)) return false;
	if (len > 0) memcpy(m_pos, buf, len * sizeof(int16));
	m_pos += len * sizeof(int16);
	return true;
}

bool Aone2Coder::set_uint16_array(const uint16* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(uint16)) return false;
	if (len > 0) memcpy(m_pos, buf, len * sizeof(uint16));
	m_pos += len * sizeof(uint16);
	return true;
}

bool Aone2Coder::set_int32_array(const int32* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(int32)) return false;
	if (len > 0) memcpy(m_pos, buf, len * sizeof(int32));
	m_pos += len * sizeof(int32);
	return true;
}

bool Aone2Coder::set_uint32_array(const uint32* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(uint32)) return false;
	if (len > 0) memcpy(m_pos, buf, len * sizeof(uint32));
	m_pos += len * sizeof(uint32);
	return true;
}

bool Aone2Coder::set_int64_array(const int64* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(int64)) return false;
	if (len > 0) memcpy(m_pos, buf, len * sizeof(int64));
	m_pos += len * sizeof(int64);
	return true;
}

bool Aone2Coder::set_uint64_array(const uint64* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(uint64)) return false;
	if (len > 0) memcpy(m_pos, buf, len * sizeof(uint64));
	m_pos += len * sizeof(uint64);
	return true;
}

bool Aone2Coder::set_float32_array(const float32* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(float32)) return false;
	if (len > 0) memcpy(m_pos, buf, len * sizeof(float32));
	m_pos += len * sizeof(float32);
	return true;
}

bool Aone2Coder::set_float64_array(const float64* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(float64)) return false;
	if (len > 0) memcpy(m_pos, buf, len * sizeof(float64));
	m_pos += len * sizeof(float64);
	return true;
}

bool Aone2Coder::set_double_array(const double* buf, uint32 len)
{
	if (buf == NULL) return false;
	if (remain() < len * sizeof(double)) return false;
	if (len > 0) memcpy(m_pos, buf, len * sizeof(double));
	m_pos += len * sizeof(double);
	return true;
}

bool Aone2Coder::set_string_array(const string* buf, uint32 len)
{
	if (buf == NULL) return false;
	for (uint32 i = 0; i < len; i++)
	{
		if (!set_string(buf[i])) return false;
	}
	return true;
}

bool Aone2Coder::set_bool_vector(const vector<bool>& vals)
{
	if (!set_uint32(vals.size())) return false;
	if (remain() < vals.size() * sizeof(byte)) return false;
	for (int i = 0; i < (int)vals.size(); i++)
	{
		*m_pos++ = vals[i] ? 1 : 0;
	}
	return true;
}

bool Aone2Coder::set_byte_vector(const vector<byte>& vals)
{
	if (!set_uint32(vals.size())) return false;
	if (remain() < vals.size() * sizeof(byte)) return false;
	if (!vals.empty()) memcpy(m_pos, &vals[0], vals.size() * sizeof(byte));
	m_pos += vals.size() * sizeof(byte);
	return true;
}

bool Aone2Coder::set_int8_vector(const vector<int8>& vals)
{
	if (!set_uint32(vals.size())) return false;
	if (remain() < vals.size() * sizeof(int8)) return false;
	if (!vals.empty()) memcpy(m_pos, &vals[0], vals.size() * sizeof(int8));
	m_pos += vals.size() * sizeof(int8);
	return true;
}

bool Aone2Coder::set_uint8_vector(const vector<uint8>& vals)
{
	if (!set_uint32(vals.size())) return false;
	if (remain() < vals.size() * sizeof(uint8)) return false;
	if (!vals.empty()) memcpy(m_pos, &vals[0], vals.size() * sizeof(uint8));
	m_pos += vals.size() * sizeof(uint8);
	return true;
}

bool Aone2Coder::set_int16_vector(const vector<int16>& vals)
{
	if (!set_uint32(vals.size())) return false;
	if (remain() < vals.size() * sizeof(int16)) return false;
	if (!vals.empty()) memcpy(m_pos, &vals[0], vals.size() * sizeof(int16));
	m_pos += vals.size() * sizeof(int16);
	return true;
}

bool Aone2Coder::set_uint16_vector(const vector<uint16>& vals)
{
	if (!set_uint32(vals.size())) return false;
	if (remain() < vals.size() * sizeof(uint16)) return false;
	if (!vals.empty()) memcpy(m_pos, &vals[0], vals.size() * sizeof(uint16));
	m_pos += vals.size() * sizeof(uint16);
	return true;
}

bool Aone2Coder::set_int32_vector(const vector<int32>& vals)
{
	if (!set_uint32(vals.size())) return false;
	if (remain() < vals.size() * sizeof(int32)) return false;
	if (!vals.empty()) memcpy(m_pos, &vals[0], vals.size() * sizeof(int32));
	m_pos += vals.size() * sizeof(int32);
	return true;
}

bool Aone2Coder::set_uint32_vector(const vector<uint32>& vals)
{
	if (!set_uint32(vals.size())) return false;
	if (remain() < vals.size() * sizeof(uint32)) return false;
	if (!vals.empty()) memcpy(m_pos, &vals[0], vals.size() * sizeof(uint32));
	m_pos += vals.size() * sizeof(uint32);
	return true;
}

bool Aone2Coder::set_int64_vector(const vector<int64>& vals)
{
	if (!set_uint32(vals.size())) return false;
	if (remain() < vals.size() * sizeof(int64)) return false;
	if (!vals.empty()) memcpy(m_pos, &vals[0], vals.size() * sizeof(int64));
	m_pos += vals.size() * sizeof(int64);
	return true;
}

bool Aone2Coder::set_uint64_vector(const vector<uint64>& vals)
{
	if (!set_uint32(vals.size())) return false;
	if (remain() < vals.size() * sizeof(uint64)) return false;
	if (!vals.empty()) memcpy(m_pos, &vals[0], vals.size() * sizeof(uint64));
	m_pos += vals.size() * sizeof(uint64);
	return true;
}

bool Aone2Coder::set_float32_vector(const vector<float32>& vals)
{
	if (!set_uint32(vals.size())) return false;
	if (remain() < vals.size() * sizeof(float32)) return false;
	if (!vals.empty()) memcpy(m_pos, &vals[0], vals.size() * sizeof(float32));
	m_pos += vals.size() * sizeof(float32);
	return true;
}

bool Aone2Coder::set_float64_vector(const vector<float64>& vals)
{
	if (!set_uint32(vals.size())) return false;
	if (remain() < vals.size() * sizeof(float64)) return false;
	if (!vals.empty()) memcpy(m_pos, &vals[0], vals.size() * sizeof(float64));
	m_pos += vals.size() * sizeof(float64);
	return true;
}

bool Aone2Coder::set_double_vector(const vector<double>& vals)
{
	if (!set_uint32(vals.size())) return false;
	if (remain() < vals.size() * sizeof(double)) return false;
	if (!vals.empty()) memcpy(m_pos, &vals[0], vals.size() * sizeof(double));
	m_pos += vals.size() * sizeof(double);
	return true;
}

bool Aone2Coder::set_string_vector(const vector<string>& vals)
{
	if (!set_uint32(vals.size())) return false;
	for (size_t i = 0; i < vals.size(); i++)
	{
		if (!set_string(vals[i])) return false;
	}
	return true;
}

uint32 Aone2Coder::size_bool(bool val)
{
	return sizeof(byte);
}

uint32 Aone2Coder::size_byte(byte val)
{
	return sizeof(byte);
}

uint32 Aone2Coder::size_int8(int8 val)
{
	return sizeof(int8);
}

uint32 Aone2Coder::size_uint8(uint8 val)
{
	return sizeof(uint8);
}

uint32 Aone2Coder::size_int16(int16 val)
{
	return sizeof(int16);
}

uint32 Aone2Coder::size_uint16(uint16 val)
{
	return sizeof(uint16);
}

uint32 Aone2Coder::size_int32(int32 val)
{
	return sizeof(int32);
}

uint32 Aone2Coder::size_uint32(uint32 val)
{
	return sizeof(uint32);
}

uint32 Aone2Coder::size_int64(int64 val)
{
	return sizeof(int64);
}

uint32 Aone2Coder::size_uint64(uint64 val)
{
	return sizeof(uint64);
}

uint32 Aone2Coder::size_float32(float32 val)
{
	return sizeof(float32);
}

uint32 Aone2Coder::size_float64(float64 val)
{
	return sizeof(float64);
}

uint32 Aone2Coder::size_double(double val)
{
	return sizeof(double);
}

uint32 Aone2Coder::size_string(const string& val)
{
	return sizeof(uint32) + val.size();
}

uint32 Aone2Coder::size_bool_array(const bool* buf, uint32 len)
{
	return len * sizeof(byte);
}

uint32 Aone2Coder::size_byte_array(const byte* buf, uint32 len)
{
	return len * sizeof(byte);
}

uint32 Aone2Coder::size_int8_array(const int8* buf, uint32 len)
{
	return len * sizeof(int8);
}

uint32 Aone2Coder::size_uint8_array(const uint8* buf, uint32 len)
{
	return len * sizeof(int16);
}

uint32 Aone2Coder::size_int16_array(const int16* buf, uint32 len)
{
	return len * sizeof(int16);
}

uint32 Aone2Coder::size_uint16_array(const uint16* buf, uint32 len)
{
	return len * sizeof(uint16);
}

uint32 Aone2Coder::size_int32_array(const int32* buf, uint32 len)
{
	return len * sizeof(int32);
}

uint32 Aone2Coder::size_uint32_array(const uint32* buf, uint32 len)
{
	return len * sizeof(uint32);
}

uint32 Aone2Coder::size_int64_array(const int64* buf, uint32 len)
{
	return len * sizeof(int64);
}

uint32 Aone2Coder::size_uint64_array(const uint64* buf, uint32 len)
{
	return len * sizeof(uint64);
}

uint32 Aone2Coder::size_float32_array(const float32* buf, uint32 len)
{
	return len * sizeof(float32);
}

uint32 Aone2Coder::size_float64_array(const float64* buf, uint32 len)
{
	return len * sizeof(float64);
}

uint32 Aone2Coder::size_double_array(const double* buf, uint32 len)
{
	return len * sizeof(double);
}

uint32 Aone2Coder::size_string_array(const string* buf, uint32 len)
{
	uint32 sz = 0;
	for (uint32 i = 0; i < len; i++)
	{
		sz += size_string(buf[i]);
	}
	return sz;
}

uint32 Aone2Coder::size_bool_vector(const vector<bool>& vals)
{
	return vals.size() * sizeof(byte);
}

uint32 Aone2Coder::size_byte_vector(const vector<byte>& vals)
{
	return vals.size() * sizeof(byte);
}

uint32 Aone2Coder::size_int8_vector(const vector<int8>& vals)
{
	return vals.size() * sizeof(int8);
}

uint32 Aone2Coder::size_uint8_vector(const vector<uint8>& vals)
{
	return vals.size() * sizeof(uint8);
}

uint32 Aone2Coder::size_int16_vector(const vector<int16>& vals)
{
	return vals.size() * sizeof(int16);
}

uint32 Aone2Coder::size_uint16_vector(const vector<uint16>& vals)
{
	return vals.size() * sizeof(uint16);
}

uint32 Aone2Coder::size_int32_vector(const vector<int32>& vals)
{
	return vals.size() * sizeof(int32);
}

uint32 Aone2Coder::size_uint32_vector(const vector<uint32>& vals)
{
	return vals.size() * sizeof(uint32);
}

uint32 Aone2Coder::size_int64_vector(const vector<int64>& vals)
{
	return vals.size() * sizeof(int64);
}

uint32 Aone2Coder::size_uint64_vector(const vector<uint64>& vals)
{
	return vals.size() * sizeof(uint64);
}

uint32 Aone2Coder::size_float32_vector(const vector<float32>& vals)
{
	return vals.size() * sizeof(float32);
}

uint32 Aone2Coder::size_float64_vector(const vector<float64>& vals)
{
	return vals.size() * sizeof(float64);
}

uint32 Aone2Coder::size_double_vector(const vector<double>& vals)
{
	return vals.size() * sizeof(double);
}

uint32 Aone2Coder::size_string_vector(const vector<string>& vals)
{
	uint32 sz = 0;
	for (size_t i = 0; i < vals.size(); i++)
	{
		sz += size_string(vals[i]);
	}
	return sz;
}

}//namespace aone2

