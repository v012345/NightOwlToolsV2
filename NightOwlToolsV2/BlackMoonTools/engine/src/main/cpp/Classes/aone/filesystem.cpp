

#include "filesystem.h"

#include <cocos2d.h>
USING_NS_CC;

#include <stdio.h>
#include <string.h>


#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)

#	include <Windows.h>
#	include <fcntl.h>
#	include <io.h>

cchr FileSystem::NOSP = '/';
cchr FileSystem::SP = '\\';

bool FileSystem::mkdir_(cchr *path)
{
	return CreateDirectoryA(path, NULL) == TRUE;
}
void FileSystem::rmdir_(cchr *path)
{
	RemoveDirectoryA(path);
}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)

#	include <Windows.h>
#	include <fcntl.h>
#	include <io.h>

cchr FileSystem::NOSP = '\\';
cchr FileSystem::SP = '/';

bool FileSystem::mkdir_(cchr *path)
{
	return CreateDirectoryA(path, NULL) == TRUE;
}
void FileSystem::rmdir_(cchr *path)
{
	RemoveDirectoryA(path);
}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include <sys/stat.h>
#include <unistd.h>

cchr FileSystem::NOSP = '\\';
cchr FileSystem::SP = '/';

bool FileSystem::mkdir_(cchr *path)
{
	return ::mkdir(path, 0777) != -1;
}
void FileSystem::rmdir_(cchr *path)
{
	::rmdir(path);
}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include <sys/stat.h>
#include <unistd.h>

cchr FileSystem::NOSP = '\\';
cchr FileSystem::SP = '/';

bool FileSystem::mkdir_(cchr *path)
{
	return ::mkdir(path, 0777) != -1;
}
void FileSystem::rmdir_(cchr *path)
{
	rmdir(path);
}

#endif


string FileSystem::fixpath(cchr* path)
{
	string temp = path;
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

void FileSystem::fixpath_(cchr* path, char* temp)
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

bool FileSystem::mkdir(cchr *path)
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
		if (access(temp, 0) != 0)
		if (!mkdir_(temp))
			return false;
		temp[i] = SP;
	}

	return true;
}
bool FileSystem::mdbyf(cchr *path)
{
	char temp[1024] = { 0 };
	fixpath_(path, temp);
	int l = strlen(temp);

	for (int i = 1; i < l; i++)
	{
		if (path[i] != SP)
			continue;

		temp[i] = 0;
		if (access(temp, 0) != 0)
		if (!mkdir_(temp))
			return false;
		temp[i] = SP;
	}
	return true;
}
void FileSystem::rmdir(cchr *path)
{
	rmdir_(path);
}
bool FileSystem::exist(cchr *path)
{
	return CCFileUtils::getInstance()->isFileExist(path);
}
void FileSystem::remove(cchr *path)
{
	::remove(path);
}
void FileSystem::rename(cchr *oldn, cchr *newn)
{
	::rename(oldn, newn);
}

bool FileSystem::writedata(void* data, size_t size, const char* path)
{
	FILE* fp = fopen(path, "wb+");
	if (fp == nullptr)
		return false;

	bool ret = fwrite(data, 1, size, fp) != size;
	fclose(fp);

	return ret;
}

std::string FileSystem::dlcdir(void)
{
	static string dlc_dir = fixpath((CCFileUtils::getInstance()->getWritablePath() + "cache/dlc/").c_str());
	return dlc_dir;
}

std::string FileSystem::dlcfullpath(cchr* file)
{
	string tmp = dlcdir() + file;
	tmp = fixpath(tmp.c_str());
	return tmp;
}

string FileSystem::pkgfullpath(cchr* file)
{
	static string dir = CCFileUtils::getInstance()->fullPathForFilename("list.csv");
	int pos = dir.rfind("list.csv");
	if (pos > 0 && pos<300)
	{
		dir = dir.substr(0, pos);
	}

	string tmp = dir + file;
	tmp = fixpath(tmp.c_str());
	return tmp;
}
