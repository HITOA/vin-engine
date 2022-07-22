#pragma once

#include <luainterface.hpp>

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
}
