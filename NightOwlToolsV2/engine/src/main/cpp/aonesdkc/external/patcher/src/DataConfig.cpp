#include "DataConfig.h"
#include "CPFileUtils.h"
#include "PatcherException.h"

#include <vector>
#include <cstdlib>

namespace AonePatcher
{
	DataConfig::DataConfig()
	{
		parseData();
	}

	static DataConfig* _instance = NULL;
	DataConfig* DataConfig::getInstance()
	{
		if (_instance == NULL)
		{
			_instance = new DataConfig();
		}

		return _instance;
	}

	void DataConfig::strRemove(std::string& line)
	{
		int begin = 0;

		begin = line.find(" ", begin);

		while (begin != -1)
		{
			line.replace(begin, 1, "");

			begin = line.find(" ", begin);
		}
	}

	void DataConfig::strSplit(std::string line, std::string& dataType, int& data)
	{
		int pos = line.find(":");
		if (pos <= 0 || pos > (int)line.size())
			throw(PatcherException(PatchErrorCode::Common, 0, "format error: patcher_config.txt"));

		dataType = line.substr(0, pos);
		std::string a = line.substr(pos + 1);
		int b = std::atoi(a.c_str());
		data = std::atoi(a.c_str());
	}

	void DataConfig::parseData()
	{
		std::vector<std::string> originalFile;
		std::string line;
		std::string data = CPUtils::CPFileUtils::getStringFromFile("patcher_config.txt");
		std::istringstream stream(data);
		while (!CPUtils::CPFileUtils::safeGetline(stream, line).eof())
		{
			if (line != "")
				originalFile.push_back(line);
		}
		if (originalFile.size() == 0)
			return;

		std::vector<std::string>::iterator it = originalFile.begin();
		for (; it != originalFile.end(); ++it)
		{
			strRemove(*it);
			std::string dataType;
			int data = 0;
			strSplit(*it, dataType, data);
			if (m_configData.find(dataType) != m_configData.end())
			{
				m_configData[dataType] = data;
				continue;
			}

			m_configData.insert(make_pair(dataType, data));
		}
	}

	int DataConfig::getThreadNum()
	{
		if (m_configData["thread_num"] == 0 || m_configData.find("thread_num") == m_configData.end())
			return 5;
		return m_configData["thread_num"];
	}

	int DataConfig::getFailRetryNum()
	{
		if (m_configData["fail_retry_num"] == 0 || m_configData.find("fail_retry_num") == m_configData.end())
			return 3;
		return m_configData["fail_retry_num"];
	}

	int DataConfig::getFrameNumOneSec()
	{
		if (m_configData["frame_num"] == 0 || m_configData.find("frame_num") == m_configData.end())
			return 30;
		return m_configData["frame_num"];
	}

	int DataConfig::getDLSpeedLimit()
	{
		if (m_configData["speed_limit"] == 0 || m_configData.find("speed_limit") == m_configData.end())
			return 2;
		return m_configData["speed_limit"];
	}

	int DataConfig::getDLSpeedTime()
	{
		if (m_configData["speed_time"] == 0 || m_configData.find("speed_time") == m_configData.end())
			return 20;
		return m_configData["speed_time"];
	}

	DataConfig::~DataConfig()
	{
		m_configData.clear();
	}
}