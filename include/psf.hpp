#pragma once
#include <luaaa/luaaa.hpp>
#include <vector>

namespace Vin::Psf {

	bool Init();
	void Terminate();
	bool Exists(const char* filename);
	std::vector<char> ReadFile(const char* filename);

	namespace LLib {
#define PSFLUA_LIBNAME "psf"
		int psf_llib(lua_State* L);
	}
}
