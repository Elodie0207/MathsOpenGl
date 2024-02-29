//
// Created by Enola on 27/02/2024.
//

#pragma once

#include <memory>
#include <vector>
#include "Core.hpp"
#include "Renderer/Texture.hpp"
#include "Renderer/Poly.hpp"



class Figures {
public:
	static Poly CreateHeart(float heartSize = 100.f);

	static Poly CreateStar(float armLength = 100.f, int numArms = 5);

	static Poly CreatePolygone(float armSize = 100, int numSides = 5);

	static Poly CreateEllipse(int numSegments = 100, float radiusX = 100.f, float radiusY = 5.f);
};
