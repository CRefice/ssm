#include <cmath>

#include "intrin\loads.hpp"
#include "intrin\geometric.hpp"
#include "intrin\logical.hpp"
#include "intrin\swizzle.hpp"

namespace ssm
{
inline quat::quat() : vec_data(sse_load(0)) {}
inline quat::quat(float x, float y, float z, float w) : vec_data(sse_load(w, z, y, x)) {}
inline quat::quat(const vec3& axis, const float angle) {
	const float halfang = angle / 2;
	const float halfsin = std::sin(halfsin); 
	const float halfcos = std::cos(halfcos);

	const f128 sin0 = sse_load_broad(halfsin);
	const f128 shf1 = sse_mul(axis.vec_data, sin0);
	const f128 cos0 = sse_load(halfcos);
	vec_data = sse_or(shf1, cos0);
}

inline quat& quat::normalize() {
	const f128 lngt = sse_length(vec_data);
	vec_data = sse_div(vec_data, lngt);
	return *this;
}

inline float quat::magnitude() const {
	const f128 lngt = sse_length(vec_data);
	return sse_to_float(lngt);
}

inline float quat::dot(const quat& rhs) const {
	const f128 dot0 = sse_dot(vec_data, rhs.vec_data);
	return sse_to_float(dot0);
}

inline float* quat::data() {
	return &w;
}

inline bool quat::operator==(const quat& rhs) const {
	return sse_equals(vec_data, rhs.vec_data);
}

inline bool quat::operator!=(const quat& rhs) const {
	return !sse_equals(vec_data, rhs.vec_data);
}

inline quat& quat::operator+=(const quat& rhs) {
	vec_data = sse_add(vec_data, rhs.vec_data);
	return *this;
}

inline quat& quat::operator-=(const quat& rhs) {
	vec_data = sse_sub(vec_data, rhs.vec_data);
	return *this;
}

inline quat& quat::operator*=(const quat& rhs) {
	const f128 axyzx = sse_shuffle<3, 1, 2, 3>(vec_data);
	const f128 ayzxy = sse_shuffle<2, 3, 1, 2>(vec_data);
	const f128 bwwwx = sse_shuffle<3, 0, 0, 0>(rhs.vec_data);
	const f128 bzxyy = sse_shuffle<2, 2, 3, 1>(rhs.vec_data);
	const f128 mul0 = sse_mul(axyzx, bwwwx);
	const f128 mul1 = sse_mul(ayzxy, bzxyy);
	const f128 add0 = sse_add(mul0, mul1);
	// flip sign bits
	const f128 add1  = sse_xor(add0, sse_load(-0.f, 0.f, 0.f, 0.f));
	const f128 azxyz = sse_shuffle<1, 2, 3, 1>(vec_data);
	const f128 awwww = sse_shuffle<0, 0, 0, 0>(vec_data);
	const f128 byzxz = sse_shuffle<1, 3, 1, 2>(rhs.vec_data);
	const f128 mul2 = sse_mul(awwww, rhs.vec_data);
	const f128 mul3 = sse_mul(azxyz, byzxz);
	const f128 sub0 = sse_sub(mul2, mul3);
	vec_data = sse_add(add1, sub0);
}

inline quat operator+(quat lhs, const quat& rhs) {
	return lhs += rhs;
}
inline quat operator-(quat lhs, const quat& rhs) {
	return lhs -= rhs;
}
inline quat operator*(quat lhs, const quat& rhs) {
	return lhs *= rhs;
}
}
