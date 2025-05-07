#ifndef  _AONE_PATCHER_H_
#define _AONE_PATCHER_H_

#define PATCHER_VERSION 1.1.0

typedef unsigned int uint32;

#define PATCHER_RESULT_SUCCESS 0
#define PATCHER_RESULT_COMMON_ERROR 1
#define PATCHER_RESULT_NETWORK_ERROR 2
#define PATCHER_RESULT_MD5_ERROR 3
#define PATCHER_RESULT_ZLIB_ERROR 4
#define PATCHER_RESULT_STORAGE_ERROR 5
#define PATCHER_RESULT_CANCEL 6
#define PATCHER_RESULT_INVALID_ARG 7

extern "C"
{

void patcher_check(struct patcher* p);

struct patcher* patcher_get_default();
struct patcher* patcher_create();
void patcher_free(struct patcher* p);

void patcher_set_url(struct patcher* p, const char* list_url);
void patcher_start(struct patcher* p);
void patcher_cancel(struct patcher* p);
const char* patcher_path(struct patcher* p);

void patcher_get_list(struct patcher*p);

typedef void(*patcher_progress_cb)(struct patcher* p, void* ud, uint32 write_bytes, uint32 expect_bytes);
typedef void(*patcher_finish_cb)(struct patcher* p, void* ud);
typedef void(*patcher_error_cb)(struct patcher* p, void* ud, uint32 code);
void patcher_callback(struct patcher* p, void* ud, patcher_progress_cb progress_cb, patcher_finish_cb finish_cb, patcher_error_cb error_cb);
void patcher_notify(struct patcher* p);

uint32 patcher_get_result(struct patcher* p, uint32* inner_code, const char** msg);
uint32 patcher_copy_result(struct patcher* p, uint32* inner_code, char* msg);
void patcher_set_path(struct patcher* p, const char* path);
void patcher_set_package_res_path(struct patcher* p, const char* package_res_path);

void patcher_remove_useless_files(struct patcher* p);

}
#endif // ! _AONE_PATCHER_H_
