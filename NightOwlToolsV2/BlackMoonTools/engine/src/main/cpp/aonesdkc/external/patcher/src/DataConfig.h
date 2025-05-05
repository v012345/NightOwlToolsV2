#include <map>

namespace AonePatcher
{
	class DataConfig
	{
	public:
		~DataConfig();

		static DataConfig* getInstance();

		void parseData();

		int getThreadNum();
		int getFailRetryNum();
		int getFrameNumOneSec();
		int getDLSpeedLimit();
		int getDLSpeedTime();
	private:
		DataConfig();

		void strRemove(std::string& line);
		void strSplit(std::string line, std::string& dataType, int& data);

	private:
		std::map<std::string, int> m_configData;
	};
}
