#pragma once

#include "vector.hpp"

namespace ssm
{
template <typename T, int M, int N>
struct matrix
{
	using value_type = vector<T, M>;

	matrix() = default;
	matrix(T val) {
		for (int i = 0; i < N; ++i)
			data[i] = val;
	}

	vector<T, M>& operator[](int index) {
		assert(index < N);
		return data[index];
	}
	const vector<T, M>& operator[](int index) const {
		assert(index < N);
		return data[index];
	}

	vector<T, M> data[N] = { 0.f };
};

template <typename T, int M, int N>
inline matrix<T, M, N> operator-(matrix<T, M, N> mat) {
	detail::unroll<0, N>::negate(mat);
	return mat;
}

template <typename T, int M, int N>
inline matrix<T, M, N>& operator+=(matrix<T, M, N>& a, const matrix<T, M, N>& b) {
	detail::unroll<0, N>::add(a, b);
	return a;
}

template <typename T, int M, int N>
inline matrix<T, M, N>& operator-=(matrix<T, M, N>& a, const matrix<T, M, N>& b) {
	detail::unroll<0, N>::sub(a, b);
	return a;
}

template <typename T, int M, int N>
inline matrix<T, M, N> operator+(matrix<T, M, N> a, const matrix<T, M, N>& b) { return a += b; }
template <typename T, int M, int N>
inline matrix<T, M, N> operator-(matrix<T, M, N> a, const matrix<T, M, N>& b) { return a -= b; }


template <typename T, int M, int N>
inline bool operator==(const matrix<T, M, N>& a, const matrix<T, M, N>& b) {
	return detail::unroll<0, N>::equals(a, b);
}

template <typename T, int M, int N>
inline bool operator!=(const matrix<T, M, N>& a, const matrix<T, M, N>& b) {
	return !(a == b);
}

#include "detail/matrix-mul.inl"

template <typename T, int M, int N>
inline vector<T, M> operator*(const vector<T, N>& vec, const matrix<T, M, N>& mat) {
	return detail::mat_unroll<0, N>::mul(vec, mat);
}

template <typename T, int M, int N>
inline vector<T, M> operator*(const matrix<T, M, N>& mat, const vector<T, N>& vec) {
	return vec * mat;
}

template <typename T, int M, int N>
inline matrix<T, M, N>& operator*=(matrix<T, M, N>& mat, T val) {
	detail::unroll<0, N>::mul(mat, ssm::vector<T, M>(val));
	return mat;
}

template <typename T, int N>
inline vector<T, N>& operator*=(vector<T, N>& vec, const matrix<T, N, N>& mat) {
	vec = detail::mat_unroll<0, N>::mul(vec, mat);
	return vec;
}

template <typename T, int M, int N>
inline matrix<T, M, N> operator*(matrix<T, M, N> mat, T val) {
	return mat *= val;
}

template <typename T, int M, int N>
inline matrix<T, M, N> operator*(T val, matrix<T, M, N> mat) {
	return mat *= val;
}

template <typename T, int M, int N, int P>
inline matrix<T, M, P> operator*(const matrix<T, M, N>& a, const matrix<T, N, P> b) {
	matrix<T, M, P> ret;
	detail::mat_unroll<0, P>::mul(ret, a, b);
	return ret;
}

template <typename T, int N>
inline matrix<T, N, N>& operator*=(matrix<T, N, N>& a, const matrix<T, N, N> b) {
	a = a * b;
	return a;
}

template <typename T, int M, int N>
inline matrix<T, M, N>& operator/=(matrix<T, M, N>& mat, T val) {
	detail::unroll<0, N>::div(mat, ssm::vector<T, M>(val));
	return mat;
}

template <typename T, int M, int N>
inline matrix<T, M, N> operator/(matrix<T, M, N> mat, T val) {
	return mat /= val;
}

//----------------------------------------------
// Utility typedefs
//----------------------------------------------
using imat2 = matrix<int, 2, 2>;
using imat3 = matrix<int, 3, 3>;
using imat4 = matrix<int, 4, 4>;
using mat2 = matrix<float, 2, 2>;
using mat3 = matrix<float, 3, 3>;
using mat4 = matrix<float, 4, 4>;
using dmat2 = matrix<double, 2, 2>;
using dmat3 = matrix<double, 3, 3>;
using dmat4 = matrix<double, 4, 4>;
}
