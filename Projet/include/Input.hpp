//
// Created by ianpo on 29/12/2023.
//

#pragma once

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE 1
#endif

#include "GLFW/glfw3.h"

enum class MouseButton
{
	Left = GLFW_MOUSE_BUTTON_LEFT,
	Middle = GLFW_MOUSE_BUTTON_MIDDLE,
	Right = GLFW_MOUSE_BUTTON_RIGHT,
};

enum class PressState
{
	Up = GLFW_RELEASE,
	Down = GLFW_PRESS,
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