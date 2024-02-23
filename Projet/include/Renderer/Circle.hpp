//
// Created by Sayama on 23/02/2024.
//

#pragma once

#include <vector>
#include "Core.hpp"
#include "Math.hpp"

class Circle {
public:
	enum CircleRenderMode
	{
		Fill,
		Outline
	};
	inline Circle() = default;
	inline ~Circle() = default;
	Circle(Vec2 center, float radius);
	Circle(Vec2 center, float radius, Color color);
	void Draw(const Mat4& ViewProjMatrix) const;
public:
	Color Color = {1,1,1,1};
	Vec2 Center = {0,0};
	float Radius = 100;
	int num_segments = 100;
	CircleRenderMode RenderMode = Outline;
};
