#pragma once

#include "ssm\arch.hpp"

namespace ssm
{
template<unsigned int mask>
inline f128 sse_shuffle(f128 val) {
	return _mm_shuffle_ps(val, val, mask);
}
template<unsigned int f3, unsigned int f2, unsigned int f1, unsigned int f0>
inline f128 sse_shuffle(f128 val) {
	return _mm_shuffle_ps(val, val, ((f3 << 6) | (f2 << 4) | (f1 << 2) | f0));
}
}
