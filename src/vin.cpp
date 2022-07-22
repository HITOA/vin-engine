#include <vin.hpp>
#include <luainterface.hpp>
#include <physfs.h>
#include <logger.hpp>
#include <lualib.hpp>

namespace Vin {
    bool Initialize()
    {
		if (PHYSFS_init(NULL) == 0) {
			Logger::Err("Physfs was not able to init : {}", PHYSFS_getLastError());
			return false;
		}

		int n = 0;

		if (PHYSFS_mount("./data", NULL, true) != 0)
			n++;

		if (PHYSFS_mount("./data.zip", NULL, true) != 0)
			n++;

		if (n == 0) {
			Logger::Err("Failed to mount any data directory.");
			PHYSFS_deinit();
			return false;
		}

		return true;
    }

	void Terminate()
	{
		PHYSFS_deinit();
	}

	Vin* CreateApp()
	{
		Vin* app = new Vin{};

		return app;
	}

	void DestroyApp(Vin* app)
	{
		delete app;
	}

	void Vin::Init(const char* luaentry)
	{
		this->luaentry = luaentry;
		InitLua();
	}

	void Vin::Run()
	{
		if (Llib::Physfs::LuaLoadFromFile(luaState.get(), luaentry) != 0) {
			Logger::Err("Lua error: {}", LuaInterface::GetLastError(luaState.get()));
			return;
		}
		if (LuaInterface::Run(luaState.get(), 0, 0) != 0) {
			Logger::Err("Lua error: {}", LuaInterface::GetLastError(luaState.get()));
			return;
		}
	}

	void Vin::InitLua()
	{
		luaState = std::unique_ptr<lua_State, LuaStateDeleter>{ LuaInterface::CreateLuaState() };

		LuaInterface::OpenLibs(luaState.get(), (luaL_Reg*)LuaInterface::luaDefaultLibs);

		LuaInterface::OpenLib(luaState.get(), { LLIB_PHYSFS_NAME, Llib::Physfs::LlibPhysfs });
	}

}
