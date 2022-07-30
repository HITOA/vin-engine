#pragma once

#define VIN_RENDERER_OPENGL
//#define VIN_RENDERER_VULKAN
//#define VIN_RENDERER_D3D12

#if !defined(VIN_RENDERER_OPENGL)
	#error "NO VALID RENDERING API MACRO DEFINED"
#endif // !VIN_RENDERER_OPENGL


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
#endif // !VIN_RENDERER_OPENGL

#ifdef VIN_DEBUG
	#define VIN_ENABLE_ASSERT
#endif // DEBUG

#define BIT(x) (1 << x)