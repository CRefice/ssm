#pragma once

#include "ssm\arch.hpp"

namespace ssm
{
f128 sse_or(f128 lhs, f128 rhs) {
	return _mm_or_ps(lhs, rhs);
}
f128 sse_xor(f128 lhs, f128 rhs) {
	return _mm_xor_ps(lhs, rhs);
}
f128 sse_and(f128 lhs, f128 rhs) {
	return _mm_and_ps(lhs, rhs);
}
}
