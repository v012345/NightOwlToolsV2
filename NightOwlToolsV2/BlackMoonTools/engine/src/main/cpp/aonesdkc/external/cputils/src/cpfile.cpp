#include "cpfile.h"
#include "platform/CCFileUtils.h"
#include <cstring>
#include <string>

using namespace cputils;
static int expity_date = 0;
static int _retain_days = 0;

extern "C"
{
	unsigned char* cpfile_read(const char* path, unsigned long* read_size)
	{
		Data data = FileUtils::getInstance()->getDataFromFile(path);
		unsigned char* bytes = data.getBytes();
		if (read_size != NULL) *read_size = data.getSize();
		unsigned char* ret = (unsigned char*)malloc(*read_size);
		memcpy(ret, bytes, data.getSize());
		return ret;
	}

	unsigned char* cpfile_readstr(const char* path)
	{
		Data data = FileUtils::getInstance()->getDataFromFile(path);
		unsigned char* bytes = data.getBytes();
		unsigned char* ret = (unsigned char*)malloc(data.getSize()+1);
		memset(ret, 0, data.getSize() + 1);
		memcpy(ret, bytes, data.getSize());
		return ret;
	}

	bool cpfile_exist(const char* path)
	{
		return FileUtils::getInstance()->isFileExist(path);
	}

	void cpfile_purgecache()
	{
		FileUtils::getInstance()->purgeCachedEntries();
	}

	static int _remove_file_walker(const char* path, const struct cpfile_stat* s)
	{
		if (cpfile_remove(path)) return 0;
		return -1;
	}

	bool cpfile_remove_all(const char* path)
	{
		cpfile_fws(path, _remove_file_walker, CPFILE_WALK_DEPTH | CPFILE_WALK_RECURSION);
		return true;
	}
	
	bool cpfile_remove(const char* path)
	{
		return FileUtils::getInstance()->removeFile(path);
	}

    static long long getNowTime()
    {
        long long now_time;
        now_time = time(NULL);
        return now_time;
    }
    
    static bool isAudioCache(const char* path)
    {
        std::string cachePath = path;
        
        cachePath = cachePath.substr(cachePath.length()-4, 4);
        
        if (cachePath == ".wav" ) {
            return true;
        }else{
            return false;
        }
    }
    
	static int _delete_expiry_cache(const char* path, const struct cpfile_stat* s)
	{
        //if path is a file, file exist and file is audio cache , delete it
        if ( (s->file_status == CPFILE_STATUS_F) &&( cpfile_exist(path) ) && (isAudioCache(path) ) ) {
            
            long long last_write_time=s->last_write_time;
            long long now_time=getNowTime();
            
            if (now_time-last_write_time >= expity_date) {
                // over the expiry date ,delete it
                bool ret = cpfile_remove(path);
                const char* log=(ret==true)?"success":"fail";
                ZF_LOGW("\ndelele expiry cache, filename=[%s]\nresult=[%s]",path, log);
            }
        }
        return 0;
    }
    
    void cpfile_delete_expiry_cache(const char* path , int expityDate)
    {
        expity_date=expityDate;
        cpfile_fws(path, _delete_expiry_cache, CPFILE_WALK_RECURSION);
    }
   
#ifdef WIN32
#define S_ISDIR(m) (((m) & 0170000) == (0040000))
#include "platform/win32/dirent.h"
#else
#include "dirent.h"
#endif

#include "errno.h"
#include <sys/stat.h>
    
    static struct cpfile_stat* get_stat(const char* path, cpfile_stat* cpstat){
        struct stat statbuf;
#ifndef WIN32
        if (lstat(path, &statbuf) < 0) {
            CP_LOGE("get stat error, path is %s, errno is %d", path, errno);
        }
        else{
            cpstat->file_status = S_ISDIR(statbuf.st_mode) ? CPFILE_STATUS_D : CPFILE_STATUS_F;
            cpstat->last_write_time = statbuf.st_mtime;
        }
#else
		if (stat(path, &statbuf) < 0) {
			CP_LOGE("get stat error, path is %s, errno is %d", path, errno);
		}
		else{
			cpstat->file_status = S_ISDIR(statbuf.st_mode) ? CPFILE_STATUS_D : CPFILE_STATUS_F;
			cpstat->last_write_time = statbuf.st_mtime;
		}
#endif
        
        return cpstat;
    }

	int cpfile_fws(const char* path, CPFILE_WALKER_CB cb, int walk_params)
	{
		struct stat statbuf;
		struct cpfile_stat cpstat;
		struct dirent *dirp;
		DIR* dp;

#ifndef WIN32
		if (lstat(path, &statbuf) < 0) {
			CP_LOGE("get stat error, path is %s, errno is %d", path, errno);
			return -1;
		}
#else
		if (stat(path, &statbuf) < 0) {
			CP_LOGE("get stat error, path is %s, errno is %d", path, errno);
			return -1;
		}
#endif
		if (S_ISDIR(statbuf.st_mode) == 0){
			cb(path, get_stat(path, &cpstat));
			return 0;
		}

		if ((walk_params & CPFILE_WALK_DEPTH) == 0) {
			cb(path, get_stat(path, &cpstat));
		}

		if ((dp = opendir(path)) == NULL) {
			CP_LOGE("opendir fail , path is %s, errno is %d", path, errno);
			return -1;
		}
        
        while ((dirp = readdir(dp)) != NULL) {
			if (dirp->d_name == NULL ||
				strcmp(dirp->d_name, ".") == 0 ||
                strcmp(dirp->d_name, "..") == 0) {
                continue;
            }
            
            std::string np = path;
            
            if ( np[np.length()-1] == '/' ) {
                //删除最后一个字符"/"
                np = np.substr(0,np.length()-1);
            }
            
            np = np + "/" + dirp->d_name;
            
            if ((walk_params & CPFILE_WALK_RECURSION) != 0) {
                int ret = cpfile_fws(np.c_str(), cb, walk_params);
                if (ret != 0) {
                    return ret;
                }
            }
            else{
                cb(np.c_str(), get_stat(np.c_str(), &cpstat));
            }
        }
        
        if ((walk_params & CPFILE_WALK_DEPTH) != 0) {
            cb(path, get_stat(path, &cpstat));
        }
        
        closedir(dp);
        return 0;
	}
    
}

void cpfile_add_search_path(const char* path)
{
	FileUtils::getInstance()->addSearchPath(path);
}


