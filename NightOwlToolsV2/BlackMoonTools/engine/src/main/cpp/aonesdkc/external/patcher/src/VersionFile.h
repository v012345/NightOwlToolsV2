#ifndef _VERSION_FILE_H_
#define _VERSION_FILE_H_

#include <string>
#include "def.h"

namespace AonePatcher
{
	class VersionFile
	{
	public:
		VersionFile(std::string filePath);
		bool isNewThan(VersionFile& v);
		bool isDiffer(VersionFile& v);

	private:
		uint32 m_majorVersion;
		uint32 m_minorVersion;
		uint32 m_revisionVersion;
	};
}

#endif