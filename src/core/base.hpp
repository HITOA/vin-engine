#pragma once

#ifdef _WIN32
#define VIN_PLATFORM_WINDOWS
#endif // _WIN32

#ifdef unix
#define VIN_PLATFORM_UNIX
#error "UNIX NOT SUPPORTED"
#endif // unix

#ifdef __APPLE__
#define VIN_PLATFORM_APPLE
#error "APPLE NOT SUPPORTED"
#endif // __APPLE__

#if !defined(VIN_PLATFORM_WINDOWS) && !defined(VIN_PLATFORM_UNIX) && !defined(VIN_PLATFORM_APPLE)
#error "NO PLATFORM DEFINED"
#endif

#ifndef __GNUC__
#if defined(_WIN32) || defined(_WIN64)
#ifdef _WIN64
#define VIN_64
#else
#define VIN_32
#endif
#endif
#else
#if defined(__x86_64__) || defined(__ppc64__)
#define VIN_64
#else
#define VIN_32
#endif
#endif

#ifdef _DEBUG
#define VIN_DEBUG
#endif

#ifdef VIN_DEBUG
#define VIN_ENABLE_ASSERT
#define VIN_ENABLE_HARD_ASSERT
#define VIN_ENABLE_SOFT_ASSERT
#endif // DEBUG

namespace Vin {
	typedef unsigned char byte;

	typedef long long i64;
	typedef unsigned long ui64;

	typedef int i32;
	typedef unsigned int ui32;

#ifdef VIN_64
	typedef i64 isize;
	typedef ui64 usize;
#else
	typedef i32 isize;
	typedef ui32 usize;
#endif
}
