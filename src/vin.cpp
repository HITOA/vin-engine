#include <vin.hpp>
#include <vincore.hpp>
#include <luaaa/luaaa.hpp>
#include <stdexcept>
#include <physfs.h>
#include <logger.hpp>

namespace Vin {
	namespace {
		bool _isInit;
		lua_State* _ls;
	}

	void InitPhysfs() {
		if (PHYSFS_init(NULL) == 0)
			throw std::runtime_error{ PHYSFS_getLastError() };

		int n = 0;

		if (PHYSFS_mount("./data", NULL, true) != 0)
			n++;

		if (PHYSFS_mount("./data.zip", NULL, true) != 0)
			n++;

		if (n == 0)
			throw std::runtime_error{ "Failed to mount any data directory." };
	}

	void InitLua() {
		const luaL_Reg lualibs[] = {
			{ LUA_COLIBNAME, luaopen_base },
			{ LUA_LOADLIBNAME, luaopen_package },
			{ LUA_TABLIBNAME, luaopen_table },
			{ LUA_IOLIBNAME, luaopen_io },
			{ LUA_OSLIBNAME, luaopen_os },
			{ LUA_STRLIBNAME, luaopen_string },
			{ LUA_MATHLIBNAME, luaopen_math },
			{ LUA_DBLIBNAME, luaopen_debug },
			{ NULL, NULL }
		};

		_ls = luaL_newstate();

		if (_ls == NULL)
			throw std::runtime_error{ "Lua failed to init." };

		const luaL_Reg* lib = lualibs;
		for (; lib->func; lib++) {
			lua_pushcfunction(_ls, lib->func);
			lua_pushstring(_ls, lib->name);
			lua_call(_ls, 1, 0);
		}
	}

	void InitVincore() {
		if (PHYSFS_exists("init.lua") == 0)
			throw std::runtime_error{ "init.lua is missing." };

		PHYSFS_file* handle = PHYSFS_openRead("init.lua");
		const long long size = PHYSFS_fileLength(handle);
		char* buffer = new char[size];
		
		PHYSFS_readBytes(handle, buffer, size);
		PHYSFS_close(handle);

		int err = luaL_loadbuffer(_ls, buffer, size, "init.lua");
		if (err == 0)
			err = lua_pcall(_ls, 0, 0, 0);

		if (err) {
			Logger::Err(lua_tostring(_ls, -1));
			lua_pop(_ls, 1);
		}

		delete[] buffer;
	}

	void Init()
	{
		if (_isInit)
			return;

		InitPhysfs();
		InitLua();
		InitVincore();

		_isInit = true;
	}

	void Terminate()
	{
		if (!_isInit)
			return;

		Vin::Core::Terminate();
		lua_close(_ls);
		PHYSFS_deinit();

		_isInit = false;
	}
}
