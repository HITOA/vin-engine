#include "shaderutils.hpp"

#include "assets/asset.hpp"

std::string SolveInclude(std::string solved) {
	for (
		size_t pos = solved.find("#include", 0);
		pos != std::string::npos;
		pos = solved.find("#include", 0)) {

		size_t beg = solved.find('"', pos) + 1;
		size_t end = solved.find('"', beg);

		std::string path = solved.substr(beg, end - beg);

		Vin::Asset<std::string> include = Vin::AssetDatabase::LoadAsset<std::string>(path);

		solved.replace(pos, end - pos + 1, *include.Get());
	}

	return solved;
}

std::string SolveInclude(Vin::Asset<std::string> shader) {
	std::string solved{ *shader.Get() };

	for (
		size_t pos = solved.find("#include", 0);
		pos != std::string::npos; 
		pos = solved.find("#include", 0)) {

		size_t beg = solved.find('"', pos) + 1;
		size_t end = solved.find('"', beg);

		std::string path = solved.substr(beg, end - beg);

		Vin::Asset<std::string> include = Vin::AssetDatabase::LoadAsset<std::string>(path);

		solved.replace(pos, end - pos + 1, *include.Get());
	}

	return solved;
}

std::shared_ptr<Vin::Program> Vin::LoadProgram(std::string_view vspath, std::string_view fspath)
{
	std::shared_ptr<Vin::Program> program = Vin::Program::Create();

	Vin::Asset<std::string> vsfile = Vin::AssetDatabase::LoadAsset<std::string>(std::string{ vspath });
	Vin::Asset<std::string> fsfile = Vin::AssetDatabase::LoadAsset<std::string>(std::string{ fspath });

	program->AddShader(Vin::ShaderType::VertexShader, SolveInclude(vsfile).data());
	program->AddShader(Vin::ShaderType::FragmentShader, SolveInclude(fsfile).data());

	program->CompileProgram();

	Vin::AssetDatabase::Unload(vsfile);
	Vin::AssetDatabase::Unload(fsfile);
	
	return program;
}

#include "vinshader.hpp"
#include "vininputshader.hpp"
#include "vinlightingshader.hpp"
#include "vinrealtimelightsshader.hpp"
#include "vinshadowsshader.hpp"
#include "vinsurfaceshader.hpp"

void Vin::LoadShaderLibrary()
{
	Vin::AssetDatabase::AddAsset<std::string>(SolveInclude(vinshader), "vin.glsl").SetFlags(Vin::AssetFlag::Persistent);
	Vin::AssetDatabase::AddAsset<std::string>(SolveInclude(vininputshader), "vininput.glsl").SetFlags(Vin::AssetFlag::Persistent);
	Vin::AssetDatabase::AddAsset<std::string>(SolveInclude(vinsurfaceshader), "vinsurface.glsl").SetFlags(Vin::AssetFlag::Persistent);
	Vin::AssetDatabase::AddAsset<std::string>(SolveInclude(vinrealtimelightsshader), "vinrealtimelights.glsl").SetFlags(Vin::AssetFlag::Persistent);
	Vin::AssetDatabase::AddAsset<std::string>(SolveInclude(vinshadowsshader), "vinshadows.glsl").SetFlags(Vin::AssetFlag::Persistent);
	Vin::AssetDatabase::AddAsset<std::string>(SolveInclude(vinlightingshader), "vinlighting.glsl").SetFlags(Vin::AssetFlag::Persistent);
}
