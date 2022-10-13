#include <vin.hpp>

#include "assets/serdes/textureserdes.hpp"

#include <module/windowing/windowmodule.hpp>
#include <module/rendering/renderingmodule.hpp>
#include <module/editor/editormodule.hpp>

#include <utils/gltfutils.hpp>

#include "scene/mesh.hpp"

#include "assets/assetdatabase.hpp"

#include "ecs/registry.hpp"
#include "vfs/vfs.hpp"

#include "utils/pakmaker.hpp"

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

class TestModule : public Vin::Module {
	Vin::Asset<Vin::Texture> tex;
	std::shared_ptr<Vin::Program> program;
	std::shared_ptr<Vin::Program> program2;

	std::shared_ptr<Vin::VertexBuffer> vbo;
	std::shared_ptr<Vin::IndexBuffer> ibo;
	std::shared_ptr<Vin::VertexArray> vao;

	std::shared_ptr<Vin::RenderTarget> renderTarget;
	Vin::Material mat;
	Vin::Material mat2;

	double updateT = 0;
	double processT = 0;
	int updateC = 0;
	int processC = 0;

	void Start() {
		Vin::Logger::Log("Module started.");
		Vin::VFS::AddFileSystem(std::make_shared<Vin::NativeFS>("./bin"));
		
		{
			Vin::Asset<std::string> vsfile = Vin::AssetDatabase::LoadAsset<std::string>("data/vs.glsl");
			Vin::Asset<std::string> fsfile = Vin::AssetDatabase::LoadAsset<std::string>("data/fs.glsl");

			program = Vin::Program::Create();

			program->AddShader(Vin::ShaderType::VertexShader, vsfile->data());
			program->AddShader(Vin::ShaderType::FragmentShader, fsfile->data());

			program->CompileProgram();

			Vin::AssetDatabase::Unload(vsfile);
			Vin::AssetDatabase::Unload(fsfile);
		}

		{
			Vin::Asset<std::string> vsfile = Vin::AssetDatabase::LoadAsset<std::string>("data/blit/blitvs.glsl");
			Vin::Asset<std::string> fsfile = Vin::AssetDatabase::LoadAsset<std::string>("data/blit/blitfs.glsl");

			program2 = Vin::Program::Create();

			program2->AddShader(Vin::ShaderType::VertexShader, vsfile->data());
			program2->AddShader(Vin::ShaderType::FragmentShader, fsfile->data());

			program2->CompileProgram();

			Vin::AssetDatabase::Unload(vsfile);
			Vin::AssetDatabase::Unload(fsfile);
		}

		mat = Vin::Material{ program };
		mat2 = Vin::Material{ program2 };

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

		tex = Vin::AssetDatabase::LoadAsset<Vin::Texture>("data/aerial_grass_rock_diff_1k.jpg");
		mat.SetTexture("ourTexture", tex);

		Vin::Asset<Vin::WindowInfo> windowInfo = Vin::AssetDatabase::GetAsset<Vin::WindowInfo>(VIN_WINDOWINFO_ASSETPATH);

		Vin::RenderTargetSpecification spec{ (size_t)windowInfo->width, (size_t)windowInfo->height, 8 };
		spec.AddRenderBuffer({ Vin::RenderBufferFormat::RGBA32, true });
		spec.AddRenderBuffer({ Vin::RenderBufferFormat::DEPTH24_STENCIL8, false });

		renderTarget = Vin::RenderTarget::Create(spec);

		mat2.SetInt("texSamples", renderTarget->GetSpecification().sample);
		mat2.SetTexture("srcTexture", renderTarget->GetTexture(0));
	}

	void Process() {
		processT += GetApp()->GetDeltaTime().GetMillisecond();
		processC += 1;

		if (processT > 1000) {
			//Vin::Logger::Log("Average process rate : {} ps ({} ms)", round(1000 / (processT / processC)), (processT / processC));
			processT = 0;
			processC = 0;
		}
	}

	void Update() {
		updateT += GetApp()->GetDeltaTime().GetMillisecond();
		updateC += 1;

		if (updateT > 1000) {
			//Vin::Logger::Log("Average update rate : {} ps ({} ms)", round(1000 / (updateT / updateC)), (updateT / updateC));

			updateT = 0;
			updateC = 0;
		}
	}

	void Render() {
		Vin::Asset<Vin::WindowInfo> windowInfo = Vin::AssetDatabase::GetAsset<Vin::WindowInfo>(VIN_WINDOWINFO_ASSETPATH);

		Vin::Matrix4x4<float> mat4{ Vin::Matrix4x4<float>::identity };

		Vin::Scale(mat4, Vin::Vector3<float>{100, 100, 100});
		Vin::Rotate(mat4, Vin::Vector3<float>{1.0f, 0, 0}, 90.0f * (float)Vin::deg2rad);
		Vin::Translate(mat4, Vin::Vector3<float>{0, -6.0f, -6.0f});

		Vin::Matrix4x4<float> projection = Vin::Perspective<float>(90 * Vin::deg2rad, (float)windowInfo->width / (float)windowInfo->height, 0.1, 1000);
		mat4 = mat4 * projection;

		mat.SetMat4("vin_matrix_mvp", mat4.data);
		//mat->SetFloat3("color", Vin::Color{ 0.2, (float)t, 0.2 }.data);

		mat.SetTexture("ourTexture", tex);

		renderTarget->Bind();

		Vin::Renderer::Clear(0.85, 0.85, 1.0, 1.0f);

		mat.Bind();
		Vin::Renderer::DrawIndexed(vao);

		renderTarget->Unbind(); 
		
		Vin::Renderer::Clear(0, 0, 0, 1.0f);

		mat2.Bind();
		Vin::Renderer::DrawIndexed(vao);
	}

	void OnEvent(Vin::EventHandler handler) {
		if (Vin::WindowCloseEvent* event = handler.GetEvent<Vin::WindowCloseEvent>())
			GetApp()->Stop();
		if (Vin::WindowResizeEvent* event = handler.GetEvent<Vin::WindowResizeEvent>())
			renderTarget->Resize(event->width, event->height);
	}
};

class TestApp : public Vin::App {
public:
	void Build() {
		Vin::WindowInfo winfo{};
		winfo.title = "Test Application";

		Vin::AssetDatabase::AddAsset<Vin::WindowInfo>(std::move(winfo), VIN_WINDOWINFO_ASSETPATH);

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
