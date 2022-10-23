#include "application.hpp"
#include "logger/logger.hpp"
#include "renderer/renderer.hpp"

#include "assets/assetdatabase.hpp"
#include "vinshader.hpp"
#include "vinbaseshader.hpp"

extern Vin::App* Vin::CreateApp();
extern void Vin::DestroyApp(Vin::App* app);

int main(int, char*[]) {
	Vin::Renderer::SetApi(Vin::Renderer::Count);

	Vin::AssetDatabase::AddAsset<std::string>(vinshader, "vin.glsl");
	Vin::AssetDatabase::AddAsset<std::string>(vinbaseshader, "vinbase.glsl");

	Vin::App* app = Vin::CreateApp();

	app->Build();
	app->Run();

	Vin::DestroyApp(app);

	return 0;
}
