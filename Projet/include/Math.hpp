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
#include <iostream>

typedef glm::vec2 Vec2;
typedef glm::ivec2 Vec2Int;
typedef glm::vec3 Vec3;
typedef glm::u8vec3 Vec3UB;
typedef glm::vec4 Vec4;
typedef glm::u8vec4 Vec4UB;
typedef glm::quat Quat;

typedef glm::mat4 Mat4;

class Math
{
public:
    template<typename T>
    inline static std::string ToString(const T& value)
    {
        return glm::to_string(value);
    }

	template<typename T>
	inline static T Normalize(const T& val)
	{
		return glm::normalize(val);
	}

	static Mat4 TRS(Vec3 position);
	static Mat4 TRS(Vec3 position, Quat rotation);
	static Mat4 TRS(Vec3 position, Vec3 scale);
	static Mat4 TRS(Vec3 position, Quat Rotation, Vec3 scale);

	static Mat4 Inverse(Mat4 mat);

};

inline std::ostream& operator << (std::ostream & os, const Mat4& value){
	return os << Math::ToString(value);
}
inline std::ostream& operator << (std::ostream & os, const Vec2Int& value){
	return os << Math::ToString(value);
}
inline std::ostream& operator << (std::ostream & os, const Vec4& value){
	return os << Math::ToString(value);
}
inline std::ostream& operator << (std::ostream & os, const Vec3& value){
	return os << Math::ToString(value);
}
inline std::ostream& operator << (std::ostream & os, const Vec2& value){
	return os << Math::ToString(value);
}
inline std::ostream& operator << (std::ostream & os, const Quat& value){
	return os << Math::ToString(value);
}
