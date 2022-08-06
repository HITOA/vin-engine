#include <vin.hpp>

#include <renderer/rendering.hpp>
#include <core/math/vector.hpp>

class TestModule : public Vin::Module {
	std::shared_ptr<Vin::Program> program;
	std::shared_ptr<Vin::VertexBuffer> vbo;
	std::shared_ptr<Vin::IndexBuffer> ibo;
	std::shared_ptr<Vin::VertexArray> vao;

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
			"void main()\n"
			"{\n"
			"	FragColor = vec4(1.0f, 0.2f, 0.5f, 1.0f);\n"
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
		unsigned short indices[] = {  // note that we start from 0!
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
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

		Vin::Vector3<float> vec3{ 3, 5, 2 };
		Vin::Vector3<float> vec0{ 0 };
		Vin::Vector3<float> vec11{ 11 };

		vec3.x = 4;
		vec3.x += 12;
		vec3.x = vec3.x + vec3.y + (vec3.zz + vec3.xy).x;
		vec3.x *= 4;

		if (vec3.x > 4)
			vec3.x = 4;

		vec3.z = vec3.y;

		vec3 = vec3.xzx;

		Vin::Vector2<float> vec2 = vec3.yz;

		vec2 += vec2.xx / vec3.xz;

		Vin::Vector4<float> vec4 = vec3.xxxz + vec2.xyxy;
		vec4 += vec3.zzzz;

		Vin::Vector3<float>::Add(vec3, vec2.xxy, vec3);

		Vin::Vector3<float>::Ceil(vec3);
		Vin::Vector3<float>::Clamp(vec3, vec0, vec11);

		Vin::Logger::Log("Vec2 : {}", vec2);		
		Vin::Logger::Log("Vec3 : {}", vec3);
		Vin::Logger::Log("Vec4 : {}", vec4);
	}

	void OnProcess(Vin::TimeStep ts) {
		//Vin::Logger::Log("Process rate : {} ps", round(1000 / ts.GetMillisecond()));
	}

	void OnUpdate(Vin::TimeStep ts) {
		//Vin::Logger::Log("Update rate : {} ps", round(1000 / ts.GetMillisecond()));
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
