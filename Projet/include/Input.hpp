//
// Created by ianpo on 29/12/2023.
//

#pragma once

#include "GLFW/glfw3.h"
#include "GL/freeglut.h"

enum class MouseButton
{
	Left = GLUT_LEFT_BUTTON,
	Middle = GLUT_MIDDLE_BUTTON,
	Right = GLUT_RIGHT_BUTTON,
};

enum class MousePressState
{
	Up = GLUT_UP,
	Down = GLUT_DOWN,
};

struct KeyState
{
	bool pressed = false;
	double timePressed = 0.0;
	double timeReleased = 0.0;
	inline double GetTimePressed() const { return timeReleased - timePressed; }
};

struct MouseState
{
	bool pressed = false;
	double timePressed = 0.0;
	double timeReleased = 0.0;
	Vec2Int positionPressed = Vec2Int(0);
	Vec2Int positionReleased = Vec2Int(0);

	inline Vec2Int GetTravelDistance() const { return positionReleased - positionPressed; }
	inline double GetTimePressed() const { return timeReleased - timePressed; }
};