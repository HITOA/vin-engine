#include "application.hpp"
#include "logger/logger.hpp"
#include "filesystem/filesystem.hpp"
#include "filesystem/gamefilesystem.hpp"
#include "renderer/renderer.hpp"

extern Vin::App* Vin::CreateApp();
extern void Vin::DestroyApp(Vin::App* app);

int main(int argc, char* argv[]) {

	Vin::GameFilesystem::Init();
	Vin::Renderer::SetApi(Vin::Renderer::Count);

	Vin::App* app = Vin::CreateApp();

	app->Build();
	app->Run();

	Vin::DestroyApp(app);

	return 0;
}
