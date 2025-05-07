/**************************************************************************
*  @Copyright (c) 2017, A-one Soft, All rights reserved.

*  @file     	: AEMD5.h
*  @version	: ver 1.0.0

*  @author	: andy-zhao
*  @date		: 2017/03/10
*  @brief		:
**************************************************************************/
#ifndef _AEMD5_H_
#define _AEMD5_H_

#include "platform/AEStdC.h"

NS_AE_BEGIN

class AEMD5
{
public:
	AEMD5();

	void initialize(void);

	void update(const byte* input, uint32 inputLen);

	void final(byte(&digest)[16]);

	std::string final();

private:
	void transform(const byte* block);

	void encode(byte* output, const uint32* input, uint32 inputLen);

	void decode(uint32* output, const byte* input, uint32 inputLen);

private:
	uint32 m_count[2];
	uint32 m_state[4];
	byte m_buffer[64];
};

void md5(const void* src, uint32 size, byte(&digest)[16]);

std::string md5(const void* src, uint32 size);

void hmac_md5(const void* src, uint32 size_, const void* key, uint32 key_size_, byte(&digest)[16]);

std::string hmac_md5(const void* src, uint32 size_, const void* key, uint32 key_size_);

// 计算文件md5,失败返回空字符串
std::string md5_file(const std::string& filepath);

NS_AE_END

#endif // _AEMD5_H_