

#include "encoding.h"
#include <string.h>

/*

n |  Unicode token range   |  UTF-8 encoding style
--+------------------------+------------------------------------------------------
1 |      0000 -      007F  |                                              0xxxxxxx
2 |      0080 -      07FF  |                                     110xxxxx 10xxxxxx
3 |      0800 -      FFFF  |                            1110xxxx 10xxxxxx 10xxxxxx
4 | 0001 0000 - 0010 FFFF  |                   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
5 | 0020 0000 - 03FF FFFF  |          111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
6 | 0400 0000 - 7FFF FFFF  | 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx

*/

#define match0xxxxxxx(c) ( 0x00 == ((c)&0x80) )
#define match10xxxxxx(c) ( 0x80 == ((c)&0xC0) )
#define match110xxxxx(c) ( 0xC0 == ((c)&0xE0) )
#define match1110xxxx(c) ( 0xE0 == ((c)&0xF0) )
#define match11110xxx(c) ( 0xF0 == ((c)&0xF8) )
#define match111110xx(c) ( 0xF8 == ((c)&0xFC) )
#define match1111110x(c) ( 0xFC == ((c)&0xFE) )

static const int MFC = 6;
static const function<bool(char)> MFS[MFC] = {
	[](char c){ return match0xxxxxxx(c); },
	[](char c){ return match110xxxxx(c); },
	[](char c){ return match1110xxxx(c); },
	[](char c){ return match11110xxx(c); },
	[](char c){ return match111110xx(c); },
	[](char c){ return match1111110x(c); },
};

int utf8_compute_word_length_by_head_byte(char c)
{
	for (int i = 0; i < MFC; ++i)
	{
		if (MFS[i](c)){
			return i + 1;
		}
	}

	return 0;
}
bool utf8_verify_word_tail(const char* str, int len)
{
	for (int i = 1; i < len; ++i)
	{
		char c = str[i];

		if (false == match10xxxxxx(c))
			return false;
	}

	return true;
}
bool utf8_verify_word(const char* str, int len)
{
	if (str == nullptr || len <= 0 || strlen(str) < len)
		return false;

	int l = utf8_compute_word_length_by_head_byte(*str);

	if (false == utf8_verify_word_tail(str, l))
		return false;

	return true;
}

namespace Encoding
{
	bool utf8::verifyString(const char* str)
	{
		int len = strlen(str);
		int offset = 0;

		while (offset < len)
		{
			const char * word_s = str + offset;
			int          word_l = utf8_compute_word_length_by_head_byte(*word_s);

			if (word_l > 3)
				return false;

			if (false == utf8_verify_word_tail(word_s, word_l))
				return false;

			uint32_t word_u;
			
			if (false == utils::convert_word_utf8_to_unicode(word_s, word_l, &word_u))
				return false;

			if (false == unicode::verifyWord(word_u))
				return false;

			offset += word_l;
		}

		return true;
	}

	int utf8::computeWordNumber(const char* str)
	{
		int len = strlen(str);
		int offset = 0;
		int count = 0;

		while (offset < len)
		{
			offset += utf8_compute_word_length_by_head_byte(*(str + offset));
			count++;
		}

		return count;
	}

	int utf8::computeWordLength_ext(const char* str)
	{
		int len = strlen(str);
		int offset = 0;
		int count = 0;

		while (offset < len)
		{
			int l = utf8_compute_word_length_by_head_byte(*(str + offset));
			offset += l;

			l == 1 ? count += 1 : count += 2;
		}

		return count;
	}

}
