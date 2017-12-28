#pragma once

#include "detail/setup.hpp"
#include "simd/traits.hpp"
#include "vector-data.hpp"

namespace ssm
{
template <typename T, size_t N>
struct tvecn : vector_data<T, N>
{
	using vector_data<T, N>::vector_data;

	const T& operator[](size_t index) const {
		assert(index < N);
		return *(data_ptr(*this) + index);
	}
	T& operator[](size_t index) {
		assert(index < N);
		return *(data_ptr(*this) + index);
	}
};

template <typename T, size_t N>
struct tnormn : vector_data<T, N>
{
	using vector_data<T, N>::vector_data;
	operator tvecn<T, N>() const { return tvecn<T, N>(data); }

	const T& operator[](size_t index) const {
		assert(index < N);
		return *(data_ptr(*this) + index);
	}
	T& operator[](size_t index) {
		assert(index < N);
		return *(data_ptr(*this) + index);
	}
};

#include "detail/vector.inl"

template <typename T, size_t N>
inline T dot(const tvecn<T, N>& a, const tvecn<T, N>& b) {
	return detail::vec_impl<T, N>::dot(a, b);
}

template <typename T, size_t N>
inline T sqlength(const tvecn<T, N>& vec) {
	return dot(vec, vec);
}

template <typename T, size_t N>
inline T length(const tvecn<T, N>& vec) {
	return T{std::sqrt(sqlength(vec))};
}

template <typename T, size_t N>
inline tnormn<T, N> normalize(const tvecn<T, N>& vec) {
	return detail::vec_impl<T, N>::normalize(vec);
}

template <typename T, size_t N>
inline tvecn<T, N> cross(const tvecn<T, N>& a, const tvecn<T, N>& b) {
	static_assert(N == 3, "Cross product is only defined for 3D vectors");
	return tvecn<T, N>(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}

template <typename T, size_t N>
inline tnormn<T, N> cross(const tnormn<T, N>& a, const tnormn<T, N>& b) {
	static_assert(N == 3, "Cross product is only defined for 3D vectors");
	return tnormn<T, N>(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}

template <typename T, size_t N>
inline tvecn<T, N>& operator+=(tvecn<T, N>& a, const tvecn<T, N>& b) {
	return detail::vec_impl<T, N>::add(a, b);
}

template <typename T, size_t N>
inline tvecn<T, N>& operator-=(tvecn<T, N>& a, const tvecn<T, N>& b) {
	return detail::vec_impl<T, N>::sub(a, b);
}

template <typename T, size_t N>
inline tvecn<T, N>& operator*=(tvecn<T, N>& a, const tvecn<T, N>& b) {
	return detail::vec_impl<T, N>::mul(a, b);
}

template <typename T, size_t N>
inline tvecn<T, N>& operator*=(tvecn<T, N>& a, T b) {
	return detail::vec_impl<T, N>::mul(a, b);
}

template <typename T, size_t N>
inline tvecn<T, N>& operator/=(tvecn<T, N>& a, const tvecn<T, N>& b) {
	return detail::vec_impl<T, N>::div(a, b);
}

template <typename T, size_t N>
inline tvecn<T, N>& operator/=(tvecn<T, N>& a, T b) {
	return detail::vec_impl<T, N>::div(a, b);
}

template <typename T, size_t N>
inline tvecn<T, N> operator+(tvecn<T, N> a, const tvecn<T, N>& b) { return a += b; }
template <typename T, size_t N>
inline tvecn<T, N> operator-(tvecn<T, N> a, const tvecn<T, N>& b) { return a -= b; }
template <typename T, size_t N>
inline tvecn<T, N> operator*(tvecn<T, N> a, const tvecn<T, N>& b) { return a *= b; }
template <typename T, size_t N>
inline tvecn<T, N> operator*(tvecn<T, N> a, T b) { return a *= b; }
template <typename T, size_t N>
inline tvecn<T, N> operator/(tvecn<T, N> a, const tvecn<T, N>& b) { return a /= b; }

template <typename T, size_t N>
inline tvecn<T, N> operator-(const tvecn<T, N>& vec) {
	return detail::vec_impl<T, N>::negate(vec);
}

template <typename T, size_t N>
inline tnormn<T, N> operator-(const tnormn<T, N>& vec) {
	return tnormn<T, N>(detail::vec_impl<T, N>::negate(vec).data);
}

template <typename T, size_t N>
inline bool operator==(const tvecn<T, N>& a, const tvecn<T, N>& b) {
	return detail::vec_impl<T, N>::cmp_eq(a, b);
}

template <typename T, size_t N>
inline bool operator!=(const tvecn<T, N>& a, const tvecn<T, N>& b) {
	return !(a == b);
}

template <typename T, size_t N>
inline const T* data_ptr(const tvecn<T, N>& a) { return &(a.data[0]); }
template <typename T, size_t N>
inline const T* begin(const tvecn<T, N>& a) { return data_ptr(a); }
template <typename T, size_t N>
inline const T* end(const tvecn<T, N>& a) { return begin(a) + N; }

template <typename T, size_t N>
inline T* data_ptr(tvecn<T, N>& a) { return &(a.data[0]); }
template <typename T, size_t N>
inline T* begin(tvecn<T, N>& a) { return data_ptr(a); }
template <typename T, size_t N>
inline T* end(tvecn<T, N>& a) { return begin(a) + N; }

//----------------------------------------------
// Utility typedefs
//----------------------------------------------
template <typename T, size_t N>
using vector = tvecn<T, N>;
template <typename T, size_t N>
using normal = tvecn<T, N>;

using ivec2 = tvecn<int, 2>;
using ivec3 = tvecn<int, 3>;
using ivec4 = tvecn<int, 4>;
using vec2 = tvecn<float, 2>;
using vec3 = tvecn<float, 3>;
using vec4 = tvecn<float, 4>;
using dvec2 = tvecn<double, 2>;
using dvec3 = tvecn<double, 3>;
using dvec4 = tvecn<double, 4>;

using inorm2 = tnormn<int, 2>;
using inorm3 = tnormn<int, 3>;
using inorm4 = tnormn<int, 4>;
using norm2 = tnormn<float, 2>;
using norm3 = tnormn<float, 3>;
using norm4 = tnormn<float, 4>;
using dnorm2 = tnormn<double, 2>;
using dnorm3 = tnormn<double, 3>;
using dnorm4 = tnormn<double, 4>;
}
