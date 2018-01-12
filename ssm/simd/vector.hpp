#pragma once

#include <array>

namespace ssm
{
namespace simd
{
template <typename T, size_t N>
struct make_vector
{
	using type = std::array<T, N>;
};

template <typename T, size_t N>
using vector = typename make_vector<T, N>::type;

template <typename T>
vector<T, 2> set_vector(T x, T y) { return vector<T, 2>({ x, y }); }

template <typename T>
vector<T, 3> set_vector(T x, T y, T z) { return vector<T, 3>({ x, y, z }); }

template <typename T>
vector<T, 4> set_vector(T x, T y, T z, T w) { return vector<T, 4>({ x, y, z, w }); }

template <typename T, size_t N>
vector<T, N> set_wide(T val) {
	vector<T, N> ret;
	for (int i = 0; i < N; ++i)
		ret[i] = val;
	return ret;
}
}
}
