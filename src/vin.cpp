#include <vin.hpp>
#include <luainterface.hpp>
#include <physfs.h>
#include <logger.hpp>
#include <lualib.hpp>
#include <functional>

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

	void InitLua(Vin* app) {
		app->luaState = LuaInterface::CreateLuaState();

		lua_pushglobaltable(app->luaState);
		lua_pushlightuserdata(app->luaState, app);
		lua_setfield(app->luaState, -2, "app");

		LuaInterface::OpenLibs(app->luaState, (luaL_Reg*)LuaInterface::luaDefaultLibs);

		LuaInterface::OpenLib(app->luaState, { LLIB_PHYSFS_NAME, Llib::Physfs::LlibPhysfs });
		LuaInterface::OpenLib(app->luaState, { LLIB_VIN_NAME, Llib::Vin::LlibVin });

	}

	void TerminateLua(Vin* app) {
		LuaInterface::DestroyLuaState(app->luaState);
	}

	Vin* CreateApp()
	{
		Vin* app = new Vin{};

		app->luaentry = "init.lua";

		InitLua(app);

		return app;
	}

	void DestroyApp(Vin* app)
	{
		TerminateLua(app);

		delete app;
	}

	void Run(Vin* app)
	{
		if (Llib::Physfs::LuaLoadFromFile(app->luaState, app->luaentry) != 0) {
			Logger::Err("Lua error: {}", LuaInterface::GetLastError(app->luaState));
			return;
		}
		if (LuaInterface::Run(app->luaState, 0, 0) != 0) {
			Logger::Err("Lua error: {}", LuaInterface::GetLastError(app->luaState));
			return;
		}
	}

	/*void Vin::Init(const char* luaentry)
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

	int Vin::LlibVin(lua_State* ls)
	{
		static const luaL_Reg vin_funcs[] = {
			{"register", std::bind(&Vin::LuaRegister, this)},
			{NULL, NULL}
		};
	}

	int Vin::LuaRegister(lua_State* ls)
	{

	}*/

}
