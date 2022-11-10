#include <vin.hpp>

#include "assets/serdes/textureserdes.hpp"
#include "assets/serdes/staticmesheserdes.hpp"

#include <module/windowing/windowmodule.hpp>
#include <module/editor/editormodule.hpp>
#include <module/forwardrenderer/forwardrenderer.hpp>

#include <utils/gltfutils.hpp>

#include "scene/mesh.hpp"
#include "scene/camera.hpp"

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

struct MoveComponent {
	float speed{ 0.0004f };
};

class TestModule : public Vin::Module {
	Vin::Asset<Vin::Texture> tex;
	std::shared_ptr<Vin::Program> program;

	Vin::Material mat;

	Vin::Asset<Vin::StaticMesh> mesh;

	Vin::Scene scene{};
	std::shared_ptr<Vin::Camera> camera;

	std::shared_ptr<Vin::Scene> sponzascene{};

	double t = 0;

	double updateT = 0;
	double processT = 0;
	int updateC = 0;
	int processC = 0;

	float r;

	Vin::Vector2<int> mouseLastPos{ 0 };

	static void MoveRandomSystem(Vin::Query<Vin::Transform<float>> query, float r) {
		for (auto [transform] : query) {
			transform->position = Vin::Vector3<float>{ 0, 0, -25 };
			transform->rotation = Vin::Quaternion<float>::Euler(Vin::Vector3<float>{ 0, 0, 0 });
			//transform->position = Vin::Vector3<float>{ 0, 0, -25 };
			//Vin::Logger::Log("Scale : {}", transform->scale);
		}
	}

	static void SetRot(Vin::Query<Vin::Transform<float>> query) {
		int i = 0; 
		for (auto [transform] : query) {
			++i;
			transform->rotation = Vin::Quaternion<float>::Euler(Vin::Vector3<float>{ 180, 0, 0 });
			//transform->position = Vin::Vector3<float>{ 0, 0, -25 };
			//Vin::Logger::Log("Scale : {}", transform->scale);
		}
		Vin::Logger::Log("Entity : {}", i);
	}

	static void MoveComponentSystem(Vin::Query<Vin::Transform<float>, MoveComponent> query, float deltaTime) {

		for (auto [transform, movecomp] : query) {
			transform->position.z += movecomp->speed * deltaTime;
			//Vin::Logger::Log("position z : {}", (float)transform->position.z);
		}
	}

	void Start() {
		Vin::Logger::Log("Module started.");
		Vin::VFS::AddFileSystem(std::make_shared<Vin::NativeFS>("./bin"));

		program = Vin::LoadProgram("data/pbr/pbrvs.glsl", "data/pbr/pbrfs.glsl");

		Vin::AssetDatabase::AddAsset(program, "DefaultProgramThings");

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

		camera->SetFOV(52.5);
		camera->SetNearPlane(0.1);
		camera->SetFarPlane(4000);

		sponzascene = Vin::LoadGLTF("data/sponzagltf/Sponza.gltf");
		//(*sponzascene)->Process(SetRot);

		Vin::Transform<float> transform{ Vin::Vector3<float>{0.0f, 10.0f, 0.0f} };

		transform.scale *= 0.05;

		(*sponzascene)->CreateEntity(
			transform, Vin::MeshRenderer{ mesh.Get() }, MoveComponent{});

		Vin::Light mainLight{};
		mainLight.mainLight = true;

		mainLight.color = Vin::Vector3<float>{ 1.0f, 1.0f, 1.0f };
		mainLight.shadow.distance = 20;

		mainLight.direction = Vin::Vector3<float>{ 0.25, 1, 0.15 }.Normalize();

		Vin::Light additionalLight1{};
		additionalLight1.type = Vin::LightType::Spot;
		additionalLight1.direction = Vin::Vector3<float>{ 1.0f, 0.25f, 0.0f };
		additionalLight1.position = Vin::Vector3<float>{ 0.0f, 0.35f, 0.0f };
		additionalLight1.range = 1.0f;

		Vin::Light additionalLight2{};
		additionalLight2.type = Vin::LightType::Point;
		additionalLight2.color = Vin::Vector3<float>{ 0.2f, 1.0f, 0.2f };
		additionalLight2.position = Vin::Vector3<float>{ 3.0f, 1.0f, 0.0f };
		additionalLight2.range = 0.3f;

		(*sponzascene)->CreateEntity(mainLight);
		(*sponzascene)->CreateEntity(additionalLight1);
		(*sponzascene)->CreateEntity(additionalLight2);

	}

	float pitch = 0;
	float yaw = 0;
	bool lock = true;

	void Process() {
		processT += GetApp()->GetDeltaTime().GetMillisecond();
		processC += 1;

		if (processT > 1000) {
			Vin::Logger::Log("Average process rate : {} ps ({} ms)", round(1000 / (processT / processC)), (processT / processC));
			processT = 0;
			processC = 0;
		}

		float deltaTime = GetApp()->GetDeltaTime().GetMillisecond();

		const float speed = 1;

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

		if (Vin::Input::IsKeyPressed(Vin::Key::Key_W)) {
			translation += Vin::Vector3<float>{ 0, 0, -1 };
		}
		if (Vin::Input::IsKeyPressed(Vin::Key::Key_S)) {
			translation += Vin::Vector3<float>{ 0, 0, 1 };
		}
		if (Vin::Input::IsKeyPressed(Vin::Key::Key_A)) {
			translation += Vin::Vector3<float>{ -1 , 0, 0};
		}
		if (Vin::Input::IsKeyPressed(Vin::Key::Key_D)) {
			translation += Vin::Vector3<float>{ 1 , 0, 0 };
		}
		if (Vin::Input::IsKeyPressed(Vin::Key::Space)) {
			camera->position += Vin::Vector3<float>{ 0, speed * deltaTime * 0.01f, 0};
		}
		if (Vin::Input::IsKeyPressed(Vin::Key::LeftControl)) {
			camera->position += Vin::Vector3<float>{ 0, -speed * deltaTime * 0.01f, 0};
		}

		translation = translation.Normalize();

		translation *= deltaTime * 0.01f * speed;

		translation = (camera->rotation.GetRotationMatrix() * Vin::Vector4<float>{ translation.xyz, 1.0f }).xyz;

		if (Vin::Input::IsKeyPressed(Vin::Key::LeftShift))
			translation *= 3;

		camera->position += translation;

		if (Vin::Input::IsKeyDown(Vin::Key::Tab)) {
			Vin::WindowMouseState event{};

			if (lock)
				event.state = Vin::WindowMouseState::Lock;
			else
				event.state = Vin::WindowMouseState::Normal;

			lock = !lock;

			Vin::EventHandler handler{};
			handler.Bind(event);

			DispatchEvent(handler);
		}

		(*sponzascene)->Process(MoveComponentSystem, deltaTime);
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
		AddModule<Vin::ForwardRendererModule>();
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
