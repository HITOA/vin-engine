#include "lualib.hpp"
#include <logger.hpp>
#include <functional>
#include <vin.hpp>

#pragma region LlibPhysfs
const char* Vin::Llib::Physfs::GetPhysfs(lua_State* ls, void* ud, size_t* size)
{
	LoadPhysfs* lf = (LoadPhysfs*)ud;
	(void)ls;

	if (PHYSFS_eof(lf->f)) return NULL;
	*size = PHYSFS_readBytes(lf->f, lf->buff, BUFSIZ);

	return lf->buff;
}

int Vin::Llib::Physfs::LuaLoadFromFile(lua_State* ls, const char* filename)
{
	LoadPhysfs lf{ 0 };

	if (PHYSFS_exists(filename) == 0) {
		Logger::Err("file {} not founds.", filename);
		return 1;
	}

	lf.f = PHYSFS_openRead(filename);
	int status = LuaInterface::Load(ls, GetPhysfs, &lf, filename);
	PHYSFS_close(lf.f);

	return status;
}

int CheckLoad(lua_State* L, int stat, const char* filename) {
	if (stat == 0) {
		lua_pushstring(L, filename);
		return 2;
	}
	else
		return luaL_error(L, "error loading module '%s' from file '%s':\n\t%s",
			lua_tostring(L, 1), filename, lua_tostring(L, -1));
}

int SearcherPhysfs(lua_State* ls) {
	const char* name = luaL_checkstring(ls, 1);
	char filename[256];

	strncpy(filename, name, sizeof(filename));
	strncat(filename, ".lua", sizeof(filename));

	return CheckLoad(ls, Vin::Llib::Physfs::LuaLoadFromFile(ls, filename), filename);
}

int Vin::Llib::Physfs::LlibPhysfs(lua_State* ls)
{
	lua_getglobal(ls, LUA_LOADLIBNAME);
	if (!lua_istable(ls, -1))
		return 0;

	lua_getfield(ls, -1, "searchers");
	if (!lua_istable(ls, -1))
		return 0;

	lua_pushvalue(ls, -2);
	lua_pushcclosure(ls, SearcherPhysfs, 1);
	lua_rawseti(ls, -2, 4 + 1);
	lua_setfield(ls, -2, "searchers");

	return 0;
}
#pragma endregion

#pragma region LlibVin

namespace Vin::Llib::Vin {
	Vin* GetApp(lua_State* ls) {
		lua_getglobal(ls, "app");
		if (!lua_islightuserdata(ls, -1))
			return nullptr;

		Vin* app = (Vin*)lua_touserdata(ls, -1);

		lua_pop(ls, 1);

		return app;
	}

	int InitVin(lua_State* ls) {
		Vin* app = GetApp(ls);

		if (app == nullptr)
			return 1;

		const char* apiname = luaL_checkstring(ls, 1);

		if (strcmp(apiname, "opengl") == 0) {
			app->engine = new Engine(RenderingAPI::OPENGL);
			app->engine->SetProcessCallback([app]() {
					InvokeEvent(app->luaState, "vin_update");
				});
			return 0;
		}

		app->engine = new Engine(RenderingAPI::NONE);

		return 0;
	}

	void CreateEvent(lua_State* ls, const char* eventname) {
		lua_createtable(ls, 1, 0);
		lua_setfield(ls, -2, eventname);
	}

	int RegisterVin(lua_State* ls) {
		if (lua_gettop(ls) != 2 || !lua_isfunction(ls, -1))
			return 0;

		const char* eventname = luaL_checkstring(ls, 1);

		lua_getglobal(ls, LLIB_VIN_NAME);
		if (!lua_istable(ls, -1))
			return 0;

		lua_getfield(ls, -1, "event");
		if (!lua_istable(ls, -1))
			return 0;

		lua_getfield(ls, -1, eventname);
		if (!lua_istable(ls, -1)) {
			lua_pop(ls, 1); //pop false lua get field result
			CreateEvent(ls, eventname);
			lua_getfield(ls, -1, eventname);
		}

		size_t size = lua_rawlen(ls, -1);

		lua_pushvalue(ls, -4);
		lua_rawseti(ls, -2, size + 1);

		return 0;
	}

	int SetWindowSizeVin(lua_State* ls) {
		Vin* app = GetApp(ls);

		if (app == nullptr)
			return 1;

		int width = luaL_checkinteger(ls, 1);
		int height = luaL_checkinteger(ls, 2);

		app->engine->GetWindow()->SetWindowSize(width, height);

		return 0;
	}

	int RunVin(lua_State* ls) {
		Vin* app = GetApp(ls);

		if (app == nullptr)
			return 1;

		app->engine->Run();

		return 0;
	}

	void CreateEventTable(lua_State* ls) {
		lua_createtable(ls, 1, 0);
		lua_setfield(ls, -2, "event");
	}
}

int Vin::Llib::Vin::InvokeEvent(lua_State* ls, const char* eventname)
{
	lua_getglobal(ls, LLIB_VIN_NAME);
	if (!lua_istable(ls, -1))
		return 0;

	lua_getfield(ls, -1, "event");
	if (!lua_istable(ls, -1))
		return 0;

	lua_getfield(ls, -1, eventname);
	if (!lua_istable(ls, -1))
		return 0;

	size_t size = lua_rawlen(ls, -1);

	for (int i = 1; i <= size; i++) {
		if (lua_rawgeti(ls, 3, i) == LUA_TFUNCTION)
			lua_pcall(ls, 0, 0, 0);
	}

	lua_settop(ls, 0);

	return 0;
}

int Vin::Llib::Vin::LlibVin(lua_State* ls)
{
	static const luaL_Reg vin_funcs[] = {
		{"init", InitVin},
		{"register", RegisterVin},
		{"setwindowsize", SetWindowSizeVin},
		{"run", RunVin},
		{NULL, NULL}
	};

	luaL_newlib(ls, vin_funcs);
	CreateEventTable(ls);

	return 1;
}

#pragma endregion
