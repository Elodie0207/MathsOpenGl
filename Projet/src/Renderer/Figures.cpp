//
// Created by Enola on 27/02/2024.
//

#include "Renderer/Figures.h"
#include "Core.hpp"
#include <utility>
#include "Renderer/Texture.hpp"

Poly Figures::CreateHeart(float heartSize) {
    //Mat4 modelMatrix = Math::TRS(m_Position, m_Size);
    const float PI = 3.14159265f;

	std::vector<Vec2> points;
	points.reserve(std::max((int)std::ceil((2.0f * PI) / 0.01f), 0));

    for (float t = 0.0f; t <= 2.0f * PI; t += 0.01f) {
        float x = 16.0f * sin(t) * sin(t) * sin(t);
        float y = 13.0f * cos(t) - 5.0f * cos(2.0f * t) - 2.0f * cos(3.0f * t) - cos(4.0f * t);

		points.emplace_back(x * heartSize, y * heartSize);
    }

	return points;
}

Poly Figures::CreateStar(float armLength, int numArms) {

    const float PI = 3.14159265f;

	std::vector<Vec2> points;
	points.reserve(std::max(numArms * 2, 0));

    for (int i = 0; i <= numArms * 2; ++i) {
        float angle = static_cast<float>(i) * PI / numArms;
        float radius = i % 2 == 0 ? 0.5f : 1.0f;

        float x = radius * armLength * cos(angle);
        float y = radius * armLength * sin(angle);

		points.emplace_back(x,y);
    }

	return points;
}

Poly Figures::CreatePolygone(float armSize, int numSides) {
    const float PI = 3.14159265f;

	std::vector<Vec2> points;
	points.reserve(std::max(numSides, 0));

    for (int i = 0; i < numSides; ++i) {
        float angle = static_cast<float>(i) * 2.0f * PI / numSides;
        float x = cos(angle) * armSize;
        float y = sin(angle) * armSize;
		points.emplace_back(x,y);
    }
    return points;
}

Poly Figures::CreateEllipse(int numSegments , float radiusX, float radiusY) {
    const float PI = 3.14159265f;

	std::vector<Vec2> points;
	points.reserve(std::max(numSegments, 0));

    for (int i = 0; i <= numSegments; ++i) {
        float angle = static_cast<float>(i) * 2.0f * PI / numSegments;
        float x = cos(angle) * radiusX;
        float y = sin(angle) * radiusY;
		points.emplace_back(x, y);
    }
    return points;
}