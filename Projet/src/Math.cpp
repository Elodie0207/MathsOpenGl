//
// Created by ianpo on 29/12/2023.
//

#include "Math.hpp"

Mat4 Math::TRS(Vec3 position)
{
	return TRS(position, glm::identity<glm::quat>(), Vec3(1.0f, 1.0f, 1.0f));
}

Mat4 Math::TRS(Vec3 position, Quat rotation)
{
	return TRS(position, rotation, Vec3(1.0f, 1.0f, 1.0f));
}

Mat4 Math::TRS(Vec3 position, Vec3 scale)
{
	return TRS(position, glm::identity<glm::quat>(), scale);
}

Mat4 Math::TRS(Vec3 position, Quat rotation, Vec3 scale) {
	glm::mat4 trs(1.0f);
	trs = glm::translate(trs, position); // Translation Matrix
	trs = trs * glm::toMat4(rotation); // Rotation Matrix
	trs = glm::scale(trs, scale); // Scale Matrix
	return trs; // Translation * Rotation * Scale => TRS Matrix.
}

Mat4 Math::Inverse(Mat4 mat)
{
	return glm::inverse(mat);
}