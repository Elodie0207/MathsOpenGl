//
// Created by ianpo on 29/12/2023.
//

#include "Renderer/DrawObject.hpp"

DrawObject::DrawObject(const std::vector<Vec2>& points) : m_Color(1, 1, 1, 1), m_Points(points)
{
}
DrawObject::DrawObject(const std::initializer_list<Vec2>& points) : m_Color(1, 1, 1, 1), m_Points(points)
{}

DrawObject::~DrawObject() {

}

void DrawObject::Draw(const Mat4& ViewProjMatrix) const
{
	switch (m_RenderMode) {
		case RenderMode::Triangle:
			glBegin(GL_TRIANGLES);
			break;
		case RenderMode::TriangleFan:
			glBegin(GL_TRIANGLE_FAN);
			break;
		case RenderMode::Loop:
			glBegin(GL_LINE_LOOP);
			break;
	}

	for (auto& pos : m_Points)
	{
		glColor4fv(&m_Color[0]);
		Vec3 vertexPos = ViewProjMatrix * Vec4(pos.x, pos.y, 0.0, 1.0);
		glVertex3fv(&vertexPos[0]);
	}

	glEnd();
}
