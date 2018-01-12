#pragma once

#include "vec3.hpp"
#include "mat4.hpp"
#include "quat.hpp"

namespace ssm
{
mat4 translate(const vec3& pos);
mat4 rotate(const quat& rot);
mat4 scale(const vec3& scale);
mat4 perspective(float fovy, float aspect, float znear, float zfar);
mat4 look_at(const vec3& eye, const vec3& target, const vec3& up);
}

#include "src\transform.inl"
