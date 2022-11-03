#include "application.hpp"
#include "logger/logger.hpp"
#include "renderer/renderer.hpp"

#include "utils/shaderutils.hpp"
#include "assets/asset.hpp"

extern Vin::App* Vin::CreateApp();
extern void Vin::DestroyApp(Vin::App* app);

int main(int, char*[]) {
	Vin::Renderer::SetApi(Vin::Renderer::Count);

	Vin::LoadShaderLibrary();

	Vin::App* app = Vin::CreateApp();

	app->Build();
	app->Run();

	Vin::DestroyApp(app);

	return 0;
}
