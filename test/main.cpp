#include <vin.hpp>

#include "assets/serdes/textureserdes.hpp"
#include "assets/serdes/staticmesheserdes.hpp"

#include <module/windowing/windowmodule.hpp>
#include <module/rendering/renderingmodule.hpp>
#include <module/editor/editormodule.hpp>

#include <utils/gltfutils.hpp>

#include "scene/mesh.hpp"

#include "assets/assetdatabase.hpp"

#include "ecs/registry.hpp"
#include "vfs/vfs.hpp"

#include "utils/pakmaker.hpp"

#include "utils/shaderutils.hpp"

float vertices[] = {
	// positions          // colors           // texture coords
	 1.0f,  1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // top right
	 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   // bottom left
	-1.0f,  1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};

unsigned short indices[] = {
	0, 1, 3,
	1, 2, 3
}; 

struct SpeedRotation {
	float speed{ 1 };

	SpeedRotation(float speed) : speed{ speed } {};
};

class TestModule : public Vin::Module {
	Vin::Asset<Vin::Texture> tex;
	std::shared_ptr<Vin::Program> program;
	std::shared_ptr<Vin::Program> program2;

	Vin::Material mat;
	
	Vin::Asset<Vin::StaticMesh> mesh;

	Vin::Scene<Vin::ArchetypeMemoryLayout::Contiguous> scene{};
	std::shared_ptr<Vin::Camera> camera;

	double t = 0;

	double updateT = 0;
	double processT = 0;
	int updateC = 0;
	int processC = 0;

	static float r;

	static void MoveRandomSystem(Vin::Query<Vin::ArchetypeMemoryLayout::Contiguous, Vin::Transform<float>, SpeedRotation> query) {
		for (auto& [transform, sr] : query) {
			transform->rotation = Vin::Quaternion<float>::Euler(Vin::Vector3<float>{ 0, (float)(r * Vin::deg2rad * sr->speed), 0 });
		}
	}

	void Start() {
		Vin::Logger::Log("Module started.");
		Vin::VFS::AddFileSystem(std::make_shared<Vin::NativeFS>("./bin"));

		program = Vin::LoadProgram("data/vs.glsl", "data/fs.glsl");

		mat = Vin::Material{ program };

		tex = Vin::AssetDatabase::LoadAsset<Vin::Texture>("data/aerial_grass_rock_diff_1k.jpg");
		mat.SetTexture("_MainTex", tex);

		Vin::Asset<Vin::WindowInfo> windowInfo = Vin::AssetDatabase::GetAsset<Vin::WindowInfo>(VIN_WINDOWINFO_ASSETPATH);

		camera = std::shared_ptr<Vin::Camera>{ new Vin::Camera{{windowInfo->width, windowInfo->height} } };

		mesh = Vin::AssetDatabase::LoadAsset<Vin::StaticMesh>("data/suzane.obj");

		for (auto& primitive : *mesh.Get()) {
			primitive.material = std::make_shared<Vin::Material>(mat);
		}

		size_t k = 0;

		for (int j = -3000; j <= 3000; j += 75) {
			for (int i = -3000; i <= 3000; i += 75) {
				++k;
				scene->CreateEntity(
					Vin::Transform<float>{Vin::Vector3<float>{(float)i, (float)j, -3000.0f}},
					Vin::MeshRenderer{ mesh.Get() },
					SpeedRotation{(float)(rand() % 100) / 25});
			}
		}

		Vin::Logger::Log("Number of entity : {}", k);

		scene->CreateEntity(
			Vin::Transform<float>{Vin::Vector3<float>{0, 0.0f, -100.0f}}, 
			Vin::MeshRenderer{ mesh.Get() },
			SpeedRotation{ (float)(rand() % 100) / 25 });

		scene->CreateEntity(
			Vin::Transform<float>{Vin::Vector3<float>{-75.0f, 0.0f, -100.0f}},
			Vin::MeshRenderer{ mesh.Get() },
			SpeedRotation{ (float)(rand() % 100) / 25 });

		scene->CreateEntity(
			Vin::Transform<float>{Vin::Vector3<float>{75.0f, 0.0f, -100.0f}},
			Vin::MeshRenderer{ mesh.Get() },
			SpeedRotation{ (float)(rand() % 100) / 25 });

		mat.SetFloat2("_MainTexTiling", Vin::Vector2<float>{10, 10}.data);

		scene.SetCamera(camera);

		camera->SetFOV(95);
		camera->SetNearPlane(10);
		camera->SetFarPlane(3500);
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

		r += GetApp()->GetDeltaTime().GetMillisecond() * 0.01;
	}

	void Render() {
		scene->Process(MoveRandomSystem);
		scene.Render();
	}

	void OnEvent(Vin::EventHandler handler) {
		if (Vin::WindowCloseEvent* event = handler.GetEvent<Vin::WindowCloseEvent>())
			GetApp()->Stop();
		if (Vin::WindowResizeEvent* event = handler.GetEvent<Vin::WindowResizeEvent>())
			camera->Resize({ event->width, event->height });
	}
};

float TestModule::r{ 0 };

class TestApp : public Vin::App {
public:
	void Build() {
		Vin::WindowInfo winfo{};
		winfo.title = "Test Application";

		Vin::AssetDatabase::AddAsset<Vin::WindowInfo>(std::move(winfo), VIN_WINDOWINFO_ASSETPATH);

		SetUpdateRate(249304934);
		SetProcessRate(493849348);

		AddModule<Vin::WindowModule>();
		AddModule<Vin::RenderingModule>();
		AddModule<Vin::EditorModule>();
		AddModule<TestModule>();
	}
};

Vin::App* Vin::CreateApp() {
	Logger::AddDefaultLogOutputStream();

	return new TestApp{};
}

void Vin::DestroyApp(Vin::App* app) {
	delete app;
}
