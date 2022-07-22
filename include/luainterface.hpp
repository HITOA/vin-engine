#pragma once

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace Vin::LuaInterface {

	/**
	 * Default lua libraries.
	 */
	static const luaL_Reg luaDefaultLibs[] = {
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

	/**
	 * Create new lua state.
	 */
	lua_State* CreateLuaState();
	/**
	 * Destroy lua state.
	 */
	void DestroyLuaState(lua_State* ls);
	/**
	 * Load libraries.
	 */
	void OpenLibs(lua_State* ls, luaL_Reg* libs);
	/**
	 * Load library.
	 */
	void OpenLib(lua_State* ls, const luaL_Reg& lib);

	/**
	 * Load a chunk without running it.
	 */
	int Load(lua_State* ls, lua_Reader reader, void* data, const char* chunkname);

	/**
	 * Run the function in the stack.
	 */
	int Run(lua_State* ls, int nargs, int nresults);

	/**
	 * Get last error msg and pop it.
	 */
	const char* GetLastError(lua_State* ls);
}
