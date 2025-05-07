#include "DownloadedMgr.h"
#include "CPFileUtils.h"
#include "cplog.h"
#include "PatcherException.h"
#include "decompress.h"
#include <sstream>

using namespace CPUtils;

namespace AonePatcher
{
	static const std::string RECORD_FILE_PATH = "downloaded.record";

	bool DownloadedMgr::Load(std::string storeDirPath)
	{
		m_dict.clear();
		if (CPFileUtils::exist(storeDirPath.c_str()) == false)
		{
			if (CPFileUtils::mkdir(storeDirPath.c_str()) == false)
			{
				CP_LOGE("create dir %s fail", storeDirPath.c_str());
				return false;
			}
		}
        m_storePath = CPUtils::CPFileUtils::concateFilePath(storeDirPath, RECORD_FILE_PATH);
		m_storeDirPath = storeDirPath;
		if (CPFileUtils::exist(m_storePath.c_str()))
		{
			std::string content = CPFileUtils::getDataFromFile(m_storePath);
			std::istringstream stream(content);
			std::string line;
			while (!CPUtils::CPFileUtils::safeGetline(stream, line).eof())
			{
				m_dict.insert(line);
			}
		}
		return true;
	}

	void DownloadedMgr::Add(const std::string path)
	{
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_dict.insert(path);
			std::string line = path + "\n";
			CPFileUtils::appenddata((void*)line.c_str(), line.size(), m_storePath.c_str());
		}
	}

	bool DownloadedMgr::IsIn(const std::string path)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_dict.find(path) != m_dict.end();
	}

	AonePatcher::FilePathList DownloadedMgr::GetList()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		FilePathList pathList;
		for (auto& filePath : m_dict)
		{
			pathList.push_back(filePath);
		}
		return std::move(pathList);
	}

	void DownloadedMgr::Clear()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_dict.clear();
		CPUtils::CPFileUtils::remove(m_storePath.c_str());
	}

	void DownloadedMgr::MoveFilesTo(std::string dstDir)
	{
		std::string downloadedDir = m_storeDirPath;
		std::list<std::string> downloadedList = GetList();
		for (auto& downloadedName : downloadedList)
		{
			downloadedName = Decompress::getUncompressedFileName(downloadedName);
		}
		uint32 currentCount = 0;
		uint32 totalCount = downloadedList.size();
		for (auto& downloadedName : downloadedList)
		{
			std::string srcPath = CPUtils::CPFileUtils::concateFilePath(downloadedDir, downloadedName);
			std::string dstPath = CPUtils::CPFileUtils::concateFilePath(dstDir, downloadedName);
			if (!CPUtils::CPFileUtils::exist(srcPath.c_str())){
				CP_LOGE("after downloading src file not exist, path is %s", srcPath.c_str());
				continue;
			}

			if (CPUtils::CPFileUtils::mdbyf(dstPath.c_str()) == false)
			{
				std::string errMsg = "after downloading create dir fail, path is " + dstPath;
				throw(PatcherException(PatchErrorCode::Storage, errno, errMsg));
			}
			bool isMoveMode = true; //是否是移动模式 优先这个模式 如果失败了改用复制模式

			if (CPUtils::CPFileUtils::exist(dstPath.c_str())){
				//删除之前的热更文件
				if (!CPUtils::CPFileUtils::remove(dstPath.c_str())){
					//删除失败只能改用复制模式
					isMoveMode = false;
					CP_LOGE("after downloading old file force remove fail, path is %s, error is %d", dstPath.c_str(), errno);
				}
			}
			if (isMoveMode){
				if (CPUtils::CPFileUtils::rename(srcPath.c_str(), dstPath.c_str())){
					// CP_LOGI("after downloading move success: %s -> %s", srcPath.c_str(), dstPath.c_str());
				}
				else{
					CP_LOGE("after downloading move file fail,switch copy mode, path is %s, backup path is %s,error is %d", srcPath.c_str(), dstPath.c_str(), errno);
					//移动失败改用复制模式
					isMoveMode = false;
				}
			}
			if(!isMoveMode){ //复制模式
				std::string srcContent = CPUtils::CPFileUtils::getDataFromFile(srcPath);
				CP_LOGI("after downloading copy:%s -> %s", srcPath.c_str(), dstPath.c_str());
				if (CPUtils::CPFileUtils::writedata((void*)srcContent.c_str(), srcContent.size(), dstPath.c_str()) == false)
				{
					std::string errMsg = "after downloading write file fail, path is " + dstPath;
					throw(PatcherException(PatchErrorCode::Storage, errno, errMsg));
				}
				CPUtils::CPFileUtils::remove(srcPath.c_str());
				// CP_LOGI("after downloading remove:%s -> %s", srcPath.c_str(), dstPath.c_str());
			}
		}

		Clear();
	}

}
