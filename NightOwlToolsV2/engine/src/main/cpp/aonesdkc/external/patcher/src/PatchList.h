#ifndef _PATCH_LIST_H_
#define _PATCH_LIST_H_

#include <string>
#include <map>
#include <list>
#include <memory>

#include "def.h"

namespace AonePatcher
{
	class FileInfo{
	public:
        /**文件路径， 相对路径*/
		std::string m_path;
        
		/** 原始文件MD5 + "|" + "文件名加密"*/
		std::string m_md5;
        
        /** 文件压缩之后的大小 */
		unsigned int m_size;
        
        /** 校验MD5， 文件加密压缩之后的MD5*/
        std::string m_verify_md5;
	};

	typedef std::map<std::string, FileInfo> FileInfoDict;
	typedef std::list<std::string> FilePathList;

	class IPatchList;
	typedef std::shared_ptr<IPatchList> IPatchListPtr;
	class IPatchList
	{
	public:
		virtual ~IPatchList() = 0;

        /**
         Find differents from two object and make up a new one.
         */
		virtual IPatchListPtr DiffList(IPatchList& oldList) = 0;
        
        
        /**
         Read content from CSV files path and place all the content in `m_fileDict` with the file name as the key and the remaining three elements as the `FileInfo` objects as the value.

         @discusston CSV format: file name, md5, path, file size.
         */
		virtual bool Load(std::string path) = 0;
        
        
        /**
         Determine if there is a file in the `m_fileDict` from a key `filePath`.
         */
		virtual bool HasFile(std::string filePath) = 0;
        
        
        /**
         Get a `FileInfo` object from `filePath`.
         */
		virtual const FileInfo& GetFileInfo(std::string filePath) = 0;
		
        
        /**
         Get a list of all file paths.
         
         Except from `version.gz` file.
         */
		virtual FilePathList GetFileList() = 0;
		
        
        /**
         Remove all the file from the path in `pathList`.
         */
		virtual void RemoveByList(FilePathList& pathList) = 0;
		
        
        /**
         Get the total size of all files in the csv file.
         */
		virtual uint32 GetTotalSize() = 0;
        
        virtual void SetListType(bool isNew) = 0;
        
        /**
         Indicate whether the list file is new type.
         */
        virtual bool GetNewList() = 0;
	};

    /**
     This class is manager all the download files information.
     */
	class PatchList : public IPatchList
	{
	public:
		virtual IPatchListPtr DiffList(IPatchList& oldList) override;
		bool Load(std::string path) override;
		bool HasFile(std::string filePath) override;
		const FileInfo& GetFileInfo(std::string filePath) override;
		FilePathList GetFileList() override;
		void RemoveByList(FilePathList& pathList) override;
		uint32 GetTotalSize() override;
        void SetListType(bool isNew) override;
        bool GetNewList() override;
        
	protected:
		FileInfoDict m_fileDict;
        bool m_is_new_list;
	};
}

#endif
