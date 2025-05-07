/************************************************************************** 
    *  @Copyright (c) 2017, A-one Soft, All rights reserved. 
 
    *  @file			: AEColliderDraw.h 
    *  @version	: ver 1.0.0 
 
    *  @author	: andy-zhao 
    *  @date		: 2017/3/30 
    *  @brief		: 碰撞体 
**************************************************************************/
#ifndef _AEColliderDraw_H_
#define _AEColliderDraw_H_

#include "platform/AEStdC.h"
#include <Box2D/Box2D.h>

NS_AE_BEGIN

class AECollision;
class AEColliderRender : public Node
{
public:
	AEColliderRender();
	AEColliderRender(AECollision* pCollision);
	virtual ~AEColliderRender();

	/**
		创建。
	*/
	static AEColliderRender* create(AECollision* pCollision);

	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;

protected:
	void onDraw(const cocos2d::Mat4& transform, uint32_t flags);
	cocos2d::CustomCommand _customCmd;

private:
	// 碰撞管理
	AECollision* m_pCollision;

};

class AEColliderDraw : public b2Draw
{
public:
	AEColliderDraw();
	AEColliderDraw(float32 ratio);
	virtual ~AEColliderDraw();

	void initShader(void);

	virtual void DrawPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color);

	virtual void DrawSolidPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color);

	virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);

	virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

	virtual void DrawTransform(const b2Transform& xf);

	virtual void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);

	virtual void DrawString(int x, int y, const char* string, ...);

	virtual void DrawAABB(b2AABB* aabb, const b2Color& color);

private:
	float32 m_fRatio;
	cocos2d::GLProgram* m_pShaderProgram;
	GLint  m_iColorLocation;

};

NS_AE_END

#endif // _AEColliderDraw_H_