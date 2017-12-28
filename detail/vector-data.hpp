#pragma once

#include "type-traits.hpp"
#include "../simd/access.hpp"

namespace ssm
{
namespace detail
{
template <typename T, int N, int M, typename Simd = void>
struct make_component
{
	using type = T;
};
template <typename T, int N, int M>
struct make_component<T, N, enable_if_t<is_simd<T, N>::value, void>>
{
	using type = simd::vector_accessor<T, N, M>;
};
template <typename T, int N, int M>
using component = make_component<T, N, M>::type;
}

template <typename T, size_t N>
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
