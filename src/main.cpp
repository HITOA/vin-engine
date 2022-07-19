#include <vin.hpp>
#include <error.hpp>
#include <exception>
#include <luaaa/luaaa.hpp>
#include <ecs.hpp>
#include <iostream>
#include <logger.hpp>

int main(int argc, char* argv[]) {
	try {
        Vin::Logger::AddLogOutputStream(&std::cout);
        Vin::Logger::Log("Log working.");

        Vin::Init();

        Vin::Terminate();
	}
	catch (const std::exception& err) {
		Vin::HandleUnmanagedError(err, 1);
	}
}