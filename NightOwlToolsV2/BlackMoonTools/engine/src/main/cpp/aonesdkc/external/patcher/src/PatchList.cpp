#include <memory>
#include <errno.h>

#include "PatchList.h"
#include "CPFileUtils.h"
#include "CSVparser.hpp"
#include "cplog.h"
#include "PatcherException.h"


namespace AonePatcher
{
	IPatchList::~IPatchList(){}

	AonePatcher::IPatchListPtr PatchList::DiffList(IPatchList& oldList)
	{
		std::shared_ptr<PatchList> patchList = std::shared_ptr<PatchList>(new PatchList());

		for (FileInfoDict::iterator iterNew = m_fileDict.begin(); iterNew != m_fileDict.end(); ++iterNew)
		{
			if (oldList.HasFile(iterNew->first))
			{
				FileInfo fileInfo = oldList.GetFileInfo(iterNew->first);
				if (iterNew->second.m_md5 != fileInfo.m_md5)
				{
					patchList->m_fileDict.insert(*iterNew);
				}
			}
			else
			{
				patchList->m_fileDict.insert(*iterNew);
			}
		}

		return patchList;
	}

	bool PatchList::Load(std::string path)
	{
		try
		{
			if (!CPUtils::CPFileUtils::exist(path.c_str()))
			{
				throw(PatcherException(PatchErrorCode::Common, errno, "PatchList::Load path is not exist!"));
				return false;
			}

			std::string sCSVInfo = CPUtils::CPFileUtils::getStringFromFile(path);
			if (sCSVInfo == "")
			{
				CP_LOGE("PatchList::Load: no sCSVInfo!");
				return true;
			}

			csv::Parser csvParser(sCSVInfo, csv::DataType::ePURE);

			int columnCount = csvParser.columnCount();
			if (columnCount != 3 && columnCount != 4) {
				CP_LOGE("the csv file format error, columnCount is %d .", columnCount);
				return false;
			}

			CP_LOGV("the column is %d, the row is %d\n", columnCount, csvParser.rowCount());

			for (uint32 i = 0; i < csvParser.rowCount(); i++)
			{
				csv::Row row = csvParser.getRow(i);
				FileInfo fileInfo;

				fileInfo.m_md5 = row.getValue<std::string>(0);
				fileInfo.m_size = row.getValue<int>(1);
				fileInfo.m_path = row[2];
                if (columnCount == 4) {
                    fileInfo.m_verify_md5 = row[3];
                }
				m_fileDict.insert(make_pair(fileInfo.m_path, fileInfo));
			}

			return true;
		}
		catch (std::exception &e)
		{
			CP_LOGE("load file error: %s\n", e.what());
			return false;
		}
		
		return false;
	}

	bool PatchList::HasFile(std::string filePath)
	{
		return m_fileDict.find(filePath) != m_fileDict.end();
	}

	const FileInfo& PatchList::GetFileInfo(std::string filePath)
	{
		if (!HasFile(filePath))
		{
			CP_LOGE("file path is incorrent, file path is %s", filePath.c_str());
		}

		return m_fileDict[filePath];
	}

	AonePatcher::FilePathList PatchList::GetFileList()
	{
		FilePathList resultList;

		for (FileInfoDict::iterator iter = m_fileDict.begin(); iter != m_fileDict.end(); ++iter)
		{
			if (iter->first == "version.gz")
			{
				resultList.push_front(iter->first);
				continue;
			}

			resultList.push_back(iter->first);
		}

		return std::move(resultList);
	}

	void PatchList::RemoveByList(FilePathList& pathList)
	{
		for (FilePathList::iterator iter = pathList.begin(); iter != pathList.end(); ++iter)
		{
			if (HasFile(*iter))
			{
				m_fileDict.erase(*iter);
			}
		}
	}

	uint32 PatchList::GetTotalSize()
	{
		uint32 totalSize = 0;

		for (FileInfoDict::iterator iter = m_fileDict.begin(); iter != m_fileDict.end(); ++iter)
		{
			totalSize += iter->second.m_size;
		}

		return totalSize;
	}
    
    void PatchList::SetListType(bool isNew)
    {
        m_is_new_list = isNew;
    }
    
    bool PatchList::GetNewList()
    {
        return m_is_new_list;
    }
}
