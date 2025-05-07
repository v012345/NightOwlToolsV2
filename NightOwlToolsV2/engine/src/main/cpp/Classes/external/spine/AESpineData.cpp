#include "external/spine/AESpineData.h"

extern int g_iAtlasAsync;

NS_AE_BEGIN

std::map<std::string, AESpineDataPtr> AESpineData::s_mSpineData;

AESpineDataPtr AESpineData::createSpineData(const std::string& skeletonDataFile, AESkeletonType type, const std::string& atlasFile, float32 scale, bool async)
{
	std::map<std::string, AESpineDataPtr>::iterator iterData = s_mSpineData.find(skeletonDataFile);
	if (s_mSpineData.end() != iterData)
	{
		AESpineDataPtr ptrSpineData = iterData->second;
		ptrSpineData->retain();
		return ptrSpineData;
	}

	g_iAtlasAsync = (int)async;
	//AELOGINFO("Create Atlas %s!", atlasFile.c_str());
	spAtlas* atlas = spAtlas_createFromFile(atlasFile.c_str(), 0);
	AEASSERT(atlas, "Error reading atlas file.");

	spAttachmentLoader* attachmentLoader = SUPER(Cocos2dAttachmentLoader_create(atlas));

	spSkeletonData* skeletonData = nullptr;
	if (AESkeletonType::ST_JSON == type)
	{
		spSkeletonJson* json = spSkeletonJson_createWithLoader(attachmentLoader);
		json->scale = scale;
		skeletonData = spSkeletonJson_readSkeletonDataFile(json, skeletonDataFile.c_str());
		AEASSERT(skeletonData, json->error ? json->error : "Error reading skeleton data file.");
		spSkeletonJson_dispose(json);
	}
	else if (AESkeletonType::ST_BINARY == type)
	{
		spSkeletonBinary* binary = spSkeletonBinary_createWithLoader(attachmentLoader);
		binary->scale = scale;
		skeletonData = spSkeletonBinary_readSkeletonDataFile(binary, skeletonDataFile.c_str());
		AEASSERT(skeletonData, binary->error ? binary->error : "Error reading skeleton data file.");
		spSkeletonBinary_dispose(binary);
	}

	AESpineDataPtr ptrSpineData = AESpineDataPtr(new AESpineData());
	ptrSpineData->m_pSkeletonData = skeletonData;
	ptrSpineData->m_pAtlas = atlas;
	ptrSpineData->m_pAttachmentLoader = attachmentLoader;
	ptrSpineData->m_sSkeletonDataFile = skeletonDataFile;
	s_mSpineData.insert(std::map<std::string, AESpineDataPtr>::value_type(skeletonDataFile, ptrSpineData));

	return ptrSpineData;
}

void AESpineData::destroySpineData(const std::string& skeletonDataFile)
{
	std::map<std::string, AESpineDataPtr>::iterator iterData = s_mSpineData.find(skeletonDataFile);
	if (s_mSpineData.end() != iterData)
	{
		AESpineDataPtr ptrSpineData = iterData->second;
		ptrSpineData->release();
	}
}

AESpineData::AESpineData()
: m_pSkeletonData(nullptr)
, m_pAtlas(nullptr)
, m_pAttachmentLoader(nullptr)
, m_iRefCount(1)
{

}

AESpineData::~AESpineData()
{
	//AELOGINFO("AESpineData Destruct %s !", m_sSkeletonDataFile.c_str());
	if (m_pSkeletonData) spSkeletonData_dispose(m_pSkeletonData);
	if (m_pAtlas) spAtlas_dispose(m_pAtlas);
	if (m_pAttachmentLoader) spAttachmentLoader_dispose(m_pAttachmentLoader);
}

void AESpineData::retain()
{
	++m_iRefCount;
}

void AESpineData::release()
{
	--m_iRefCount;

	if (0 == m_iRefCount)
	{
		s_mSpineData.erase(m_sSkeletonDataFile);
	}
}

NS_AE_END
