#include <vin.hpp>
#include <vincore.hpp>
#include <luaaa/luaaa.hpp>
#include <stdexcept>
#include <logger.hpp>
#include <linit.hpp>
#include <psf.hpp>

namespace Vin {
	namespace {
		bool _isInit;
		lua_State* _ls;
	}

	void InitVincore() {
		if (!Psf::Exists("init.lua"))
			throw std::runtime_error{ "init.lua is missing." };

		std::vector<char> buffer = Psf::ReadFile("init.lua");

		Vin::Lua::LoadBuffer(buffer.data(), buffer.size(), "init.lua");
		Vin::Lua::Run();
	}

	void Init()
	{
		if (_isInit)
			return;

		Psf::Init();
		Lua::Init();

		Lua::LoadLibrary(PSFLUA_LIBNAME, Psf::LLib::psf_llib);
		Lua::LoadLibrary(VINLUA_LIBNAME, Core::LLib::vin_llib);

		InitVincore();

		_isInit = true;
	}

	void Terminate()
	{
		if (!_isInit)
			return;

		Vin::Core::Terminate();
		Vin::Lua::Terminate();
		Psf::Terminate();

		_isInit = false;
	}
}
