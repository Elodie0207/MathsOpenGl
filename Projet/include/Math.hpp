//
// Created by ianpo on 29/12/2023.
//

#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/hash.hpp>
#include <cmath>
#include <cstdint>

typedef glm::vec2 Vec2;
typedef glm::ivec2 Vec2Int;

class Math
{
    template<typename T>
    std::string to_string(const T& value)
    {
        return glm::to_string(value);
    }
};
