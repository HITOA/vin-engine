#pragma once

#include <core.hpp>
#include <vinpch.hpp>

#ifdef VIN_ENABLE_ASSERT
	#define VIN_ASSERT(x, msg) if (!(x)) {printf("Assertion failed: %s, in file %s line %i. \"%s\"\n", #x, __FILE__, __LINE__, msg); abort();}
#else
	#define VIN_ASSERT(x, msg) 
#endif // VIN_ENABLE_ASSERT
