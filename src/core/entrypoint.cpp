#include "application.hpp"
#include "logger/logger.hpp"

extern Vin::Application* Vin::CreateApp();
extern void Vin::DestroyApp(Vin::Application* app);

int main(int argc, char* argv[]) {
	Vin::Application* app = Vin::CreateApp();

	app->Run();

	Vin::DestroyApp(app);

	return 0;
}
