

#ifndef ___file_system_h___
#define ___file_system_h___

typedef const char cchr;


#include <string>
using namespace std;


class FileSystem
{
public:
	/*
	 * warning :
	 *     
	 *     This function return a share temp string.
	 *
	 */
	static string fixpath(cchr* path);


	static bool mkdir(cchr *path);
	static void rmdir(cchr *path);
	static bool mdbyf(cchr *path); // make dir by file
	static bool exist(cchr *path);
	static void remove(cchr *path);
	static void rename(cchr *oldn, cchr *newn);

	static bool writedata(void* data, size_t size, cchr *path);

	static string dlcdir(void);
	static string dlcfullpath(cchr* file);

	static string pkgfullpath(cchr* file);

protected:
	static bool mkdir_(cchr *path);
	static void rmdir_(cchr *path);

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

	FileSystem (void);
	FileSystem (const FileSystem& );
	void operator= (const FileSystem & );
	
};


#endif

