namespace detail
{
template <typename T, int M, int N, int P = N, typename = void>
struct mat_unroll
{
	template <int Q>
	static inline vector<T, M> mul(const vector<T, N>& vec, const matrix<T, M, N>& mat) {
		return vec[Q] * mat[Q]
			+ mul<Q - 1>(vec, mat);
	}
	template <>
	static inline vector<T, M> mul<0>(const vector<T, N>& vec, const matrix<T, M, N>& mat) {
		return vec[0] * mat[0]; 
	}

	template <int Q>
	static inline void mul(matrix<T, M, P>& ret, const matrix<T, M, N>& a, const matrix<T, N, P>& b) {
		ret[Q] = mul<N - 1>(b[Q], a);
		mul<Q - 1>(ret, a, b);
	}
	template <>
	static inline void mul<0>(matrix<T, M, P>& ret, const matrix<T, M, N>& a, const matrix<T, N, P>& b) {
		ret[0] = mul<N - 1>(b[0], a);
	}
};

template <typename T, int N>
struct mat_unroll<T, N, N, N>
{
	template <int M>
	static inline vector<T, N> mul(const vector<T, N>& vec, const matrix<T, N, N>& mat) {
		return mat[M] * (vector<T, N>)(simd::shuffle<M>(vec.data))
			+ mul<M - 1>(vec, mat);
	}
	template <>
	static inline vector<T, N> mul<0>(const vector<T, N>& vec, const matrix<T, N, N>& mat) {
		return mat[0] * vector<T, N>(simd::shuffle<0>(vec.data));
	}

	template <int M>
	static inline void mul(matrix<T, N, N>& ret, const matrix<T, N, N>& a, const matrix<T, N, N>& b) {
		ret[M] = mul<N - 1>(b[M], a);
		mul<M - 1>(ret, a, b);
	}
	template <>
	static inline void mul<0>(matrix<T, N, N>& ret, const matrix<T, N, N>& a, const matrix<T, N, N>& b) {
		ret[0] = mul<N - 1>(b[0], a);
	}
};
}
