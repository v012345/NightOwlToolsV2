#ifndef _AESpineColliderManager_H_
#define _AESpineColliderManager_H_

#include <iostream>
#include "platform/AEStdC.h"
#include "external/spine/AESpineColliderData.h"
#include "common/AEByteBuffer.h"

NS_AE_BEGIN

class AESpineColliderManager
{
private:
	AESpineColliderManager();
	~AESpineColliderManager();
public:


	static AESpineColliderManager* getInstance();
	static void destroyInstance();

	bool writeSpineColliderToFile(AESpineColliderData* data, std::string fileNameNoSuffix, std::string destPath);
	void readSpineColliderFromFile(std::string path);
	void getVertices(string spineName, string animationName, float32 time, uint32*  vertextCount, float32* vertex);

private:

	AEByteBuffer getData(const std::string filename, bool forString);



	static AESpineColliderManager* _pSpineColliderManager;

	std::map<std::string, AESpineColliderData*> spineColliderMap;
	std::map<std::string, bool> spineColliderNameMap;
};

NS_AE_END

#endif // _AESpineColliderManager_H_