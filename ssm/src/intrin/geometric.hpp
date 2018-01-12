#pragma once

#include "arithmetic.hpp"
#include "swizzle.hpp"

namespace ssm
{
inline f128 sse_dot(f128 lhs, f128 rhs) {
	const f128 mul0 = sse_mul(lhs, rhs);
	const f128 swp0 = sse_shuffle<2, 3, 0, 1>(mul0);
	const f128 add0 = sse_add(mul0, swp0);
	const f128 swp1 = sse_shuffle<0, 1, 2, 3>(add0);
	const f128 add1 = sse_add(add0, swp1);
	return add1;
}

inline f128 sse_length(f128 val) {
	const f128 dot0 = sse_dot(val, val);
	const f128 rsqt = sse_rsqrt(dot0);
	return sse_mul(val, rsqt);
}

inline f128 sse_cross(f128 lhs, f128 rhs) {
	const f128 swp0 = sse_shuffle<3, 0, 2, 1>(lhs);
	const f128 swp1 = sse_shuffle<3, 1, 0, 2>(lhs);
	const f128 swp2 = sse_shuffle<3, 0, 2, 1>(rhs);
	const f128 swp3 = sse_shuffle<3, 1, 0, 2>(rhs);
	const f128 mul0 = sse_mul(swp0, swp3);
	const f128 mul1 = sse_mul(swp1, swp2);
	const f128 sub0 = sse_sub(mul0, mul1);
	return sub0;
}
}
