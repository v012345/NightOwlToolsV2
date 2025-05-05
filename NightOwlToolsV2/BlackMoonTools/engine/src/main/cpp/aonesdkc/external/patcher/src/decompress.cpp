
#include "decompress.h"
#include <stdlib.h>
#include "def.h"
#include "cplog.h"
#include "PatcherException.h"
#include "errno.h"

extern "C"
{
#include "zlib.h"
}

namespace AonePatcher
{
	enum EDcErr
	{
		None
		, NoMemoryForCache
		, CannotOpenZipFile
		, CannotOpenDezFile // can not open decompress target file
		, GzipReadError
		, WriteFileError
	};

	static const char* CDcErr[] = {
		"",
		"NOT ENOUGH MEMORY FOR CACHE!",
		"CANNOT OPEN GZIP FILE : ",
		"CANNOT OPEN TARGET FILE : ",
		"GZIP READ ERROR!",
		"WRITE FILE ERROR!",
	};

	class DecompressImp
	{
	public:
		gzFile zipp_;
	};

	Decompress::Decompress(void)
		: zipf_("")
		, dzff_("")
		, imp_(DecompressImpPtr(new DecompressImp()))
		, dzfp_(nullptr)
		, errcod_(0)
		, csize_(20 * 1024)
		, cache_(nullptr)
		, errmsg_("")
	{

	}

	Decompress::~Decompress(void)
	{
		cleanup_();
	}

	void Decompress::zipfile(const char* val)
	{
		zipf_ = val;
	}

	void Decompress::locfile(const char* val)
	{
		dzff_ = val;
	}

	void Decompress::cache(int val)
	{
		if (val > 0)
			csize_ = val;

		if (csize_ < 1024){
			csize_ = 1024;
		}
	}

	int Decompress::decompress(void)
	{
		do
		{
			errcod_ = initcache_();
			if (EDcErr::None != errcod_){
				errmsg_ = CDcErr[errcod_];
				break;
			}

			errcod_ = openzip_();
			if (EDcErr::None != errcod_){
				errmsg_ = CDcErr[errcod_] + zipf_;
				break;
			}

			errcod_ = opendzf_();
			if (EDcErr::None != errcod_){
				errmsg_ = CDcErr[errcod_] + dzff_;
				break;
			}

			errcod_ = decompress_();
			if (EDcErr::None != errcod_){
				errmsg_ = CDcErr[errcod_];
				break;
			}

			errcod_ = EDcErr::None;
		} while (false);

		cleanup_();

		return resultCode();
	}

	std::string Decompress::getUncompressedFileName(const string& compressedFileName)
	{
		const string gzPostfix = ".gz";
		string retStr = compressedFileName;
		if (retStr.size() < gzPostfix.size())
		{
			throw(PatcherException(PatchErrorCode::Common, 0, "too short compressed name" + compressedFileName));
		}

		if (retStr.substr(retStr.size() - gzPostfix.size()) != gzPostfix)
		{
            printf("\nPatcher ----, size = %lu, fix size = %lu, sub = %s, gz = %s\n",retStr.size(), gzPostfix.size(), retStr.substr(retStr.size() - gzPostfix.size()).c_str(), gzPostfix.c_str());
			throw(PatcherException(PatchErrorCode::Common, 0, "compressed name not end by .gz" + compressedFileName));
		}
		retStr.erase(retStr.size() - gzPostfix.size());
		return retStr;
	}

	int Decompress::initcache_(void)
	{
		cache_ = (char*)malloc(csize_);

		if (NULL == cache_)
			return EDcErr::NoMemoryForCache;

		return EDcErr::None;
	}

	int Decompress::openzip_(void)
	{
		imp_->zipp_ = gzopen(zipf_.c_str(), "r");

		if (NULL == imp_->zipp_)
			return EDcErr::CannotOpenZipFile;

		return EDcErr::None;
	}

	int Decompress::opendzf_(void)
	{
		dzfp_ = fopen(dzff_.c_str(), "wb+");

		if (NULL == dzfp_)
			return EDcErr::CannotOpenDezFile;

		return EDcErr::None;
	}


	int Decompress::decompress_(void)
	{
		while (0 == gzeof(imp_->zipp_))
		{
			int size = gzread(imp_->zipp_, cache_, csize_);

			if (size <= 0)
			{
				if (0 != gzeof(imp_->zipp_))
				{
					CP_LOGW("gzread return 0 and gzeof is true");
					return EDcErr::None;
				}
				else
				{
					int gzErrorNum;
					const char* errorStr = gzerror(imp_->zipp_, &gzErrorNum);
					CP_LOGE("decompress fail, error str is %s, gzErrorNum is %d, errno is %d", errorStr, gzErrorNum, errno);
					if (gzErrorNum == Z_ERRNO)
					{
						return 20000 + errno;
					}
					else
					{
						return 30000 + gzErrorNum;
					}
				}
			}

			if (fwrite(cache_, 1, size, dzfp_) != size)
				return EDcErr::WriteFileError;
		}

		return EDcErr::None;
	}

	void Decompress::cleanup_(void)
	{
		if (nullptr != cache_)
			free(cache_);

		if (NULL != imp_->zipp_)
			gzclose(imp_->zipp_);

		if (nullptr != dzfp_)
			fclose(dzfp_);

		cache_ = nullptr;
		imp_->zipp_ = NULL;
		dzfp_ = nullptr;
	}

	int decompress_gzip(const char* gz, const char* uz, int cache_size /*= -1 */)
	{
		Decompress g;
		g.zipfile(gz);
		g.locfile(uz);
		g.cache(cache_size);

		int ret = g.decompress();
		if (ret != 0)
			ZF_LOGE(" -- msg : decompress error %d, %s", ret, gz);

		return ret;
	}
}
