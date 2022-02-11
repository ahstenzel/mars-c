/*
 *  platform.h
 *  Declaration for platform detection code referenced throughout the project.
 */
#ifndef MARS_PLATFORM_H
#define MARS_PLATFORM_H

 /* Detect compiler */
#if defined(__clang__)
# define MARS_CMP_CLANG
#elif (defined(__GNUC__) || defined(__GNUG__)) && !defined(__INTEL_COMPILER)
# define MARS_CMP_GCC
#elif defined(_MSC_VER)
# define MARS_CMP_MSVC
#elif
# define MARS_CMP_UNKNOWN
#endif

/* Detect architecture */
#if defined(__arm__) || defined(__thumb__) ||\
    defined(_M_ARM)  || defined(_M_ARMT)
# define MARS_ARCH_ARM
#elif defined(__aarch64__) || defined (_M_ARM64)
# define MARS_ARCH_ARM64
#elif defined(i386) || defined(__i386) || defined(__i386__) ||\
      defined(i486) || defined(__i486) || defined(__i486__) ||\
      defined(i586) || defined(__i586) || defined(__i586__) ||\
      defined(i686) || defined(__i686) || defined(__i686__) ||\
      defined(_M_IX86) || defined(_M_X86) || defined(_X86_)
# define MARS_ARCH_X86
#elif defined(__x86_64) || defined(__x86_64__) ||\
      defined(__amd64) || defined(__amd64__) ||\
      defined(_M_X64) || defined(_M_AMD64)
# define MARS_ARCH_X64
#else
# define MARS_ARCH_UNKNOWN
#endif

/* SIMD Definitions */
#define MARS_LEGACY
#if (defined(MARS_ARCH_X86) || defined(MARS_ARCH_X64)) && !defined(MARS_LEGACY) && !defined(MARS_CMP_UNKNOWN)
# define MARS_SIMD_AVX
#include <immintrin.h>

#elif (defined(MARS_ARCH_ARM) || defined(MARS_ARCH_ARM64)) && !defined(MARS_LEGACY) && !defined(MARS_CMP_UNKNOWN)
# define MARS_SIMD_NEON
#include <arm_neon.h>

#else
# define MARS_SIMD_NONE

#endif

#endif