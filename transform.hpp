#pragma once

#include "vec3.hpp"
#include "mat4.hpp"
#include "quat.hpp"

namespace ssm
{
mat4 translate(const vec3& pos);
mat4 rotate(const quat& rot);
mat4 scale(const vec3& scale);
mat4 project(float fovy, float aspect, float znear, float zfar);

//Not sure if I should define these in mat4.hpp
vec4 operator*(const mat4& mat, const vec4& vec);
mat4 operator*(const mat4& lhs, const mat4& rhs);
//Count here should be even!!!
void batch_transform(const mat4& mat, vec4* vin, vec4* vout, unsigned count);
}

#include "src\transform.inl"