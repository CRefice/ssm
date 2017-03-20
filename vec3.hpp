#pragma once

#include "arch.hpp"

namespace ssm
{
struct alignas(16) vec3
{
	union
	{
		f128 vec_data;
		struct { float x, y, z; };
		struct { float r, g, b; };
	};

	vec3();
	vec3(float x, float y, float z);
	explicit vec3(float val);
	explicit vec3(f128 vals);

	vec3& normalize();
	float length() const;
	
	float* data();
	float* begin();
	float* end();
	const float* data() const;
	const float* begin() const;
	const float* end() const;

	float& operator[](unsigned int index);
	const float& operator[](unsigned int index) const;

	bool operator==(const vec3& rhs) const;
	bool operator!=(const vec3& rhs) const;

	vec3& operator+=(const vec3& rhs);
	vec3& operator-=(const vec3& rhs);
	vec3& operator*=(const vec3& rhs);
	vec3& operator*=(float scale);
};

float dot(const vec3& lhs, const vec3& rhs);
vec3 cross(const vec3& lhs, const vec3& rhs);
vec3 normalize(vec3 vec);

vec3 operator+(vec3 lhs, const vec3& rhs);
vec3 operator-(vec3 lhs, const vec3& rhs);
vec3 operator*(vec3 lhs, const vec3& rhs);
vec3 operator*(vec3 lhs, float scale); 

vec3 operator-(const vec3& vec);
}

#include "src\vec3.inl"
