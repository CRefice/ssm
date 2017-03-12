#pragma once

#include "ssm\arch.hpp"

namespace ssm
{
inline f128 sse_add(f128 lhs, f128 rhs) {
	return _mm_add_ps(lhs, rhs);
}

inline f128 sse_sub(f128 lhs, f128 rhs) {
	return _mm_sub_ps(lhs, rhs);
}

inline f128 sse_mul(f128 lhs, f128 rhs) {
	return _mm_mul_ps(lhs, rhs);
}

inline f128 sse_div(f128 lhs, f128 rhs) {
	return _mm_div_ps(lhs, rhs);
}

template<int a, int b, int c, int d>
inline f128 sse_sign(f128 val) {
	enum : int {
		sa = (a << 31),
		sb = (b << 31),
		sc = (c << 31),
		sd = (d << 31)
	};
	const __mm128i mask = _mm_set_epi32(sa, sb, sc, sd);
	return _mm_xor_ps(val, _mm_castsi128_ps(mask));
}

inline f128 sse_sign(f128 val) {
	return _mm_xor_ps(val, _mm_set_ps1(-0.f));
}

inline f128 sse_sqrt(f128 val) {
	return _mm_sqrt_ps(val);
}

inline f128 sse_rsqrt(f128 val) {
	return _mm_rsqrt_ps(val);
}
}
