//
// Created by Sayama on 23/02/2024.
//

#include "Renderer/Circle.hpp"

Circle::Circle(Vec2 center, float radius) : Center(center), Radius(radius) {}
Circle::Circle(Vec2 center, float radius, ::Color color) : Center(center), Radius(radius), Color(color) {}


void Circle::Draw(const Mat4& ViewProjMatrix) const
{
	const GLenum mode = RenderMode == Fill ? GL_TRIANGLE_FAN : GL_LINE_LOOP;

	glColor4f(Color.x, Color.y, Color.z, Color.w); // Set the color
	glBegin(mode);

	for (int ii = 0; ii <= num_segments; ++ii) {
		float theta = 2.0f * Math::Pi<float>() * float(ii) / float(num_segments); // current angle
		float dx = Radius * glm::cos(theta); // calculate x position
		float dy = Radius * glm::sin(theta); // calculate y position

		Vec3 vertexPos = ViewProjMatrix * Vec4(Center.x + dx, Center.y + dy, 0.0, 1.0);
		glVertex3fv(&vertexPos[0]);
	}

	glEnd();
}