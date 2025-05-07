

#ifndef ___encoding_unicode_h___
#define ___encoding_unicode_h___

#include "encoding.h"

namespace Encoding
{
	class unicode
	{
	public:
		
		static bool verifyWord(uint32_t u);
		static void addUnicodeScope(int min, int max);
	
	
	protected:
		unicode (void);
		unicode (const unicode& );
		void operator= (const unicode & );
		
	};

}

#endif
