// The following are types and functions for achieving "compile-time loops",
// using simple SFINAE template metaprogramming.

namespace ssm
{
namespace detail
{
template <std::size_t Start, std::size_t End>
struct unroll
{
	static_assert(Start < End, "Error in template parameters: \"Start\" must be less than \"End\"");
	using next = unroll<Start + 1, End>;

	template <typename Arr1, typename Arr2>
	inline static void assign(Arr1& dest, const Arr2& src) {
		dest.template set<Start>(src.template get<Start>());
		next::assign(dest, src);
	}


	// Assigns the same value to elements of the matrix.
	template <typename Arr, typename T>
	inline static void diagonal_assign_val(Arr& arr, T val) {
		arr[Start].template set<Start>(val);
		next::diagonal_assign_val(arr, val);
	}

	// Assumes 2-dimensional vector
	template <typename Arr1, typename Arr2>
	inline static void diagonal_assign_vec(Arr1& dest, const Arr2& src) {
		dest[Start].template set<Start>(src.template get<Start>());
		next::diagonal_assign_vec(dest, src);
	}

	template <typename Vec, typename Mat>
	static inline typename Mat::value_type inner_product(Vec& vec, const Mat& mat) {
		return mat[Start] * vec.template get<Start>()
			+ next::inner_product(vec, mat);
	}
};

template <std::size_t End>
struct unroll<End, End>
{
	template <typename Arr1, typename Arr2>
	inline static void assign(Arr1& dest, const Arr2& src) {}

	template <typename Arr, typename T>
	inline static void diagonal_assign_val(Arr& arr, T val) {}

	template <typename Arr1, typename Arr2>
	inline static void diagonal_assign_vec(Arr1& dest, const Arr2& src) {}

	template <typename Vec, typename Mat>
	static inline typename Mat::value_type inner_product(Vec& vec, const Mat& mat) { return {}; }
};
}
}
