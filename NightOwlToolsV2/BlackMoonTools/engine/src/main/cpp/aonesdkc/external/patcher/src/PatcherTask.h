#ifndef _PATCHER_TASK_H_
#define _PATCHER_TASK_H_

#include <memory>
#include <string>
#include <functional>
#include <list>
#include <map>
#include <condition_variable>
#include <thread>
#include "def.h"
#include "PatchList.h"
#include <vector>

#include "http_download.h"

namespace AonePatcher{

	enum class PatchErrorCode : uint32;
	class IPatcherTask;
	typedef std::shared_ptr<IPatcherTask> IPatcherTaskPtr;

	class IPatcherTask : public std::enable_shared_from_this<IPatcherTask>{
	public:
		virtual ~IPatcherTask() = 0;
        
        /**
         Download function.
         */
		virtual void Do() = 0;
        
        /**
         Cancel the download.
         */
		virtual void Cancel() = 0;

        /** The url from download. */
		std::string m_url;
		
        /**
         The path write to the local disk.
         Default is `DownloadingPath`
         */
		std::string m_path;
        
        /** The file's name. */
		std::string m_name;
        
        /** The file's md5 (file not be compress and encrypt). */
		std::string m_md5;
        
        /** The downloaded file's md5 (file was compressed and encrypted). */
        std::string m_verify_md5;
        
        /** The file total size. */
		uint32 m_size;
        
        /** Current download size. */
		uint32 m_curTotalSize;
        
        /** Determine whether support break point, default is false. */
		bool m_breakPointEnable;
        
        /** Determine whether support check speed, default is false. */
		bool m_isCheckSpeed;
        
        /**
         Determine Indicate whether check downloaded file's md5, default is true.
         
         @discussion If download `list.csv`, don't need check md5.
         */
        bool m_isCheckMD5;

		PatchErrorCode m_error;
		uint32 m_innerError;
		std::string m_errorMsg;
		
		std::function<void(IPatcherTaskPtr)> m_endCB;
		std::function<void(IPatcherTaskPtr, uint32 cur, uint32 total)> m_progressCB;
		std::function<void(IPatcherTaskPtr, uint32 curTotalSize)> m_downFailedCB;
	};

	class IPrepareListTask{
	public:
		virtual ~IPrepareListTask() = 0;
        
        
        /**
         下载list文件
         
         @param list_file_path 文件名
         @param loc_downloading_path 下载文件路径
         @param remote_res_url 下载URL
         @return 下载错误码
         */
		virtual PatchErrorCode Try(std::string list_file_path,
                                     std::string loc_downloading_path,
                                     std::string remote_res_url) = 0;
        /**
         下载list文件，并比较计算需要更新的文件信息

         1. 下载list-v2.csv文件
            - 下载失败
                - 下载list.csv文件
                    - 下载失败 返回
         2. 比较需要更新的文件信息.
         @param package_res_path 包内资源路径
         @param loc_res_path 本地资源路径
         @param loc_downloading_path 下载文件路径
         @param remote_res_url_list 下载URL
         @return 需要更新的文件信息
         */
		virtual IPatchListPtr Do(std::string package_res_path,
                                 std::string loc_res_path,
                                 std::string loc_downloading_path,
								 vector<std::string> remote_res_url_list) = 0;
        
	};
	typedef std::shared_ptr<IPrepareListTask> IPrepareListTaskPtr;

    /**
     下载list文件
     */
	class PrepareListTask : public IPrepareListTask{
	public:
		virtual IPatchListPtr Do(std::string package_res_path,
                                 std::string loc_res_path,
                                 std::string loc_downloading_path,
								 vector<std::string> remote_res_url_list) override;
        
		virtual PatchErrorCode Try(std::string list_file_path,
									std::string loc_downloading_path,
									std::string remote_res_url) override;

		IPatchListPtr Get(std::string package_res_path, 
							std::string loc_res_path,
							std::string loc_downloading_path,
							std::string cur_list_file_name);
	};

    /**
     This class is for download remote resource.
     */
	class PatcherTask : public IPatcherTask{
	public:
		PatcherTask();

		virtual void Do() override;
		virtual void Cancel() override;

	protected:
		int PatcherDownfile(const string& url, const string& loc);
        
	private:
		HttpDownload m_download;
	};

	class IPatcherTaskFactory{
	public:
        
        /**
         Convenient to create a task to download resource file.

         Support break point and should check speed.
         */
		virtual IPatcherTaskPtr CreateTask() = 0;
        
        /**
         Convenient to create a task to download `list.csv`.

         Not support break point, but should check speed.
         */
		virtual IPrepareListTaskPtr CreatePrepareListTask() = 0;
	};

	class PatcherTaskFactory : public IPatcherTaskFactory{
		virtual IPatcherTaskPtr CreateTask() override;
		virtual IPrepareListTaskPtr CreatePrepareListTask() override;
	};
	typedef std::shared_ptr<IPatcherTaskFactory> IPatcherTaskFactoryPtr;
}

#endif
