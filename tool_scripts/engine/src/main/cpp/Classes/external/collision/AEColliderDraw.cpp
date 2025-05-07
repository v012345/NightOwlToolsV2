#include "external/collision/AEColliderDraw.h"
#include "external/collision/AECollision.h"

NS_AE_BEGIN

AEColliderRender::AEColliderRender()
{

}

AEColliderRender::AEColliderRender(AECollision* pCollision)
{
	m_pCollision = pCollision;
}

AEColliderRender::~AEColliderRender()
{

}

AEColliderRender* AEColliderRender::create(AECollision* pCollision)
{
	return new AEColliderRender(pCollision);
}

void AEColliderRender::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
	Node::draw(renderer, transform, flags);

	_customCmd.init(_globalZOrder, transform, flags);
	_customCmd.func = CC_CALLBACK_0(AEColliderRender::onDraw, this, transform, flags);
	renderer->addCommand(&_customCmd);
}

void AEColliderRender::onDraw(const cocos2d::Mat4& transform, uint32_t flags)
{
	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when seting matrix stack");
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);

	GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);
	if (m_pCollision)
	{
		m_pCollision->getB2World()->DrawDebugData();
	}
	CHECK_GL_ERROR_DEBUG();

	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

AEColliderDraw::AEColliderDraw()
: m_fRatio(1.0f)
{
	this->initShader();
}

AEColliderDraw::AEColliderDraw(float32 ratio)
: m_fRatio(ratio)
{
	this->initShader();
}

AEColliderDraw::~AEColliderDraw()
{

}

void AEColliderDraw::initShader(void)
{
	m_pShaderProgram = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_U_COLOR);
	m_iColorLocation = glGetUniformLocation(m_pShaderProgram->getProgram(), "u_color");
}

void AEColliderDraw::DrawPolygon(const b2Vec2* old_vertices, int vertexCount, const b2Color& color)
{
	m_pShaderProgram->use();
	m_pShaderProgram->setUniformsForBuiltins();

	b2Vec2* vertices = new b2Vec2[vertexCount];
	for (int i = 0; i<vertexCount; i++)
	{
		vertices[i] = old_vertices[i];
		vertices[i] *= m_fRatio;
	}

	m_pShaderProgram->setUniformLocationWith4f(m_iColorLocation, color.r, color.g, color.b, 1);

	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glDrawArrays(GL_LINE_LOOP, 0, vertexCount);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, vertexCount);

	CHECK_GL_ERROR_DEBUG();

	delete[] vertices;
}

void AEColliderDraw::DrawSolidPolygon(const b2Vec2* old_vertices, int vertexCount, const b2Color& color)
{
	m_pShaderProgram->use();
	m_pShaderProgram->setUniformsForBuiltins();

	b2Vec2* vertices = new b2Vec2[vertexCount];
	for (int i = 0; i<vertexCount; i++)
	{
		vertices[i] = old_vertices[i];
		vertices[i] *= m_fRatio;
	}

	m_pShaderProgram->setUniformLocationWith4f(m_iColorLocation, color.r*0.5f, color.g*0.5f, color.b*0.5f, 0.5f);

	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);

	glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);

	m_pShaderProgram->setUniformLocationWith4f(m_iColorLocation, color.r, color.g, color.b, 1);
	glDrawArrays(GL_LINE_LOOP, 0, vertexCount);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(2, vertexCount * 2);

	CHECK_GL_ERROR_DEBUG();

	delete[] vertices;
}

void AEColliderDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	m_pShaderProgram->use();
	m_pShaderProgram->setUniformsForBuiltins();

	const float32 k_segments = 16.0f;
	int vertexCount = 16;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;

	GLfloat* glVertices = new (std::nothrow) GLfloat[vertexCount * 2];
	for (int i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertices[i * 2] = v.x * m_fRatio;
		glVertices[i * 2 + 1] = v.y * m_fRatio;
		theta += k_increment;
	}

	m_pShaderProgram->setUniformLocationWith4f(m_iColorLocation, color.r, color.g, color.b, 1);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, glVertices);

	glDrawArrays(GL_LINE_LOOP, 0, vertexCount);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, vertexCount);

	CHECK_GL_ERROR_DEBUG();

	delete[] glVertices;
}

void AEColliderDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	m_pShaderProgram->use();
	m_pShaderProgram->setUniformsForBuiltins();

	const float32 k_segments = 16.0f;
	int vertexCount = 16;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;

	GLfloat* glVertices = new (std::nothrow) GLfloat[vertexCount * 2];
	for (int i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertices[i * 2] = v.x * m_fRatio;
		glVertices[i * 2 + 1] = v.y * m_fRatio;
		theta += k_increment;
	}

	m_pShaderProgram->setUniformLocationWith4f(m_iColorLocation, color.r*0.5f, color.g*0.5f, color.b*0.5f, 0.5f);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, glVertices);
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);

	m_pShaderProgram->setUniformLocationWith4f(m_iColorLocation, color.r, color.g, color.b, 1);
	glDrawArrays(GL_LINE_LOOP, 0, vertexCount);

	// Draw the axis line
	DrawSegment(center, center + radius*axis, color);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(2, vertexCount * 2);

	CHECK_GL_ERROR_DEBUG();

	delete[] glVertices;
}

void AEColliderDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	m_pShaderProgram->use();
	m_pShaderProgram->setUniformsForBuiltins();

	m_pShaderProgram->setUniformLocationWith4f(m_iColorLocation, color.r, color.g, color.b, 1);

	GLfloat glVertices[] =
	{
		p1.x * m_fRatio, p1.y * m_fRatio,
		p2.x * m_fRatio, p2.y * m_fRatio
	};
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, glVertices);

	glDrawArrays(GL_LINES, 0, 2);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 2);

	CHECK_GL_ERROR_DEBUG();
}

void AEColliderDraw::DrawTransform(const b2Transform& xf)
{
	b2Vec2 p1 = xf.p, p2;
	const float32 k_axisScale = 0.4f;
	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	DrawSegment(p1, p2, b2Color(1, 0, 0));

	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	DrawSegment(p1, p2, b2Color(0, 1, 0));
}

void AEColliderDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	m_pShaderProgram->use();
	m_pShaderProgram->setUniformsForBuiltins();

	m_pShaderProgram->setUniformLocationWith4f(m_iColorLocation, color.r, color.g, color.b, 1);

	// glPointSize(size);
	GLfloat glVertices[] = 
	{
		p.x * m_fRatio, p.y * m_fRatio
	};

	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, glVertices);

	glDrawArrays(GL_POINTS, 0, 1);
	// glPointSize(1.0f);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 1);

	CHECK_GL_ERROR_DEBUG();
}

void AEColliderDraw::DrawString(int x, int y, const char *string, ...)
{
	// NSLog(@"DrawString: unsupported: %s", string);
	// printf(string);
	/* Unsupported as yet. Could replace with bitmap font renderer at a later date */
}

void AEColliderDraw::DrawAABB(b2AABB* aabb, const b2Color& color)
{
	m_pShaderProgram->use();
	m_pShaderProgram->setUniformsForBuiltins();

	m_pShaderProgram->setUniformLocationWith4f(m_iColorLocation, color.r, color.g, color.b, 1);

	GLfloat glVertices[] = 
	{
		aabb->lowerBound.x * m_fRatio, aabb->lowerBound.y * m_fRatio,
		aabb->upperBound.x * m_fRatio, aabb->lowerBound.y * m_fRatio,
		aabb->upperBound.x * m_fRatio, aabb->upperBound.y * m_fRatio,
		aabb->lowerBound.x * m_fRatio, aabb->upperBound.y * m_fRatio
	};

	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, glVertices);
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 4);

	CHECK_GL_ERROR_DEBUG();
}

NS_AE_END
