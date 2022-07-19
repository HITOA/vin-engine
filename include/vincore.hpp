#pragma once

#include <string>
#include <functional>
#include <luaaa/luaaa.hpp>
/**
 * Core of the engine, A big part of the rendering is made here.
 */

namespace Vin::Core {
	/**
	 * Init the rendering engine. (Create a window and opengl context)
	 */
	void Init(const std::string& wTitle, int wWidth, int wHeight);
	void Run();
	void Terminate();
	void SetUpdateCallback(std::function<void()> callback);
	unsigned int CompileShader(const char* vSrc, const char* fSrc);

	namespace LLib {
#define VINLUA_LIBNAME "vin"
		int vin_invoke(lua_State* L, const char* eventname);
		int vin_llib(lua_State* L);
	}
}
