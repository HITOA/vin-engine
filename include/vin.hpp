#pragma once

#include <memory>

#include <engine.hpp>
#include <luainterface.hpp>

#define LLIB_VIN_NAME "vin"

namespace Vin {

	struct Vin {
		const char* luaentry{ "" };
		Engine* engine{};
		lua_State* luaState{};
	};

	bool Initialize();
	void Terminate();
	Vin* CreateApp();
	void DestroyApp(Vin* app);
	void Run(Vin* app);
	void Process(Vin* app);
}
