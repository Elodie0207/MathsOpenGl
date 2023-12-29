//
// Created by ianpo on 29/12/2023.
//

#pragma once

#include <memory>
#include <vector>
#include "Core.hpp"
#include "Renderer/Texture.hpp"


class Quad {
public:
	Quad(Vec2 position = Vec2(0.0), Vec2 size = Vec2(1.0));
	~Quad() = default;

	void Draw(const Mat4& ViewProjMatrix) const;

	Vec2 Position = Vec2(0.0);
	Vec2 Size = Vec2(1.0);
	std::shared_ptr<Texture> Texture = nullptr;
private:
	Mat4 GetTransformMatrix() const;
	std::array<Vec3, 4> vertices = {Vec3(-0.5, -0.5, 0.0), Vec3(+0.5, -0.5, 0.0), Vec3(+0.5, +0.5, 0.0), Vec3(-0.5, +0.5, 0.0)};
	std::array<Vec2, 4> uvPos = { Vec2(1.0, 0.0), Vec2(1.0, 1.0), Vec2(0.0, 1.0), Vec2(0.0, 0.0)};
};
