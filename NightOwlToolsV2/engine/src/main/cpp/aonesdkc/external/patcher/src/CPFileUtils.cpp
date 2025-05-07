#include "CPFileUtils.h"
#include "cpfile.h"
//#include "platform/CCFileUtils.h"
#include "cplog.h"
#include <errno.h>
#include <sys/stat.h>

#if (defined UTILS_PLATFORM_ANDROID) || (defined UTILS_PLATFORM_IOS)
extern "C"
{
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
}
#endif
namespace CPUtils
{
#ifdef UTILS_PLATFORM_WIN32

#	include <Windows.h>
#	include <fcntl.h>
#	include <io.h>

	cchr CPFileUtils::NOSP = '/';
	cchr CPFileUtils::SP = '\\';

	bool CPFileUtils::mkdir_(cchr *path)
	{
		return CreateDirectoryA(path, NULL) == TRUE;
	}
	static std::string StringWideCharToUtf8(const std::wstring& strWideChar)
	{
		std::string ret;
		if (!strWideChar.empty())
		{
			int nNum = WideCharToMultiByte(CP_UTF8, 0, strWideChar.c_str(), -1, NULL, 0, NULL, FALSE);
			if (nNum)
			{
				char* utf8String = new char[nNum + 1];
				utf8String[0] = 0;

				nNum = WideCharToMultiByte(CP_UTF8, 0, strWideChar.c_str(), -1, utf8String, nNum + 1, NULL, FALSE);

				ret = utf8String;
				delete[] utf8String;
			}
			else
			{
				CP_LOGE("Wrong convert to Utf8 code:0x%x", GetLastError());
			}
		}

		return ret;
	}
	static std::wstring StringUtf8ToWideChar(const std::string& strUtf8)
	{
		std::wstring ret;
		if (!strUtf8.empty())
		{
			int nNum = MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, NULL, 0);
			if (nNum)
			{
				WCHAR* wideCharString = new WCHAR[nNum + 1];
				wideCharString[0] = 0;

				nNum = MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, wideCharString, nNum + 1);

				ret = wideCharString;
				delete[] wideCharString;
			}
			else
			{
				CP_LOGE("Wrong convert to WideChar code:0x%x", GetLastError());
			}
		}
		return ret;
	}
	bool CPFileUtils::rmdir_(cchr *path)
	{
		//RemoveDirectoryA(path);
		std::wstring wpath = StringUtf8ToWideChar(path);
		std::wstring files = wpath + L"*.*";
		WIN32_FIND_DATA wfd;
		HANDLE  search = FindFirstFileEx(files.c_str(), FindExInfoStandard, &wfd, FindExSearchNameMatch, NULL, 0);
		bool ret = true;
		if (search != INVALID_HANDLE_VALUE)
		{
			BOOL find = true;
			while (find)
			{
				// Need check string . and .. for delete folders and files begin name.
				std::wstring fileName = wfd.cFileName;
				if (fileName != L"." && fileName != L"..")
				{
					std::wstring temp = wpath + wfd.cFileName;
					if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						temp += '/';
						std::string rmPath = StringWideCharToUtf8(temp);
						ret = ret && rmdir_(rmPath.c_str());
					}
					else
					{
						SetFileAttributes(temp.c_str(), FILE_ATTRIBUTE_NORMAL);
						ret = ret && DeleteFile(temp.c_str());
					}
				}
				find = FindNextFile(search, &wfd);
			}
			FindClose(search);
		}
		if (ret && RemoveDirectory(wpath.c_str()))
		{
			return true;
		}
		return false;
	}

#else
    cchr CPFileUtils::NOSP = '\\';
    cchr CPFileUtils::SP = '/';
    
    bool CPFileUtils::mkdir_(cchr *path)
    {
        return ::mkdir(path, 0777) != -1;
    }
    
    bool CPFileUtils::rmdir_(cchr *path)
    {
        return  cpfile_remove_all(path);
    }

#endif

	std::string CPFileUtils::getDataFromFile(const std::string& filename)
	{
		unsigned long size;
		unsigned char* data = cpfile_read(filename.c_str(), &size);
		if (data != NULL)
		{
			std::string ret = std::string((char*)data, size);
			free(data);
			return ret;
		}
		else
		{
			return "";
		}
	}

	std::string CPFileUtils::getStringFromFile(const std::string& filename)
	{
		return getDataFromFile(filename);
	}

#ifndef UTILS_PLATFORM_IOS
	std::string CPFileUtils::getWriteablePath()
	{
		return cpfile_write_path();
	}

	void CPFileUtils::skipBackupToApplicationDic()
	{
	}
#endif
    
	std::string CPFileUtils::fixpath(cchr* path)
	{
		std::string temp = path;
		int l = temp.length();

		for (int i = 0; i < l; ++i)
		{
			if (temp[i] == NOSP)
			{
				temp[i] = SP;
			}
		}
		return temp;
	}

	void CPFileUtils::fixpath_(cchr* path, char* temp)
	{
		strcpy(temp, path);

		int length = strlen(temp);
		for (int i = 0; i < length; ++i)
		{
			if (temp[i] == NOSP)
			{
				temp[i] = SP;
			}
		}
	}

	bool CPFileUtils::mkdir(cchr *path)
	{
		char temp[1024] = { 0 };
		fixpath_(path, temp);
		int l = strlen(temp);

		if (temp[l - 1] != SP)
			temp[l] = SP, l++;

		for (int i = 1; i < l; i++)
		{
			if (temp[i] != SP)
				continue;

			temp[i] = 0;
			if (access(temp, 0) == -1)
			if (!mkdir_(temp))
				return false;
			temp[i] = SP;
		}

		return true;
	}
	bool CPFileUtils::mdbyf(cchr *path)
	{
		char temp[1024] = { 0 };
		fixpath_(path, temp);
		int l = strlen(temp);

		for (int i = 1; i < l; i++)
		{
			if (temp[i] != SP)
				continue;

			temp[i] = 0;
			if (access(temp, 0) != 0)
			{
				if (!mkdir_(temp))
				{
					CP_LOGE("mkdir %s fail, errno %u", temp, errno);
					return false;
				}
			}
			temp[i] = SP;
		}
		return true;
	}
	bool CPFileUtils::rmdir(cchr *path)
	{
		return rmdir_(path);
	}
	bool CPFileUtils::exist(cchr *path)
	{
		return cpfile_exist(path);
	}
	bool CPFileUtils::remove(cchr *path)
	{
		return ::remove(path) == 0;
	}
	bool CPFileUtils::rename(cchr *src, cchr *dist)
	{
		std::string srcpath = src;
		std::string distpath = dist;

		if (::rename(srcpath.c_str(), distpath.c_str()) == 0){
			return true;
		}
		else{
			CP_LOGE("\ncpfile_move failed, srcpath=[%s] distpath=[%s] error=[%d]", srcpath.c_str(), distpath.c_str(), errno);
			return false;
		}
	}

	bool CPFileUtils::writedata(void* data, size_t size, const char* path)
	{
		FILE* fp = fopen(path, "wb+");
		if (fp == NULL)
			return false;

		bool ret = fwrite(data, 1, size, fp) == size;
		fclose(fp);

		return ret;
	}

	bool CPFileUtils::appenddata(void* data, size_t size, cchr *path)
	{
		FILE* fp = fopen(path, "ab+");
		if (fp == NULL)
			return false;

		bool ret = fwrite(data, 1, size, fp) == size;
		fclose(fp);

		return ret;
	}

	bool CPFileUtils::copy(cchr *srcPath, cchr *destPath)
	{
		FILE *src_fp;
		if ((src_fp = fopen(srcPath, "rb")) == NULL)
		{
			printf("fopen %s failed.\n", srcPath);
			return false;
		}

		fseek(src_fp, 0L, SEEK_END);
		int src_len = ftell(src_fp);
		char *src_buf = new char[src_len];
		fseek(src_fp, 0L, SEEK_SET);
		fread(src_buf, src_len, 1, src_fp);
		fclose(src_fp);

		FILE *dest_fp;
		if ((dest_fp = fopen(destPath, "wb")) == NULL)
		{
			printf("fopen %s failed.\n", destPath);
			return false;
		}

		fwrite(src_buf, src_len, 1, dest_fp);
		fclose(dest_fp);

		return true;
	}

	void CPFileUtils::rmdirAll(const std::string& strDirPath)
	{
		rmdir(strDirPath.c_str());
	}

	int CPFileUtils::getFileSize(cchr *file)
	{
		
		FILE *fp;
		if ((fp = fopen(file, "rb")) == NULL)
		{
			printf("fopen %s failed.\n", file);
			return 0;
		}

		fseek(fp, 0L, SEEK_END);

		int size = ftell(fp);

		fclose(fp);
		
		return size;
	}

	void CPFileUtils::purgeCachedEntries()
	{
		cpfile_purgecache();
	}

	std::istream& CPFileUtils::safeGetline(std::istream& is, std::string& t)
	{
		t.clear();

		// The characters in the stream are read one-by-one using a std::streambuf.
		// That is faster than reading them one-by-one using the std::istream.
		// Code that uses streambuf this way must be guarded by a sentry object.
		// The sentry object performs various tasks,
		// such as thread synchronization and updating the stream state.

		std::istream::sentry se(is, true);
		std::streambuf* sb = is.rdbuf();

		for (;;) {
			int c = sb->sbumpc();
			switch (c) {
			case '\n':
				return is;
			case '\r':
				if (sb->sgetc() == '\n')
					sb->sbumpc();
				return is;
			case EOF:
				// Also handle the case when the last line has no line ending
				if (t.empty())
					is.setstate(std::ios::eofbit);
				return is;
			default:
				t += (char)c;
			}
		}
	}
    
    std::string CPFileUtils::concateFilePath(const std::string &dirPath, const std::string &relFilePath)
    {
        if (dirPath.empty()) {
            return relFilePath;
        }
        
        std::string _dirPath = dirPath;
        if (_dirPath[_dirPath.size()-1] != '/') {
            _dirPath = _dirPath + "/";
        }
        
        return _dirPath + relFilePath;
    }
}
