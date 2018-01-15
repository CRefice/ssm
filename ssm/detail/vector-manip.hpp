#pragma once

// This class contains the bare minimum implementation needed to derive other vector functions.
// Why is it a class and not a bunch of free functions? I'm glad you asked!
// The idea is to allow to choose between the scalar and vectorized version based on whether there exists an appropriate SSE data type, so as to achieve optimal efficiency both if it does and if it doesn't.
// Unfortunately, C++ does not allow partial specialization of template functions.
// So the next best thing is to emulate it using a class with only static member functions, and then call those from free functions.
// It's a complicated mess, but it should be completely transparent to the end user
#include <cmath>

#include "loop-unroll.hpp"
#include "type-traits.hpp"
#include "vector-data.hpp"
#include "../simd/types.hpp"

namespace ssm
{
namespace detail
{
template <typename T, int N, typename = void>
struct vec_impl
{
	static inline T dot(const vector_data<T, N>& a, const vector_data<T, N>& b) {
		return unroll<simd::vector<T, N>, 0, N>::dot(a.data, b.data);
	}

	static inline void normalize(vector_data<T, N>& vec) {
		T length = static_cast<T>(std::sqrt(unroll<simd::vector<T, N>, 0, N>::dot(vec, vec)));
		unroll<simd::vector<T, N>, 0, N>::div(vec.data, length);
	}

	static inline void add(vector_data<T, N>& a, const vector_data<T, N>& b) {
		unroll<simd::vector<T, N>, 0, N>::add(a.data, b.data);
	}

	static inline void sub(vector_data<T, N>& a, const vector_data<T, N>& b) {
		unroll<simd::vector<T, N>, 0, N>::sub(a.data, b.data);
	}

	static inline void mul(vector_data<T, N>& a, const vector_data<T, N>& b) {
		unroll<simd::vector<T, N>, 0, N>::mul(a.data, b.data);
	}

	static inline void div(vector_data<T, N>& a, const vector_data<T, N>& b) {
		unroll<simd::vector<T, N>, 0, N>::div(a.data, b.data);
	}

	static inline void negate(vector_data<T, N>& vec) {
		unroll<simd::vector<T, N>, 0, N>::negate(vec.data);
	}

	static inline bool cmp_eq(const vector_data<T, N>& a, const vector_data<T, N>& b) {
		return unroll<simd::vector<T, N>, 0, N>::cmp_eq(a.data, b.data);
	}
};

template <typename T, int N>
struct vec_impl<T, N, enable_if_t<simd::is_simd<T, N>::value, void>>
{
	static inline T dot(const vector_data<T, N>& a, const vector_data<T, N>& b) {
		return simd::get_element<T, N, 0>(simd::dot(a.data, b.data));
	}

	static inline void normalize(vector_data<T, N>& vec) {
		const simd::vector<T, N> sqlen = simd::dot(vec.data, vec.data);
		const simd::vector<T, N> rsqrt = simd::rsqrt(sqlen);
		vec.data = simd::mul(vec.data, rsqrt);
	}

	static inline void add(vector_data<T, N>& a, const vector_data<T, N>& b) {
		a.data = simd::add(a.data, b.data);
	}

	static inline void sub(vector_data<T, N>& a, const vector_data<T, N>& b) {
		a.data = simd::sub(a.data, b.data);
	}

	static inline void mul(vector_data<T, N>& a, const vector_data<T, N>& b) {
		a.data = simd::mul(a.data, b.data);
	}

	static inline void div(vector_data<T, N>& a, const vector_data<T, N>& b) {
		a.data = simd::div(a.data, b.data);
	}

	static inline void negate(vector_data<T, N>& a) {
		a.data = simd::negate(a.data);
	}

	static inline bool cmp_eq(const vector_data<T, N>& a, const vector_data<T, N>& b) {
		const simd::vector<T, N> cmp = simd::cmp_eq(a.data, b.data);
		return static_cast<bool>(simd::get_element<T, N, 0>(cmp));
	}
};
}
}
