#include <psf.hpp>
#include <physfs.h>
#include <logger.hpp>

namespace Vin::Psf {
	namespace {
		bool _isInit;
	}

	bool Init()
	{
		if (_isInit)
			return _isInit;

		if (PHYSFS_init(NULL) == 0) {
			Logger::Err("Physfs was not able to init : {}", PHYSFS_getLastError());
			return _isInit;
		}

		int n = 0;

		if (PHYSFS_mount("./data", NULL, true) != 0)
			n++;

		if (PHYSFS_mount("./data.zip", NULL, true) != 0)
			n++;

		if (n == 0) {
			Logger::Err("Failed to mount any data directory.");
			PHYSFS_deinit();
			return _isInit;
		}

		_isInit = false;
		return _isInit;
	}

	void Terminate()
	{
		if (!_isInit)
			return;

		PHYSFS_deinit();

		_isInit = false;
	}

	bool Exists(const char* filename)
	{
		return PHYSFS_exists(filename);
	}

	std::vector<char> ReadFile(const char* filename)
	{
		if (!Exists(filename)) {
			Logger::Log("File {} not founds.", filename);
			return std::vector<char>{};
		}

		PHYSFS_file* handle = PHYSFS_openRead("init.lua");
		const long long size = PHYSFS_fileLength(handle);

		std::vector<char> buffer{};
		buffer.resize(size);

		PHYSFS_readBytes(handle, buffer.data(), buffer.size());
		PHYSFS_close(handle);

		return buffer;
	}
	
	namespace LLib {
		int searcher_psf(lua_State* L) {
			const char* name = luaL_checkstring(L, 1);
			printf("Module loaded : %s\n", name);
			return 1;
		}

		int psf_llib(lua_State* L)
		{
			lua_getglobal(L, LUA_LOADLIBNAME);
			if (!lua_istable(L, -1))
				return 0;

			lua_getfield(L, -1, "searchers");
			if (!lua_istable(L, -1))
				return 0;

			lua_pushvalue(L, -2);
			lua_pushcclosure(L, searcher_psf, 1);
			lua_rawseti(L, -2, 4 + 1);
			lua_setfield(L, -2, "searchers");

			return 0;
		}

	}
}
