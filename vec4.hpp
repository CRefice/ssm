#pragma once

#include "vec3.hpp"

namespace ssm
{
struct alignas(16) vec4
{
	union
	{
		f128 vec_data;
		struct { float x, y, z, w; };
		struct { float r, g, b, a; };
	};

	vec4();
	vec4(float x, float y, float z, float w);
	vec4(const vec2& vec, float z, float w);
	vec4(const vec3& vec, float w);
	explicit vec4(float val);
	explicit vec4(f128 data);

	vec4& normalize();
	float length() const;

	float* data();
	float* begin();
	float* end();
	const float* data() const;
	const float* begin() const;
	const float* end() const;

	float& operator[](int index);
	const float& operator[](int index) const;

	bool operator==(const vec4& rhs) const;
	bool operator!=(const vec4& rhs) const;

	vec4& operator+=(const vec4& rhs);
	vec4& operator-=(const vec4& rhs);
	vec4& operator*=(const vec4& rhs);
	vec4& operator*=(float scale);

	explicit operator vec3();
};

float dot(const vec4& lhs, const vec4& rhs);
vec4 cross(const vec4& lhs, const vec4& rhs);
vec4 normalize(vec4 vec);

vec4 operator+(vec4 lhs, const vec4& rhs);
vec4 operator-(vec4 lhs, const vec4& rhs);
vec4 operator*(vec4 lhs, const vec4& rhs);
vec4 operator*(vec4 lhs, float scale);

vec4 operator-(const vec4& vec);
}

#include "src\vec4.inl"
