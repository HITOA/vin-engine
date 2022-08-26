#include <vin.hpp>

#include <resources/rawfile.hpp>
#include <resources/image.hpp>
#include <ecs/registry.hpp>

struct Position {
	float x;
	float y;
	float z;
};

struct Velocity {
	float dx;
	float dy;
	float dz;
};

struct Acceleration {
	double acc;
};

class TestModule : public Vin::Module {
	eastl::shared_ptr<Vin::Program> program;
	eastl::shared_ptr<Vin::VertexBuffer> vbo;
	eastl::shared_ptr<Vin::IndexBuffer> ibo;
	eastl::shared_ptr<Vin::VertexArray> vao;
	eastl::shared_ptr<Vin::Texture> tex;

	double t = 0;

	double updateT = 0;
	double processT = 0;
	int updateC = 0;
	int processC = 0;

	void OnStart() {
		Vin::Logger::Log("Module is working.");

		Vin::GameFilesystem::Mount("./bin");

		{
			eastl::shared_ptr<Vin::RawFile> vsfile = Vin::Resources::Load<Vin::RawFile>("data/vs.glsl");
			eastl::shared_ptr<Vin::RawFile> fsfile = Vin::Resources::Load<Vin::RawFile>("data/fs.glsl");

			if (vsfile && fsfile) {
				program = Vin::Program::Create();

				program->AddShader(Vin::ShaderType::VertexShader, vsfile->GetData());
				program->AddShader(Vin::ShaderType::FragmentShader, fsfile->GetData());

				program->CompileProgram();

				if (!program->IsShaderComplete())
					Vin::Logger::Log("Program could not compile?");
			}
		}


		float vertices[] = {
			// positions          // colors           // texture coords
			 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
			 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
			-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
			-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
		};

		unsigned short indices[] = {
			0, 1, 3,
			1, 2, 3 
		};

		vbo = Vin::VertexBuffer::Create(sizeof(float) * 32);

		vbo->SetData(&vertices, sizeof(float) * 32, 0);

		Vin::BufferLayout layout{ Vin::BufferElementType::Float3, Vin::BufferElementType::Float3, Vin::BufferElementType::Float2 };

		Vin::Logger::Log("Layout stride is : {}", layout.GetStride());

		vbo->SetBufferLayout(layout);

		ibo = Vin::IndexBuffer::Create(Vin::BufferIndexType::UnsignedInt16);

		ibo->SetData(&indices, 6);

		vao = Vin::VertexArray::Create();

		vao->AddVertexBuffer(vbo);
		vao->SetIndexBuffer(ibo);

		{
			eastl::shared_ptr<Vin::Image> img = Vin::Resources::Load<Vin::Image>("data/container.jpg");

			tex = Vin::Texture::Create(img->GetWidth(), img->GetHeight(), Vin::TextureFormat::RGB24);
			tex->SetData(img->GetData());

			tex->Bind(0);
		}

		Vin::Resources::UnloadUnused();

		Position pos{};
		Velocity vel{};
		Acceleration acc{};

		pos.x = 1;
		pos.y = 2;
		pos.z = 3;
		vel.dx = 4;
		vel.dy = 5;
		vel.dz = 6;
		acc.acc = 7;

		Vin::Registry<Vin::ArchetypeMemoryLayout::Contiguous> registry{};

		Vin::EntityId entity1 = registry.CreateEntity(pos, vel);
		Vin::EntityId entity2 = registry.CreateEntity(vel, acc);
		acc.acc = 2;
		Vin::EntityId entity3 = registry.CreateEntity(vel, acc);
		acc.acc = 4;
		Vin::EntityId entity4 = registry.CreateEntity(vel, acc);
		Vin::EntityId entity5 = registry.CreateEntity(vel, acc, pos);

		Position* pos1 = registry.GetComponent<Position>(entity1);
		Acceleration* acc1 = registry.GetComponent<Acceleration>(entity3);

		Vin::Logger::Log("{}, {}, {}", pos1->x, pos1->y, pos1->z);
		Vin::Logger::Log("{}", acc1->acc);

		registry.DeleteEntity(entity2);

		Acceleration* acc2 = registry.GetComponent<Acceleration>(entity3);

		Vin::Logger::Log("{}", acc2->acc);

		//Vin::Logger::Log("EntityId : {}", registry.CreateEntity(vel, pos));
		
	}

	void OnProcess(Vin::TimeStep ts) {
		processT += ts.GetMillisecond();
		processC += 1;

		if (processT > 1000) {
			Vin::Logger::Log("Average process rate : {} ps ({} ms)", round(1000 / (processT / processC)), (processT / processC));
			processT = 0;
			processC = 0;
		}
	}

	void OnUpdate(Vin::TimeStep ts) {
		updateT += ts.GetMillisecond();
		updateC += 1;

		if (updateT > 1000) {
			Vin::Logger::Log("Average update rate : {} ps ({} ms)", round(1000 / (updateT / updateC)), (updateT / updateC));
			updateT = 0;
			updateC = 0;
		}

		t += 0.1 * ts.GetSecond();
		t = t > 1 ? 0 : t;

		Vin::Matrix4x4<float> mat4{ Vin::Matrix4x4<float>::identity };

		Vin::Scale(mat4, Vin::Vector3<float>{1000, 1000, 1000});
		Vin::Rotate(mat4, Vin::Vector3<float>{1.0f, 0, 0}, 90.0f * (float)Vin::deg2rad);
		Vin::Translate(mat4, Vin::Vector3<float>{0, -6.0f, -6.0f});

		Vin::Matrix4x4<float> projection = Vin::Perspective<float>(90 * Vin::deg2rad, 600.0f / 400.0f, 0.1, 1000);
		mat4 = mat4 * projection;

		program->SetMat4("randommat", mat4.data);

		program->SetFloat3("color", Vin::Color{ 0.2, (float)t, 0.2 }.data);

		Vin::Renderer::Clear(0.05, 0.025, 0, 0.1f);
		program->Bind();
		Vin::Renderer::DrawIndexed(vao);

	}
};

class TestApp : public Vin::Application {
public:
	TestApp(const Vin::ApplicationInfo& info) : Application{ info } {
		AddModule(eastl::make_unique<TestModule>());

		Vin::Renderer::SetViewport(0, 0, 600, 400);

		SetProcessRate(120);
		SetUpdateRate(60);
	}
};

Vin::Application* Vin::CreateApp() {
	Vin::Logger::AddDefaultLogOutputStream();

	Vin::ApplicationInfo info{};

	info.name = "Application test";

	return new TestApp{ info };
}

void Vin::DestroyApp(Vin::Application* app) {
	delete app;
}
