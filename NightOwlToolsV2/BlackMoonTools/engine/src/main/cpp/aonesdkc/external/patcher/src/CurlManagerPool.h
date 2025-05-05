#ifndef _CURL_MANAGER_POOL_H_
#define _CURL_MANAGER_POOL_H_

#include <curl/curl.h>
#include <vector>
#include <condition_variable>

namespace AonePatcher
{
	struct CurlInfo
	{
		CURL* curl;
		bool isUsed;
		int tag;
		struct curl_slist *list = NULL;
	};

	class CurlManagerPool
	{
	public:
		static CurlManagerPool* GetInstance();
		CurlInfo GetAvailableCurl();
		void TabCurlUnusedByTag(int tag);
		void CleanUpAllCurls();

		~CurlManagerPool();

	private:
		CurlManagerPool();
		void CreateCurl();


	private:
		std::mutex m_mutex;
		int m_maxCount;
		std::vector<CurlInfo> m_allCurls;
	};
}

#endif // _CURL_MANAGER_POOL_H_