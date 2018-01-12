#include "intrin\loads.hpp"
#include "intrin\compare.hpp"
#include "intrin\geometric.hpp"
#include "intrin\logical.hpp"

namespace ssm
{
inline vec2::vec2() : vec_data(sse_load(0.f)) {}
inline vec2::vec2(float x, float y) : vec_data(sse_load(0.f, 0.f, y, x)) {}
inline vec2::vec2(float val) : vec_data(sse_load(0.f, 0.f, val, val)) {}
inline vec2::vec2(f128 vals) {
	const __m128i mask =_mm_set_epi32(0, 0, 0xFFFFFFFF, 0xFFFFFFFF);
	vec_data = sse_and(vals, _mm_castsi128_ps(mask));
}

inline vec2& vec2::normalize() {
	const f128 dot0 = sse_dot(vec_data, vec_data);
	const f128 rsqrt = sse_rsqrt(dot0);
	vec_data = sse_mul(vec_data, rsqrt);
	return *this;
}

inline float vec2::length() const {
	const f128 lngt = sse_length(vec_data);
	return sse_to_float(lngt);
}

inline float* vec2::data() {
	return &x;
}

inline float* vec2::begin() {
	return data();
}

inline float* vec2::end() {
	return data() + 2;
}

inline const float* vec2::data() const {
	return &x;
}

inline const float* vec2::begin() const {
	return data();
}

inline const float* vec2::end() const {
	return data() + 2;
}

inline float& vec2::operator[](unsigned int index) {
	return *(data() + index);
}

inline const float& vec2::operator[](unsigned int index) const {
	return *(data() + index);
}

inline bool vec2::operator==(const vec2& rhs) const {
	return sse_equals(vec_data, rhs.vec_data);
}

inline bool vec2::operator!=(const vec2& rhs) const {
	return !sse_equals(vec_data, rhs.vec_data);
}

inline vec2& vec2::operator+=(const vec2& rhs) {
	vec_data = sse_add(vec_data, rhs.vec_data);
	return *this;
}

inline vec2& vec2::operator-=(const vec2& rhs) {
	vec_data = sse_sub(vec_data, rhs.vec_data);
	return *this;
}

inline vec2& vec2::operator*=(const vec2& rhs) {
	vec_data = sse_mul(vec_data, rhs.vec_data);
	return *this;
}

inline vec2& vec2::operator*=(float scale) {
	const f128 scl0 = sse_load_broad(scale);
	vec_data = sse_mul(vec_data, scl0);
	return *this;
}

inline float dot(const vec2& lhs, const vec2& rhs) {
	const f128 dot0 = sse_dot(lhs.vec_data, rhs.vec_data);
	return sse_to_float(dot0);
}

inline vec2 cross(const vec2& lhs, const vec2& rhs) {
	const f128 vals = sse_cross(lhs.vec_data, rhs.vec_data);
	return vec2(vals);
}

inline vec2 normalize(vec2 vec) {
	return vec.normalize();
}

inline vec2 operator+(vec2 lhs, const vec2& rhs) {
	return lhs += rhs;
}

inline vec2 operator-(vec2 lhs, const vec2& rhs) {
	return lhs -= rhs;
}

inline vec2 operator*(vec2 lhs, const vec2& rhs) {
	return lhs *= rhs;
}

inline vec2 operator*(vec2 lhs, float scale) {
	return lhs *= scale;
}

inline vec2 operator-(const vec2& vec) {
	const f128 mask = sse_load(0.f, 0.f, -0.f, -0.f);
	const f128 sign = sse_xor(vec.vec_data, mask);
	return vec2(sign);
}
}
