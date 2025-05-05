

#ifndef ___utf8_encoding_h___
#define ___utf8_encoding_h___

#include "encoding.h"

namespace Encoding
{
	class utf8
	{
	public:

		static bool verifyString(const char* str);
		static int  computeWordNumber(const char* str);

		// ×ÖÄ¸ x1 byte, ºº×Ö x2 byte
		static int  computeWordLength_ext(const char* str);

	protected:
		utf8(void);
		utf8(const utf8&);
		void operator= (const utf8 &);

	};
}


#endif
