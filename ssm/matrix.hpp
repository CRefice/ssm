#pragma once

#include "vector.hpp"

namespace ssm
{
template <typename T, int M, int N>
struct matrix
{
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
	detail::unroll<matrix<T, M, N>, 0, N>::negate(mat);
	return mat;
}

template <typename T, int M, int N>
inline matrix<T, M, N>& operator+=(matrix<T, M, N>& a, const matrix<T, M, N>& b) {
	detail::unroll<matrix<T, M, N>, 0, N>::add(a, b);
	return a;
}

template <typename T, int M, int N>
inline matrix<T, M, N>& operator-=(matrix<T, M, N>& a, const matrix<T, M, N>& b) {
	detail::unroll<matrix<T, M, N>, 0, N>::sub(a, b);
	return a;
}

template <typename T, int M, int N>
inline matrix<T, M, N> operator+(matrix<T, M, N> a, const matrix<T, M, N>& b) { return a += b; }
template <typename T, int M, int N>
inline matrix<T, M, N> operator-(matrix<T, M, N> a, const matrix<T, M, N>& b) { return a -= b; }

#include "detail/matrix-mul.inl"

template <typename T, int M, int N>
inline vector<T, M> operator*(const vector<T, N>& vec, const matrix<T, M, N>& mat) {
	return detail::mat_unroll<T, M, N>::mul<N - 1>(vec, mat);
}

template <typename T, int M, int N>
inline vector<T, M> operator*(const matrix<T, M, N>& mat, const vector<T, N>& vec) {
	return vec * mat;
}

template <typename T, int N>
inline vector<T, N>& operator*=(vector<T, N>& vec, const matrix<T, N, N>& mat) {
	vec = detail::mat_unroll<T, N, N>::mul<N - 1>(vec, mat);
	return vec;
}

template <typename T, int M, int N, int P>
inline matrix<T, M, P> operator*(const matrix<T, M, N>& a, const matrix<T, N, P> b) {
	matrix<T, M, P> ret;
	detail::mat_unroll<T, M, N, P>::mul<P - 1>(ret, a, b);
	return ret;
}

template <typename T, int N>
inline matrix<T, N, N>& operator*=(matrix<T, N, N>& a, const matrix<T, N, N> b) {
	a = a * b;
	return a;
}
}