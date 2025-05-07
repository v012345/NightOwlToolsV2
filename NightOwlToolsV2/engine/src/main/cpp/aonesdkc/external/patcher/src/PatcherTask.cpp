#include "PatcherTask.h"
#include "decompress.h"

#include "CPFileUtils.h"
#include "cplog.h"
#include "PatcherException.h"
#include "DataConfig.h"
#include "md5.h"


namespace AonePatcher
{
	IPatcherTaskPtr PatcherTaskFactory::CreateTask()
	{
		return std::shared_ptr<PatcherTask>(new PatcherTask());
	}

	IPrepareListTaskPtr PatcherTaskFactory::CreatePrepareListTask()
	{
		return std::shared_ptr<PrepareListTask>(new PrepareListTask());
	}
    
	int PatcherTask::PatcherDownfile(const string& url, const string& loc)
	{
		m_curTotalSize = 0;

		m_download.address(url);
		m_download.locpath(loc);
		m_download.notify([this, url](double current, double total){
			m_progressCB(shared_from_this(), (uint32)current - m_curTotalSize, (uint32)total);
			m_curTotalSize = (uint32)current;
		});

		return m_download.download();
	}

	PatcherTask::PatcherTask()
	{
		m_breakPointEnable = false;
		m_isCheckSpeed = false;
        m_isCheckMD5 = true;
	}

	void PatcherTask::Do()
	{
        std::string resUrl = CPUtils::CPFileUtils::concateFilePath(m_url, m_name);
        std::string diskDirPath = CPUtils::CPFileUtils::concateFilePath(m_path, m_name);
		if (CPUtils::CPFileUtils::mdbyf(diskDirPath.c_str()) == false)
		{
			m_innerError = errno;
			m_error = PatchErrorCode::Storage;
			m_errorMsg = "mkdir failed!";
			m_endCB(shared_from_this());
			return;
		}

		int times = 0;
		int ret = -1;
		m_download.setBreakPointEnabled(m_breakPointEnable);
		m_download.setIsCheckSpeed(m_isCheckSpeed);
		while (ret != 0 && ++times < DataConfig::getInstance()->getFailRetryNum())
		{
            // synchronized download one file.
			ret = PatcherDownfile(resUrl, diskDirPath);
			
            // download success or cancel download, will break.
            if (m_download.getInterrupt() || ret == 0) {
                break;
            }

            if (ret != 0) {
                CP_LOGW("download failed once, inner error is [%d],time is [%d], resUrl is %s, diskDirPath is %s\n", ret, times, resUrl.c_str(), diskDirPath.c_str());
				m_downFailedCB(shared_from_this(), m_curTotalSize);
			}
		}

		if (ret == 0)
		{
			string srcPath = diskDirPath;
			const char* path = diskDirPath.c_str();

			if (CPUtils::CPFileUtils::getFileSize(path) == 0)
			{
				CPUtils::CPFileUtils::remove(srcPath.c_str());
                
				m_innerError = 0;
				m_error = PatchErrorCode::NoError;
				m_errorMsg = "Download and Decompress Success!";
			}
			else
			{
				string dstPath = Decompress::getUncompressedFileName(srcPath);
				CPUtils::CPFileUtils::remove(dstPath.c_str());
                
				if (CPUtils::CPFileUtils::mdbyf(dstPath.c_str()))
				{
                    // begin check md5
                    bool _isMD5Mismatch = false;
                    if (m_isCheckMD5)
                    {
                        // list 文件 不需要检查MD5文件
                        // 比较压缩之后的文件MD5信息
                        string _computed_md5 = MD5().MD5File(srcPath.c_str());
                        string _record_md5 = this -> m_verify_md5;
                        if (_computed_md5 != _record_md5) {
                            printf("\nPatcher --- file's real md5 = %s\n", _computed_md5.c_str());
                            _isMD5Mismatch = true;
                        }
                    }
                    
                    // begin decompress
                    int zipRet = decompress_gzip(srcPath.c_str(), dstPath.c_str());
                    if (zipRet == 0)
                    {
                        CPUtils::CPFileUtils::remove(srcPath.c_str());
                        
                        m_innerError = 0;
                        if (_isMD5Mismatch) {
                            m_error = PatchErrorCode::MD5;
                            m_errorMsg = "md5 mismatch";
                            printf("\nPatcher --- PatherTask end, MD5 Mismatch, path = %s\n, verify_md5 = %s\n", dstPath.c_str(), this->m_verify_md5.c_str());
                        } else {
                            m_error = PatchErrorCode::NoError;
                            m_errorMsg = "Download and Decompress Success!";
                        }
                    }
                    else
                    {
                        m_innerError = zipRet;
                        m_error = PatchErrorCode::ZLIB;
                        m_errorMsg = "Decompress failed!";
                        m_downFailedCB(shared_from_this(), m_curTotalSize);
                    }
                    
				}
				else
				{
					CP_LOGE("PatcherTask: create directory fail!");
					m_error = PatchErrorCode::ZLIB;
					m_errorMsg = "Decompress failed!";
					m_downFailedCB(shared_from_this(), m_curTotalSize);
				}
			}
		}
		else
		{
			m_innerError = ret;
			m_error = PatchErrorCode::Network;
			m_errorMsg = "Download failed!";
			if (m_download.getInterrupt())
			{
				m_error = PatchErrorCode::Cancel;
				m_errorMsg = "Download Cancle!";
			}
			m_downFailedCB(shared_from_this(), m_curTotalSize);
		}
			
		m_endCB(shared_from_this());
	}

	void PatcherTask::Cancel()
	{
		m_download.interrupt();
	}

	IPatcherTask::~IPatcherTask()
	{

	}

	IPrepareListTask::~IPrepareListTask()
	{

	}

	PatchErrorCode PrepareListTask::Try(std::string list_file_name, std::string loc_downloading_path, std::string remote_res_url)
    {
        string list_file_name_gz = list_file_name + ".gz";
        
        std::string downloadPath = CPUtils::CPFileUtils::concateFilePath(loc_downloading_path, list_file_name);
		PatchErrorCode error_code = PatchErrorCode::NoError;
		if (!CPUtils::CPFileUtils::exist(downloadPath.c_str()))
        {
            std::shared_ptr<PatcherTask> taskPtr = std::shared_ptr<PatcherTask>(new PatcherTask());
            taskPtr->m_name = list_file_name_gz;
            taskPtr->m_url = remote_res_url;
            taskPtr->m_path = loc_downloading_path;
            taskPtr->m_breakPointEnable = false;
            taskPtr->m_isCheckSpeed = true;
            taskPtr->m_isCheckMD5 = false;
            taskPtr->m_progressCB = [](IPatcherTaskPtr, uint32 cur, uint32 total){
            };
            taskPtr->m_downFailedCB = [](IPatcherTaskPtr ptr, uint32 curTotal) {
            };
            taskPtr->m_endCB = [&](IPatcherTaskPtr ptr) {
                if (static_cast<uint32>(ptr->m_error) != 0) {
                    printf("\nPatcher --- PrepareListTask %s: Try Download failed! error %u, innerError %d, errorMsg %s",
						list_file_name.c_str(), (uint32)ptr->m_error, ptr->m_innerError, ptr->m_errorMsg.c_str());
					error_code = ptr->m_error;
                }
            };
            printf("\nPatcher--- begin down list %s, url = %s, path = %s\n", list_file_name.c_str(), taskPtr->m_url.c_str(), taskPtr->m_path.c_str());
            taskPtr->Do();
		}
        
		return error_code;
    }

	AonePatcher::IPatchListPtr PrepareListTask::Get(std::string package_res_path, std::string loc_res_path, std::string loc_downloading_path, std::string cur_list_file_name)
	{
		std::string list_file_name = "list.csv";
		std::string list_v2_file_name = "list-v2.csv";
		// 获取本地的list文件顺序
		// 1. 本地资源文件夹下的list-v2.csv
		// 2. 本地资源文件夹下的list.csv
		// 3. 包内资源文件夹下的list-v2.csv
		// 4. 包资源文件夹下的list.csv
		std::string locResV2Path = CPUtils::CPFileUtils::concateFilePath(loc_res_path, list_v2_file_name);
		std::string oldVerPath = locResV2Path;
		if (!CPUtils::CPFileUtils::exist(locResV2Path.c_str())) {
			oldVerPath = CPUtils::CPFileUtils::concateFilePath(loc_res_path, list_file_name);
			if (!CPUtils::CPFileUtils::exist(oldVerPath.c_str())) {
				oldVerPath = CPUtils::CPFileUtils::concateFilePath(package_res_path, list_v2_file_name);
				if (!CPUtils::CPFileUtils::exist(oldVerPath.c_str())) {
					oldVerPath = CPUtils::CPFileUtils::concateFilePath(package_res_path, list_file_name);
				}
			}
		}

		// 本地的list文件
		PatchList oldList;
		bool isLoadOld = oldList.Load(oldVerPath);
		if (!isLoadOld)
		{
			CP_LOGE("PrepareListTask: Load old list file failed,path is %s", oldVerPath.c_str());
			return NULL;
		}

		std::string downloadPath = CPUtils::CPFileUtils::concateFilePath(loc_downloading_path, cur_list_file_name);
		// 下载的list文件
		PatchList newList;
		bool isLoadNew = newList.Load(downloadPath);
		if (!isLoadNew)
		{
			CP_LOGE("PrepareListTask: Load new list file failed, path is %s", downloadPath.c_str());
			return NULL;
		}


		IPatchListPtr updateList = newList.DiffList(oldList);
		updateList->SetListType(cur_list_file_name.find("-v2") != cur_list_file_name.npos);

		return updateList;
	}

	AonePatcher::IPatchListPtr PrepareListTask::Do(std::string package_res_path, std::string loc_res_path, std::string loc_downloading_path, vector<std::string> remote_res_url_list)
	{
		string list_file_name = "list.csv";
		string list_v2_file_name = "list-v2.csv";
		bool is_v2_list = false;
		// 下载list-v2.csv
		for (int i = 0; i < remote_res_url_list.size(); i++)
		{
			PatchErrorCode err = this->Try(list_v2_file_name, loc_downloading_path, remote_res_url_list[i]);
			if (err == PatchErrorCode::NoError)
			{
				is_v2_list = true;
				break;
			}
		}
		// list-v2.csv下载成功
		if (is_v2_list)
        {
			// 获取更新文件list
			IPatchListPtr updateList = this->Get(package_res_path, loc_res_path, loc_downloading_path, list_v2_file_name);
			if (updateList == NULL)
			{
				throw(PatcherException(PatchErrorCode::Common, errno, "get different file info list failed"));
			}
			return updateList;
        }
        
		string list_v2_file = CPUtils::CPFileUtils::concateFilePath(loc_downloading_path, list_v2_file_name);
		string list_v2_gz_file = CPUtils::CPFileUtils::concateFilePath(loc_downloading_path, list_v2_file_name + ".gz");
        if (CPUtils::CPFileUtils::exist(list_v2_file.c_str())) {
            CPUtils::CPFileUtils::remove(list_v2_file.c_str());
        }
        if (CPUtils::CPFileUtils::exist(list_v2_gz_file.c_str())) {
            CPUtils::CPFileUtils::remove(list_v2_gz_file.c_str());
        }
        
		// 下载list.csv
		PatchErrorCode err = PatchErrorCode::NoError;
		for (int i = 0; i < remote_res_url_list.size(); i++)
		{
			err = this->Try(list_file_name, loc_downloading_path, remote_res_url_list[i]);
			if (err == PatchErrorCode::NoError)
			{
				break;
			}
		}
		if (err != PatchErrorCode::NoError)
		{
			throw(PatcherException(err, errno, "download list file failed"));
		}
		printf("\nPatcher --- download finish, begin compare....\n");
		// 获取更新文件list
		IPatchListPtr updateList = this->Get(package_res_path, loc_res_path, loc_downloading_path, list_file_name);
		if (updateList == NULL)
		{
			throw(PatcherException(PatchErrorCode::Common, errno, "get different file info list failed"));
		}
		return updateList;
	}

}
