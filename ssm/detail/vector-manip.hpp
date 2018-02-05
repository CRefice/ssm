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
		return unroll<0, N>::dot(a.data, b.data);
	}

	static inline void normalize(vector_data<T, N>& vec) {
		T length = static_cast<T>(std::sqrt(unroll<0, N>::dot(vec.data, vec.data)));
		unroll<0, N>::div(vec.data, length);
	}

	static inline void add(vector_data<T, N>& a, const vector_data<T, N>& b) {
		unroll<0, N>::add(a.data, b.data);
	}

	static inline void sub(vector_data<T, N>& a, const vector_data<T, N>& b) {
		unroll<0, N>::sub(a.data, b.data);
	}

	static inline void mul(vector_data<T, N>& a, const vector_data<T, N>& b) {
		unroll<0, N>::mul(a.data, b.data);
	}

	static inline void mul(vector_data<T, N>& a, T b) {
		unroll<0, N>::mul(a.data, b);
	}

	static inline void div(vector_data<T, N>& a, const vector_data<T, N>& b) {
		unroll<0, N>::div(a.data, b.data);
	}

	static inline void div(vector_data<T, N>& a, T b) {
		unroll<0, N>::div(a.data, b);
	}

	static inline void negate(vector_data<T, N>& vec) {
		unroll<0, N>::negate(vec.data);
	}

	static inline void quat_mul(vector_data<T, N>& a, const vector_data<T, N>& b) {
		a.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
		a.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
		a.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
		a.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
	}

	static inline void quat_conjugate(vector_data<T, 4>& a) {
		// Negate all members but the real part (w)
		unroll<0, 3>::negate(a.data);
	}

	static inline bool cmp_eq(const vector_data<T, N>& a, const vector_data<T, N>& b) {
		return unroll<0, N>::equal(a.data, b.data);
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

	static inline void mul(vector_data<T, N>& a, T b) {
		a.data = simd::mul(a.data, simd::set_wide<T, N>(b));
	}

	static inline void div(vector_data<T, N>& a, const vector_data<T, N>& b) {
		a.data = simd::div(a.data, b.data);
	}

	static inline void div(vector_data<T, N>& a, T b) {
		a.data = simd::mul(a.data, simd::set_wide<T, N>(b));
	}

	static inline void negate(vector_data<T, N>& a) {
		a.data = simd::negate(a.data);
	}

	static inline void quat_mul(vector_data<T, 4>& a, const vector_data<T, 4>& b) {
		const simd::vector<T, 4> awwww = simd::shuffle<3>(a.data);
		const simd::vector<T, 4> axyzx = simd::shuffle<0, 1, 2, 0>(a.data);
		const simd::vector<T, 4> ayzxy = simd::shuffle<1, 2, 0, 1>(a.data);
		const simd::vector<T, 4> azxyz = simd::shuffle<2, 0, 1, 2>(a.data);
		const simd::vector<T, 4> bwwwx = simd::shuffle<3, 3, 3, 0>(b.data);
		const simd::vector<T, 4> bzxyy = simd::shuffle<2, 0, 1, 1>(b.data);
		const simd::vector<T, 4> byzxz = simd::shuffle<1, 2, 0, 2>(b.data);
		// Data dependency on 1 and 2, do them first
		const simd::vector<T, 4> mul1 = simd::mul(axyzx, bwwwx);
		const simd::vector<T, 4> mul2 = simd::mul(ayzxy, bzxyy);
		const simd::vector<T, 4> mul0 = simd::mul(awwww, b.data);
		const simd::vector<T, 4> mul3 = simd::mul(azxyz, byzxz);
		const simd::vector<T, 4> add0 = simd::add(mul1, mul2);
		const simd::vector<T, 4> sub0 = simd::sub(mul0, mul3);
		const simd::vector<T, 4> sign = simd::negate<0, 0, 0, 1>(add0);
		a.data = simd::add(sub0, sign);
	}

	static inline void quat_conjugate(vector_data<T, 4>& a) {
		a.data = simd::negate<1, 1, 1, 0>(a.data);
	}

	static inline bool cmp_eq(const vector_data<T, N>& a, const vector_data<T, N>& b) {
		const simd::vector<T, N> cmp = simd::cmp_eq(a.data, b.data);
		return static_cast<bool>(simd::get_element<T, N, 0>(cmp));
	}
};
}
}
