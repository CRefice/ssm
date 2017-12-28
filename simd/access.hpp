#pragma once

#include "vector.hpp"

// Types and functions for accessing individual elements within a vector.

namespace ssm
{
namespace simd
{
namespace detail
{
template <typename T, size_t N, size_t M>
struct access_impl
{
	static simd::vector<T, N> set(simd::vector<T, N> vec, T val) {
		vec[M] = val;
		return vec;
	}

	static T get(simd::vector<T, N> vec) {
		return vec[M];
	}
};
}

template <typename T, size_t N, size_t M>
inline vector<T, N> set_element(vector<T, N> vec, T val) {
	static_assert(M < N && M >= 0, "Vector access element is out of bounds");
	return detail::access_impl<T, N, M>::set(vec, val);
}

template <typename T, size_t N, size_t M>
inline T get_element(vector<T, N> vec) {
	static_assert(M < N && M >= 0, "Vector access element is out of bounds");
	return detail::access_impl<T, N, M>::get(vec);
}

template <typename T, size_t N, size_t M>
struct accessor
{
	accessor() = default;
	accessor(T value) : vec{} {
		vec = set_element<T, N, M>(vec, value);
	}

	vector<T, N>& operator=(T value) {
		vec = set_element<T, N, M>(vec, value);
		return vec;
	}

	operator T() {
		return get_element<T, N, M>(vec);
	}

	vector<T, N> vec;
};
}
}
