#pragma once

#include "../simd/access.hpp"

namespace ssm
{
namespace detail
{
template <typename T, int N>
struct vector_data {
	vector_data() = default;
	vector_data(simd::vector<T, N> data) : data(data) {}
	simd::vector<T, N> data;
};

template <typename T>
struct vector_data<T, 2> {
	vector_data() = default;
	vector_data(simd::vector<T, 2> data) : data(data) {}
	vector_data(T x, T y) : data(simd::set_vector(x, y)) {}
	vector_data(T xy) : data(simd::set_wide<T, 2>(xy)) {}

	union {
		simd::accessor<T, 2, 0> x;
		simd::accessor<T, 2, 1> y;
		simd::vector<T, 2> data;
	};
};

template <typename T>
struct vector_data<T, 3> {
	vector_data() = default;
	vector_data(simd::vector<T, 3> data) : data(data) {}
	vector_data(T x, T y, T z) : data(simd::set_vector(x, y, z)) {}
	vector_data(T xyz) : data(simd::set_wide<T, 2>(xyz)) {}

	union {
		simd::accessor<T, 3, 0> x;
		simd::accessor<T, 3, 1> y;
		simd::accessor<T, 3, 2> z;
		simd::vector<T, 3> data;
	};
};

template <typename T>
struct vector_data<T, 4> {
	vector_data() = default;
	vector_data(simd::vector<T, 4> data) : data(data) {}
	vector_data(T x, T y, T z, T w) : data(simd::set_vector(x, y, z, w)) {}
	vector_data(T xyzw) : data(simd::set_wide<T, 4>(xyzw)) {}

	union {
		simd::accessor<T, 4, 0> x;
		simd::accessor<T, 4, 1> y;
		simd::accessor<T, 4, 2> z;
		simd::accessor<T, 4, 3> w;
		simd::vector<T, 4> data;
	};
};
}

//---------------------------
// Data-accessing functions
//---------------------------
template <typename T, int N>
T* data_ptr(detail::vector_data<T, N>& vec) { return reinterpret_cast<T*>(&(vec.data)); }
template <typename T, int N>
T* begin(detail::vector_data<T, N>& vec) { return data_ptr(vec); }
template <typename T, int N>
T* end(detail::vector_data<T, N>& vec) { return data_ptr(vec) + N; }

template <typename T, int N>
const T* data_ptr(const detail::vector_data<T, N>& vec) {
	return reinterpret_cast<const T*>(&(vec.data));
}
template <typename T, int N>
const T* begin(const detail::vector_data<T, N>& vec) { return data_ptr(vec); }
template <typename T, int N>
const T* end(const detail::vector_data<T, N>& vec) { return data_ptr(vec) + N; }
}
