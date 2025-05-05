#ifndef _AESpineCollider_H_
#define _AESpineCollider_H_

#include <iostream>
#include "platform/AEPlatformDefine.h"
#include "common/AEByteBuffer.h"
using namespace std;

NS_AE_BEGIN

class ColliderData
{
public:
	ColliderData();
	~ColliderData();
	void setData(float32 time, uint32  vertextCount, float32 vertex[]);
	void getData(float32* time, uint32*  vertextCount, float32* vertex);
	float32 getTime();

	uint32 calculateLength();
	void toBytes(AEByteBuffer& buffer);
	void parseBytes(AEByteBuffer& buffer);
private:
	uint32  _vertextCount;
	float32 _vertex[8];
	float32 _time;

public:
	static float32 ColiderZERO[8];
};

class SpineAnimationColliderData
{
public:
	SpineAnimationColliderData();
	~SpineAnimationColliderData();

	void setAnimationName(string name);
	string getAnimaitonName();
	void addColliderData(ColliderData* data);

	vector<ColliderData*>* getColliderDataVector();
	void getData(float32 time, uint32*  vertextCount, float32* vertex);


	uint32 calculateLength();
	void toBytes(AEByteBuffer& buffer);
	void parseBytes(AEByteBuffer& buffer);
private:
	string _AnimationName;
	vector<ColliderData*> * colliderDataVector;
};



class AESpineColliderData
{
public:
	AESpineColliderData();
	~AESpineColliderData();
	void setSpineName(string name);
	string getSpineName();

	void addAnimationColliderMapData(string animationName, SpineAnimationColliderData* aniData);
	SpineAnimationColliderData* getSpineColliderData(string animationName);

	void getVertices(string animationName, float32 time, uint32*  vertextCount, float32* vertex);

	uint32 calculateLength();
	void toBytes(AEByteBuffer& buffer);
	void parseBytes(AEByteBuffer& buffer);
private:
	string _SpineName;
	map<string, SpineAnimationColliderData*> _AnimaitonColliderMapData;
};
NS_AE_END
#endif // _AESpineCollider_H_