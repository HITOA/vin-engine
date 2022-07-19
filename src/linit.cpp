#include <linit.hpp>
#include <vector>
#include <string>
#include <logger.hpp>

namespace Vin::Lua {

	namespace {
		bool _isInit;
		lua_State* _ls;
	}

	void LuaOpenLibs() {
		static const luaL_Reg loadedlibs[] = {
			{LUA_GNAME, luaopen_base},
			{LUA_LOADLIBNAME, luaopen_package},
			{LUA_COLIBNAME, luaopen_coroutine},
			{LUA_TABLIBNAME, luaopen_table},
			{LUA_IOLIBNAME, luaopen_io},
			{LUA_OSLIBNAME, luaopen_os},
			{LUA_STRLIBNAME, luaopen_string},
			{LUA_MATHLIBNAME, luaopen_math},
			{LUA_UTF8LIBNAME, luaopen_utf8},
			{LUA_DBLIBNAME, luaopen_debug},
			{NULL, NULL}
		};

		const luaL_Reg* lib;

		for (lib = loadedlibs; lib->func; lib++) {
			luaL_requiref(_ls, lib->name, lib->func, 1);
			lua_pop(_ls, 1);
		}
	}

	void Init()
	{
		if (_isInit)
			return;

		_ls = luaL_newstate();
		LuaOpenLibs();

		_isInit = true;
	}

	void Terminate()
	{
		if (!_isInit)
			return;

		lua_close(_ls);

		_isInit = false;
	}

	void LoadLibrary(const char* name, lua_CFunction func)
	{
		luaL_requiref(_ls, name, func, 1);
		lua_pop(_ls, 1);
	}

	void HandleLuaErr() {
		Logger::Err(lua_tostring(_ls, -1));
		lua_pop(_ls, 1);
	}

	bool LoadBuffer(char* buffer, const long long size, const char* name)
	{
		if (luaL_loadbuffer(_ls, buffer, size, name) != 0) {
			HandleLuaErr();
			return false;
		}
		return true;
	}

	bool Run()
	{
		if (lua_pcall(_ls, 0, 0, 0) != 0) {
			HandleLuaErr();
			return false;
		}
		return true;
	}

	lua_State* GetCtx()
	{
		return _ls;
	}

}
