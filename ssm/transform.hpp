#pragma once

#include "matrix.hpp"
#include "quaternion.hpp"

// Functions for transformations in any number of dimensions.

namespace ssm
{
// Returns an N+1-dimensional vector, with its last component set to 1.
template <typename T, std::size_t N>
inline vector<T, N + 1> homogenize(const vector<T, N>& vec) {
	vector<T, N + 1> ret;
	unroll<T, N>::assign(ret, vec);
	ret.set<N>(T(1));
	return ret;
}

// Returns an N-dimensional identity matrix.
template <typename T, std::size_t N>
inline matrix<T, N, N> identity() {
	matrix<T, N, N> ret;
	unroll<0, N>::diagonal_assign(ret, T(1));
	return ret;
}

// Note that translation requires a homogenous matrix to be represented as one,
// hence the N+1-sized matrix.
template <typename T, std::size_t N>
inline matrix<T, N + 1, N + 1> translation(const vector<T, N>& pos) {
	auto ret = identity<T, N + 1>();
	ret[N] = homogenize(pos);
	return ret;
}

template <typename T, std::size_t N>
inline matrix<T, N, N> scaling(const vector<T, N>& s) {
	auto ret = identity<T, N>();
	unroll<0, N>::diagonal_assign(ret, s);
	return ret;
}

template <typename T>
inline matrix<T, 4, 4> rotation(const quaternion<T>& rot) {
	const simd::vector<T, 4> wwww = simd::shuffle<3, 3, 3, 3>(rot.data);
	const simd::vector<T, 4> xyzw = rot.data;
	const simd::vector<T, 4> zxyw = simd::shuffle<2, 0, 1, 3>(rot.data);
	const simd::vector<T, 4> yzxw = simd::shuffle<1, 2, 0, 3>(rot.data);

	const simd::vector<T, 4> xyzw2 = simd::add(xyzw, xyzw);
	const simd::vector<T, 4> zxyw2 = simd::shuffle<3, 1, 0, 2>(xyzw2);
	const simd::vector<T, 4> yzxw2 = simd::shuffle<3, 0, 2, 1>(xyzw2);
	
	simd::vector<T, 4> tmp0 = simd::sub(simd::fill(1.f), simd::mul(yzxw2, yzxw));
	tmp0 = simd::sub(tmp0, simd::mul(zxyw2, zxyw));

	simd::vector<T, 4> tmp1 = simd::mul(yzxw2, xyzw);
	tmp1 = simd::add(tmp1, simd::mul(zxyw2, wwww));

	simd::vector<T, 4> tmp2 = simd::mul(zxyw2, xyzw);
	tmp2 = simd::sub(tmp2, simd::mul(yzxw2, wwww));

	// There's probably a better, more politically correct way of doing this...
	result[0].data = simd::load(
			0.0f,
			reinterpret_cast<float*>(&tmp2)[0],
			reinterpret_cast<float*>(&tmp1)[0],
			reinterpret_cast<float*>(&tmp0)[0]);

	result[1].data = simd::load(
			0.0f,
			reinterpret_cast<float*>(&tmp1)[1],
			reinterpret_cast<float*>(&tmp0)[1],
			reinterpret_cast<float*>(&tmp2)[1]);

	result[2].data = simd::load(
			0.0f,
			reinterpret_cast<float*>(&tmp0)[2],
			reinterpret_cast<float*>(&tmp2)[2],
			reinterpret_cast<float*>(&tmp1)[2]);

	return result;
}

template <typename T>
inline matrix<T, 4, 4> perspective(T fovy, T aspect, T znear, T zfar);
template <typename T>
inline matrix<T, 4, 4> look_at(const vector<T, 3>& eye, const vector<T, 3>& target, const normal<T, 3>& up);

inline matrix<T, 4, 4> perspective(float fovy, float aspect, float znear, float zfar) {
	const float halftan = std::tan(fovy / 2.f);

	simd::vector<T, 4> vecs[4];
	vecs[0] = simd::load(0.f, 0.f, 0.f, 1.f / aspect * halftan);
	vecs[1] = simd::load(0.f, 0.f, 1.f / halftan, 0.f);
	vecs[2] = simd::load(-1.f, -(zfar + znear) / (zfar - znear), 0.f, 0.f);
	vecs[3] = simd::load(0.f, -2 * (zfar * znear) / (zfar - znear), 0.f, 0.f);
	return mat4(vecs);
}

inline mat4 look_at(const vec3& eye, const vec3& target, const vec3& up) {
	const vec3 f = normalize(eye - target);
	const vec3 s = normalize(cross(up, f));
	const vec3 u = cross(f, s);

	simd::vector<T, 4> vecs[4];
	vecs[0] = simd::load(s.x, u.x, f.x, 0.f);
	vecs[1] = simd::load(s.y, u.y, f.y, 0.f);
	vecs[2] = simd::load(s.z, u.z, f.z, 0.f);
	vecs[3] = simd::load(-dot(s, eye), -dot(u,eye), -dot(f, eye), 1.f);
	return mat4(vecs);
}
}
