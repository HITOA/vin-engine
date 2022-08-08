#include <vin.hpp>

#include <renderer/rendering.hpp>
#include <core/math/vector.hpp>
#include <core/math/matrix.hpp>

class TestModule : public Vin::Module {
	std::shared_ptr<Vin::Program> program;
	std::shared_ptr<Vin::VertexBuffer> vbo;
	std::shared_ptr<Vin::IndexBuffer> ibo;
	std::shared_ptr<Vin::VertexArray> vao;

	double t = 0;

	void OnStart() {
		Vin::Logger::Log("Module is working.");

		const char* vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0";

		const char* fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"uniform vec3 color;\n"
			"void main()\n"
			"{\n"
			"	FragColor = vec4(color.xyz, 1.0f);\n"
			"}\n";

		program = Vin::Program::Create();

		program->AddShader(Vin::ShaderType::VertexShader, vertexShaderSource);
		program->AddShader(Vin::ShaderType::FragmentShader, fragmentShaderSource);

		program->CompileProgram();

		Vin::Vector3<float> vertices[] = {
			{0.5f,  0.5f, 0.0f},  // top right
			{0.5f, -0.5f, 0.0f},  // bottom right
			{-0.5f, -0.5f, 0.0f},  // bottom left
			{-0.5f,  0.5f, 0.0f}   // top left 
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

		Vin::Color color{ 0.4, 0.9, 0.3 };

		program->SetFloat3("color", color.data);

		/*Vin::Matrix<float, 3, 3> identity{
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f };

		Vin::Matrix<float, 3, 3> mat{
			1.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 1.0f };
		
		Vin::Logger::Log("Mattrix : {}", identity - mat);
		Vin::Logger::Log("Mattrix : {}", identity - 4.0f);

		Vin::Matrix<float, 2, 2> v1{
			1.0f, 2.0f,
			3.0f, 4.0f
		};
		Vin::Matrix<float, 2, 2> v2{
			5.0f, 6.0f,
			7.0f, 8.0f
		};

		Vin::Logger::Log("Mattrix : {}", v1 * v2);*/
	
		Vin::Matrix3x3<float> v3{ 2.0f };

		v3(0, 0) = 1;

		Vin::Logger::Log("Mattrix : {}", (Vin::Matrix<float, 3, 3>)v3);
	}

	void OnProcess(Vin::TimeStep ts) {
		//Vin::Logger::Log("Process rate : {} ps", round(1000 / ts.GetMillisecond()));
	}

	void OnUpdate(Vin::TimeStep ts) {
		//Vin::Logger::Log("Update rate : {} ps", round(1000 / ts.GetMillisecond()));

		t += 1 * ts.GetSecond();
		t = t > 1 ? 0 : t;

		program->SetFloat3("color", Vin::Color{ 0.2, (float)t, 0.2 }.data);

		Vin::Renderer::Clear(0.2, 0.2, 0.2, 1);
		program->Bind();
		Vin::Renderer::DrawIndexed(vao);

	}
};

class TestApp : public Vin::Application {
public:
	TestApp(const Vin::ApplicationInfo& info) : Application{ info } {
		AddModule(new TestModule{});

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
