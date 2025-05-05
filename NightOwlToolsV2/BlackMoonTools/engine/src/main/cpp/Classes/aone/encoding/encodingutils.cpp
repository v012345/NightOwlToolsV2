
#include "encoding.h"

namespace Encoding
{
	bool utils::convert_word_utf8_to_unicode(const char* str, int len, uint32_t *output)
	{
		uint32_t u = 0;
		char c0, c1, c2;
		uint32_t h, l;

		switch (len)
		{
		case 1:
			u = (uint32_t)str[0];
			break;
		case 2:
			c0 = (str[0] & 0x1F);
			c1 = (str[1] & 0x3F);

			h = (c0 >> 2);
			l = (((c0 & 0x03) << 6) | c1);

			u = (h << 8) | l;
			break;
		case 3:
			c0 = (str[0] & 0x0F);
			c1 = (str[1] & 0x3F);
			c2 = (str[2] & 0x3F);

			h = ((c0 << 4) | (c1 >> 2));
			l = ((c1 << 6) | c2);

			u = (h << 8) | l;
			break;
		case 4:
		case 5:
		case 6:
		default:
			
			return false;
		};

		if (output != nullptr)
			*output = u;

		return true;
	}
}