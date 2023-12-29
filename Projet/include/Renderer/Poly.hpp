//
// Created by ianpo on 29/12/2023.
//

#pragma once

#include "Core.hpp"
#include "Math.hpp"

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

	std::vector<Vec2Int>::iterator begin();
	std::vector<Vec2Int>::iterator end();

	std::vector<Vec2Int>::const_iterator cbegin() const;
	std::vector<Vec2Int>::const_iterator cend() const;

	const Vec2Int& operator[](int index) const;
	Vec2Int& operator[](int index);

	Vec4 m_Color = Vec4(1.0f);

	std::vector<Vec2Int> m_Points;
};
