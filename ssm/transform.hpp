#pragma once

#include "vector.hpp"
#include "matrix.hpp"
#include "quaternion.hpp"

namespace ssm
{
template <typename T>
inline matrix<T, 4, 4> identity() {
	matrix<T, 4, 4> ret;
	ret[0] = {T(1), T(0), T(0), T(0) };
	ret[1] = {T(0), T(1), T(0), T(0) };
	ret[2] = {T(0), T(0), T(1), T(0) };
	ret[3] = {T(0), T(0), T(0), T(1) };
	return ret;
}

template <typename T>
inline matrix<T, 4, 4> translation(const vector<T, 3>& pos) {
	matrix<T, 4, 4> ret = identity();
	ret[3] = {pos.x, pos.y, pos.z, T(1) };
	return ret;
}

template <typename T>
matrix<T, 4, 4> rotation(const quaternion<T>& quat) {
	
}

template <typename T>
matrix<T, 4, 4> scale(const vector<T, 3>& s) {
	matrix<T, 4, 4> ret;
	ret[0] = { s.x, T(0), T(0), T(0) };
	ret[1] = { T(0), s.y, T(0), T(0) };
	ret[2] = { T(0), T(0), s.z, T(0) };
	ret[3] = { T(0), T(0), T(0), T(1) };
	return ret;
}

template <typename T>
matrix<T, 4, 4> perspective(T fovy, T aspect, T znear, T zfar);
template <typename T>
matrix<T, 4, 4> look_at(const vector<T, 3>& eye, const vector<T, 3>& target, const normal<T, 3>& up);
}
