#pragma once

// This class contains the bare minimum implementation needed to derive other vector functions.
// Why is it a class and not a bunch of free functions? I'm glad you asked!
// The idea is to allow to choose between the scalar and vectorized version based on whether there exists an appropriate SSE data type, so as to achieve optimal efficiency both if it does and if it doesn't.
// Unfortunately, C++ does not allow partial specialization of template functions.
// So the next best thing is to emulate it using a class with only static member functions, and then call those from free functions.
// It's a complicated mess, but it should be completely transparent to the end user

#include "vector-data.hpp"
#include "type-traits.hpp"

namespace ssm
{
namespace detail
{
template <typename T, size_t N, typename Simd = void>
struct vec_impl
{
	static inline T dot(const vector_data<T, N>& a, const vector_data<T, N>& b) {
		return unroll::dot<N - 1>(a, b);
	}

	static inline void normalize(vector_data<T, N>& vec) {
		T length = static_cast<T>(std::sqrt(unroll::dot<N - 1>(vec, vec)));
		unroll::div<N - 1>(vec, length);
	}

	static inline void add(vector_data<T, N>& a, const vector_data<T, N>& b) {
		unroll::add<N - 1>(a, b);
	}

	static inline void sub(vector_data<T, N>& a, const vector_data<T, N>& b) {
		unroll::sub<N - 1>(a, b);
	}

	static inline void mul(vector_data<T, N>& a, const vector_data<T, N>& b) {
		unroll::mul<N - 1>(a, b);
	}

	static inline void div(vector_data<T, N>& a, const vector_data<T, N>& b) {
		unroll::div<N - 1>(a, b);
	}

	static inline void negate(vector_data<T, N>& vec) {
		unroll::negate<N - 1>(vec);
	}

	static inline bool cmp_eq(const vector_data<T, N>& a, const vector_data<T, N>& b) {
		return unroll::cmp_eq<N - 1>(a, b);
	}

private:
	struct unroll
	{
		template <int M>
		static inline T dot(const vector_data<T, N>& a, const vector_data<T, N>& b) {
			return a.data[M] * b.data[M] + dot<M - 1>(a, b);
		}
		template <>
		static inline T dot<0>(const vector_data<T, N>& a, const vector_data<T, N>& b) {
			return a.data[0] * b.data[0];
		}

		template <int M>
		static inline void add(vector_data<T, N>& a, const vector_data<T, N>& b) {
			a.data[M] += b.data[M];
			add<M - 1>(a, b);
		}
		template <>
		static inline void add<0>(vector_data<T, N>& a, const vector_data<T, N>& b) {
			a.data[0] += b.data[0];
		}

		template <int M>
		static inline void sub(vector_data<T, N>& a, const vector_data<T, N>& b) {
			a.data[M] -= b.data[M];
			sub<M - 1>(a, b);
		}
		template <>
		static inline void sub<0>(vector_data<T, N>& a, const vector_data<T, N>& b) {
			a.data[0] -= b.data[0];
		}

		template <int M>
		static inline void mul(vector_data<T, N>& a, const vector_data<T, N>& b) {
			a.data[M] *= b.data[M];
			mul<M - 1>(a, b);
		}
		template <>
		static inline void mul<0>(vector_data<T, N>& a, const vector_data<T, N>& b) {
			a.data[0] *= b.data[0];
		}

		template <int M>
		static inline void div(vector_data<T, N>& a, const vector_data<T, N>& b) {
			a.data[M] /= b.data[M];
			div<M - 1>(a, b);
		}
		template <>
		static inline void div<0>(vector_data<T, N>& a, const vector_data<T, N>& b) {
			a.data[0] /= b.data[0];
		}

		template <int M>
		static inline bool cmp_eq(const vector_data<T, N>& a, const vector_data<T, N>& b) {
			return a.data[M] == b.data[M] && cmp_eq<M - 1>(a, b);
		}
		template <>
		static inline bool cmp_eq<0>(const vector_data<T, N>& a, const vector_data<T, N>& b) {
			return a.data[0] == b.data[0];
		}

		template <int M>
		static inline void negate(vector_data<T, N>& a) {
			a.data[M] = -a.data[M];
			negate<M - 1>(a);
		}
		template <>
		static inline void negate<0>(vector_data<T, N>& a) {
			a.data[0] = -a.data[0];
		}
	};
};

template <typename T, size_t N>
struct vec_impl<T, N, enable_if_t<simd::is_simd<T, N>::value, void>>
{
	static T dot(const vector_data<T, N>& a, const vector_data<T, N>& b) {
		return simd::get_element<T, N, 0>(simd::dot(a.data, b.data));
	}

	static void normalize(vector_data<T, N>& vec) {
		const simd::vector<T, N> sqlen = simd::dot(vec.data, vec.data);
		const simd::vector<T, N> rsqrt = simd::rsqrt(sqlen);
		vec.data = simd::mul(vec.data, rsqrt);
	}

	static void add(vector_data<T, N>& a, const vector_data<T, N>& b) {
		a.data = simd::add(a.data, b.data);
	}

	static void sub(vector_data<T, N>& a, const vector_data<T, N>& b) {
		a.data = simd::sub(a.data, b.data);
	}

	static void mul(vector_data<T, N>& a, const vector_data<T, N>& b) {
		a.data = simd::mul(a.data, b.data);
	}

	static void div(vector_data<T, N>& a, const vector_data<T, N>& b) {
		a.data = simd::div(a.data, b.data);
	}

	static void negate(vector_data<T, N>& a) {
		a.data = simd::negate(a.data);
	}

	static bool cmp_eq(const vector_data<T, N>& a, const vector_data<T, N>& b) {
		const simd::vector<T, N> cmp = simd::cmp_eq(a.data, b.data);
		return static_cast<bool>(simd::get_element<T, N, 0>(cmp));
	}
};
}
}
