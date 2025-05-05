/**************************************************************************
*  @Copyright (c) 2017, A-one Soft, All rights reserved.

*  @file     	: XERC4.h
*  @version	: ver 1.0.0

*  @author	: andy-zhao
*  @date		: 2017/03/10
*  @brief		:
**************************************************************************/
#ifndef _XERC4_H_
#define _XERC4_H_

#include "xenet/XEStdC.h"

NS_XE_BEGIN

class XERC4
{
public:
	XERC4();

	// 使用字节序列初始化S-box
	void init(byte* key, byte keylen);

	// 使用字符串初始化S-box
	void init(const std::string& key);

	// 加/解密
	void update(const byte* input, byte* output, uint32 length);

private:
	byte  m_sbox[256];
	byte  m_index_i;
	byte  m_index_j;
};

NS_XE_END

#endif // _XERC4_H_