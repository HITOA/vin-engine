#pragma once

#include "vinpch.hpp"

#include "core/core.hpp"

#ifdef VIN_ENABLE_ASSERT
	#define VIN_ASSERT(x, msg) if (!(x)) {printf("Assertion failed: \"%s\": %s, in file %s line %i.\n", msg, #x, __FILE__, __LINE__); abort();}
#else
	#define VIN_ASSERT(x, msg) 
#endif // VIN_ENABLE_ASSERT
