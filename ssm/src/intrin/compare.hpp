#pragma once

#include "ssm\arch.hpp"

namespace ssm
{
inline bool sse_equals(f128 lhs, f128 rhs) {
	const __m128i li = _mm_castps_si128(lhs), ri = _mm_castps_si128(rhs);
	const __m128i vcmp = _mm_cmpeq_epi8(li, ri);
	const short mmsk = _mm_movemask_epi8(vcmp);
	return mmsk == 0xffff;
}
}
