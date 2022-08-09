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

		Vin::Matrix3x3<float> v1{
			1.0f, 0.0f, 0.0f, 
			0.0f, 1.0f, 0.0f, 
			0.0f, 0.0f, 1.0f };
		Vin::Matrix3x3<float> v2 = v1;
		Vin::Matrix3x3<float> v3{ 2 };
		Vin::Matrix<float, 3, 3> v4{};

		v4.data[3] = 3;

		Vin::Matrix3x3<float> v5{ v4 };
		
		decltype(v4)::MatrixType::type v6{ v4 };

		v2(0, 0) = 4;
		v2 += v1;

		v1 += 24;

		Vin::Matrix2x3<float> v7{ 2, 3, 0, 0, 4, 5 };
		Vin::Matrix3x4<float> v8{ 2, 0, 4, 0, 0, 3, 0, 0, 0, 0, 5, 0 };

		auto v9 = v7 * v8;

		Vin::Logger::Log("Matrix : {}", (Vin::Matrix<float, 3, 3>)v1);
		Vin::Logger::Log("Matrix : {}", (Vin::Matrix<float, 3, 3>)v2);
		Vin::Logger::Log("Matrix : {}", (Vin::Matrix<float, 3, 3>)v3);
		Vin::Logger::Log("Matrix : {}", (Vin::Matrix<float, 3, 3>)v4);
		Vin::Logger::Log("Matrix : {}", (Vin::Matrix<float, 3, 3>)v5);
		Vin::Logger::Log("Matrix : {}", (Vin::Matrix<float, 3, 3>)v6);
		Vin::Logger::Log("Matrix : {}", (Vin::Matrix<float, 2, 3>)v7);
		Vin::Logger::Log("Matrix : {}", (Vin::Matrix<float, 3, 4>)v8);
		Vin::Logger::Log("Matrix : {}", (Vin::Matrix<float, 2, 4>)v9);

		Vin::Matrix4x4<float> v10{ 
			2, 0, 0, 3,
			0, 2, 0, 3,
			0, 0, 2, 3,
			0, 0, 0, 1 };

		Vin::Vector4<float> vec4{ 10, 23, 3, 1 };

		Vin::Logger::Log("Transform matrix : {}", (Vin::Matrix<float, 4, 4>)v10);
		Vin::Logger::Log("Vector4 : {}", vec4);

		auto r = v10 * vec4;

		Vin::Matrix4x4<float> v11{};

		Vin::Logger::Log("Result : {}", r);
		Vin::Logger::Log("Transform matrix : {}", (Vin::Matrix<float, 4, 4>)v11);
		Vin::Logger::Log("Transform matrix : {}", (Vin::Matrix<float, 2, 2>)Vin::Matrix2x2<float>::identity);
		Vin::Logger::Log("Transform matrix : {}", (Vin::Matrix<float, 3, 3>)Vin::Matrix3x3<float>::identity);
		Vin::Logger::Log("Transform matrix : {}", (Vin::Matrix<float, 4, 4>)Vin::Matrix4x4<float>::identity);
	}

	void OnProcess(Vin::TimeStep ts) {
		//Vin::Logger::Log("Process rate : {} ps", round(1000 / ts.GetMillisecond()));
	}

	void OnUpdate(Vin::TimeStep ts) {
		//Vin::Logger::Log("Update rate : {} ps", round(1000 / ts.GetMillisecond()));

		t += 1 * ts.GetSecond();
		t = t > 1 ? 0 : t;

		program->SetFloat3("color", Vin::Color{ 0.2, (float)t, 0.2 }.data);

		Vin::Renderer::Clear(0, 0, 0, 0.0f);
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
