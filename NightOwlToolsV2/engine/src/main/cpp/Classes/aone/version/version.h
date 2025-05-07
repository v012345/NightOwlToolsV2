

#ifndef ___version_h___
#define ___version_h___

#include <string>
using namespace std;


class Version
{
public:
	Version (void);
	Version (string& file);
	virtual ~Version (void);

	virtual void load(string& file);

	virtual bool operator>(const Version& ver);
	virtual bool operator<(const Version& ver);
	virtual bool operator>=(const Version& ver);
	virtual bool operator<=(const Version& ver);
	virtual bool operator==(const Version& ver);

	virtual int major(void);
	virtual int minor(void);
	virtual int revise(void);

	virtual int mainv(void);
	virtual int subsv(void);

	virtual const char* tostr(void);

protected:

	int major_;
	int minor_;
	int revise_;
};

#endif
