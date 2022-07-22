#include "lualib.hpp"
#include <logger.hpp>

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
	LoadPhysfs lf{0};

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
