

#ifndef ___uncompress_gzip_h___
#define ___uncompress_gzip_h___

#include <stdio.h>
#include <string>
#include <memory>
#include "def.h"
using namespace std;

namespace AonePatcher
{
	class DecompressImp;
	typedef shared_ptr<DecompressImp> DecompressImpPtr;

	class Decompress
	{
	public:
		Decompress(void);
		virtual ~Decompress(void);

		virtual void zipfile(const char* val);
		virtual void locfile(const char* val);
		virtual void cache(int val);
		virtual int decompress(void);

		virtual int
			resultCode(void){ return errcod_; }
		virtual string&
			resultText(void){ return errmsg_; }

		static string getUncompressedFileName(const string& compressedFileName);

	protected:

		int initcache_(void);
		int openzip_(void);
		int opendzf_(void);
		int decompress_(void);
		void cleanup_(void);

		string	zipf_;
		string	dzff_;

		int		csize_;
		char*	cache_;

		DecompressImpPtr	imp_;
		FILE*	dzfp_;

		int32 errcod_;
		string  errmsg_;
	};

	int decompress_gzip(
		const char* zipf,
		const char* locf,
		int cache_size = -1
		);
}
#endif




