#include "external/spine/AESpineColliderData.h"
#include <cmath>
NS_AE_BEGIN

float32 ColliderData::ColiderZERO[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
ColliderData::ColliderData() 
:_vertextCount(0),
_time(0)
{
	for (int i = 0; i < 8; i++)
	{
		_vertex[i] = 0;
	}
	
}

ColliderData::~ColliderData()
{

}

void ColliderData::setData(float32 time, uint32 vertextCount, float32* vertex)
{
	_time = time;
	_vertextCount = vertextCount;


	for (int i = 0; i < _vertextCount; i ++)
	{
		_vertex[i] = vertex[i];
	}

}

void ColliderData::getData(float32* time, uint32* vertextCount, float32* vertex)
{
	*time = _time;
	*vertextCount = _vertextCount;

	for (int i = 0; i < _vertextCount; i++)
	{
		vertex[i] = _vertex[i];
	}
	
}

float32 ColliderData::getTime()
{
	return _time;
}

uint32 ColliderData::calculateLength()
{
	uint32 length = 0;

	//_vertextCount
	length += AEByteBuffer::sizeOfUint32(_vertextCount);

	//_time
	length += AEByteBuffer::sizeOfFloat32(_time);

	//_vertex
	uint32 vertexLen = 8;
	length += AEByteBuffer::sizeOfUint32(vertexLen);

	for (int i = 0; i < vertexLen; i++)
	{
		length += AEByteBuffer::sizeOfFloat32(_vertex[i]);
	}

	return length;
}

void ColliderData::toBytes(AEByteBuffer& buffer)
{
	//_vertextCount
	buffer.writeUint32(_vertextCount);

	//_time
	buffer.writeFloat32(_time);

	//_vertex
	uint32 vertexLen = 8;
	buffer.writeUint32(vertexLen);
	for (int i = 0; i < vertexLen; i++)
	{
		buffer.writeFloat32(_vertex[i]);
	}




}


void ColliderData::parseBytes(AEByteBuffer& buffer)
{
	//_vertextCount
	_vertextCount = buffer.readUint32();

	//_time
	_time = buffer.readFloat32();
	//std::cout << "Time:" << _time << endl;

	//_vertex
	uint32 vertexLen = buffer.readUint32();
	for (int i = 0; i < vertexLen; i++)
	{
		_vertex[i] = buffer.readFloat32();
		//std::cout << "p[" << i << "] = " << _vertex[i] << endl;
	}

}
///////////////////////////SpineAnimationColliderData///////////////////////////////////////////////

SpineAnimationColliderData::SpineAnimationColliderData()
{
	colliderDataVector = new vector<ColliderData*>();
}
SpineAnimationColliderData::~SpineAnimationColliderData()
{
	delete colliderDataVector;
}

void SpineAnimationColliderData::setAnimationName(string name)
{
	_AnimationName = name;
}

string SpineAnimationColliderData::getAnimaitonName()
{
	return _AnimationName;
}

void SpineAnimationColliderData::addColliderData(ColliderData* data)
{
	colliderDataVector->push_back(data);
}

vector<ColliderData*>* SpineAnimationColliderData::getColliderDataVector()
{
	return colliderDataVector;
}

uint32 SpineAnimationColliderData::calculateLength()
{
	uint32 length = 0;

	//_AnimationName
	length += AEByteBuffer::sizeOfString(_AnimationName);

	//colliderDataVector
	uint32 vLen = colliderDataVector->size();

	length += AEByteBuffer::sizeOfUint32(vLen);

	for (int i = 0; i < vLen; i++)
	{
		length += colliderDataVector->at(i)->calculateLength();
	}
	return length;
}

void SpineAnimationColliderData::toBytes(AEByteBuffer& buffer)
{
	//_AnimationName
	buffer.writeString(_AnimationName);

	//colliderDataVector
	buffer.writeUint32(colliderDataVector->size());
	for (int i = 0; i < colliderDataVector->size(); i++)
	{
		colliderDataVector->at(i)->toBytes(buffer);
	}
}

void SpineAnimationColliderData::parseBytes(AEByteBuffer& buffer)
{
	//_AnimationName
	_AnimationName = buffer.readString();
	//std::cout << _AnimationName << endl;

	//colliderDataVector
	uint32 dataSize = buffer.readUint32();
	for (int i = 0; i < dataSize; i++)
	{
		ColliderData* data = new ColliderData();
		data->parseBytes(buffer);
		colliderDataVector->push_back(data);
	}
}

//////////////////////////////AESpineColliderData////////////////////////////////////////////

AESpineColliderData::AESpineColliderData()
{

}

AESpineColliderData::~AESpineColliderData()
{
	map<string, SpineAnimationColliderData*>::iterator iter = _AnimaitonColliderMapData.begin();
	while (iter != _AnimaitonColliderMapData.end()) {
		delete iter->second;
		iter++;
	}
	_AnimaitonColliderMapData.clear();
}

void AESpineColliderData::setSpineName(string name)
{
	_SpineName = name;
}
string AESpineColliderData::getSpineName()
{
	return _SpineName;
}

void AESpineColliderData::addAnimationColliderMapData(string animationName, SpineAnimationColliderData* aniData)
{
	_AnimaitonColliderMapData[animationName] = aniData;
}
SpineAnimationColliderData* AESpineColliderData::getSpineColliderData(string animationName)
{
	return  _AnimaitonColliderMapData[animationName];
}

void AESpineColliderData::toBytes(AEByteBuffer& buffer)
{
	//_SpineName
	buffer.writeString(_SpineName);

	//_AnimaitonColliderMapData
	buffer.writeUint32(_AnimaitonColliderMapData.size());
	map<string, SpineAnimationColliderData*>::iterator it = _AnimaitonColliderMapData.begin();
	while (it != _AnimaitonColliderMapData.end())
	{
		 it->second->toBytes(buffer);
		 it++;
	}
}

void AESpineColliderData::parseBytes(AEByteBuffer& buffer)
{
	//_SpineName
	_SpineName = buffer.readString();
	//std::cout << _SpineName << endl;

	//_AnimaitonColliderMapData
	uint32 mapSize = buffer.readUint32();
	for (int i = 0; i < mapSize; i++)
	{
		SpineAnimationColliderData* data = new(std::nothrow) SpineAnimationColliderData();
		data->parseBytes(buffer);
		_AnimaitonColliderMapData[data->getAnimaitonName()] = data;
	}
}

uint32 AESpineColliderData::calculateLength()
{
	uint32 length = 0;

	//_SpineName
	length += AEByteBuffer::sizeOfString(_SpineName);

	//_AnimaitonColliderMapData
	length += AEByteBuffer::sizeOfUint32(_AnimaitonColliderMapData.size());
	map<string, SpineAnimationColliderData*>::iterator it = _AnimaitonColliderMapData.begin();
	while (it != _AnimaitonColliderMapData.end())
	{
		length += it->second->calculateLength();
		it++;
	}
	return length;
}

void AESpineColliderData::getVertices(string animationName, float32 time, uint32*  vertextCount, float32* vertex)
{
	if (_AnimaitonColliderMapData[animationName] == nullptr)
	{
		//cout << "AESpineColliderData::getVertices spine:" << _SpineName.c_str() << " no animation :" << animationName .c_str() << endl;
		*vertextCount = 8;
		vertex = ColliderData::ColiderZERO;
		return;
	}

	vector<ColliderData*> * colliderDataVector = _AnimaitonColliderMapData[animationName]->getColliderDataVector();
	uint32 dataLen = colliderDataVector->size();
	if (dataLen == 0)
	{
		*vertextCount = 8;
		vertex = ColliderData::ColiderZERO;
		return;
	}

	int preIndex = -1;
	int curIndex = -1;
	static const float EPSINON = 0.00001;

	for (int i = 0; i < dataLen; i++)
	{
		float32 curTime = (colliderDataVector->at(i))->getTime();
		float32 dValue = time - curTime;
		if (fabs(dValue) <= EPSINON)
		{
			curIndex = i;
			break;
		}
		else if (dValue > EPSINON)
		{
			preIndex = i;
		}
		else if (dValue < -EPSINON)
		{
			curIndex = i;
			break;
		}	
	}

	AEASSERT(curIndex != -1,"AESpineColliderData::getData curIndex != -1")
	float32 time_ = 0;
	float32 curTime = (colliderDataVector->at(curIndex))->getTime();
	if (preIndex == -1 || fabs(curTime - time) < EPSINON)
	{
		(colliderDataVector->at(curIndex))->getData(&time_, vertextCount, vertex);
	}
	else{
		(colliderDataVector->at(preIndex))->getData(&time_, vertextCount, vertex);
	}
}

NS_AE_END