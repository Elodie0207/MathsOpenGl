//
// Created by ianpo on 29/12/2023.
//

#include "Camera.hpp"
#include "Math.hpp"

Camera::Camera(uint32_t width, uint32_t height) : m_Width(width), m_Height(height), m_AspectRatio(1)
{
	REI_ASSERT(width > 0 && height > 0, "The Aspect Ration ({0}/{1}) is no valid.", width, height);
	m_AspectRatio = static_cast<float>(m_Width) / static_cast<float>(m_Height);
	UpdateViewMatrix(false);
	UpdateProjMatrix(true);
}

Camera::~Camera()
{
}

void Camera::ChangeAspectRatio(uint32_t width, uint32_t height)
{
	REI_ASSERT(width > 0 && height > 0, "The Aspect Ration ({0}/{1}) is no valid.", width, height);
	m_Width = width;
	m_Height = height;
	m_AspectRatio = static_cast<float>(m_Width) / static_cast<float>(m_Height);
	UpdateProjMatrix();
}

void Camera::SetPosition(const Vec2 &newPosition) {
	m_Position = newPosition;
	UpdateViewMatrix();
}

void Camera::MovePosition(const Vec2 &movement) {
	m_Position += movement;
	UpdateViewMatrix();
}

void Camera::SetSize(float size)
{
	m_Size = size;
	UpdateProjMatrix();
}

Mat4 Camera::GetViewMatrix() const {
	return m_ViewMatrix;
}
Mat4 Camera::GetProjMatrix() const {
	return m_ProjMatrix;
}
Mat4 Camera::GetViewProjMatrix() const {
	return m_ViewProjMatrix;
}

void Camera::UpdateViewMatrix(bool updateViewProj) {
	m_ViewMatrix = glm::inverse(Math::TRS(Vec3(m_Position.x, m_Position.y, 0.0f)));
	if(updateViewProj) {
		UpdateViewProjMatrix();
	}
}

void Camera::UpdateProjMatrix(bool updateViewProj) {
	float left = -m_Size * m_AspectRatio * 0.5f;
	float right = +m_Size * m_AspectRatio * 0.5f;
	float top = +m_Size * 0.5f;
	float bottom = -m_Size * 0.5f;
	m_ProjMatrix = glm::ortho(left, right, bottom, top, m_ZNear, m_ZFar);
	if(updateViewProj) {
		UpdateViewProjMatrix();
	}
}

void Camera::UpdateViewProjMatrix() {
	m_ViewProjMatrix = m_ProjMatrix * m_ViewMatrix;
}

Vec2 Camera::ScreenToGameSpace(Vec2 pos) const {

	float y = 1 - static_cast<float>(pos.y) / (float)m_Height;
	y -= 0.5f;
	y *= GetSize();
	y += GetPosition().y;

	float x = static_cast<float>(pos.x) / (float)m_Width;
	x -= 0.5f;
	x *= (GetSize() * GetAspectRatio());
	x += GetPosition().x;

	return {x, y};
}

Vec2 Camera::GameToScreenSpace(Vec2 pos) const {

	float y = pos.y - GetPosition().y;
	y /= GetSize();
	y += 0.5f;
	y = (1 - y) * m_Height;

	float x = pos.x - GetPosition().x;
	x /= GetSize() * GetAspectRatio();
	x += 0.5f;
	x = x * m_Width;

	return {x, y};
}
