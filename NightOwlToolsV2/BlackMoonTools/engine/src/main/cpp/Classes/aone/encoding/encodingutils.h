

#ifndef ___encoding_utils_h___
#define ___encoding_utils_h___

#include "encoding.h"

namespace Encoding
{
	class utils
	{
	public:

		static bool convert_word_utf8_to_unicode(const char* str, int len, uint32_t *output);
	
	
	protected:
		utils (void);
		utils (const utils& );
		void operator= (const utils & );
		
	};



}


#endif
