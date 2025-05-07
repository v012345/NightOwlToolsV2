#include "CurlManagerPool.h"
#include "DataConfig.h"

namespace AonePatcher
{
	CurlManagerPool::CurlManagerPool()
	{
		m_maxCount = DataConfig::getInstance()->getThreadNum();
	}
	
	static CurlManagerPool* _instance;
	CurlManagerPool* CurlManagerPool::GetInstance()
	{
		if (_instance == NULL)
			_instance = new CurlManagerPool();

		return _instance;
	}

	CurlInfo CurlManagerPool::GetAvailableCurl()
	{
		std::lock_guard<std::mutex> locker(m_mutex);

		CurlInfo curlInfo;

		if ((int)m_allCurls.size() < m_maxCount)
		{
			CreateCurl();
			m_allCurls[m_allCurls.size() - 1].isUsed = true;

			return m_allCurls[m_allCurls.size() - 1];
		}
		else
		{
			for (auto& curlInfo : m_allCurls)
			{
				if (curlInfo.isUsed == false)
				{
					curlInfo.isUsed = true;
					return curlInfo;
				}
			}
		}

		return std::move(curlInfo);
	}

	void CurlManagerPool::TabCurlUnusedByTag(int tag)
	{
		std::lock_guard<std::mutex> locker(m_mutex);

		for (auto& curlInfo : m_allCurls)
		{
			if (curlInfo.tag == tag)
				curlInfo.isUsed = false;
		}
	}

	void CurlManagerPool::CleanUpAllCurls()
	{
		std::lock_guard<std::mutex> locker(m_mutex);

		for (auto& curlInfo : m_allCurls)
		{
			if (NULL != curlInfo.curl)
			{
				curl_slist_free_all(curlInfo.list);
				curl_easy_cleanup(curlInfo.curl);
			}
		}

		m_allCurls.clear();
	}

	void CurlManagerPool::CreateCurl()
	{
		int index = (int)m_allCurls.size() + 1;

		CurlInfo curlInfo;
		curlInfo.curl = curl_easy_init();
		curlInfo.isUsed = false;
		curlInfo.tag = index;

		curlInfo.list = curl_slist_append(curlInfo.list, "Connection: keep-alive");
		curl_easy_setopt(curlInfo.curl, CURLOPT_HTTPHEADER, curlInfo.list);

		m_allCurls.push_back(curlInfo);
	}

	CurlManagerPool::~CurlManagerPool()
	{
		delete _instance;
		_instance = NULL;
	}
}
