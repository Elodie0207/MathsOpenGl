//
// Created by ianpo on 29/12/2023.
//

#pragma once

#include <vector>
#include "Core.hpp"
#include "Math.hpp"

class DrawObject {
	enum class RenderMode
	{
		TriangleFan,
		Triangle,
		Loop,
	};
public:
	DrawObject() = default;
	DrawObject(const std::vector<Vec2>& points);
	DrawObject(const std::initializer_list<Vec2>& points);
	~DrawObject();
	void Draw(const Mat4& ViewProjMatrix) const;
public:
	RenderMode m_RenderMode = RenderMode::TriangleFan;
	Vec4 m_Color;
	std::vector<Vec2> m_Points;
};
