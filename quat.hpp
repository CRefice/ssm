#pragma once

#include "vec3.hpp"

namespace ssm
{
struct alignas(16) quat
{
	union
	{
		f128 vec_data;
		struct { float x, y, z, w; };
	};

	quat();
	quat(float x, float y, float z, float w);
	quat(const vec3& axis, const float angle);

	quat& normalize();

	float magnitude() const;
	float dot(const quat& rhs) const;

	float* data();
	const float* data() const;
	
	bool operator==(const quat& rhs) const;
	bool operator!=(const quat& rhs) const;

	quat& operator+=(const quat& rhs);
	quat& operator-=(const quat& rhs);
	quat& operator*=(const quat& rhs);
};

quat operator+(quat lhs, const quat& rhs);
quat operator-(quat lhs, const quat& rhs);
quat operator*(quat lhs, const quat& rhs);
}

#include "src\quat.inl"
