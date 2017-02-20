#pragma once

#include "vec3.hpp"
#include "mat4.hpp"
#include "quat.hpp"

namespace ssm
{
mat4 translation_matrix(const vec3& pos);
mat4 rotation_matrix(const quat& rot);
mat4 scale_matrix(const vec3& scale);

//Not sure if I should define these in mat4.hpp
vec4 operator*(const mat4& mat, const vec4& vec);
mat4 operator*(const mat4& lhs, const mat4& rhs);
//Count here should be even!!!
void batch_transform(const mat4& mat, vec4* vin, vec4* vout, int count);
}

#include "src\transform.inl"
