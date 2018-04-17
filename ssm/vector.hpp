#pragma once

#include "detail/setup.hpp"
#include "detail/vector-manip.hpp"

namespace ssm
{
template <typename T, std::size_t N>
struct normal : generic_vec<T, N>
{
	using generic_vec<T, N>::generic_vec;
};

template <typename T, std::size_t N>
struct vector : generic_vec<T, N>
{
	vector<T, N>() = default;
	vector<T, N>(const normal<T, N>& norm) : generic_vec<T, N>(norm.data) {}
	using generic_vec<T, N>::generic_vec;
};

template <template <class, std::size_t> class A,
				 template <class, std::size_t> class B,
				 typename T, std::size_t N>
inline T dot(const A<T, N>& a, const B<T, N>& b) {
	return detail::vec_impl<T, N>::dot(a, b);
}

template <template <class, std::size_t> class Vec, typename T, std::size_t N>
inline T sqlength(const Vec<T, N>& vec) {
	return dot(vec, vec);
}

template <template <class, std::size_t> class Vec, typename T, std::size_t N>
inline T length(const Vec<T, N>& vec) {
	return static_cast<T>(std::sqrt(sqlength(vec)));
}

template <template <class, std::size_t> class A,
				 template <class, std::size_t> class B,
				 typename T, std::size_t N>
inline vector<T, N> cross(const A<T, N>& a, const B<T, N>& b) {
	static_assert(N == 3, "Cross product is only defined for 3D vectors");
	return vector<T, N>(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
			);
}

template <template <class, std::size_t> class Vec, typename T, std::size_t N>
inline normal<T, N> normalize(const Vec<T, N>& vec) {
	normal<T, N> norm(vec.data);
	detail::vec_impl<T, N>::normalize(norm);
	return norm;
}

template <template <class, std::size_t> class Vec,
				 typename T, std::size_t N>
inline vector<T, N>& operator+=(vector<T, N>& a, const Vec<T, N>& b) {
	detail::vec_impl<T, N>::add(a, b);
	return a;
}

template <template <class, std::size_t> class Vec,
				 typename T, std::size_t N>
inline vector<T, N>& operator-=(vector<T, N>& a, const Vec<T, N>& b) {
	detail::vec_impl<T, N>::sub(a, b);
	return a;
}

template <template <class, std::size_t> class Vec,
				 typename T, std::size_t N>
inline vector<T, N>& operator*=(vector<T, N>& a, const Vec<T, N>& b) {
	detail::vec_impl<T, N>::mul(a, b);
	return a;
}

template <template <class, std::size_t> class Vec,
				 typename T, std::size_t N>
inline vector<T, N>& operator/=(vector<T, N>& a, const Vec<T, N>& b) {
	detail::vec_impl<T, N>::div(a, b);
	return a;
}

template <typename T, std::size_t N>
inline vector<T, N>& operator*=(vector<T, N>& a, T b) {
	detail::vec_impl<T, N>::mul(a, b);
	return a;
}

template <typename T, std::size_t N>
inline vector<T, N>& operator/=(vector<T, N>& a, T b) {
	detail::vec_impl<T, N>::div(a, b);
	return a;
}

template <template <class, std::size_t> class A,
				 template <class, std::size_t> class B,
				 typename T, std::size_t N>
inline vector<T, N> operator+(const A<T, N>& a, const B<T, N>& b) {
	return static_cast<vector<T, N>>(a) += b;
}

template <template <class, std::size_t> class A,
				 template <class, std::size_t> class B,
				 typename T, std::size_t N>
inline vector<T, N> operator-(const A<T, N>& a, const B<T, N>& b) {
	return static_cast<vector<T, N>>(a) -= b;
}

template <template <class, std::size_t> class A,
				 template <class, std::size_t> class B,
				 typename T, std::size_t N>
inline vector<T, N> operator*(const A<T, N>& a, const B<T, N>& b) {
	return static_cast<vector<T, N>>(a) *= b;
}

template <template <class, std::size_t> class A,
	typename T, std::size_t N>
inline vector<T, N> operator*(const A<T, N>& a, T b) {
	return static_cast<vector<T, N>>(a) *= b;
}

template <template <class, std::size_t> class A,
	typename T, std::size_t N>
inline vector<T, N> operator*(T a, const A<T, N>& b) {
	return b * a;
}

template <template <class, std::size_t> class A,
				 template <class, std::size_t> class B,
				 typename T, std::size_t N>
inline vector<T, N> operator/(const A<T, N>& a, const B<T, N>& b) {
	return static_cast<vector<T, N>>(a) /= b;
}

template <template <class, std::size_t> class A,
	typename T, std::size_t N>
inline vector<T, N> operator/(const A<T, N>& a, T b) {
	return static_cast<vector<T, N>>(a) /= b;
}

template <template <class, std::size_t> class Vec, typename T, std::size_t N>
inline Vec<T, N> operator-(Vec<T, N> vec) {
	detail::vec_impl<T, N>::negate(vec);
	return vec;
}

template <template <class, std::size_t> class A,
				 template <class, std::size_t> class B,
				 typename T, std::size_t N>
inline bool operator==(const A<T, N>& a, const B<T, N>& b) {
	return detail::vec_impl<T, N>::equals(a, b);
}

template <template <class, std::size_t> class A,
				 template <class, std::size_t> class B,
				 typename T, std::size_t N>
inline bool operator!=(const A<T, N>& a, const B<T, N>& b) {
	return !(a == b);
}

//----------------------------------------------
// Utility typedefs
//----------------------------------------------
using ivec2 = vector<std::size_t, 2>;
using ivec3 = vector<std::size_t, 3>;
using ivec4 = vector<std::size_t, 4>;
using vec2 = vector<float, 2>;
using vec3 = vector<float, 3>;
using vec4 = vector<float, 4>;
using dvec2 = vector<double, 2>;
using dvec3 = vector<double, 3>;
using dvec4 = vector<double, 4>;

using inorm2 = normal<std::size_t, 2>;
using inorm3 = normal<std::size_t, 3>;
using inorm4 = normal<std::size_t, 4>;
using norm2 = normal<float, 2>;
using norm3 = normal<float, 3>;
using norm4 = normal<float, 4>;
using dnorm2 = normal<double, 2>;
using dnorm3 = normal<double, 3>;
using dnorm4 = normal<double, 4>;
}
