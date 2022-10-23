#pragma once

#include "core/base.hpp"

#if defined(_MSC_VER)
    #define DEBUGBREAK() __debugbreak()
#elif defined(__GNUC__)
    #include "signal.h"
    #define DEBUGBREAK() raise(SIGTRAP)
#else
    #define DEBUGBREAK()
#endif

#ifdef VIN_ENABLE_ASSERT
	#define VIN_ASSERT(x, msg) if (!(x)) {printf("Assertion failed: \"%s\": %s, in file %s line %i.\n", msg, #x, __FILE__, __LINE__); DEBUGBREAK();}
#else
	#define VIN_ASSERT(x, msg) 
#endif

#ifdef VIN_ENABLE_HARD_ASSERT
	#define VIN_HARD_ASSERT(x, msg) if (!(x)) {printf("Assertion failed: \"%s\": %s, in file %s line %i.\n", msg, #x, __FILE__, __LINE__); abort();}
#else
	#define VIN_HARD_ASSERT(x, msg) 
#endif

#ifdef VIN_ENABLE_SOFT_ASSERT
	#define VIN_SOFT_ASSERT(x, msg) if (!(x)) {printf("Assertion failed: \"%s\": %s, in file %s line %i.\n", msg, #x, __FILE__, __LINE__);}
#else
	#define VIN_SOFT_ASSERT(x, msg) 
#endif

#define VIN_ASSERT_NOMSG(x) VIN_ASSERT(x, "")
#define VIN_HARD_ASSERT_NOMSG(x) VIN_HARD_ASSERT(x, "")
#define VIN_SOFT_ASSERT_NOMSG(x) VIN_SOFT_ASSERT(x, "")
