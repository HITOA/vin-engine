#include <vincore.hpp>
#include <error.hpp>
#include <exception>
#include <luaaa/luaaa.hpp>

int main(int argc, char* argv[]) {
	try {
		lua_State* ls = luaL_newstate();
	}
	catch (const std::exception& err) {
		Vin::HandleUnmanagedError(err, 1);
	}
}