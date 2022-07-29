#pragma once

#define VIN_RENDERER_OPENGL
//#define VIN_RENDERER_VULKAN
//#define VIN_RENDERER_D3D12

#ifdef _WIN32
	#define VIN_PLATFORM_WINDOWS
#endif // _WIN32

#ifdef unix
	#define VIN_PLATFORM_UNIX
#endif // unix

#ifdef __APPLE__
	#define VIN_PLATFORM_APPLE
#endif // __APPLE__

#ifdef VIN_DEBUG
	#define VIN_ENABLE_ASSERT
#endif // DEBUG

#define BIT(x) (1 << x)