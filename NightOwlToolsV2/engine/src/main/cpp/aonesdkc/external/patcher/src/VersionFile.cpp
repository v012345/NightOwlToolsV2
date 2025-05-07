#include "VersionFile.h"

#include "CPFileUtils.h"
#include "PatcherException.h"
#include <cstdlib>
#include "cplog.h"
#include <vector>

namespace AonePatcher
{
	VersionFile::VersionFile(std::string filePath)
	{
		m_majorVersion = 0;
		m_minorVersion = 0;
		m_revisionVersion = 0;

        std::string versionPath = CPUtils::CPFileUtils::concateFilePath(filePath, "version");
		if (!CPUtils::CPFileUtils::exist(versionPath.c_str()))
		{
			CP_LOGE("VersionFile: no version in this filepath");
			return;
		}

		std::string sVersion = CPUtils::CPFileUtils::getDataFromFile(versionPath);
        printf("Patcher --- version path = %s\nversion is %s\n", versionPath.c_str(), sVersion.c_str());
		if (sVersion.size() == 0)
			throw(PatcherException(PatchErrorCode::Common, 0, "no data in version"));

		int index = 0;
		std::vector<std::string> splitStr{"0", "0", "0"};
		
		for (int i = 0; i < (int)sVersion.size(); ++i)
		{
			int pointPos = sVersion.find(".", i);
			if (pointPos < (int)sVersion.size())
			{
				if (pointPos < 0)
				{
					splitStr[index++] = sVersion.substr(i);
					break;
				}

				splitStr[index++] = sVersion.substr(i, pointPos);
				i = pointPos;
			}
			
		}

		m_majorVersion = std::atoi(splitStr[0].c_str());
		m_minorVersion = std::atoi(splitStr[1].c_str());
		m_revisionVersion = std::atoi(splitStr[2].c_str());
	}

	bool VersionFile::isNewThan(VersionFile& v)
	{
		if (m_majorVersion > v.m_majorVersion)
			return true;

		if (m_majorVersion == v.m_majorVersion && m_minorVersion > v.m_minorVersion)
			return true;

		if (m_majorVersion == v.m_majorVersion && m_minorVersion == v.m_minorVersion && m_revisionVersion > v.m_revisionVersion)
			return true;

		return false;
	}

	bool VersionFile::isDiffer(VersionFile& v)
	{
		if (m_majorVersion == v.m_majorVersion && m_minorVersion == v.m_minorVersion && m_revisionVersion == v.m_revisionVersion)
			return false;

		return true;
	}
}
