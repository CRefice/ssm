#pragma once

#include "type-traits.hpp"

namespace ssm
{
namespace detail
{
// A utility class used to unroll loops,
// starting from Start (inclusive) and ending in End (exclusive)
template <int Start, int End>
struct unroll
{
	static_assert(Start < End, "struct unroll: Error in template parameter:\
			\"Start\" must be less than \"End\"");

	using next = unroll<Start + 1, End>;

	template <typename T>
	inline static void add(T& a, const T& b) {
		a[Start] += b[Start];
		next::add(a, b);
	}
	template <typename T>
	inline static void add(T& a, const elem_type<T>& b) {
		a[Start] += b;
		next::add(a, b);
	}

	template <typename T>
	inline static void sub(T& a, const T& b) {
		a[Start] -= b[Start];
		next::sub(a, b);
	}
	template <typename T>
	inline static void sub(T& a, const elem_type<T>& b) {
		a[Start] -= b;
		next::sub(a, b);
	}

	template <typename T>
	inline static void mul(T& a, const T& b) {
		a[Start] *= b[Start];
		next::mul(a, b);
	}
	template <typename T>
	inline static void mul(T& a, const elem_type<T>& b) {
		a[Start] *= b;
		next::mul(a, b);
	}

	template <typename T>
	inline static void div(T& a, const T& b) {
		a[Start] /= b[Start];
		next::div(a, b);
	}
	template <typename T>
	inline static void div(T& a, const elem_type<T>& b) {
		a[Start] /= b;
		next::div(a, b);
	}

	template <typename T>
	inline static void negate(T& a) {
		a[Start] = -a[Start];
		next::negate(a);
	}

	template <typename T>
	inline static bool equal(const T& a, const T& b) {
		return a[Start] == b[Start] && next::equal(a, b);
	}

	//return == a[0] * b[0] + a[1] * b[1] ...
	template <typename T>
	inline static elem_type<T> dot(const T& a, const T& b) {
		return a[Start] * b[Start] + next::dot(a, b);
	}
	template <typename T>
	inline static elem_type<T> dot(const T& a, const elem_type<T>& b) {
		return a[Start] * b + next::dot(a, b);
	}	
};

template <int End>
struct unroll<End, End>
{
	template <typename T>
	inline static void add(T& a, const T& b) { }
	template <typename T>
	inline static void add(T& a, const elem_type<T>& b) { }

	template <typename T>
	inline static void sub(T& a, const T& b) { }
	template <typename T>
	inline static void sub(T& a, const elem_type<T>& b) { }

	template <typename T>
	inline static void mul(T& a, const T& b) { }
	template <typename T>
	inline static void mul(T& a, const elem_type<T>& b) { }

	template <typename T>
	inline static void div(T& a, const T& b) { }
	template <typename T>
	inline static void div(T& a, const elem_type<T>& b) { }

	template <typename T>
	inline static void negate(T& a) { }

	template <typename T>
	inline static bool equal(const T& a, const T& b) { return true; }

	//return == a[0] * b[0] + a[1] * b[1] ...
	template <typename T>
	inline static elem_type<T> dot(const T& a, const T& b) { return {}; }
	template <typename T>
	inline static elem_type<T> dot(const T& a, const elem_type<T>& b) { return {}; }
};
}
}
