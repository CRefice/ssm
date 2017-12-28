// This class contains the bare minimum implementation needed to derive other vector functions.
// Why is it a class and not a bunch of free functions? I'm glad you asked!
// The idea is to allow to choose between the scalar and vectorized version based on whether there exists an appropriate SSE data type, so as to achieve optimal efficiency both if it does and if it doesn't.
// Unfortunately, C++ does not allow partial specialization of template functions.
// So the next best thing is to emulate it using a class with only static member functions, and then call those from free functions.
// It's a complicated mess, but it should be completely transparent to the end user
namespace detail
{
template <typename T, size_t N, typename Simd = void>
struct vec_impl
{
	static T dot(const tvecn<T, N>& a, const tvecn<T, N>& b) {
		T result = T{0};
		for (int i = 0; i < N; ++i)
			result += a[i] * b[i];
		return result;
	}

	static tnormn<T, N> normalize(tvecn<T, N> vec) {
		T len = length(vec);
		for (int i = 0; i < N; ++i)
			vec.data[i] /= len;
		return tnormn<T, N>(vec.data);
	}

	static tvecn<T, N> add(tvecn<T, N> a, const tvecn<T, N>& b) {
		for (int i = 0; i < N; ++i)
			a[i] += b[i];
		return a;
	}

	static tvecn<T, N> sub(tvecn<T, N> a, const tvecn<T, N>& b) {
		for (int i = 0; i < N; ++i)
			a[i] -= b[i];
		return a;
	}

	static tvecn<T, N> mul(tvecn<T, N> a, const tvecn<T, N>& b) {
		for (int i = 0; i < N; ++i)
			a[i] *= b[i];
		return a;
	}

	static tvecn<T, N> mul(tvecn<T, N> a, T b) {
		for (int i = 0; i < N; ++i)
			a[i] *= b;
		return a;
	}

	static tvecn<T, N> div(tvecn<T, N> a, const tvecn<T, N>& b) {
		for (int i = 0; i < N; ++i)
			a[i] /= b[i];
		return a;
	}

	static tvecn<T, N> negate(tvecn<T, N> vec) {
		for (int i = 0; i < N; ++i)
			vec[i] = -vec[i];
		return vec;
	}

	static bool cmp_eq(const tvecn<T, N>& a, const tvecn<T, N>& b) {
		for (int i = 0; i < N; ++i) {
			if (a[i] != b[i])
				return false;
		}
		return true;
	}
};

// Could use the standard library ones, but don't really want to include the whole thing just for enable_if
template<bool B, typename T = void>
struct enable_if {};
 
template<typename T>
struct enable_if<true, T> { typedef T type; };

template <bool B, typename T>
using enable_if_t = typename enable_if<B, T>::type;

template <typename T, size_t N>
struct vec_impl<T, N, enable_if_t<simd::is_simd<T, N>::value, void>>
{
	static T dot(const tvecn<T, N>& a, const tvecn<T, N>& b) {
		return simd::get_element<T, N, 0>(simd::dot(a.data, b.data));
	}

	static tnormn<T, N> normalize(tvecn<T, N> vec) {
		const simd::vector<T, N> sqlen = simd::dot(vec.data, vec.data);
		const simd::vector<T, N> rsqrt = simd::rsqrt(sqlen);
		vec.data = simd::mul(vec.data, rsqrt);
		return tnormn<T, N>(vec.data);
	}

	static tvecn<T, N> add(tvecn<T, N> a, const tvecn<T, N>& b) {
		a.data = simd::add(a.data, b.data);
		return a;
	}

	static tvecn<T, N> sub(tvecn<T, N> a, const tvecn<T, N>& b) {
		a.data = simd::sub(a.data, b.data);
		return a;
	}

	static tvecn<T, N> mul(tvecn<T, N> a, const tvecn<T, N>& b) {
		a.data = simd::mul(a.data, b.data);
		return a;
	}

	static tvecn<T, N> mul(tvecn<T, N> a, T b) {
		const simd::vector<T, N> wideb = set_wide<T, N>(b);
		a.data = simd::mul(a.data, wideb);
		return a;
	}

	static tvecn<T, N> div(tvecn<T, N> a, const tvecn<T, N>& b) {
		a.data = simd::div(a.data, b.data);
		return a;
	}

	static tvecn<T, N> div(tvecn<T, N> a, T b) {
		const simd::vector<T, N> wideb = set_wide<T, N>(1.0f / b);
		a.data = simd::mul(a.data, wideb);
		return a;
	}

	static tvecn<T, N> negate(tvecn<T, N> a) {
		a.data = simd::negate(a.data);
		return a;
	}

	static bool cmp_eq(const tvecn<T, N>& a, const tvecn<T, N>& b) {
		const simd::vector<T, N> cmp = simd::cmp_eq(a.data, b.data);
		return static_cast<bool>(simd::get_element<T, N, 0>(cmp));
	}
};
}
