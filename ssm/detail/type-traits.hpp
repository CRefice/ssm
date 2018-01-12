#pragma once

namespace ssm
{
// Could use the standard library ones, but don't really want to include the whole thing just for enable_if
template<bool B, typename T = void>
struct enable_if {};
 
template<typename T>
struct enable_if<true, T> { typedef T type; };

template <bool B, typename T>
using enable_if_t = typename enable_if<B, T>::type;
namespace simd
{
// value is true if the specified vector type can be accelerated using SIMD,
// for example is_simd<float, 4> or is_simd<double, 2>
template<typename T, int N>
struct is_simd { static constexpr bool value = false; };
}
}
