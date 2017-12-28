#pragma once

namespace ssm
{
namespace simd
{
// value is true if the specified vector type can be accelerated using SIMD,
// for example is_simd<float, 4> or is_simd<double, 2>
template<typename T, size_t N>
struct is_simd { static constexpr bool value = false; };
}
}
