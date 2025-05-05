
#include "cocos2d.h"
#include "version.h"

Version::Version(void)
: major_(0)
, minor_(0)
, revise_(0)
{

}

Version::Version(string& file)
: major_(0)
, minor_(0)
, revise_(0)
{
	load(file);
}

Version::~Version(void)
{

}

bool Version::operator>(const Version& ver)
{
	if (major_ != ver.major_)
		return major_ > ver.major_;

	if (minor_ != ver.minor_)
		return minor_ > ver.minor_;

	return revise_ > ver.revise_;
}

bool Version::operator<(const Version& ver)
{
	if (major_ != ver.major_)
		return major_ < ver.major_;

	if (minor_ != ver.minor_)
		return minor_ < ver.minor_;

	return revise_ < ver.revise_;
}

bool Version::operator>=(const Version& ver)
{
	if (major_ != ver.major_)
		return major_ > ver.major_;

	if (minor_ != ver.minor_)
		return minor_ > ver.minor_;

	return revise_ >= ver.revise_;
}

bool Version::operator<=(const Version& ver)
{
	if (major_ != ver.major_)
		return major_ < ver.major_;

	if (minor_ != ver.minor_)
		return minor_ < ver.minor_;

	return revise_ <= ver.revise_;
}


bool Version::operator==(const Version& ver)
{
	if (major_ != ver.major_)
		return false;

	if (minor_ != ver.minor_)
		return false;

	return revise_ == ver.revise_;
}

void Version::load(string& file)
{
	if (false == cocos2d::FileUtils::getInstance()->isFileExist(file))
		return;

	string content = cocos2d::FileUtils::getInstance()->getStringFromFile(file);
	sscanf(content.c_str(), "%d.%d.%d", &major_, &minor_, &revise_);
}

int Version::major(void)
{
	return major_;
}

int Version::minor(void)
{
	return minor_;
}

int Version::revise(void)
{
	return revise_;
}

int Version::mainv(void)
{
	return major_;
}

int Version::subsv(void)
{
	return minor_ * 100 + revise_;
}

const char* Version::tostr(void)
{
	static char cache[10];
	sprintf(cache, "%d.%d.%d", major_, minor_, revise_);
	return cache;
}
