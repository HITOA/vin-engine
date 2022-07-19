#pragma once

#include <luaaa/luaaa.hpp>

namespace Vin::Lua {
	void Init();
	void Terminate();
	void LoadLibrary(const char* name, lua_CFunction func);
	bool LoadBuffer(char* buffer, const long long size, const char* name);
	bool Run();
	lua_State* GetCtx();
}
