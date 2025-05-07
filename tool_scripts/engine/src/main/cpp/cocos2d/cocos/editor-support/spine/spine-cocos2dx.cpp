/******************************************************************************
 * Spine Runtimes Software License
 * Version 2.3
 * 
 * Copyright (c) 2013-2015, Esoteric Software
 * All rights reserved.
 * 
 * You are granted a perpetual, non-exclusive, non-sublicensable and
 * non-transferable license to use, install, execute and perform the Spine
 * Runtimes Software (the "Software") and derivative works solely for personal
 * or internal use. Without the written permission of Esoteric Software (see
 * Section 2 of the Spine Software License Agreement), you may not (a) modify,
 * translate, adapt or otherwise create derivative works, improvements of the
 * Software or develop new applications using the Software or (b) remove,
 * delete, alter or obscure any trademarks or any copyright, trademark, patent
 * or other intellectual property or proprietary rights notices on or in the
 * Software, including any copy thereof. Redistributions in binary or source
 * form must include this license and terms.
 * 
 * THIS SOFTWARE IS PROVIDED BY ESOTERIC SOFTWARE "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL ESOTERIC SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include <spine/spine-cocos2dx.h>
#include <spine/extension.h>

USING_NS_CC;

int g_iAtlasAsync = 0;

GLuint wrap (spAtlasWrap wrap) {
	return wrap == SP_ATLAS_CLAMPTOEDGE ? GL_CLAMP_TO_EDGE : GL_REPEAT;
}

GLuint filter (spAtlasFilter filter) {
	switch (filter) {
	case SP_ATLAS_UNKNOWN_FILTER:
		break;
	case SP_ATLAS_NEAREST:
		return GL_NEAREST;
	case SP_ATLAS_LINEAR:
		return GL_LINEAR;
	case SP_ATLAS_MIPMAP:
		return GL_LINEAR_MIPMAP_LINEAR;
	case SP_ATLAS_MIPMAP_NEAREST_NEAREST:
		return GL_NEAREST_MIPMAP_NEAREST;
	case SP_ATLAS_MIPMAP_LINEAR_NEAREST:
		return GL_LINEAR_MIPMAP_NEAREST;
	case SP_ATLAS_MIPMAP_NEAREST_LINEAR:
		return GL_NEAREST_MIPMAP_LINEAR;
	case SP_ATLAS_MIPMAP_LINEAR_LINEAR:
		return GL_LINEAR_MIPMAP_LINEAR;
	}
	return GL_LINEAR;
}

void _spAtlasPage_createTexture (spAtlasPage* self, const char* path) {
	MALLOC_STR(self->path, path);

	// load texture
	const std::function<void(Texture2D*)>& callback = [self](Texture2D* texture) {
		CCASSERT(texture != nullptr, "Invalid image");
		texture->retain();

		if (SP_ATLAS_MIPMAP_LINEAR_LINEAR == self->minFilter && SP_ATLAS_MIPMAP_LINEAR_LINEAR == self->magFilter)
		{
			texture->generateMipmap();
		}

		Texture2D::TexParams textureParams = { filter(self->minFilter), filter(self->magFilter), wrap(self->uWrap), wrap(self->vWrap) };
		texture->setTexParameters(textureParams);

		self->rendererObject = texture;
		self->width = texture->getPixelsWide();
		self->height = texture->getPixelsHigh();
	};

	if (0 == g_iAtlasAsync) {
		Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(path);
		if (texture) callback(texture);
	}
	else {
		Director::getInstance()->getTextureCache()->addImageAsync(path, callback);
		g_iAtlasAsync = 0;
	}
}

void _spAtlasPage_disposeTexture (spAtlasPage* self) {
	Texture2D * texture = ((Texture2D*)self->rendererObject);
	if (nullptr == texture) {
		Director::getInstance()->getTextureCache()->unbindImageAsync(self->path);
	}
	else {
		texture->release();
	}
	FREE(self->path);
}

char* _spUtil_readFile (const char* path, int* length) {
	Data data;
	static std::map<std::string, Data> _s_chached_skeleton_json_data;
	auto iter = _s_chached_skeleton_json_data.find(path);
	if (iter == _s_chached_skeleton_json_data.end()){
		data = FileUtils::getInstance()->getDataFromFile(FileUtils::getInstance()->fullPathForFilename(path));
		_s_chached_skeleton_json_data.insert(std::make_pair(path, data));
		//CCLOG("cached [%s] , first using", path);
	}
	else{
		//CCLOG("already cached [%s] , use it right now", path);
		data = iter->second;
	}
	//*length = data.getSize();
	//char* bytes = MALLOC(char, *length);
	//memcpy(bytes, data.getBytes(), *length);

	static bool flag = false;
	if (flag == false) {
		//异步更新之后需要重新加载下动画配置文件，以免spine错乱
		auto _backToForegroundlistener = EventListenerCustom::create("REMOVE_UNUSED_SPINE_DATA", [&](EventCustom*) {
			auto iter = _s_chached_skeleton_json_data.begin();
			while (iter != _s_chached_skeleton_json_data.end()) {
				CCLOG("---->clear the cached spine's json_atlas_data [%s]", iter->first.c_str());
				iter++;
			}
			std::map<std::string, Data> tmp;
			_s_chached_skeleton_json_data.swap(tmp);
			CCLOG("clear cache spine json and altas REMOVE_UNUSED_SPINE_DATA");
		});
		Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backToForegroundlistener, -1);
		flag = true;
		CCLOG("add EventDispatcher REMOVE_UNUSED_SPINE_DATA to cache spine json and altas");
	}
	if (data.isNull()) return 0;

	// avoid buffer overflow (int is shorter than ssize_t in certain platforms)
#if COCOS2D_VERSION >= 0x00031200
	ssize_t tmpLen;
	char *ret = (char*)data.takeBuffer(&tmpLen);
	*length = static_cast<int>(tmpLen);
	return ret;
#else
    *length = static_cast<int>(data.getSize());
    char* bytes = MALLOC(char, *length);
    memcpy(bytes, data.getBytes(), *length);
    return bytes;
#endif
}
