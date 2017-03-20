#include "intrin\loads.hpp"
#include "intrin\compare.hpp"
#include "intrin\geometric.hpp"
#include "intrin\logical.hpp"

namespace ssm
{
inline vec3::vec3() : vec_data(sse_load(0.f)) {}
inline vec3::vec3(float x, float y, float z) : vec_data(sse_load(0.f, z, y, x)) {}
inline vec3::vec3(float val) : vec_data(sse_load(0.f, val, val, val)) {}
inline vec3::vec3(f128 vals) {
	const __m128i mask =_mm_set_epi32(0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
	vec_data = sse_and(vals, _mm_castsi128_ps(mask));
}

inline vec3& vec3::normalize() {
	const f128 dot0 = sse_dot(vec_data, vec_data);
	const f128 rsqrt = sse_rsqrt(dot0);
	vec_data = sse_mul(vec_data, rsqrt);
	return *this;
}

inline float vec3::length() const {
	const f128 lngt = sse_length(vec_data);
	return sse_to_float(lngt);
}

inline float* vec3::data() {
	return &x;
}

inline float* vec3::begin() {
	return data();
}

inline float* vec3::end() {
	return data() + 3;
}

inline const float* vec3::data() const {
	return &x;
}

inline const float* vec3::begin() const {
	return data();
}

inline const float* vec3::end() const {
	return data() + 3;
}

inline float& vec3::operator[](unsigned int index) {
	return *(data() + index);
}

inline const float& vec3::operator[](unsigned int index) const {
	return *(data() + index);
}

inline bool vec3::operator==(const vec3& rhs) const {
	return sse_equals(vec_data, rhs.vec_data);
}

inline bool vec3::operator!=(const vec3& rhs) const {
	return !sse_equals(vec_data, rhs.vec_data);
}

inline vec3& vec3::operator+=(const vec3& rhs) {
	vec_data = sse_add(vec_data, rhs.vec_data);
	return *this;
}

inline vec3& vec3::operator-=(const vec3& rhs) {
	vec_data = sse_sub(vec_data, rhs.vec_data);
	return *this;
}

inline vec3& vec3::operator*=(const vec3& rhs) {
	vec_data = sse_mul(vec_data, rhs.vec_data);
	return *this;
}

inline vec3& vec3::operator*=(float scale) {
	const f128 scl0 = sse_load_broad(scale);
	vec_data = sse_mul(vec_data, scl0);
	return *this;
}

inline float dot(const vec3& lhs, const vec3& rhs) {
	const f128 dot0 = sse_dot(lhs.vec_data, rhs.vec_data);
	return sse_to_float(dot0);
}

inline vec3 cross(const vec3& lhs, const vec3& rhs) {
	const f128 vals = sse_cross(lhs.vec_data, rhs.vec_data);
	return vec3(vals);
}

inline vec3 normalize(vec3 vec) {
	return vec.normalize();
}

inline vec3 operator+(vec3 lhs, const vec3& rhs) {
	return lhs += rhs;
}

inline vec3 operator-(vec3 lhs, const vec3& rhs) {
	return lhs -= rhs;
}

inline vec3 operator*(vec3 lhs, const vec3& rhs) {
	return lhs *= rhs;
}

inline vec3 operator*(vec3 lhs, float scale) {
	return lhs *= scale;
}

inline vec3 operator-(const vec3& vec) {
	const f128 mask = sse_load(0.f, -0.f, -0.f, -0.f);
	const f128 sign = sse_xor(vec.vec_data, mask);
	return vec3(sign);
}
}
