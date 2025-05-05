#ifndef _CPFILEUTILS_H_
#define _CPFILEUTILS_H_

//cross platform file utils
#include <string>
#include <sstream>
#include <vector>

namespace CPUtils
{
	typedef const char cchr;

	class CPFileUtils
	{
	public:
		static std::string getDataFromFile(const std::string& filename);
		static std::string getStringFromFile(const std::string& filename);
		static std::string getWriteablePath();
		static void skipBackupToApplicationDic();
        static std::string concateFilePath(const std::string& dirPath, const std::string& relFilePath);
	public:
		static std::string fixpath(cchr* path);

		static bool mkdir(cchr *path);
		static bool rmdir(cchr *path);
		static bool mdbyf(cchr *path); // make dir by file
		static bool exist(cchr *path);
		static bool remove(cchr *path);
		static bool rename(cchr *src, cchr *dist);

		static bool copy(cchr *srcPath, cchr *destPath);
		static void rmdirAll(const std::string& strDirPath);
		static int getFileSize(cchr *file);

		static void purgeCachedEntries();

		static bool writedata(void* data, size_t size, cchr *path);
		static bool appenddata(void* data, size_t size, cchr *path);
		static std::istream& safeGetline(std::istream& is, std::string& t);

	protected:
		static bool mkdir_(cchr *path);
		static bool rmdir_(cchr *path);

		/*
		* warning :
		*
		*     PLEASE confirm that THE "temp" is enough to hold the path.
		*
		*/
		static void fixpath_(cchr* path, char *temp);

	protected:

		static cchr SP;
		static cchr NOSP;
	};
}

#endif
