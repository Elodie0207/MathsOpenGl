//
// Created by ianpo on 29/12/2023.
//

#include <utility>

#include "Core.hpp"
#include "Renderer/Quad.hpp"
#include "Renderer/Texture.hpp"

void Quad::Draw(const Mat4 &ViewProjMatrix) const {
	if (m_Texture != nullptr) {
		m_Texture->Bind();
	}

	glBegin(GL_TRIANGLE_FAN);

	Mat4 modelMatrix = Math::TRS(m_Position, m_Size);
	for (int i = 0; i < vertices.size(); ++i)
	{
		const Vec3 &vertice = vertices[i];
		const Vec2 &uv = uvPos[i];
		auto pos = modelMatrix * Vec4(vertice, 1.0);
		Vec3 v = ViewProjMatrix * pos;
		glColor3f(1.0, 1.0, 1.0);
		glVertex3fv(&v[0]);
		glTexCoord2fv(&uv[0]);
	}

	glEnd();

	if (m_Texture != nullptr) {
		Texture::Unbind();
	}
}


Mat4 Quad::GetTransformMatrix() const {
	return Math::TRS(Vec3(m_Position.x, m_Position.y, 0.0f), Vec3(m_Size.x, m_Size.y, 1.0));
}

Quad::Quad(Vec2 position, Vec2 size, Ref<Texture> texture) : m_Position(position), m_Size(size), m_Texture(std::move(texture))
{
}
