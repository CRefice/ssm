#pragma once

#include "detail/setup.hpp"
#include "detail/vector-manip.hpp"

namespace ssm
{
template <typename T, int N>
struct vector : detail::vector_data<T, N>
{
	using detail::vector_data<T, N>::vector_data;

	const T& operator[](size_t index) const {
		assert(index < N);
		return *(data_ptr(*this) + index);
	}
	T& operator[](size_t index) {
		assert(index < N);
		return *(data_ptr(*this) + index);
	}
};

template <typename T, int N>
struct normal : detail::vector_data<T, N>
{
	using detail::vector_data<T, N>::vector_data;
	operator vector<T, N>() const { return vector<T, N>(this->data); }

	const T& operator[](size_t index) const {
		assert(index < N);
		return *(data_ptr(*this) + index);
	}
	T& operator[](size_t index) {
		assert(index < N);
		return *(data_ptr(*this) + index);
	}
};

template <template <class, int> class A,
				 template <class, int> class B,
				 typename T, int N>
inline T dot(const A<T, N>& a, const B<T, N>& b) {
	return detail::vec_impl<T, N>::dot(a, b);
}

template <template <class, int> class Vec, typename T, int N>
inline T sqlength(const Vec<T, N>& vec) {
	return dot(vec, vec);
}

template <template <class, int> class Vec, typename T, int N>
inline T length(const Vec<T, N>& vec) {
	return static_cast<T>(std::sqrt(sqlength(vec)));
}

template <template <class, int> class A,
				 template <class, int> class B,
				 typename T, int N>
inline vector<T, N> cross(const A<T, N>& a, const B<T, N>& b) {
	static_assert(N == 3, "Cross product is only defined for 3D vectors");
	return vector<T, N>(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
			);
}

template <typename T, int N>
inline normal<T, N> cross(const normal<T, N>& a, const normal<T, N>& b) {
	static_assert(N == 3, "Cross product is only defined for 3D vectors");
	return normal<T, N>(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
			);
}

template <template <class, int> class Vec, typename T, int N>
inline normal<T, N> normalize(const Vec<T, N>& vec) {
	normal<T, N> norm(vec.data);
	detail::vec_impl<T, N>::normalize(norm);
	return norm;
}

template <template <class, int> class Vec,
				 typename T, int N>
inline vector<T, N>& operator+=(vector<T, N>& a, const Vec<T, N>& b) {
	detail::vec_impl<T, N>::add(a, b);
	return a;
}
template <template <class, int> class Vec,
				 typename T, int N>
inline vector<T, N>& operator-=(vector<T, N>& a, const Vec<T, N>& b) {
	detail::vec_impl<T, N>::sub(a, b);
	return a;
}
template <template <class, int> class Vec,
				 typename T, int N>
inline vector<T, N>& operator*=(vector<T, N>& a, const Vec<T, N>& b) {
	detail::vec_impl<T, N>::mul(a, b);
	return a;
}

template <template <class, int> class Vec,
	typename T, int N>
	inline vector<T, N>& operator*=(vector<T, N>& a, T b) {
	detail::vec_impl<T, N>::mul(a, b);
	return a;
}

template <template <class, int> class Vec,
				 typename T, int N>
inline vector<T, N>& operator/=(vector<T, N>& a, const Vec<T, N>& b) {
	detail::vec_impl<T, N>::div(a, b);
	return a;
}

template <typename T, int N>
inline vector<T, N>& operator/=(vector<T, N>& a, T b) {
	detail::vec_impl<T, N>::div(a, b);
	return a;
}

template <template <class, int> class A,
				 template <class, int> class B,
				 typename T, int N>
inline vector<T, N> operator+(A<T, N> a, const B<T, N>& b) {
	return static_cast<vector<T, N>&>(a) += b;
}

template <template <class, int> class A,
				 template <class, int> class B,
				 typename T, int N>
inline vector<T, N> operator-(A<T, N> a, const B<T, N>& b) {
	return static_cast<vector<T, N>&>(a) -= b;
}

template <template <class, int> class A,
				 template <class, int> class B,
				 typename T, int N>
inline vector<T, N> operator*(A<T, N> a, const B<T, N>& b) {
	return static_cast<vector<T, N>&>(a) *= b;
}

template <template <class, int> class A,
	typename T, int N>
inline vector<T, N> operator*(A<T, N> a, T b) {
	return static_cast<vector<T, N>&>(a) *= b;
}

template <template <class, int> class A,
	typename T, int N>
inline vector<T, N> operator*(T a, A<T, N> b) {
	return b * a;
}

template <template <class, int> class A,
				 template <class, int> class B,
				 typename T, int N>
inline vector<T, N> operator/(A<T, N> a, const B<T, N>& b) {
	return static_cast<vector<T, N>&>(a) /= b;
}

template <template <class, int> class A,
	typename T, int N>
inline vector<T, N> operator/(A<T, N> a, T b) {
	return static_cast<vector<T, N>&>(a) /= b;
}

template <template <class, int> class Vec, typename T, int N>
inline Vec<T, N> operator-(Vec<T, N> vec) {
	detail::vec_impl<T, N>::negate(vec);
	return vec;
}

template <template <class, int> class A,
				 template <class, int> class B,
				 typename T, int N>
inline bool operator==(const A<T, N>& a, const B<T, N>& b) {
	return detail::vec_impl<T, N>::cmp_eq(a, b);
}

template <template <class, int> class A,
				 template <class, int> class B,
				 typename T, int N>
inline bool operator!=(const A<T, N>& a, const B<T, N>& b) {
	return !(a == b);
}

//----------------------------------------------
// Utility typedefs
//----------------------------------------------
using ivec2 = vector<int, 2>;
using ivec3 = vector<int, 3>;
using ivec4 = vector<int, 4>;
using vec2 = vector<float, 2>;
using vec3 = vector<float, 3>;
using vec4 = vector<float, 4>;
using dvec2 = vector<double, 2>;
using dvec3 = vector<double, 3>;
using dvec4 = vector<double, 4>;

using inorm2 = normal<int, 2>;
using inorm3 = normal<int, 3>;
using inorm4 = normal<int, 4>;
using norm2 = normal<float, 2>;
using norm3 = normal<float, 3>;
using norm4 = normal<float, 4>;
using dnorm2 = normal<double, 2>;
using dnorm3 = normal<double, 3>;
using dnorm4 = normal<double, 4>;
}
