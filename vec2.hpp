#pragma once

#include "arch.hpp"

namespace ssm
{
struct alignas(16) vec2
{
	union
	{
		f128 vec_data;
		struct { float x, y; }
		struct { float r, g; }
	};

	vec2();
	vec2(float x, float y);
	explicit vec2(f128 vec_data);
	explicit vec2(float val);

	vec2& normalize();
	float length() const;

	float* data();
	float* begin();
	float* end();
	const float* data() const;
	const float* begin() const;
	const float* end() const;

	float& operator[](unsigned int index);
	const float& operator[](unsigned int index) const;

	bool operator==(const vec2& rhs) const;
	bool operator!=(const vec2& rhs) const;

	vec2& operator+=(const vec2& rhs);
	vec2& operator-=(const vec2& rhs);
	vec2& operator*=(const vec2& rhs);
	vec2& operator*=(float scale);
};

float dot(const vec2& lhs, const vec2& rhs);
vec2 cross(const vec2& lhs, const vec2& rhs);
vec2 normalize(vec2 vec);

vec2 operator+(vec2 lhs, const vec2& rhs);
vec2 operator-(vec2 lhs, const vec2& rhs);
vec2 operator*(vec2 lhs, const vec2& rhs);
vec2 operator*(vec2 lhs, float scale); 

vec2 operator-(const vec2& vec);
};

#include "src\vec2.inl"
}
