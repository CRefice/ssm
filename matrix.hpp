#pragma once

#include "vector.hpp"

namespace ssm
{
template <typename T, size_t M, size_t N>
struct matrix
{
	explicit matrix(T scale) {
		for (int i = 0; i < (M < N ? M : N); ++i)
			data[i][i] = scale;
	}
	matrix() : matrix(T{1}) {}

	vector<T, M>& operator[](size_t index) {
		assert(index < N);
		return data[index];
	}
	const vector<T, M>& operator[](size_t index) const {
		assert(index < N);
		return data[index];
	}

	vector<T, M> data[N];
};

template <typename T, size_t M, size_t P>
inline vector<T, M> operator*(const matrix<T, M, N>& mat, const vector<T, P>& vec) {
	static_assert(N == P, "Matrix-vector multiplication is only defined if size(vec) == columns(mat)");
	vector<T, M> result;
	for (int i = 0; i < N; ++i)
		result += mat[i] * vec[i];
	return result;
}

template <typename T, size_t M, size_t N, size_t Q, size_t P>
inline matrix<T, M, P> operator*(const matrix<T, M, N>& a, const matrix<T, Q, P> b) {
	static_assert(N == Q, "Matrix multiplication is only defined if rows(a) == columns(b)");
	matrix<T, M, P> result;
	for (int i = 0; i < P; ++i)
		result[i] = a * b[i];
	return a;
}
}
