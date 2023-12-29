//
// Created by ianpo on 29/12/2023.
//

#include "Renderer/Poly.hpp"


Poly::Poly(const std::vector<Vec2Int>& points) : m_Points(points)
{
}

Poly::Poly(const std::initializer_list<Vec2Int>& points) : m_Points(points)
{
}

bool Poly::IsValid() const {
	return m_Points.size() >= 3;
}

int Poly::GetPointCount() const {
	return m_Points.size();
}

std::vector<Vec2Int>::iterator Poly::begin() {
	return m_Points.begin();
}

std::vector<Vec2Int>::iterator Poly::end() {
	return m_Points.end();
}

std::vector<Vec2Int>::const_iterator Poly::cbegin() const {
	return m_Points.cbegin();
}

std::vector<Vec2Int>::const_iterator Poly::cend() const {
	return m_Points.cend();
}

const Vec2Int &Poly::operator[](int index) const {
	return m_Points[index];
}

Vec2Int &Poly::operator[](int index) {
	return m_Points[index];
}

void Poly::Draw(const Mat4 &viewProjMatrix) const {

	glBegin(GL_LINE_LOOP);

	for (auto& pos : m_Points)
	{
		glColor4fv(&m_Color[0]);
		Vec3 vertexPos = viewProjMatrix * Vec4(pos.x, pos.y, 0.0, 1.0);
		glVertex3fv(&vertexPos[0]);
	}

	glEnd();

}

Poly::~Poly() {

}
