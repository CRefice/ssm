#pragma once

#include "vec4.hpp"

namespace ssm
{
class mat4
{
public:
	mat4();
	mat4(vec4* data);
	explicit mat4(float val);
	explicit mat4(f128* data);

	vec4 slice_horiz(int row) const;
	vec4 slice_vert(int col) const;

	float* data();
	vec4* begin();
	vec4* end();
	const float* data() const;
	const vec4* begin() const;
	const vec4* end() const;

	vec4& operator[](int index);
	const vec4& operator[](int index) const;

	mat4& operator+=(const mat4& rhs);
	mat4& operator-=(const mat4& rhs);
	mat4& operator*=(float scale);

private:
	vec4 cols[4];
};

mat4 operator+(mat4 lhs, const mat4& rhs);
mat4 operator-(mat4 lhs, const mat4& rhs);
mat4 operator*(mat4 lhs, float scale);
}

#include "src\mat4.inl"