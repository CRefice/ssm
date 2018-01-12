#pragma once

namespace ssm
{
namespace detail
{
// This is a utility class used to unroll loops,
// starting from Start (inclusive) and ending in End (exclusive)
template <typename T, int Start, int End>
struct unroll
{
	static_assert(Start < End, "Error in template parameter: \"Start\" must be less than \"End\"");

	inline static void add(T& a, const T& b) {
		a[Start] += b[Start];
		unroll<T, Start + 1, End>::add(a, b);
	}
	template <typename S>
	inline static void add(T& a, const S& b) {
		a[Start] += b;
		unroll<T, Start + 1, End>::add(a, b);
	}

	inline static void sub(T& a, const T& b) {
		a[Start] -= b[Start];
		unroll<T, Start + 1, End>::sub(a, b);
	}
	template <typename S>
	inline static void sub(T& a, const S& b) {
		a[Start] -= b;
		unroll<T, Start + 1, End>::sub(a, b);
	}

	inline static void mul(T& a, const T& b) {
		a[Start] *= b[Start];
		unroll<T, Start + 1, End>::mul(a, b);
	}
	template <typename S>
	inline static void mul(T& a, const S& b) {
		a[Start] *= b;
		unroll<T, Start + 1, End>::mul(a, b);
	}

	inline static void div(T& a, const T& b) {
		a[Start] /= b[Start];
		unroll<T, Start + 1, End>::div(a, b);
	}
	template <typename S>
	inline static void div(T& a, const S& b) {
		a[Start] /= b;
		unroll<T, Start + 1, End>::div(a, b);
	}

	inline static void negate(T& a) {
		a[Start] = -a[Start];
		unroll<T, Start + 1, End>::negate(a);
	}

	//return == a[0] * b[0] + a[1] * b[1] ...
	inline static T dot(const T& a, const T& b) {
		return a[Start] * b[Start] + unroll<T, Start + 1, End>::dot(a, b);
	}
	template <typename S>
	inline static T dot(const T& a, const S& b) {
		return a[Start] * b + unroll<T, Start + 1, End>::dot(a, b);
	}
};

	template <typename T, int End>
struct unroll<T, End, End>
{
	inline static void add(T& a, const T& b) { }
	template <typename S>
	inline static void add(T& a, const S& b) { }

	inline static void sub(T& a, const T& b) { }
	template <typename S>
	inline static void sub(T& a, const S& b) { }

	inline static void mul(T& a, const T& b) { }
	template <typename S>
	inline static void mul(T& a, const S& b) { } 

	inline static void div(T& a, const T& b) { }
	template <typename S>
	inline static void div(T& a, const S& b) { }

	inline static void negate(T& a) { }

	//return == a[0] * b[0] + a[1] * b[1] ...
	inline static T dot(const T& a, const T& b) { return 0; }
	template <typename S>
	inline static T dot(const T& a, const S& b) { return 0; }
};
}
}
