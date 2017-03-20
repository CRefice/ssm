#include "intrin\loads.hpp"
#include "intrin\compare.hpp"
#include "intrin\geometric.hpp"

namespace ssm
{
inline vec4::vec4() : vec_data(sse_load(0.f)) {}
inline vec4::vec4(float x, float y, float z, float w) : vec_data(sse_load(w, z, y, x)) {}
inline vec4::vec4(const vec3& vec, float w) : vec_data(sse_load(w, vec.z, vec.y, vec.x)) {}
inline vec4::vec4(float val) : vec_data(sse_load_broad(val)) {}
inline vec4::vec4(f128 data) : vec_data(data) {}

inline vec4& vec4::normalize() {
	const f128 dot0 = sse_dot(vec_data, vec_data);
	const f128 rsqrt = sse_rsqrt(dot0);
	vec_data = sse_mul(vec_data, rsqrt);
	return *this;
}

inline float vec4::length() const {
	const f128 lngt = sse_length(vec_data);
	return sse_to_float(lngt);
}

inline float* vec4::data() {
	return &x;
}

inline float* vec4::begin() {
	return data();
}

inline float* vec4::end() {
	return data() + 4;
}

inline const float* vec4::data() const {
	return &x;
}

inline const float* vec4::begin() const {
	return data();
}

inline const float* vec4::end() const {
	return data() + 4;
}

inline float& vec4::operator[](int index) {
	return *(data() + index);
}

inline const float& vec4::operator[](int index) const {
	return *(data() + index);
}

inline bool vec4::operator==(const vec4& rhs) const {
	return sse_equals(vec_data, rhs.vec_data);
}

inline bool vec4::operator!=(const vec4& rhs) const {
	return !sse_equals(vec_data, rhs.vec_data);
}

inline vec4& vec4::operator+=(const vec4& rhs) {
	vec_data = sse_add(vec_data, rhs.vec_data);
	return *this;
}

inline vec4& vec4::operator-=(const vec4& rhs) {
	vec_data = sse_sub(vec_data, rhs.vec_data);
	return *this;
}

inline vec4& vec4::operator*=(const vec4& rhs) {
	vec_data = sse_mul(vec_data, rhs.vec_data);
	return *this;
}

inline vec4& vec4::operator*=(float scale) {
	const f128 scl0 = sse_load_broad(scale);
	vec_data = sse_mul(vec_data, scl0);
	return *this;
}

inline vec4::operator vec3() {
	return vec3(vec_data);
}

inline float dot(const vec4& lhs, const vec4& rhs) {
	const f128 dot0 = sse_dot(lhs.vec_data, rhs.vec_data);
	return sse_to_float(dot0);
}

inline vec4 cross(const vec4& lhs, const vec4& rhs) {
	const f128 crss = sse_cross(lhs.vec_data, rhs.vec_data);
	return vec4(crss);
}

inline vec4 normalize(vec4 vec) {
	return vec.normalize();
}

inline vec4 operator+(vec4 lhs, const vec4& rhs) {
	return lhs += rhs;
}

inline vec4 operator-(vec4 lhs, const vec4& rhs) {
	return lhs -= rhs;
}

inline vec4 operator*(vec4 lhs, const vec4& rhs) {
	return lhs *= rhs;
}

inline vec4 operator*(vec4 lhs, float scale) {
	return lhs *= scale;
}

inline vec4 operator-(const vec4& vec) {
	const f128 sign = sse_sign(vec.vec_data);
	return vec4(sign);
}
}
