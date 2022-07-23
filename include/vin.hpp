#pragma once

#include <memory>

#include <engine.hpp>
#include <luainterface.hpp>

#define LLIB_VIN_NAME "vin"

namespace Vin {

	/*class Vin {
	public:
		struct LuaStateDeleter {
			void operator()(lua_State* p) {
				LuaInterface::DestroyLuaState(p);
			}
		};
	public:
		void Init(const char* luaentry);

		void Run();
	private:
		void InitLua();
		int LlibVin(lua_State* ls);
		int LuaRegister(lua_State* ls);
	private:
		const char* luaentry{""};
		std::unique_ptr<Engine> engine{};
		std::unique_ptr<lua_State, LuaStateDeleter> luaState{};
	};*/

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
}
