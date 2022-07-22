#include "luainterface.hpp"

namespace Vin::LuaInterface {
    lua_State* CreateLuaState()
    {
        return luaL_newstate();
    }

    void DestroyLuaState(lua_State* ls)
    {
        lua_close(ls);
    }

    void OpenLibs(lua_State* ls, luaL_Reg* libs)
    {
        while (libs->func) {
            OpenLib(ls, *libs);
            libs++;
        }
    }

    void OpenLib(lua_State* ls, const luaL_Reg& lib)
    {
        luaL_requiref(ls, lib.name, lib.func, 1);
        lua_pop(ls, 1);
    }

    int Load(lua_State* ls, lua_Reader reader, void* data, const char* chunkname)
    {
        return lua_load(ls, reader, data, chunkname, NULL);
    }

    int Run(lua_State* ls, int nargs, int nresults)
    {
        return lua_pcall(ls, nargs, nresults, 0);
    }

    const char* GetLastError(lua_State* ls)
    {
        const char* errmsg = lua_tostring(ls, -1);
        lua_pop(ls, 1);
        return errmsg;
    }

}