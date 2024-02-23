//
// Created by ianpo on 11/02/2024.
//

#pragma once

#include "Core.hpp"

class AABB2D {
public:
	Vec2 min; // Minimum x, and y coordinates.
	Vec2 max; // Maximum x, and y coordinates.
public:
	inline AABB2D() {
		// Initialize with impossible values to make sure they get set properly
		min.x = min.y = std::numeric_limits<float>::max();
		max.x = max.y = std::numeric_limits<float>::lowest();
	}
	inline ~AABB2D() = default;

	template<typename T>
	inline void Extend(const glm::vec<2, T>& point) {
		// Update the minimum and maximum coordinates
		min.x = std::min(min.x, static_cast<float>(point.x));
		min.y = std::min(min.y, static_cast<float>(point.y));

		max.x = std::max(max.x, static_cast<float>(point.x));
		max.y = std::max(max.y, static_cast<float>(point.y));
	}

	inline bool Intersects(const AABB2D& other) const {
		// Check for intersection by comparing the coordinates of the corners.
		return !(max.x < other.min.x || min.x > other.max.x ||
				 max.y < other.min.y || min.y > other.max.y );
	}

	template<typename T>
	inline static AABB2D ComputeBoundingBox(const std::vector<glm::vec<2, T>>& points) {
		AABB2D box;
		for(const auto& point : points) {
			box.Extend(point);
		}
		return box;
	}
};