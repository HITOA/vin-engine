#include <vin.hpp>

#include <filesystem/gamefilesystem.hpp>

#include <module/windowing/windowmodule.hpp>
#include <module/rendering/renderingmodule.hpp>

#include <utils/textureutils.hpp>
#include <utils/gltfutils.hpp>

#include "scene/mesh.hpp"

#include "assets/assetregistry.hpp"

float vertices[] = {
	// positions          // colors         
	 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,  // top right
	 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,  // bottom right
	-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,  // bottom left
	-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,  // top left 
};

float uv[] = {
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f
};

unsigned short indices[] = {
	0, 1, 3,
	1, 2, 3
};
/*
class MyModule : public Vin::Module {
	Vin::Material mat{ nullptr };

	double t = 0;

	double updateT = 0;
	double processT = 0;
	int updateC = 0;
	int processC = 0;

	void Start() {
		Vin::Logger::Log("Module is working.");

		Vin::GameFilesystem::Mount("./bin");

		eastl::shared_ptr<Vin::Program> program = Vin::Program::Create();
		eastl::shared_ptr<Vin::Texture> tex = Vin::LoadTexture("data/container.jpg");

		eastl::shared_ptr<Vin::RawFile> vsfile = Vin::Resources::Load<Vin::RawFile>("data/vs.glsl");
		eastl::shared_ptr<Vin::RawFile> fsfile = Vin::Resources::Load<Vin::RawFile>("data/fs.glsl");

		if (vsfile && fsfile) {
			program->AddShader(Vin::ShaderType::VertexShader, vsfile->GetData());
			program->AddShader(Vin::ShaderType::FragmentShader, fsfile->GetData());

			program->CompileProgram();

			if (!program->IsShaderComplete())
				Vin::Logger::Log("Program could not compile?");
		}

		Vin::Resources::Unload(vsfile);
		Vin::Resources::Unload(fsfile);

		mat = Vin::Material{ program };
		mat.SetTexture("ourTexture", tex);
		mat.SetTexture("ourTexture2", tex);

		Vin::Logger::Log("{}", sizeof(Vin::Primitive));
	}

	void Process() {
		processT += GetApp()->GetDeltaTime().GetMillisecond();
		processC += 1;

		if (processT > 1000) {
			Vin::Logger::Log("Average process rate : {} ps ({} ms)", round(1000 / (processT / processC)), (processT / processC));
			processT = 0;
			processC = 0;
		}
	}

	void Update() {
		updateT += GetApp()->GetDeltaTime().GetMillisecond();
		updateC += 1;

		if (updateT > 1000) {
			Vin::Logger::Log("Average update rate : {} ps ({} ms)", round(1000 / (updateT / updateC)), (updateT / updateC));
			updateT = 0;
			updateC = 0;
		}
	}

	void Render() {
		Vin::Asset<Vin::WindowInfo> windowInfo = GetAsset<Vin::WindowInfo>(VIN_WINDOWINFO_ASSETNAME);

		CreateAsset<int>("Tam?relapute", 12);

		Vin::Matrix4x4<float> mat4{ Vin::Matrix4x4<float>::identity };

		Vin::Scale(mat4, Vin::Vector3<float>{1000, 1000, 1000});
		Vin::Rotate(mat4, Vin::Vector3<float>{1.0f, 0, 0}, 90.0f * (float)Vin::deg2rad);
		Vin::Translate(mat4, Vin::Vector3<float>{0, -6.0f, -6.0f});

		Vin::Matrix4x4<float> projection = Vin::Perspective<float>(90 * Vin::deg2rad, (float)windowInfo->width / (float)windowInfo->height, 0.1, 1000);
		mat4 = mat4 * projection;

		mat.SetMat4("randommat", mat4.data);
		mat.SetFloat3("color", Vin::Color{ 0.2, (float)t, 0.2 }.data);

		Vin::Renderer::Clear(0.3, 0.025, 0.06, 1.0f);
		mat.Bind();
	}

	void OnEvent(Vin::EventHandler handler) {
		if (Vin::WindowCloseEvent* event = handler.GetEvent<Vin::WindowCloseEvent>())
			GetApp()->Stop();
	}
};*/

class AssetTestModule : public Vin::Module {
	void Start() {
		Vin::GameFilesystem::Mount("./bin");

		Vin::Logger::Log("{}", sizeof(Vin::AssetRegistryHeader));

		Vin::AssetRegistry registry{};

		registry.AddPath("TestPuteSalope", 14);
		registry.AddPath("Il etait une fois la vie", 24);
		registry.AddPath("J'aime la viande", 16);
		Vin::AssetId id = registry.AddPath("Les chaussure", 13);
		registry.AddPath("OWOONO", 6);

		Vin::AssetRegistrySerializer::Save(registry, "registry.assetregistry");

		Vin::AssetRegistry newregistry{};

		Vin::AssetRegistrySerializer::Load(newregistry, "registry.assetregistry");
		Vin::Logger::Log("Path of AssetId {} : {}", id, newregistry.GetPath(id).path);
	}
};

class TestApp : public Vin::App {
public:
	void Build() {
		Vin::Logger::Log("Application starting.");

		//AddModule<Vin::WindowModule>();
		//AddModule<Vin::RenderingModule>();
		//AddModule<MyModule>();
		AddModule<AssetTestModule>();
	}
};

Vin::App* Vin::CreateApp() {
	Vin::Logger::AddDefaultLogOutputStream();

	return new TestApp{};
}

void Vin::DestroyApp(Vin::App* app) {
	delete app;
}
