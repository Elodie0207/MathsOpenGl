//
// Created by ianpo on 29/12/2023.
//

#pragma once

#include "Core.hpp"

class Camera {
public:
	Camera(uint32_t width, uint32_t height);
	~Camera();

	void ChangeAspectRatio(uint32_t width, uint32_t height);
	void SetPosition(const Vec2& newPosition);
	void MovePosition(const Vec2& movement);
	inline const Vec2& GetPosition() const {return m_Position;}

	Mat4 GetViewProjMatrix() const;
	Mat4 GetViewMatrix() const;
	Mat4 GetProjMatrix() const;


	inline float GetAspectRatio() const {return m_AspectRatio; }
	inline float GetSize() const {return m_Size; }

	Vec2 ScreenToGameSpace(Vec2 pos) const;
	Vec2 GameToScreenSpace(Vec2 pos) const;
private:
	void UpdateProjMatrix(bool updateViewProj = true);
	void UpdateViewMatrix(bool updateViewProj = true);
	void UpdateViewProjMatrix();
private:
	uint32_t m_Width, m_Height;
	float m_AspectRatio;
	float m_ZNear = 0.0f;
	float m_ZFar = 1.0f;
	float m_Size = 1024.0f;

	Vec2 m_Position = Vec2(0.0f, 0.0f);

	Mat4 m_ViewProjMatrix, m_ViewMatrix, m_ProjMatrix;
};
