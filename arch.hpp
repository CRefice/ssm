#pragma once

//Platform
#if defined(SSM_FORCE_PLATFORM_UNKNOWN)
  #define SSM_PLATFORM_UNKNOWN

#elif defined(_WIN32)
  #define SSM_PLATFORM_WINDOWS
#elif defined(__ANDROID__)
  #define SSM_PLATFORM_ANDROID
#elif defined(__linux)
  #define SSM_PLATFORM_LINUX
#elif defined(__APPLE__)
  #define SSM_PLATFORM_APPLE
#elif defined(__unix)
  #define SSM_PLATFORM_UNIX
#elif defined(__native_client__)
  #define SSM_PLATFORM_NATIVE
#else
  #define SSM_PLATFORM_UNKNOWN
#endif

//Compilers
#if defined(SSM_FORCE_COMPILER_UNKNOWN)
  #define SSM_COMPILER_UNKNOWN

#elif defined(__INTEL_COMPILER)
  #define SSM_COMPILER_INTEL
#elif defined(__clang)
  #define SSM_COMPILER_CLANG
#elif defined(_MSC_VER)
  #define SSM_COMPILER_VC
#elif defined(__GNUC__) || defined(__MINGW32__)
  #define SSM_COMPILER_GCC
#else
  #define SSM_COMPILER_UNKNOWN
#endif

//Processor architectures
#if defined(SSM_FORCE_PURE)
  #define SSM_ARCH_PURE
#elif defined(SSM_FORCE_MIPS)
  #define SSM_ARCH_MIPS
#elif defined(SSM_FORCE_PPC)
  #define SSM_ARCH_PPC
#elif defined(SSM_FORCE_AVX512)
  #define SSM_ARCH_AVX512
#elif defined(SSM_FORCE_AVX2)
  #define SSM_ARCH_AVX2
#elif defined(SSM_FORCE_AVX)
  #define SSM_ARCH_AVX
#elif defined(SSM_FORCE_SSE42)
  #define SSM_ARCH_SSE42
#elif defined(SSM_FORCE_SSE41)
  #define SSM_ARCH_SSE41
#elif defined(SSM_FORCE_SSSE3)
  #define SSM_ARCH_SSSE3
#elif defined(SSM_FORCE_SSE3)
  #define SSM_ARCH_SSE3
#elif defined(SSM_FORCE_SSE2)
  #define SSM_ARCH_SSE2

#else
# if defined(SSM_COMPILER_CLANG) || defined(SSM_COMPILER_GCC) || (defined(SSM_COMPILER_INTEL) && defined(SSM_PLATFORM_LINUX))
#	if defined(__AVX512BW__) && defined(__AVX512F__) && defined(__AVX512CD__) && defined(__AVX512VL__) && defined(__AVX512DQ__)
#	  define SSM_ARCH_AVX512
#	elif defined(__AVX2__)
#	  define SSM_ARCH_AVX2
#	elif defined(__AVX__)
#	  define SSM_ARCH_AVX
#	elif defined(__SSE4_2__)
#	  define SSM_ARCH_SSE42
#	elif defined(__SSE4_1__)
#	  define SSM_ARCH_SSE41
#	elif defined(__SSSE3__)
#	  define SSM_ARCH_SSSE3
#	elif defined(__SSE3__)
#	  define SSM_ARCH_SSE3
#	elif defined(__SSE2__)
#	  define SSM_ARCH_SSE2
#	elif defined(__i386__) || defined(__x86_64__)
#	  define SSM_ARCH_X86
#	elif defined(__ARM_NEON)
#	  define SSM_ARCH_ARM
#	elif defined(__arm__ )
#	  define SSM_ARCH_ARM
#	elif defined(__mips__ )
#	  define SSM_ARCH_MIPS
#	elif defined(__powerpc__ )
#	  define SSM_ARCH_PPC
#	else
#	  define SSM_ARCH_PURE
#	endif

# elif defined(SSM_COMPILER_VC) || (defined(SSM_COMPILER_INTEL) && defined(SSM_PLATFORM_WINDOWS))
#	if defined(_M_ARM)
#	  define SSM_ARCH_ARM
#	elif defined(__AVX2__)
#	  define SSM_ARCH_AVX2
#	elif defined(__AVX__)
#	  define SSM_ARCH_AVX
#	elif defined(_M_X64)
#	  define SSM_ARCH_SSE2
#	elif defined(_M_IX86_FP)
#	  if _M_IX86_FP >= 2
#	  	define SSM_ARCH_SSE2
#	  else
#	  	define SSM_ARCH_PURE
#	  endif
#	else
#	  define SSM_ARCH_PURE
#	endif
# else
#	define SSM_ARCH_PURE
#endif
#endif

// With MinGW-W64, including intrinsic headers before intrin.h will produce some errors. The problem is
// that windows.h (and maybe other headers) will silently include intrin.h, which of course causes problems.
// To fix, we just explicitly include intrin.h here.
#if defined(__MINGW64__) && !defined(SSM_ARCH_PURE)
#	include <intrin.h>
#endif

#if defined(SSM_ARCH_AVX2)
#	include <immintrin.h>
#elif defined(SSM_ARCH_AVX_BIT)
#	include <immintrin.h>
#elif defined(SSM_ARCH_SSE42)
#	include <nmmintrin.h>
#elif defined(SSM_ARCH_SSE41)
#	include <smmintrin.h>
#elif defined(SSM_ARCH_SSSE3)
#	include <tmmintrin.h>
#elif defined(SSM_ARCH_SSE3)
#	include <pmmintrin.h>
#elif defined(SSM_ARCH_SSE2)
#	include <emmintrin.h>
#endif

#ifdef SSM_ARCH_SSE2
	typedef __m128  f128;
	typedef __m128i	int128;
	typedef __m128i	uint128;
	typedef __m128i	s128;
	typedef __m128i	u128;
#endif

#ifdef SSM_ARCH_AVX
	typedef __m256d	d256;
#endif

#ifdef SSM_ARCH_AVX2
	typedef __m256i	int256;
	typedef __m256i	uint256;
	typedef __m256i	s256;
	typedef __m256i	u256;
#endif
