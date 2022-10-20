#include "application.hpp"
#include "logger/logger.hpp"
#include "renderer/renderer.hpp"

#include "assets/assetdatabase.hpp"
#include "vinshader.hpp"
#include "vinpbrshader.hpp"

extern Vin::App* Vin::CreateApp();
extern void Vin::DestroyApp(Vin::App* app);

int main(int argc, char* argv[]) {
	Vin::Renderer::SetApi(Vin::Renderer::Count);

	Vin::AssetDatabase::AddAsset<std::string>(vinshader, "vin.glsl");
	Vin::AssetDatabase::AddAsset<std::string>(vinpbrshader, "vinpbr.glsl");

	Vin::App* app = Vin::CreateApp();

	app->Build();
	app->Run();

	Vin::DestroyApp(app);

	return 0;
}
