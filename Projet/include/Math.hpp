//
// Created by ianpo on 29/12/2023.
//

#pragma once

#define GLM_ENABLE_EXPERIMENTAL 1

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
#include <vector>
#include <limits>
#include <string>

typedef glm::vec2 Vec2;
typedef glm::ivec2 Vec2Int;
typedef glm::vec3 Vec3;
typedef glm::u8vec3 Vec3UB;
typedef glm::vec4 Vec4;
typedef glm::u8vec4 Vec4UB;
typedef glm::quat Quat;

typedef glm::mat4 Mat4;

typedef glm::dvec2 Point;

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

	template<typename T>
	inline static float Distance(const T& l, const T& r)
	{
		return glm::distance(l, r);
	}

	static Mat4 TRS(Vec3 position);
	static Mat4 TRS(Vec3 position, Quat rotation);
	static Mat4 TRS(Vec3 position, Vec3 scale);
	static Mat4 TRS(Vec3 position, Quat Rotation, Vec3 scale);

	static Mat4 Inverse(Mat4 mat);
	static bool CyrusBeck(double X1, double Y1, double X2, double Y2, std::vector<Point> Poly, std::vector<Point> Normale, int Nbsom);
    static double coupe(const Point& a, const Point& b, const Point& c);
    static Point intersection(const Point& a, const Point& b, const Point& c, const Point& d);
    static std::vector<Point> sutherlandHodgman(std::vector<Point> PL, const std::vector<Point>& PW);
};

template<> inline std::string Math::ToString(const float& value) { return std::to_string(value); }
template<> inline std::string Math::ToString(const double& value) { return std::to_string(value); }
template<> inline std::string Math::ToString(const long double& value) { return std::to_string(value); }
template<> inline std::string Math::ToString(const unsigned char& value) { return std::to_string(value); }
template<> inline std::string Math::ToString(const unsigned short& value) { return std::to_string(value); }
template<> inline std::string Math::ToString(const unsigned int& value) { return std::to_string(value); }
template<> inline std::string Math::ToString(const unsigned long int& value) { return std::to_string(value); }
template<> inline std::string Math::ToString(const unsigned long long int& value) { return std::to_string(value); }
template<> inline std::string Math::ToString(const signed char& value) { return std::to_string(value); }
template<> inline std::string Math::ToString(const signed short& value) { return std::to_string(value); }
template<> inline std::string Math::ToString(const signed int& value) { return std::to_string(value); }
template<> inline std::string Math::ToString(const signed long int& value) { return std::to_string(value); }
template<> inline std::string Math::ToString(const signed long long int& value) { return std::to_string(value); }


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
