#include "external/spine/AESpine.h"
#include <spine/AttachmentVertices.h>
#include <spine/Cocos2dAttachmentLoader.h>

extern int g_iAtlasAsync;

NS_AE_BEGIN

float32 AESpine::s_fScaleTime = 1.0f;

spAtlas* AESpine::appendAtlas(const std::vector<std::string>& vec)
{
	int i = 0;
	g_iAtlasAsync = 1;
	spAtlas* pAtlas = spAtlas_createFromFile(vec.at(i).c_str(), 0);

	do
	{
		++i;
		if (i >= vec.size()) break;
		g_iAtlasAsync = 1;
		spAtlas_append(pAtlas, vec.at(i).c_str());
	}
	while (true);

	return pAtlas;
}

AESpine::AESpine()
: m_bAsync(false)
{
	init();
}

AESpine::AESpine(bool async)
: m_bAsync(async)
{
	init();

	if (async)
	{
		m_bInitComplete = false;
	}
}

AESpine::~AESpine()
{
	if (nullptr != m_ptrSpineData)
	{
		m_ptrSpineData->release();
	}
}

bool AESpine::init()
{
	m_bInitComplete = true;
	m_fDuration = 0;
	m_iTrackIndex = 0;
	m_bLoop = false;

	m_bPause = false;
	m_bCache = false;

	_isETC = false;
	_filePath = "";
	
	return true;
}

void AESpine::onEnter()
{
	scheduleUpdate();
	Node::onEnter();
}

void AESpine::onExit()
{
	Node::onExit();
	unscheduleUpdate();
}

void AESpine::update(float32 delta)
{
	if (m_bPause || !m_bInitComplete) return;
	SkeletonAnimation::update(delta * s_fScaleTime);
}

void AESpine::initialize()
{
	SkeletonAnimation::initialize();

	if (!m_bInitComplete)
	{
		m_bInitComplete = true;
		if (m_sAnimation != "")	{
			spTrackEntry* entry = SkeletonAnimation::setAnimation(m_iTrackIndex, m_sAnimation, m_bLoop);
			m_fDuration = entry->endTime;
		}
	}
}

void AESpine::draw(Renderer* renderer, const Mat4& transform, uint32_t transformFlags)
{
	if (!m_bInitComplete) return;

	SkeletonRenderer::draw(renderer, transform, transformFlags);
}

void AESpine::initSpineData(AESpineDataPtr spineData)
{
	if (strstr(spineData->m_pAtlas->pages->name, ".pkm"))
		_isETC = true;
	
	initWithData(spineData->m_pSkeletonData);
	m_sSkeletonDataFile = spineData->m_sSkeletonDataFile;
	m_ptrSpineData = spineData;
	_filePath = m_sSkeletonDataFile;
}

void AESpine::replaceAtlas(spAtlas* pAtlas)
{	
	AEASSERT(m_bInitComplete, "Error Async Init!");

	for (size_t i = 0; i < _skeleton->data->skinsCount; i++)
	{
		spSkin* skin = _skeleton->data->skins[i];
		const _Entry* entry = SUB_CAST(_spSkin, skin)->entries;
		while (entry)
		{
			switch (entry->attachment->type)
			{
				case SP_ATTACHMENT_REGION:
				{

					spRegionAttachment* attachment = (spRegionAttachment*)entry->attachment;
					spAtlasRegion* region = spAtlas_findRegion(pAtlas, ((spAtlasRegion*)(attachment->region))->name);
					((AttachmentVertices*)attachment->rendererObject)->_texture = (Texture2D*)region->page->rendererObject;
					attachment->region = region;
					spRegionAttachment_setUVs(attachment, region->u, region->v, region->u2, region->v2, region->rotate);
					attachment->regionOffsetX = region->offsetX;
					attachment->regionOffsetY = region->offsetY;
					attachment->regionWidth = region->width;
					attachment->regionHeight = region->height;
					attachment->regionOriginalWidth = region->originalWidth;
					attachment->regionOriginalHeight = region->originalHeight;
					spRegionAttachment_updateOffset(attachment);
					V3F_C4B_T2F* vertices = ((AttachmentVertices*)attachment->rendererObject)->_triangles->verts;
					for (int i = 0, ii = 0; i < 4; ++i, ii += 2) {
						vertices[i].texCoords.u = attachment->uvs[ii];
						vertices[i].texCoords.v = attachment->uvs[ii + 1];
					}
					break;
				}
			case SP_ATTACHMENT_MESH:
				{
					spMeshAttachment* attachment = (spMeshAttachment*)entry->attachment;
					spAtlasRegion* region = spAtlas_findRegion(pAtlas, ((spAtlasRegion*)(attachment->region))->name);
					((AttachmentVertices*)(attachment->rendererObject))->_texture = (Texture2D*)region->page->rendererObject;
					attachment->region = region;
					attachment->regionU = region->u;
					attachment->regionV = region->v;
					attachment->regionU2 = region->u2;
					attachment->regionV2 = region->v2;
					attachment->regionRotate = region->rotate;
					attachment->regionOffsetX = region->offsetX;
					attachment->regionOffsetY = region->offsetY;
					attachment->regionWidth = region->width;
					attachment->regionHeight = region->height;
					attachment->regionOriginalWidth = region->originalWidth;
					attachment->regionOriginalHeight = region->originalHeight;
					spMeshAttachment_updateUVs(attachment);
					V3F_C4B_T2F* vertices = ((AttachmentVertices*)attachment->rendererObject)->_triangles->verts;
					for (int i = 0, ii = 0, nn = attachment->super.worldVerticesLength; ii < nn; ++i, ii += 2) {
						vertices[i].texCoords.u = attachment->uvs[ii];
						vertices[i].texCoords.v = attachment->uvs[ii + 1];
					}
					break;
				}
			}
			entry = entry->next;
		}
	}

	spAtlas_dispose(_atlas);
	_atlas = pAtlas;
}

void AESpine::replaceSkinForSlotData(AESpine* spine, const char* skinName, const char* attachmentName)
{
	AEASSERT(m_bInitComplete, "Error Async Init!");

	if (!spine || !skinName) {
		if (!spine && attachmentName)
		{
			 spSkin* skin = _skeleton->data->defaultSkin;
			 _Entry* entry = SUB_CAST(_spSkin, skin)->entries;
			while (entry) {
				if (strcmp(entry->name, attachmentName) == 0) {
					entry->newAttachment = NULL;
					break;
				}
				entry = entry->next;
			}

		}
		return;
	}

	spSkin* skin = spSkeletonData_findSkin(spine->getSkeleton()->data, skinName);
	if (!skin) return;

	spSkin_replaceAttachment(_skeleton->data->defaultSkin, attachmentName, skin);
}

spTrackEntry* AESpine::setAnimation(int32 trackIndex, const std::string& name, bool loop)
{
	if (m_bInitComplete)
	{
		spTrackEntry* entry = SkeletonAnimation::setAnimation(trackIndex, name, loop);
		return entry;
	}
	else
	{
		m_iTrackIndex = trackIndex;
		m_sAnimation = name;
		m_bLoop = loop;
	}

	return nullptr;
}

float32 AESpine::changeAnimation(int32 trackIndex, const std::string& name, bool loop)
{
	if (m_bInitComplete)
	{
		setToSetupPose();
		spTrackEntry* entry = SkeletonAnimation::setAnimation(trackIndex, name, loop);
		return entry->endTime;
	}
	else
	{
		m_iTrackIndex = trackIndex;
		m_sAnimation = name;
		m_bLoop = loop;
	}

	return 0;
}

void AESpine::getVertices(float32* vertices, uint32* verticesCount, std::string name)
{
	if (m_bPause || !m_bInitComplete) return;

	spSlot* slot = findSlot(name);
	if (slot && slot->attachment && 	SP_ATTACHMENT_BOUNDING_BOX == slot->attachment->type)
	{
		spBoundingBoxAttachment* attachment = (spBoundingBoxAttachment*)(slot->attachment);
		*verticesCount = ((spVertexAttachment*)attachment)->verticesCount;
		spBoundingBoxAttachment_computeWorldVertices(attachment, slot, vertices);

		for (uint32 i = 0; i < *verticesCount; i++)
		{
			uint32 n = i << 1;
			vertices[n] *= getScaleX();
			vertices[n + 1] *= getScaleY();
		}
	}
}

Rect AESpine::getBoundingBox() const {
	if (!m_bInitComplete)
		return Rect(0, 0, 0, 0);
	return SkeletonAnimation::getBoundingBox();
}

void AESpine::recoveryGLProgramState()
{
	if (_isETC)
		setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_ETC1AS_POSITION_TEXTURE_COLOR_NO_MVP));
	else
		setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
}

NS_AE_END
