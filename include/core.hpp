#pragma once

#ifdef _WIN32
	#define VIN_PLATFORM_WINDOWS
#endif // _WIN32

#ifdef unix
	#define VIN_PLATFORM_UNIX
#endif // unix

#ifdef __APPLE__
	#define VIN_PLATFORM_APPLE
#endif // __APPLE__


#define BIT(x) (1 << x)