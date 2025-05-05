#ifndef _DOWNLOADED_MGR_H_
#define _DOWNLOADED_MGR_H_

#include <set>
#include <string>
#include <mutex>
#include <memory>
#include "PatchList.h"

namespace AonePatcher
{
	/**
	This class is a base class, manager all the downloaded information.
	*/
	class IDownloadedMgr
	{
	public:
        /**
         Begin load the downloadeding information, all the information recorder in the file named `downloaded.record`, which under the `storeDirPath` folder.
         */
		virtual bool Load(std::string storeDirPath) = 0;

		/**
		Add a path to the `downloaded.record` file. 
		*/
		virtual void Add(const std::string path) = 0;

		/**
		Indicate whether a path is already downlaod.
		*/
		virtual bool IsIn(const std::string path) = 0;
		
		/**
		Get a list of all the downloaded files path.
		*/
		virtual FilePathList GetList() = 0;
		
		/**
		Clear all the downloaded information.
		*/
		virtual void Clear() = 0;
		
		
		virtual void MoveFilesTo(std::string dstDir) = 0;
	};
	typedef std::shared_ptr<IDownloadedMgr> IDownloadedMgrPtr;

	class DownloadedMgr : public IDownloadedMgr
	{
	public:
		virtual bool Load(std::string storeDirPath);
		virtual void Add(const std::string path);
		virtual bool IsIn(const std::string path);
		virtual FilePathList GetList();
		virtual void Clear();
		virtual void MoveFilesTo(std::string dstDir);

	private:
		std::set<std::string> m_dict;
		std::mutex m_mutex;
		std::string m_storePath;
		std::string m_storeDirPath;
	};
}

#endif
