namespace detail
{
template <int Start, int End>
struct mat_unroll
{
	static_assert(Start < End, "mat_unroll: Error in template parameter: \"Start\" must be less than \"End\"");

	using next = mat_unroll<Start + 1, End>;

	template <typename T, int M, int N>
	static inline vector<T, M> mul(const vector<T, N>& vec, const matrix<T, M, N>& mat) {
		return mat[Start] * simd::get_element<T, N, Start>(vec.data)
			+ next::mul(vec, mat);
	}

	template <typename T, int M, int N, int P>
	static inline void mul(matrix<T, M, P>& ret, const matrix<T, M, N>& a, const matrix<T, N, P>& b) {
		ret[Start] = mat_unroll<0, N>::mul(a[Start], b);
		next::mul(ret, a, b);
	}
};

template <int End>
struct mat_unroll<End, End>
{
	template <typename T, int M, int N>
	static inline vector<T, M> mul(const vector<T, N>& vec, const matrix<T, M, N>& mat) {
		return vector<T, M>{};
	}

	template <typename T, int M, int N, int P>
	static inline void mul(matrix<T, M, P>& ret, const matrix<T, M, N>& a, const matrix<T, N, P>& b) {}
};
}
