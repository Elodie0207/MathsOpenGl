//
// Created by ianpo on 29/12/2023.
//

#pragma once

#include "Core.hpp"
#include "Math.hpp"
#include "Helper.hpp"
#include "AABB2D.hpp"

class Poly
{
public:
	Poly() = default;
	Poly(const std::vector<Vec2Int>& points);
	Poly(const std::initializer_list<Vec2Int>& points);
	~Poly();

	bool IsValid() const;
	int GetPointCount() const;

	void Draw(const Mat4& viewProjMatrix) const;

	std::vector<Point> GetLoopPoints() const;
	std::vector<Point> GetPoints() const;
	AABB2D GetBoundBox() const;

	template<typename real>
	inline std::vector<glm::vec<2, real, glm::defaultp>> GetNormals(bool clockwise = true) const {
		typedef glm::vec<2, real, glm::defaultp> vec2;
		std::vector<vec2> normals(m_Points.size());
		auto pointCount = m_Points.size();
		for (int i = 0; i < pointCount; ++i) {
			auto p1 = m_Points[i];
			auto p2 = m_Points[(i + 1) % pointCount];
			auto p1top2 = p2 - p1;
			if (clockwise) {
				normals[i] = vec2(p1top2.y, -p1top2.x);
			} else {
				normals[i] = vec2(-p1top2.y, p1top2.x);
			}
		}
		return normals;
	}
	std::vector<Vec2Int>::iterator begin();
	std::vector<Vec2Int>::iterator end();

	std::vector<Vec2Int>::const_iterator cbegin() const;
	std::vector<Vec2Int>::const_iterator cend() const;

	const Vec2Int& operator[](int index) const;
	Vec2Int& operator[](int index);

	Vec4 m_Color = ColorHelper::Random();

	std::vector<Vec2Int> m_Points;
};
