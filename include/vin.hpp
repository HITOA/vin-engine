#pragma once

#include <memory>

#include <engine.hpp>
#include <luainterface.hpp>

namespace Vin {

	class Vin {
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
	private:
		const char* luaentry{""};
		std::unique_ptr<Engine> engine{};
		std::unique_ptr<lua_State, LuaStateDeleter> luaState{};
	};


	bool Initialize();
	void Terminate();
	Vin* CreateApp();
	void DestroyApp(Vin* app);
}
