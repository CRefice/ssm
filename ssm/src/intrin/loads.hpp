#pragma once

namespace ssm
{
inline f128 sse_load(float a, float b, float c, float d) {
	return _mm_set_ps(a, b, c, d);
}

inline f128 sse_load(float val) {
	return _mm_set_ss(val);
}

inline f128 sse_load_broad(float val) {
	return _mm_set1_ps(val);
}

inline void sse_store(f128 val, float* outptr) {
	_mm_store_ps(outptr, val);
}

inline float sse_to_float(f128 val) {
	return _mm_cvtss_f32(val);
}
}
