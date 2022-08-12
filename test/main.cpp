#include <vin.hpp>

#include <renderer/rendering.hpp>
#include <core/math/math.hpp>

class TestModule : public Vin::Module {
	std::shared_ptr<Vin::Program> program;
	std::shared_ptr<Vin::VertexBuffer> vbo;
	std::shared_ptr<Vin::IndexBuffer> ibo;
	std::shared_ptr<Vin::VertexArray> vao;

	double t = 0;

	double updateT = 0;
	double processT = 0;
	int updateC = 0;
	int processC = 0;

	void OnStart() {
		Vin::Logger::Log("Module is working.");

		const char* vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"uniform mat4 randommat;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = (randommat * vec4(aPos.x, aPos.y, aPos.z, 1.0));\n"
			"}\0";

		const char* fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"uniform vec3 color;\n"
			"void main()\n"
			"{\n"
			"	FragColor = vec4(color, 1.0f);\n"
			"}\n";

		program = Vin::Program::Create();

		program->AddShader(Vin::ShaderType::VertexShader, vertexShaderSource);
		program->AddShader(Vin::ShaderType::FragmentShader, fragmentShaderSource);

		program->CompileProgram();

		Vin::Vector3<float> vertices[] = {
			{1.0f,  1.0f, 0.0f},  // top right
			{1.0f, -1.0f, 0.0f},  // bottom right
			{-1.0f, -1.0f, 0.0f},  // bottom left
			{-1.0f,  1.0f, 0.0f}   // top left 
		};
		unsigned short indices[] = {
			0, 1, 3,
			1, 2, 3 
		};

		vbo = Vin::VertexBuffer::Create(sizeof(float) * 12);

		vbo->SetData(&vertices, sizeof(float) * 12, 0);

		Vin::BufferLayout layout{ Vin::BufferElementType::Float3 };

		Vin::Logger::Log("Layout stride is : {}", layout.GetStride());

		vbo->SetBufferLayout(layout);

		ibo = Vin::IndexBuffer::Create(Vin::BufferIndexType::UnsignedInt16);

		ibo->SetData(&indices, 6);

		vao = Vin::VertexArray::Create();

		vao->AddVertexBuffer(vbo);
		vao->SetIndexBuffer(ibo);
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

		Vin::Scale(mat4, Vin::Vector3<float>{1, 1, 1});
		Vin::Rotate(mat4, Vin::Vector3<float>{0.662, 0.2, 0.722}, (float)t * 360.0f * (float)Vin::deg2rad);
		Vin::Translate(mat4, Vin::Vector3<float>{0, 0, -3.0f});

		//Vin::Matrix4x4<float> projection = Vin::Orthographic<float>(0, 600, 0, 400, 10, 10000);
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
		AddModule(new TestModule{});

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
