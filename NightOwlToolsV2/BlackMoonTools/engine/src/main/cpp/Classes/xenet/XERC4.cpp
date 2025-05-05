#include "xenet/XERC4.h"
#include "xenet/XEMD5.h"

NS_XE_BEGIN

XERC4::XERC4()
: m_index_i(0)
, m_index_j(0)
{
	// empty
}

void XERC4::init(byte* key, byte keylen)
{
	assert(key && keylen > 0);

	m_index_i = 0;
	m_index_j = 0;

	byte tmp;

	//Fill in linearly s0=0, s1=1, ...
	for (int i = 0; i < 256; i++)
	{
		m_sbox[i] = i;
	}

	int j = 0;
	for (int i = 0; i < 256; i++)
	{
		// j = (j + Si + Ki) mod 256
		j = (j + m_sbox[i] + key[i % keylen]) & 255;

		tmp = m_sbox[i];
		m_sbox[i] = m_sbox[j];
		m_sbox[j] = tmp;
	}
}

void XERC4::init(const std::string& key)
{
	byte digest1[16], digest2[16];
	md5(key.c_str(), key.size(), digest1);
	byte tmp = digest1[7];
	digest1[7] = digest1[13];
	digest1[13] = tmp;
	md5(digest1, 16, digest2);
	init(digest2, 16);
}

void XERC4::update(const byte* input, byte* output, uint32 length)
{
	byte tmp;

	while (length-- > 0)
	{
		// Get the next S box indices...
		m_index_i = (m_index_i + 1) & 255;
		m_index_j = (m_index_j + m_sbox[m_index_i]) & 255;

		// Swap Si and Sj...
		tmp = m_sbox[m_index_i];
		m_sbox[m_index_i] = m_sbox[m_index_j];
		m_sbox[m_index_j] = tmp;

		// Get the S box index for this byte...
		int t = (m_sbox[m_index_i] + m_sbox[m_index_j]) & 255;

		// Encrypt using the S box...
		*output++ = *input++ ^ m_sbox[t];
	}
}

NS_XE_END
