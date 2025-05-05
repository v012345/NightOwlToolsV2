#include "PatcherController.h"
#include "PatcherTask.h"
#include "cplog.h"
#include "CPFileUtils.h"
#include "DownloadedMgr.h"
#include "PatcherException.h"
#include "VersionFile.h"
#include "decompress.h"
#include <curl/curl.h>
#include "DataConfig.h"
#include "CurlManagerPool.h"
#include "cpfile.h"


using namespace CPUtils;

AonePatcher::PatchList patchListFiles;
std::string patcherPath;
int cbOfGetFileFromDir(const char* path, const struct cpfile_stat* s)
{
	if (s->file_status == CPFILE_STATUS_F && cpfile_exist(path))
	{
		int pos = patcherPath.length() + 1;
		std::string tmpPath(path);
		std::string tmpStr = tmpPath.substr(pos);
		if (!patchListFiles.HasFile(tmpStr + ".gz") &&
		tmpStr != "list-v2.csv" &&
        tmpStr != "list.csv" &&
		tmpStr != "PatcherOldParams/version" &&
		tmpStr != "PatcherOldParams/oldUrl"
		)
		{
//            printf("Patcher  --- cbOfGetFileFromDir: remove file %s\n", path);
            CP_LOGV("Patcher  --- cbOfGetFileFromDir: remove file %s\n", path);
			CPFileUtils::remove(path);
		}
	}

	return 0;
}

namespace AonePatcher
{
	const static std::string DownloadingDir = "PatchDownloading";
	const static std::string PatcherOldDir = "PatcherOldParams";

	PatcherController::PatcherController()
	{
        cplog_init();
        
		int maxSize = DataConfig::getInstance()->getThreadNum();
		m_taskList.SetMaxSize(maxSize);
		
        SetPath(CPFileUtils::getWriteablePath());
		
        CPUtils::CPFileUtils::skipBackupToApplicationDic();
		CPUtils::CPFileUtils::purgeCachedEntries();
		
        m_taskFactory = IPatcherTaskFactoryPtr(new PatcherTaskFactory());
		m_downMgr = IDownloadedMgrPtr(new DownloadedMgr());
		
        Init();
	}

	PatcherController::~PatcherController()
	{
		if (m_thread.joinable())
		{
			m_thread.join();
		}
	}

	void PatcherController::GetList()
	{
		Init();
		m_thread = std::thread(std::bind(&PatcherController::PrepareUpdateList, this));
	}

	void PatcherController::Start()
	{
		Init();
		m_thread = std::thread(std::bind(&PatcherController::MainLoop, this));
	}

	void PatcherController::Cancel()
	{
		m_taskList.Cancel();
	}

	void PatcherController::CheckDownload()
	{
		if (m_totalFrameNum.load() == 0)
			m_beginBytes.store(m_writtenBytes.load());

		m_totalFrameNum.store(m_totalFrameNum.load() + 1);

		if (m_totalFrameNum.load() >= DataConfig::getInstance()->getFrameNumOneSec())
		{
			if (m_beginBytes.load() != 0 &&
                m_beginBytes.load() != 0 &&
                m_beginBytes.load() !=  m_beginBytes.load() &&
                m_writtenBytes.load() - m_beginBytes.load() <= DataConfig::getInstance()->getDLSpeedLimit())
			{
                CP_LOGW("download speed below threshold one second. writen = %d, begin = %d", m_writtenBytes.load(), m_beginBytes.load());
                m_continuefailSec.store(m_continuefailSec.load() + 1);
                if (m_continuefailSec.load() >= DataConfig::getInstance()->getDLSpeedTime())
                {
                    m_continuefailSec.store(0);
                    SetError(PatchErrorCode::Network, 0, "Network connection error.");
                }
			}
			else
			{
				m_continuefailSec.store(0);
			}
            m_totalFrameNum.store(0);
		}
	}

	void PatcherController::Notify()
	{
		if (m_isCheckDownload.load() == true)
		{
			CheckDownload();
		}

		if (m_hasProgressNotify.load() == true)
		{
			m_hasProgressNotify.store(false);
			if (m_progressCB != nullptr)
			{
				m_progressCB(m_writtenBytes.load(), m_expectBytes.load());
			}
		}

		if (m_hasErrorNotify.load() == true)
		{
			m_hasErrorNotify.store(false);
			if (m_errorCB != nullptr)
			{
				m_errorCB(m_error.load());
			}
		}

		if (m_hasFinishNotify.load() == true)
		{
			m_hasFinishNotify.store(false);
			if (m_finishCB != nullptr)
			{
				m_finishCB();
			}
		}
	}

	void PatcherController::Fresh()
	{
		Init();
	}

	AonePatcher::PatchErrorCode PatcherController::GetResult(uint32& innerError, std::string& errMsg)
	{
		innerError = m_innerError.load();
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			errMsg = m_errorMsg;
		}
		return m_error;
	}

	std::string PatcherController::GetPath()
	{
		return m_path;
	}

    vector<string> PatcherController::splitString(const std::string &str, const std::string &seperator)
    {
        vector<string> tokens;
        if (str == "") return tokens;
        
        string::size_type pos1 = 0;
        string::size_type pos2 = str.find(seperator.c_str());
        while (pos2 != string::npos) {
            string sub = str.substr(pos1, pos2 - pos1);
            if (sub.length() > 0) tokens.push_back(sub);
            pos1 = pos2 + seperator.size();
            pos2 = str.find(seperator, pos1);
        }
        
        if (pos1 != str.length()) tokens.push_back(str.substr(pos1));
        
        return tokens;
    }
   
	void PatcherController::SetURL(const std::string listURL)
	{
		m_url = listURL;
		
        auto questionPos = m_url.find("?");
        if (questionPos == m_url.npos)
        { // 一个下载地址
            auto pos = m_url.rfind("/list.csv.gz");
            if (pos == m_url.npos) {
                string msg = "invalid url, not have '/list.csv.gz'";
                SetError(PatchErrorCode::InvalidArg, 0, msg);
                return;
            }
            
            m_url.erase(pos, m_url.size());
            m_url_list.push_back(m_url);
            
        } else
        {   // 多个下载地址
            // example: http://www.baidu.com?key=http://www.google.com&key=http://www.zhihu.com&key=http://www.baidu.com
            vector<string> s = splitString(listURL, "?");
            if (s.size() == 2) {
                
                m_url = s[0];
                
                auto pos = m_url.rfind("/list.csv.gz");
                if (pos != m_url.npos) {
                    m_url.erase(pos, m_url.size());
                }
                m_url_list.push_back(m_url);
                

                string backupURLStr = s[1];
                vector<string> backupURLs = splitString(backupURLStr, "key=");
                for (int i = 0; i < backupURLs.size(); i++)
                {
                    string url = backupURLs[i];
                    if (i != (backupURLs.size() - 1))
                    {
                        // delete the last `&`.
                        string _url = url.substr(0, url.length() - 1);
                        m_url_list.push_back(_url);
                    } else {
                        m_url_list.push_back(url);
                    }
                }
            } else {
                SetError(PatchErrorCode::InvalidArg, 0, "invalid url, url contains multipe ?");
                return;
            }
        }
        
		printf("Patcher --- res dir url is %s\n", m_url.c_str());
	}

	void PatcherController::SetPath(const std::string resPath)
	{
		m_path = resPath;
		if (m_path.size() > 0 && m_path[m_path.size()-1] == '/')
		{
			m_path.erase(m_path.end() - 1);
		}
        printf("Patcher --- res path = %s\n", m_path.c_str());
	}

	void PatcherController::SetPackageResPath(std::string packageResPath)
	{
		m_packageResPath = packageResPath;
        printf("Patcher --- package res path = %s\n", m_packageResPath.c_str());
	}

	void PatcherController::SetProgressCB(PatcherProgressCB progressCB)
	{
		m_progressCB = progressCB;
	}

	void PatcherController::SetFinishCB(PatcherFinishCB finishCB)
	{
		m_finishCB = finishCB;
	}

	void PatcherController::SetErrorCB(PatcherErrorCB errorCB)
	{
		m_errorCB = errorCB;
	}

	void PatcherController::SetTaskFactory(IPatcherTaskFactoryPtr factory)
	{
		m_taskFactory = factory;
	}

	void PatcherController::SetDownloadedMgrPtr(IDownloadedMgrPtr downloadedMgrPtr)
	{
		m_downMgr = downloadedMgrPtr;
	}

	void PatcherController::Init()
	{
        if (m_thread.joinable())
        {
            m_thread.join();
        }
        m_taskList.Join();

		m_totalFrameNum.store(0);
		m_continuefailSec.store(0);
		m_beginBytes.store(0);
		m_isCheckDownload.store(false);
        
		m_finish.store(false);
		m_error.store(PatchErrorCode::NoError);
		m_innerError.store(0);

		m_hasProgressNotify.store(false);
		m_hasFinishNotify.store(false);
		m_hasErrorNotify.store(false);

		curl_global_init(CURL_GLOBAL_ALL);
	}

	void PatcherController::MainLoop()
	{
		if (m_patchListPtr.get() == nullptr)
		{
			if (PrepareUpdateList() == false) return;
		}

		std::list<std::string> downloadedList = m_downMgr->GetList();
		m_patchListPtr->RemoveByList(downloadedList);
    
        m_updateFileList = m_patchListPtr->GetFileList();
		m_isCheckDownload.store(true);

		while (m_error.load() == PatchErrorCode::NoError)
		{
            std::lock_guard<std::mutex> lock(m_files_mutex);
            if (m_updateFileList.size() == 0 && m_taskList.IsEmpty())
            {
                //error may be set in another thread, but not after updateFileList and taskList is empty, so check again
                if (m_error == PatchErrorCode::NoError)
                {                    
                    m_isCheckDownload.store(false);
                    
                    // 下载完成，移动下载文件到指定位置
                    if (AfterDownloading() == false) {
                        return;
                    }
    
                    m_hasFinishNotify.store(true);
                    m_finish.store(true);

                    // finish download, and remove url info from file.
                    std::string pOldUrlPath = CPFileUtils::concateFilePath(GetPatcherOldDir(), "oldUrl");
                    if (CPFileUtils::exist(pOldUrlPath.c_str())) {
                        CPFileUtils::remove(pOldUrlPath.c_str());
                    }

                    CurlManagerPool::GetInstance()->CleanUpAllCurls();
                }
                break;
            }

            if (m_taskList.IsFull() == false)
            {
                if (m_updateFileList.size() != 0)
                {
                    // get the first element.
                    const std::string filePath = m_updateFileList.front();
                    // delete the first element.
                    m_updateFileList.pop_front();
                    
                    if (m_patchListPtr.get() != nullptr && m_patchListPtr->HasFile(filePath))
                    {
                        int url_index = 0;
                        map<string, int>::iterator it;
                        for (it = m_filesURLInfo.begin(); it != m_filesURLInfo.end(); it ++) {
                            if (it->first == filePath) {
                                url_index = it->second;
                                break;
                            }
                        }
                        
                        string url = m_url_list[url_index];
                        PatchOneFile(m_patchListPtr->GetFileInfo(filePath), url);
                    }
                    
                }
            }
            m_taskList.WaitNotFull();
		}

        printf("\n--- main loop end!\n");
		if (m_error.load() != PatchErrorCode::NoError)
		{
			m_taskList.Cancel();
			m_taskList.Join();

			CurlManagerPool::GetInstance()->CleanUpAllCurls();
		}
	}
    
    void PatcherController::PatchOneFile(const AonePatcher::FileInfo &fileInfo, std::string url)
    {
        IPatcherTaskPtr task = m_taskFactory->CreateTask();
        
        // 新类型的list文件，需要检测MD5
        task -> m_isCheckMD5 = m_patchListPtr -> GetNewList();
        
        task->m_url = url;
        task->m_path = GetDownloadingDir();
        task->m_name = fileInfo.m_path;
        task->m_md5 = fileInfo.m_md5;
        task->m_verify_md5 = fileInfo.m_verify_md5;
        task->m_size = fileInfo.m_size;
        task->m_endCB = std::bind(&PatcherController::OnTaskEnd, this, std::placeholders::_1);
        task->m_progressCB = std::bind(&PatcherController::OnTaskProgressChange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        task->m_downFailedCB = std::bind(&PatcherController::OnTaskDownFailed, this, std::placeholders::_1, std::placeholders::_2);
        
        std::string d1 = CPUtils::CPFileUtils::concateFilePath(task->m_path, fileInfo.m_path);
        std::string d2 = Decompress::getUncompressedFileName(d1);
        if (CPFileUtils::mdbyf(d1.c_str()) == false || CPFileUtils::mdbyf(d2.c_str()) == false)
        {
            SetError(PatchErrorCode::Storage, errno, "create dir fail");
            return;
        }
        
        printf("Patcher --- begin patch file, path = %s, md5 = %s, size = %d, verify_md5 = %s, url = %s\n", fileInfo.m_path.c_str(), fileInfo.m_md5.c_str(), fileInfo.m_size, fileInfo.m_verify_md5.c_str(), url.c_str());
        
        m_taskList.Add(task);
    }

	void PatcherController::OnTaskEnd(IPatcherTaskPtr task)
	{
		if (task->m_error == PatchErrorCode::NoError)
		{
            m_downMgr->Add(task->m_name);
           
            if (m_error == PatchErrorCode::NoError) {
                m_hasProgressNotify.store(true);
            }
			printf("Patcher--- end patch %s\n",task->m_name.c_str());
        } else if (task->m_error == PatchErrorCode::MD5 || task->m_error == PatchErrorCode::Network)
        {	
			// MD5错误或者因网络原因下载失败的, 重新切换URL下载
            std::lock_guard<std::mutex> lock(m_files_mutex);
            
            // 删除下载的文件
            std::string diskDirPath = CPUtils::CPFileUtils::concateFilePath(task -> m_path, task -> m_name);
            string dstPath = Decompress::getUncompressedFileName(diskDirPath);
            if (CPUtils::CPFileUtils::exist(dstPath.c_str())) {
                CPUtils::CPFileUtils::rmdir(dstPath.c_str());
            }
            
            m_writtenBytes.fetch_sub(task -> m_curTotalSize);
            
            string path = task -> m_name;
            FileInfo fileInfo = m_patchListPtr -> GetFileInfo(path);
            
            // 查找当前文件下载URL
            int url_index = 0;
            map<string, int>::iterator it;
            for (it = m_filesURLInfo.begin(); it != m_filesURLInfo.end(); it++) {
                if (it -> first == path) {
                    url_index = it -> second;
                    break;
                }
            }
            
            
            if (url_index < (m_url_list.size() - 1)) {
                url_index += 1;
            } else {
                url_index = 0;
            }
            
            if (url_index == 0)
            {
                printf("Patcher --- PatcherController ---, don't need use the default url.\n");
				SetError(task->m_error, task->m_innerError, "all url is used.");
            } else
            {
				printf("Patcher--- download error %u --- \nredownload url = %s\npath = %s\nmd5 = %s\nsize = %d\n", (uint32)task->m_error, m_url_list[url_index].c_str(), path.c_str(), fileInfo.m_verify_md5.c_str(), fileInfo.m_size);
                
                // 重新添加到updateFileList中
                m_filesURLInfo[path] = url_index;
//                m_mismatchFileList.push_back(fileInfo);
                
                m_updateFileList.push_back(fileInfo.m_path);
                printf("\n--- size = %lu\n", m_updateFileList.size());
            }
            
        } else
		{
			if (m_error == PatchErrorCode::NoError)
			{
				SetError(task->m_error, task->m_innerError, task->m_errorMsg);
			}
			else if (task->m_error != PatchErrorCode::Cancel )
			{
				CP_LOGE("after some error ,task end by non cancel, task name is %s, error is %u, inner code is %u, msg is %s",
					task->m_name.c_str(),
					(uint32)task->m_error,
					task->m_innerError,
					task->m_errorMsg.c_str());
			}
		}
        m_taskList.Remove(task);
        
	}

	void PatcherController::OnTaskProgressChange(IPatcherTaskPtr task, uint32 cur, uint32 total)
	{
		m_writtenBytes.fetch_add(cur);
        if (m_writtenBytes.load() > m_expectBytes.load()) {
            m_writtenBytes.store(m_expectBytes.load());
        }
		m_hasProgressNotify.store(true);
	}

	void PatcherController::OnTaskDownFailed(IPatcherTaskPtr task, uint32 curTotal)
	{
		m_writtenBytes.fetch_sub(curTotal);
		m_hasProgressNotify.store(true);
	}

	void PatcherController::SetError(PatchErrorCode errorCode, uint32 innerError, std::string errorMsg)
	{
		m_isCheckDownload.store(false);
		m_error.store(errorCode);
		m_innerError.store(innerError);
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_errorMsg = errorMsg;
		}
		m_hasErrorNotify.store(true);
        m_taskList.Cancel();
        printf("Patcher --- errorCode:%u, innerError:%u, msg:%s\n", (uint32)errorCode, innerError, errorMsg.c_str());
		CP_LOGE("errorCode:%u, innerError:%u, msg:%s", (uint32)errorCode, innerError, errorMsg.c_str());

		stringstream ss;
		ss << "patcher exception : " << ((int)errorCode * 100000 + innerError);
		string eType = ss.str();
		ss.clear();
		ss << "inner error " << innerError << ", msg is " << errorMsg;
		string eMsg = ss.str();
		cplog_report_exception(eType.c_str(), eMsg.c_str());
	}

	bool PatcherController::PrepareUpdateList()
	{
		CheckUrl();

		if (m_taskFactory.get() == NULL)
		{
			SetError(PatchErrorCode::InvalidArg, 0, "not set factory");
			return false;
		}

		try
		{
			m_patchListPtr = m_taskFactory->CreatePrepareListTask()->Do(m_packageResPath, m_path, GetDownloadingDir(), m_url_list);
		}
		catch (PatcherException e)
		{
			SetError(e.m_code, e.m_innerCode, e.what());
			return false;
		}
        
		if (m_downMgr->Load(GetDownloadingDir()) == false) {
			SetError(PatchErrorCode::Storage, errno, "load downloading fail");
			return false;
		}
        
		std::string csvFile = string("list-v2.csv.gz");
        if (m_patchListPtr -> GetNewList() == false) {
            csvFile = string("list.csv.gz");
        }
        
		if (m_downMgr->IsIn(csvFile) == false)
		{
			m_downMgr->Add(csvFile);
		}

		std::list<std::string> downloadedList = m_downMgr->GetList();
		uint32 wb = 0;
		for (auto& filePath : downloadedList)
		{
			if (m_patchListPtr->HasFile(filePath))
			{
				const FileInfo& finfo = m_patchListPtr->GetFileInfo(filePath);
				wb += finfo.m_size;
			}
		}

		m_writtenBytes.store(wb);
		m_expectBytes.store(m_patchListPtr->GetTotalSize());
		m_hasProgressNotify.store(true);
		
		return true;
	}

	bool PatcherController::AfterDownloading()
	{
		try{
			m_downMgr->MoveFilesTo(m_path);
		}
		catch (PatcherException e){
			SetError(e.m_code, e.m_innerCode, e.what());
			return false;
		}
		return true;
	}

	void PatcherController::CopyPVToPOldVDir()
	{
		std::string pvPath = CPFileUtils::concateFilePath(m_packageResPath, "version");
		if (!CPFileUtils::exist(pvPath.c_str()))
		{
			CP_LOGE("CopyPVToPOldVDir: packageResPath not exist!");
			return;
		}

		std::string version = CPFileUtils::getStringFromFile(pvPath.c_str());

		std::string pOldvPath = CPFileUtils::concateFilePath(GetPatcherOldDir(), "version");
		if (CPFileUtils::writedata((void*)version.c_str(), version.size(), pOldvPath.c_str()) == false)
		{
			CP_LOGE("CopyPVToPOldVDir: save version failed!");
		}
	}

	void PatcherController::SaveUrlToFile()
	{
		std::string pOldUrlPath = CPFileUtils::concateFilePath(GetPatcherOldDir(), "oldUrl");
		
		if (CPFileUtils::writedata((void*)m_url.c_str(), m_url.size(), pOldUrlPath.c_str()) == false)
		{
			CP_LOGE("SaveUrlToFile: save url failed!");
		}
	}

	void PatcherController::CheckUrl()
	{
		std::string pOldUrlPath = CPFileUtils::concateFilePath(GetPatcherOldDir(), "oldUrl");
		if (CPFileUtils::exist(pOldUrlPath.c_str()))
		{
			std::string readUrl = CPFileUtils::getStringFromFile(pOldUrlPath);
			if (m_url != readUrl)
			{
				RemoveDownloading();
			}
		}

		SaveUrlToFile();
	}

	void PatcherController::RemoveDownloading()
	{
		std::string downPath = CPFileUtils::concateFilePath(GetDownloadingDir(), "downloaded.record");
        if (!CPFileUtils::exist(downPath.c_str())){
            return;
        }

		IDownloadedMgrPtr downMgr = IDownloadedMgrPtr(new DownloadedMgr());
		if (downMgr->Load(GetDownloadingDir()) == false) {
			SetError(PatchErrorCode::Storage, errno, "load downloaded file fail");
			return;
		}

		std::list<std::string> downloadedFiles = downMgr->GetList();
		for (auto& compFileName : downloadedFiles)
		{
			std::string compFilePath = CPFileUtils::concateFilePath(GetDownloadingDir(), compFileName);
            if (CPFileUtils::exist(compFilePath.c_str())){
                CPFileUtils::remove(compFilePath.c_str());
            }

			std::string unComFileName = Decompress::getUncompressedFileName(compFileName);
			std::string unCompFilePath = CPFileUtils::concateFilePath(GetDownloadingDir(), unComFileName);
            if (CPFileUtils::exist(unCompFilePath.c_str())){
                CPFileUtils::remove(unCompFilePath.c_str());
            }
		}

		CPFileUtils::remove(downPath.c_str());

		CPUtils::CPFileUtils::purgeCachedEntries();
	}

	void PatcherController::RemoveDownloadedFile()
	{
		PatchList patchList;
        
		std::string listFilePath = CPFileUtils::concateFilePath(m_path, "list-v2.csv");
        if (CPFileUtils::exist(listFilePath.c_str()) == false) {
            listFilePath = CPFileUtils::concateFilePath(m_path, "list.csv");
        }
        CP_LOGD("Patcher--- RemoveDownloadedFile, list = %s\n", listFilePath.c_str());
        
		if (CPFileUtils::exist(listFilePath.c_str()))
		{
			bool isLoad = patchList.Load(listFilePath);
			if (isLoad)
			{
				std::list<std::string> fileList = patchList.GetFileList();
				for (std::list<std::string>::iterator iter = fileList.begin(); iter != fileList.end(); ++iter)
				{
                    string path = CPFileUtils::concateFilePath(m_path, *iter);
                    printf("Patcher--- RemoveDownloadedFile --- file path = %s\n", path.c_str());
					std::string filePath = Decompress::getUncompressedFileName(path);

					if (CPFileUtils::exist(filePath.c_str()))
					{
						printf("Patcher --- CheckPatchValid: remove file %s\n", filePath.c_str());
						CPFileUtils::remove(filePath.c_str());
					}
				}
			}
		}

        if (CPFileUtils::exist(listFilePath.c_str())){
            CPFileUtils::remove(listFilePath.c_str());
        }
	}

	std::string PatcherController::GetDownloadingDir()
	{
        return CPFileUtils::concateFilePath(m_path, DownloadingDir);
	}

	std::string PatcherController::GetPatcherOldDir()
	{
		return CPFileUtils::concateFilePath(m_path, PatcherOldDir);
	}

	bool PatcherController::WriteDataToCsv(std::string path, std::list<FileInfo> info)
	{
		std::ofstream f;
		f.open(path, std::ios::out | std::ios::trunc);
		if (!f.is_open())
			return false;

		for (list<FileInfo>::iterator iter = info.begin(); iter != info.end(); ++iter)
		{
			f << (*iter).m_md5;
			f << ",";
			f << (*iter).m_size;
			f << ",";
			f << (*iter).m_path;
            if ((*iter).m_verify_md5.size() > 0) {
                f << ",";
                f << (*iter).m_verify_md5;
            }
            
			f << std::endl;

			if (f.bad() || f.fail())
			{
				CP_LOGE("WriteDataToCsv: write data failed!");
				return false;
			}
		}

		f.close();

		return true;
	}

	void PatcherController::getFileNameFromDir(const char* path)
	{
		cpfile_fws(path, cbOfGetFileFromDir, CPFILE_WALK_RECURSION);
	}

	void PatcherController::CheckPatchValid()
	{
		bool isNewer = false;
		bool isVerDiffer = false;

		std::string pvPath = CPFileUtils::concateFilePath(m_packageResPath, "version");
        // 传入相对路径，自动去根路径下搜索
        // 游戏可忽略设置包内资源路径
        if (!CPFileUtils::exist(pvPath.c_str()))
        {
			CP_LOGE("CheckPatchValid: packageResPath not exist!");
        } else
		{
            // 包内资源版本 是否高于 热更资源目录下的版本
			AonePatcher::VersionFile packageVer(m_packageResPath);
			std::string lvPath = CPFileUtils::concateFilePath(m_path, "version");
			if (CPFileUtils::exist(lvPath.c_str()))
			{
				AonePatcher::VersionFile localResVer(m_path);
				isNewer = packageVer.isNewThan(localResVer);
			}
            
            // 包内资源版本 跟 上次检查过的资源版本是否不同
			std::string pOldvPath = CPFileUtils::concateFilePath(GetPatcherOldDir(), "version");
			if (CPFileUtils::exist(pOldvPath.c_str()))
			{
				AonePatcher::VersionFile pacOldVer(GetPatcherOldDir());
				isVerDiffer = packageVer.isDiffer(pacOldVer);
			}
		}

        // 包内资源高于热更目录资源版本
        // 包内资源版本跟上次检查版本不同
		if (isNewer || isVerDiffer)
		{
            CP_LOGD("Patcher --- isNewer = %s, isVerDiffer = %s\n", isNewer ? "true": "false", isVerDiffer ? "true": "false");
			
            std::list<FileInfo> curCsvContent;
			std::map<string, int> commonFiles;
			PatchList patchList;
			PatchList packPatchList;
            
            // 优先查找新的list文件
            bool isResourceExistNewList = true;
            std::string listFilePath = CPFileUtils::concateFilePath(m_path, "list-v2.csv");
            if (CPFileUtils::exist(listFilePath.c_str()) == false) { // 查看热更目录下lsit文件
                isResourceExistNewList = false;
                listFilePath = CPFileUtils::concateFilePath(m_path, "list.csv");
            }
            
            bool isPackageExistNewList = true;
			std::string packListPath = CPFileUtils::concateFilePath(m_packageResPath, "list-v2.csv");
            if (CPFileUtils::exist(packListPath.c_str()) == false) { // 查看包内list文件
                isPackageExistNewList = false;
                packListPath = CPFileUtils::concateFilePath(m_packageResPath, "list.csv");
            }
            
            if (CPFileUtils::exist(listFilePath.c_str()) && CPFileUtils::exist(packListPath.c_str()))
			{
				bool isLoad = patchList.Load(listFilePath);
				bool isPackLoad = packPatchList.Load(packListPath);
                if (isPackageExistNewList == true && isResourceExistNewList == false) 
                { // 新热更库，但是已经下载过老热更资源，需要删除热更资源
                    CP_LOGD("\n--- delete old version resource ---\n");
                    RemoveDownloadedFile();
                } else if (isLoad && isPackLoad) 
                {
                    // 包内list文件
					std::list<std::string> packFileList = packPatchList.GetFileList();
					for (std::list<std::string>::iterator iter = packFileList.begin(); iter != packFileList.end(); ++iter)
					{
                        // 热更目录下的list文件
						if (patchList.HasFile(*iter))
						{
							commonFiles.insert(make_pair(*iter, 0));

                            printf("Patcher --- check pathch valid, file path = %s\n", CPFileUtils::concateFilePath(m_path, *iter).c_str());
							std::string filePath = Decompress::getUncompressedFileName(CPFileUtils::concateFilePath(m_path, *iter));

							if (CPFileUtils::exist(filePath.c_str()))
							{
								printf("Patcher --- CheckPatchValid: remove file %s\n", filePath.c_str());
								CPFileUtils::remove(filePath.c_str());
							}
						}

						FileInfo info = packPatchList.GetFileInfo(*iter);
						curCsvContent.push_back(info);
					}

					std::list<std::string> fileList = patchList.GetFileList();
					for (std::list<std::string>::iterator iter = fileList.begin(); iter != fileList.end(); ++iter)
					{
						if (commonFiles.find(*iter) != commonFiles.end())
							continue;
						FileInfo info = patchList.GetFileInfo(*iter);
						curCsvContent.push_back(info);
					}

                    if (!curCsvContent.empty()) {
                        if(!WriteDataToCsv(listFilePath, curCsvContent)) {
                            CPUtils::CPFileUtils::rmdir((m_path + "/").c_str());
                        }
                    }
						
				} else
                {
                    RemoveDownloadedFile();
                }
            } else
            {
                RemoveDownloadedFile();
            }

			RemoveDownloading();
		}

		if (!CPFileUtils::exist(GetPatcherOldDir().c_str()))
		{
			if (!CPFileUtils::mkdir(GetPatcherOldDir().c_str()))
			{
				CP_LOGE("CheckPatchValid create dir %s fail", GetPatcherOldDir().c_str());
				return;
			}
		}

        // 复制包内version文件
		CopyPVToPOldVDir();
	}

	void PatcherController::RemoveUselessFiles()
	{
		string writeablePath = CPFileUtils::getWriteablePath();
		string tmpPath = "";
        if (m_path.substr(m_path.length() - 1) == "/") {
			tmpPath = m_path.substr(0, m_path.length() - 1);
        } else {
			tmpPath = m_path + "/";
        }
        
		if (m_path == writeablePath || tmpPath == writeablePath)
		{
			CP_LOGV("Patcher --- RemoveUselessFiles: patcher path is same with writeable path! path is:%s, write path is:%s\n", m_path.c_str(), writeablePath.c_str());
			return;
		}

		printf("\nPatcher --- RemoveUselessFiles: patcher path is differ with writeable path! path is:%s, write path is:%s", m_path.c_str(), writeablePath.c_str());
		std::string listFilePath = CPFileUtils::concateFilePath(m_path, string("list-v2.csv"));
        if (CPFileUtils::exist(listFilePath.c_str()) == false) {
            listFilePath = CPFileUtils::concateFilePath(m_path, string("list.csv"));
        }
		if (CPFileUtils::exist(listFilePath.c_str()))
		{
			bool isLoad = patchListFiles.Load(listFilePath);
			if (isLoad)
			{
				patcherPath = m_path;
				getFileNameFromDir(patcherPath.c_str());
			}
		}
	}
    
}
