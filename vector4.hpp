#pragma once

#include "detail/vector-data.hpp"

namespace ssm
{
template <typename T>
struct tvec4 : detail::vector_data<T, 4>
{
	using detail::vector_data<T, 4>::detail::vector_data<T, 4>;

	tvec4<T>& operator+=(const tvec4& vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
		w += vec.w;
		return *this;
	}
	tvec4<T>& operator-=(const tvec4& vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		w -= vec.w;
		return *this;
	}
	tvec4<T>& operator*=(const tvec4& vec) {
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
		w *= vec.w;
		return *this;
	}
	tvec4<T>& operator/=(const tvec4& vec) {
		x /= vec.x;
		y /= vec.y;
		z /= vec.z;
		w /= vec.w;
		return *this;
	}
};

template <typename T>
struct tnorm4
{
	tvec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

	T x, y, z, w;

	operator tvec4<T> { return tvec4<T>(x, y, z, w); }
};
}
