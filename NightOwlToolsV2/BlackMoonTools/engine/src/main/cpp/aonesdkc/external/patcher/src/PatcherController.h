
#ifndef __PATCHER_CONTROLLER_H__
#define __PATCHER_CONTROLLER_H__

#include <string>
#include <list>
#include <map>
#include <set>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <thread>
#include <memory>
#include "def.h"
#include "PatcherTaskList.h"
#include "PatchList.h"
#include <vector>
#include <fstream>
#include <vector>

int cbOfGetFileFromDir(const char* path, const struct cpfile_stat* s);

namespace AonePatcher
{
    using namespace std;
    
	//foward declare begin
	enum class PatchErrorCode : uint32;
	class IPatcherTaskFactory;
	typedef std::shared_ptr<IPatcherTaskFactory> IPatcherTaskFactoryPtr;
	class IDownloadedMgr;
	typedef std::shared_ptr<IDownloadedMgr> IDownloadedMgrPtr;
	//forward declare end

	typedef std::function<void(uint32, uint32)> PatcherProgressCB;
	typedef std::function<void(PatchErrorCode)> PatcherErrorCB;
	typedef std::function<void()> PatcherFinishCB;

	class PatcherController{
	public:
		PatcherController();
		~PatcherController();

		void GetList();

		void Start();
		void Cancel();
		void Notify();
		void Fresh();
		PatchErrorCode GetResult(uint32& innerError, std::string& errMsg);

		
		void SetURL(const std::string listURL);
        std::string GetPath();
		void SetPath(const std::string resPath);
		void SetPackageResPath(const std::string packageResPath);

        // 回调函数
		void SetProgressCB(PatcherProgressCB progressCB);
		void SetFinishCB(PatcherFinishCB finishCB);
		void SetErrorCB(PatcherErrorCB errorCB);

		void SetTaskFactory(IPatcherTaskFactoryPtr factory);
		void SetDownloadedMgrPtr(IDownloadedMgrPtr downloadedMgrPtr);
		std::string GetDownloadingDir();
		
        /**
         Return a dir path which contain the `version` file and pather-address files path in the last time.

         @return A dir path.
         */
		std::string GetPatcherOldDir();
        
		void CheckPatchValid();
		void RemoveUselessFiles();
		
	private:
		void Init();
        
		void MainLoop();
        
        /**
         热更一个文件， 使用指定URL来下载。

         @param fileInfo 文件信息。
         @param url URL地址。
         */
        void PatchOneFile(const FileInfo& fileInfo, std::string url);
		
        /**
         单独一个文件下载完成。

         @param task 下载任务task对象。
         */
        void OnTaskEnd(IPatcherTaskPtr task);
		
        /**
         文件下载进程

         @param task 下载任务。
         @param cur 当前下载字节数。
         @param total 总共需要下载字节数。
         */
        void OnTaskProgressChange(IPatcherTaskPtr task, uint32 cur, uint32 total);
        
        /**
         文件下载失败。

         @param task 下载任务。
         @param curTotal 总共下载字节数。
         
         @discussion 如果下载失败是因为MD5不匹配，则需要切换URL，重新下载。
         */
		void OnTaskDownFailed(IPatcherTaskPtr task, uint32 curTotal);
		
        void SetError(PatchErrorCode errorCode, uint32 innerError, std::string errorMsg);
		
        
        /**
         下载list文件

         @return 是否下载成功
         */
        bool PrepareUpdateList();
		bool AfterDownloading();

		void CopyPVToPOldVDir();
        
        /**
         保存URL到本地
         */
		void SaveUrlToFile();

        
        /**
         检查URL。
         1. 检查本地是否有上次记录的下载的URL
         2. 比较上次URL和本次是否一致，如果不一致，删除上次下载的文件
         3. 存储本次下载URL
         */
		void CheckUrl();
        
        /**
         删除正在下载的文件
         */
		void RemoveDownloading();
		void RemoveDownloadedFile();
		bool WriteDataToCsv(std::string path, std::list<FileInfo> info);
		void getFileNameFromDir(const char* path);
		
        
        /**
         分割字符串

         @param str 带分割字的字符串
         @param string&seperator 分割符
         @return 分割之后数组
         */
        vector<string> splitString(const string &str, const string&seperator);
        
        /**
         下载地址列表
         */
        vector<string> m_url_list;

        
        /** 下载地址*/
		std::string m_url;
        
        /**
         热更资源目录，游戏优先读取热更资源目录内的文件
         
         iOS 默认: Library/Application Support
         */
		std::string m_path;
        
        /**
         包内资源路径
         一般是在aonesdk.json文件同级目录下
         */
		std::string m_packageResPath;

		PatcherProgressCB m_progressCB;
		PatcherFinishCB m_finishCB;
		PatcherErrorCB m_errorCB;

		IPatchListPtr m_patchListPtr;
        
        /** 更新文件列表 */
		std::list<std::string> m_updateFileList;
        
        /** 文件下载URL*/
        std::map<std::string, int> m_filesURLInfo;
        
		std::atomic<bool> m_finish;
		std::atomic<PatchErrorCode> m_error;
		std::atomic<uint32> m_innerError;
		std::string m_errorMsg;

		std::atomic<uint32> m_writtenBytes;
		std::atomic<uint32> m_expectBytes;

		std::atomic<bool> m_hasProgressNotify;
		std::atomic<bool> m_hasFinishNotify;
		std::atomic<bool> m_hasErrorNotify;

		std::thread m_thread;
		std::mutex m_mutex;
        std::mutex m_files_mutex;

		PatcherTaskList m_taskList;
		IPatcherTaskFactoryPtr m_taskFactory;
		IDownloadedMgrPtr m_downMgr;

	private:
		void CheckDownload();

		std::atomic<uint32> m_totalFrameNum;
		std::atomic<uint32> m_continuefailSec;
		std::atomic<uint32> m_beginBytes;

		std::atomic<bool> m_isCheckDownload;
        
	};
};

#endif
