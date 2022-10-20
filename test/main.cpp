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
#include "resources/default.hpp"
#include "inputsystem/input.hpp"

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

	Vin::Material mat;

	Vin::Asset<Vin::StaticMesh> mesh;

	Vin::Scene<Vin::ArchetypeMemoryLayout::Contiguous> scene{};
	std::shared_ptr<Vin::Camera> camera;

	std::shared_ptr<Vin::Scene<Vin::ArchetypeMemoryLayout::Contiguous>> sponzascene{};

	double t = 0;

	double updateT = 0;
	double processT = 0;
	int updateC = 0;
	int processC = 0;

	float r;

	Vin::Vector2<int> mouseLastPos{ 0 };

	static void MoveRandomSystem(Vin::Query<Vin::ArchetypeMemoryLayout::Contiguous, Vin::Transform<float>> query, float r) {
		for (auto& [transform] : query) {
			transform->position = Vin::Vector3<float>{ 0, 0, -25 };
			transform->rotation = Vin::Quaternion<float>::Euler(Vin::Vector3<float>{ 0, 0, 0 });
			//transform->position = Vin::Vector3<float>{ 0, 0, -25 };
			//Vin::Logger::Log("Scale : {}", transform->scale);
		}
	}

	void Start() {
		Vin::Logger::Log("Module started.");
		Vin::VFS::AddFileSystem(std::make_shared<Vin::NativeFS>("./bin"));

		program = Vin::LoadProgram("data/vs.glsl", "data/fs.glsl");

		Vin::SetDefaultProgram(program);

		mat = Vin::Material{ program };

		Vin::Asset<Vin::WindowInfo> windowInfo = Vin::AssetDatabase::GetAsset<Vin::WindowInfo>(VIN_WINDOWINFO_ASSETPATH);

		tex = Vin::AssetDatabase::LoadAsset<Vin::Texture>("data/aerial_grass_rock_diff_1k.jpg");
		mat.SetTexture("_MainTex", tex);

		camera = std::shared_ptr<Vin::Camera>{ new Vin::Camera{{windowInfo->width, windowInfo->height} } };

		mesh = Vin::AssetDatabase::LoadAsset<Vin::StaticMesh>("data/suzane.obj");

		for (auto& primitive : *mesh.Get()) {
			primitive.material = std::make_shared<Vin::Material>(mat);
		}

		size_t k = 0;

		/*for (int j = -3000; j <= 3000; j += 75) {
			for (int i = -3000; i <= 3000; i += 75) {
				++k;
				scene->CreateEntity(
					Vin::Transform<float>{Vin::Vector3<float>{(float)i, (float)j, -3000.0f}},
					Vin::MeshRenderer{ mesh.Get() },
					SpeedRotation{(float)(rand() % 100) / 25});
			}
		}*/

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

		mat.SetFloat2("_MainTexTiling", Vin::Vector2<float>{10, 10});

		camera->SetFOV(52.5);
		camera->SetNearPlane(0.1);
		camera->SetFarPlane(1000);

		sponzascene = Vin::LoadGLTF("data/sponzagltf/Sponza.gltf");

		Vin::Transform<float> transform{ Vin::Vector3<float>{0.0f, 0.0f, -10.0f} };

		transform.scale *= 0.5;

		//(*sponzascene)->CreateEntity(
			//transform, Vin::MeshRenderer{ mesh.Get() });

		Vin::Light mainLight{};

		mainLight.shadow.type = Vin::ShadowType::Soft;
		mainLight.color = Vin::Vector4<float>{ 0.95f, 0.95f, 1.00f, 1.0f };

		mainLight.direction = Vin::Vector3<float>{ 0.3, 1, 0.2 }.Normalize();

		(*sponzascene)->CreateEntity(mainLight);
	}

	float pitch = 0;
	float yaw = 0;

	void Process() {
		processT += GetApp()->GetDeltaTime().GetMillisecond();
		processC += 1;

		if (processT > 1000) {
			Vin::Logger::Log("Average process rate : {} ps ({} ms)", round(1000 / (processT / processC)), (processT / processC));
			processT = 0;
			processC = 0;
		}

		float deltaTime = GetApp()->GetDeltaTime().GetMillisecond();


		Vin::Vector2<int> mouseDelta = Vin::Input::GetMousePosition() - mouseLastPos;
		mouseLastPos = Vin::Input::GetMousePosition();

		pitch -= (float)mouseDelta.y * deltaTime * 0.002f;
		yaw -= (float)mouseDelta.x * deltaTime * 0.002f;

		pitch = Vin::Clamp<float>(pitch, -90 * Vin::deg2rad, 90 * Vin::deg2rad);

		Vin::Quaternion<float> qPitch = Vin::Quaternion<float>::Euler({ pitch, 0, 0 });
		Vin::Quaternion<float> qYaw = Vin::Quaternion<float>::Euler({ 0, yaw, 0 });

		camera->rotation = (qYaw * qPitch).Normalize();
		//camera->rotation = (camera->rotation * qYaw).Normalize();

		Vin::Vector3<float> translation{ 0.0f };

		if (Vin::Input::IsKeyDown(Vin::Key::Key_W)) {
			translation += Vin::Vector3<float>{ 0, 0, -1 };
		}
		if (Vin::Input::IsKeyDown(Vin::Key::Key_S)) {
			translation += Vin::Vector3<float>{ 0, 0, 1 };
		}
		if (Vin::Input::IsKeyDown(Vin::Key::Key_A)) {
			translation += Vin::Vector3<float>{ -1 , 0, 0};
		}
		if (Vin::Input::IsKeyDown(Vin::Key::Key_D)) {
			translation += Vin::Vector3<float>{ 1 , 0, 0 };
		}
		if (Vin::Input::IsKeyDown(Vin::Key::Space)) {
			camera->position += Vin::Vector3<float>{ 0, 1 * deltaTime * 0.01f, 0};
		}
		if (Vin::Input::IsKeyDown(Vin::Key::LeftControl)) {
			camera->position += Vin::Vector3<float>{ 0, -1 * deltaTime * 0.01f, 0};
		}

		translation = translation.Normalize();

		translation *= deltaTime * 0.01f;

		translation = (camera->rotation.GetRotationMatrix() * Vin::Vector4<float>{ translation.xyz, 1.0f }).xyz;

		if (Vin::Input::IsKeyDown(Vin::Key::LeftShift))
			translation *= 3;

		camera->position += translation;

		Vin::WindowMouseState event{};
		event.state = Vin::WindowMouseState::Lock;

		Vin::EventHandler handler{};
		handler.Bind(event);

		DispatchEvent(handler);
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
		//scene->Process(MoveRandomSystem);
		//scene.Render(camera);
		sponzascene->Render(camera);
	}

	void LateRender() {
		Vin::Renderer::Blit(camera->GetRenderTarget(), nullptr);
	}

	void OnEvent(Vin::EventHandler handler) {
		if (Vin::WindowCloseEvent* event = handler.GetEvent<Vin::WindowCloseEvent>())
			GetApp()->Stop();
		if (Vin::WindowResizeEvent* event = handler.GetEvent<Vin::WindowResizeEvent>())
			camera->Resize({ event->width, event->height });
	}
};

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
		//AddModule<Vin::EditorModule>();
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
