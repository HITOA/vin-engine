#include "application.hpp"
#include "logger/logger.hpp"
#include "filesystem/filesystem.hpp"
#include "filesystem/gamefilesystem.hpp"

extern Vin::Application* Vin::CreateApp();
extern void Vin::DestroyApp(Vin::Application* app);

int main(int argc, char* argv[]) {

	Vin::GameFilesystem::Init();

	Vin::Application* app = Vin::CreateApp();

	app->Run();

	Vin::DestroyApp(app);

	return 0;
}
