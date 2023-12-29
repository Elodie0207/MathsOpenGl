//
// Created by ianpo on 29/12/2023.
//

#pragma once

#include "Math.hpp"

class Parameters
{
public:
	inline static void SetColor(const Vec4& color) { s_Color = color; }
	inline static const Vec4& GetColor() { return s_Color; }

	inline static void SetClearColor(const Vec4& color) { s_ClearColor = color; }
	inline static const Vec4& GetClearColor() { return s_ClearColor; }
private:
	static Vec4 s_Color;
	static Vec4 s_ClearColor;
};
