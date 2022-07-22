#include <vin.hpp>
#include <error.hpp>
#include <exception>
#include <iostream>
#include <logger.hpp>

int main(int argc, char* argv[]) {
	try {
        Vin::Logger::AddLogOutputStream(&std::cout);
        Vin::Logger::Log("Log working.");

		if (!Vin::Initialize())
			return -1;

		Vin::Logger::Log("Vin initialized.");

		Vin::Vin* app = Vin::CreateApp();
		app->Init("init.lua");
		app->Run();

		Vin::DestroyApp(app);
		Vin::Terminate();
	}
	catch (const std::exception& err) {
		Vin::HandleUnmanagedError(err, 1);
	}

	return 0;
}