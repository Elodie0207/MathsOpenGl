//
// Created by ianpo on 29/12/2023.
//

#include "Core.hpp"
#include "Renderer/Quad.hpp"

void Quad::Draw(const Mat4 &ViewProjMatrix) const {
	if (Texture != nullptr) {
		Texture->Bind();
	}

	glBegin(GL_TRIANGLE_FAN);

	for (int i = 0; i < vertices.size(); ++i)
	{
		const Vec3 &vertice = vertices[i];
		const Vec2 &uv = uvPos[i];
		Vec3 v = Vec3(vertice.x * Size.x, vertice.y * Size.y, 0.0);
		v += Vec3(Position, 0.0);
		v = ViewProjMatrix * Vec4(v, 1.0);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3fv(&v[0]);
		glTexCoord2fv(&uv[0]);
	}

	glEnd();

	if (Texture != nullptr) {
		Texture::Unbind();
	}
}


Mat4 Quad::GetTransformMatrix() const {
	return Math::TRS(Vec3(Position.x, Position.y, 0.0f), Vec3(Size.x, Size.y, 1.0));
}

Quad::Quad(Vec2 position, Vec2 size) : Position(position), Size(size)
{
}
