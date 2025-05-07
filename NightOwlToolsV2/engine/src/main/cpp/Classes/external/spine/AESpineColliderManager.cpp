#include "external/spine/AESpineColliderManager.h"

NS_AE_BEGIN

AESpineColliderManager* AESpineColliderManager::_pSpineColliderManager = nullptr;

AESpineColliderManager::AESpineColliderManager()
{
}

AESpineColliderManager::~AESpineColliderManager()
{
	auto iter = spineColliderMap.begin();
	for (; iter != spineColliderMap.end(); ++iter)
	{
			delete iter->second;
	}
	spineColliderMap.clear();
}

AESpineColliderManager* AESpineColliderManager::getInstance()
{
	if (_pSpineColliderManager == nullptr)
	{
		_pSpineColliderManager = new (std::nothrow) AESpineColliderManager();
	}
	return _pSpineColliderManager;
}

void AESpineColliderManager::destroyInstance()
{
	AESafe_delete(_pSpineColliderManager);
}

bool AESpineColliderManager::writeSpineColliderToFile(AESpineColliderData* spineData, std::string fileNameNoSuffix, std::string destPath)
{
	uint32 dataLen = spineData->calculateLength();
	AEByteBuffer aeBytes(dataLen);

	spineData->toBytes(aeBytes);
	std::string path = destPath + "\\" + fileNameNoSuffix + ".collider";
	FILE* fp = fopen(path.c_str(), "wb+");
	if (fp == nullptr)
		return false;
	uint32 dataWriteLen  = fwrite(aeBytes.getBuffer(), 1, dataLen, fp);
	fclose(fp);
	bool ret = dataWriteLen != dataLen;
	return ret;
}


 AEByteBuffer AESpineColliderManager::getData(const std::string filename, bool forString)
{
	if (filename.empty()) return AEByteBuffer::ZERO;
	if (!FileUtils::getInstance()->isFileExist(filename))	return AEByteBuffer::ZERO;

	Data data = FileUtils::getInstance()->getDataFromFile(FileUtils::getInstance()->fullPathForFilename(filename));

	if (data.isNull())
	{
		return AEByteBuffer::ZERO;
	}

	return AEByteBuffer((const char*)data.getBytes(),0, data.getSize());


// 	unsigned char* buffer = nullptr;
// 	size_t size = 0;
// 	size_t readsize;
// 	const char* mode = forString ? "rt" : "rb";
// 
// 	do
// 	{
// 		// Read the file from hardware
// 		std::string fullPath = filename;//AEFileUtils::getInstance()->fullPathForFilename(filename);
// 		//AEFileUtils::getInstance()->_fix_luapath(fullPath);
// 		FILE *fp = fopen(fullPath.c_str(), mode);
// 		AEBreak_if(!fp);
// 		fseek(fp, 0, SEEK_END);
// 		size = ftell(fp);
// 		fseek(fp, 0, SEEK_SET);
// 
// 		if (forString)
// 		{
// 			buffer = (unsigned char*)malloc(sizeof(unsigned char)* (size + 1));
// 			buffer[size] = '\0';
// 		}
// 		else
// 		{
// 			buffer = (unsigned char*)malloc(sizeof(unsigned char)* size);
// 		}
// 
// 		readsize = fread(buffer, sizeof(unsigned char), size, fp);
// 		fclose(fp);
// 
// 		if (forString && readsize < size)
// 		{
// 			buffer[readsize] = '\0';
// 		}
// 
// 	} while (0);
// 
// 	if (nullptr != buffer && 0 < readsize) return AEByteBuffer((const char*)buffer, (uint32)readsize);
// 
// 	AELOGINFO("Get data from file %s failed", filename.c_str());
// 
// 	return AEByteBuffer::ZERO;
}

void AESpineColliderManager::readSpineColliderFromFile(std::string path)
{
	std::string sPath = path;
	std::string keyString = "collider/";
	std::string destString = "";
	std::string::size_type pos = sPath.find(keyString.c_str());
	sPath.replace(pos, keyString.length(), destString);

	keyString = ".collider";
	destString = ".json";
	pos = sPath.find(keyString.c_str());
	sPath.replace(pos, keyString.length(), destString);

	auto it = spineColliderNameMap.find(sPath);
	if (it != spineColliderNameMap.end())
	{
		return;
	}

	AEByteBuffer buffer = getData(path, false);
	if (&buffer == &AEByteBuffer::ZERO)
	{
		return;
	}
	AESpineColliderData* data = new AESpineColliderData();
	data->parseBytes(buffer);
	spineColliderMap[data->getSpineName()] = data;
	
	spineColliderNameMap[sPath] = true;
}

void AESpineColliderManager::getVertices(string spineName, string animationName, float32 time, uint32*  vertextCount, float32* vertex)
{
	if (spineColliderMap[spineName] == nullptr)
	{
		*vertextCount = 8;
		vertex = ColliderData::ColiderZERO;
		cout << "AESpineColliderManager::getVertices no spine :" << spineName.c_str() << endl;
		return;
	}
	spineColliderMap[spineName]->getVertices(animationName, time, vertextCount, vertex);
}

NS_AE_END