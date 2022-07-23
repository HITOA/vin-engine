#pragma once

#include <luainterface.hpp>

#include <engine.hpp>
#include <physfs.h>

namespace Vin::Llib {
	namespace Physfs {
#define LLIB_PHYSFS_NAME "physfs"
		struct LoadPhysfs {
			PHYSFS_file* f;
			char buff[BUFSIZ];
		};

		const char* GetPhysfs(lua_State* ls, void* ud, size_t* size);
		int LuaLoadFromFile(lua_State* ls, const char* filename);
		int LlibPhysfs(lua_State* ls);
	}

	namespace Vin {
#define LLIB_VIN_NAME "vin"
		struct Vin {
			const char* luaentry{ "" };
			Engine* engine{};
			lua_State* luaState{};
		};

		int InvokeEvent(lua_State* ls, const char* eventname);
		int LlibVin(lua_State* ls);
	}
}
