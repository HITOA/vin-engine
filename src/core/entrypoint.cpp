#include "application.hpp"
#include "core/logger.hpp"

extern Vin::Application* Vin::CreateApp();
extern void Vin::DestroyApp(Vin::Application* app);

int main(int argc, char* argv[]) {
	Vin::Logger::AddLogOutputStream(&std::cout);

	Vin::Application* app = Vin::CreateApp();

	app->Run();

	Vin::DestroyApp(app);

	return 0;
}
