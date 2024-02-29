//
// Created by ianpo on 29/12/2023.
//

#include "Renderer/Poly.hpp"


Poly::Poly(const std::vector<Vec2Int>& points, Vec2 position) : m_Points(points.size())
{
	for (int i = 0; i < points.size(); ++i) {
		m_Points[i] = Vec2(points[i]) + position;
	}
}

Poly::Poly(const std::initializer_list<Vec2Int>& points, Vec2 position) : m_Points()
{
	m_Points.reserve(points.size());
	for(auto& p : points)
	{
		m_Points.emplace_back(Vec2(p) + position);
	}
}

Poly::Poly(const std::vector<Vec2>& points, Vec2 position) : m_Points(points)
{
	for(auto& p : m_Points)
	{
		p += position;
	}
}

Poly::Poly(const std::initializer_list<Vec2>& points, Vec2 position) : m_Points(points)
{
	for(auto& p : m_Points)
	{
		p += position;
	}
}

bool Poly::IsValid() const {
	return m_Points.size() > 2;
}

int Poly::GetPointCount() const {
	return m_Points.size();
}

std::vector<Vec2>::iterator Poly::begin() {
	return m_Points.begin();
}

std::vector<Vec2>::iterator Poly::end() {
	return m_Points.end();
}

std::vector<Vec2>::const_iterator Poly::cbegin() const {
	return m_Points.cbegin();
}

std::vector<Vec2>::const_iterator Poly::cend() const {
	return m_Points.cend();
}

const Vec2 &Poly::operator[](int index) const {
	return m_Points[index];
}

Vec2 &Poly::operator[](int index) {
	return m_Points[index];
}

void Poly::Draw(const Mat4 &viewProjMatrix) const {
	if(GetPointCount() == 0) return;

	else if(GetPointCount() == 1)
	{
		glBegin(GL_POINTS);
		glColor4fv(&m_Color[0]);
		auto pos = m_Points[0];
		Vec3 vertexPos = viewProjMatrix * Vec4(pos.x, pos.y, 0.0, 1.0);
		glVertex3fv(&vertexPos[0]);
		glEnd();
	}
	else
	{	glBegin(GL_LINE_LOOP);

		for (auto& pos : m_Points)
		{
			glColor4fv(&m_Color[0]);
			Vec3 vertexPos = viewProjMatrix * Vec4(pos.x, pos.y, 0.0, 1.0);
			glVertex3fv(&vertexPos[0]);
		}

		glEnd();
	}



}

std::vector<Point> Poly::GetLoopPoints() const
{
	std::vector<Point> points;
	points.reserve(m_Points.size() + 1);
	for(const auto& p : m_Points)
	{
		points.emplace_back((int)p.x, (int)p.y);
	}
	points.emplace_back(m_Points[0].x, m_Points[0].y);
	return points;
}

std::vector<Point> Poly::GetPoints() const
{
	std::vector<Point> points;
	points.reserve(m_Points.size());
	for(const auto& p : m_Points)
	{
		points.emplace_back((int)p.x, (int)p.y);
	}
	return points;
}


Poly::~Poly() {

}

AABB2D Poly::GetBoundBox() const {
	return AABB2D::ComputeBoundingBox(m_Points);
}
