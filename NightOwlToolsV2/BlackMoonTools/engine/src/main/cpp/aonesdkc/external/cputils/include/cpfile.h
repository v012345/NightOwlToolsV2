#ifndef _CPFILE_
#define _CPFILE_

#ifdef __cplusplus
extern "C" {
#endif

    #define	CPFILE_STATUS_F		0	/* File.  */
    #define	CPFILE_STATUS_D		1	/* Directory.  */
    
    #define CPFILE_WALK_RECURSION   0x02 /* Walk Depth prior*/
    #define CPFILE_WALK_DEPTH       0x04 /* Walk Depth prior*/

	const char NOSP = '/';
	const char SP = '\\';
    
	struct cpfile_stat
	{
        int file_status;
		long last_write_time;
	};

	typedef int (*CPFILE_WALKER_CB)(const char* path, const struct cpfile_stat* s);
    
	const char* cpfile_write_path();
	const char* cpfile_external_path();
	unsigned char* cpfile_read(const char* path, unsigned long* read_size);
	unsigned char* cpfile_readstr(const char* path);
	bool cpfile_exist(const char* path);
	bool cpfile_remove_all(const char* path);
	bool cpfile_remove(const char* path);
	int cpfile_fws(const char* path, CPFILE_WALKER_CB cb, int walk_params);
	void cpfile_purgecache();
    void cpfile_delete_expiry_cache(const char* path , int expityDate);
	void cpfile_add_search_path(const char* path);
#ifdef __cplusplus
};
#endif

#endif
