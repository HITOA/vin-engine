#include <vin.hpp>

#include <filesystem/gamefilesystem.hpp>

#include <module/windowing/windowmodule.hpp>
#include <module/rendering/renderingmodule.hpp>

#include <utils/textureutils.hpp>

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

class MyModule : public Vin::Module {
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

	void Start() {
		Vin::Logger::Log("Module is working.");

		Vin::GameFilesystem::Mount("./bin");

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

		Vin::Resources::Unload(vsfile);
		Vin::Resources::Unload(fsfile);

		vbo = Vin::VertexBuffer::Create(sizeof(float) * 32);

		vbo->SetData(&vertices, sizeof(float) * 32, 0);

		Vin::VertexBufferLayout layout{ 
			{ Vin::VertexAttribute::Position, Vin::VertexAttributeType::Float3 },
			{ Vin::VertexAttribute::Color, Vin::VertexAttributeType::Float3 },
			{ Vin::VertexAttribute::TextureCoord0, Vin::VertexAttributeType::Float2 }
		};

		Vin::Logger::Log("Layout stride is : {}", layout.GetStride());

		vbo->SetBufferLayout(layout);

		ibo = Vin::IndexBuffer::Create(Vin::BufferIndexType::UnsignedInt16);

		ibo->SetData(&indices, 6);

		vao = Vin::VertexArray::Create();

		vao->AddVertexBuffer(vbo);
		vao->SetIndexBuffer(ibo);

		tex = Vin::LoadTexture("data/container.jpg");
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

		Vin::Matrix4x4<float> mat4{ Vin::Matrix4x4<float>::identity };

		Vin::Scale(mat4, Vin::Vector3<float>{1000, 1000, 1000});
		Vin::Rotate(mat4, Vin::Vector3<float>{1.0f, 0, 0}, 90.0f * (float)Vin::deg2rad);
		Vin::Translate(mat4, Vin::Vector3<float>{0, -6.0f, -6.0f});

		Vin::Matrix4x4<float> projection = Vin::Perspective<float>(90 * Vin::deg2rad, (float)windowInfo->width / (float)windowInfo->height, 0.1, 1000);
		mat4 = mat4 * projection;

		program->SetMat4("randommat", mat4.data);

		program->SetFloat3("color", Vin::Color{ 0.2, (float)t, 0.2 }.data);

		Vin::Renderer::Clear(0.3, 0.025, 0.06, 0.1f);
		program->Bind();
		Vin::Renderer::DrawIndexed(vao);
	}

	void OnEvent(Vin::EventHandler handler) {
		if (Vin::WindowCloseEvent* event = handler.GetEvent<Vin::WindowCloseEvent>())
			GetApp()->Stop();
	}
};

class TestApp : public Vin::App {
public:
	void Build() {
		Vin::Logger::Log("Application starting.");

		AddModule<Vin::WindowModule>();
		AddModule<Vin::RenderingModule>();
		AddModule<MyModule>();
	}
};

Vin::App* Vin::CreateApp() {
	Vin::Logger::AddDefaultLogOutputStream();

	return new TestApp{};
}

void Vin::DestroyApp(Vin::App* app) {
	delete app;
}
