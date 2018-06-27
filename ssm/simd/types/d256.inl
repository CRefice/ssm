#if SSM_ARCH & SSM_ARCH_AVX_BIT
namespace ssm
{
namespace simd
{
template<>
struct is_simd<double, 4> { static constexpr bool value = true; };

typedef __m256d d256;

template <>
struct aligned_type<double, 4>
{
	using type = d256;
};

namespace detail
{
template <size_t N>
struct access_impl<double, 4, N>
{
	static inline d256 set(d256 vec, double val) {
	__m128d temp = _mm256_extractf128_pd(vec, N / 2);
	temp = set_element<N % 2>(temp, val);
	return	_mm256_insertf128_pd(vec, temp, N / 2);
	}
}
}
}
}
#endif
