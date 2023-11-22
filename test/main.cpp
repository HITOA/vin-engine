#include <iostream>
#define GLFW_EXPOSE_NATIVE_WAYLAND
#define GLFW_EXPOSE_NATIVE_EGL

#include <vin/vin.h>
#include <vin/vfs/vfs.h>
#include <vin/vfs/native/nativefilesystem.h>
#include <vin/scene/resources/shader.h>
#include <vin/scene/resources/program.h>
#include <vin/scene/resources/mesh.h>

#include <filesystem>
#include <entt/entt.hpp>

static float vertices[] = {
     0.0f,  0.0f,  0.0f, (float)0xFFFFFFFF,
     0.5f,  0.0f,  0.0f, (float)0xFFFFFFFF ,
     0.5f,  0.5f,  0.0f, (float)0xFFFFFFFF ,
     0.0f,  0.5f,  0.0f, (float)0xFFFFFFFF ,
     -0.5f,  0.0f,  0.0f, (float)0xFFFFFFFF ,
     -0.5f,  -0.5f,  0.0f, (float)0xFFFFFFFF ,
     0.0f,  -0.5f,  0.0f, (float)0xFFFFFFFF

};

static short indices[] = {
        0, 1, 2,
        0, 2, 3,
};

class TestModule : public Vin::Module {
public:
    Vin::Ref<Vin::Shader> vertexShader{};
    Vin::Ref<Vin::Shader> fragmentShader{};
    Vin::Ref<Vin::Program> program{};
    Vin::Ref<Vin::Material> material{};
    bgfx::VertexLayout layout{};
    Vin::Ref<Vin::Mesh> mesh{};

    void Initialize() final {
        vertexShader = Vin::ResourceManager::Load<Vin::Shader>("/data/shaders/examplevertex.vasset");
        fragmentShader = Vin::ResourceManager::Load<Vin::Shader>("/data/shaders/examplefragment.vasset");
        program = Vin::MakeRef<Vin::Program>(vertexShader, fragmentShader);
        material = Vin::MakeRef<Vin::Material>(program);

        Vin::MeshData<float, uint16_t, Vin::Core::AllocationStrategy::SingleFrame> meshData{};

        meshData.vertexLayout.begin();
        meshData.vertexLayout.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float);
        meshData.vertexLayout.end();

        meshData.vertices = {
                -0.5f,  0.5f,  0.5f,
                 0.5f,  0.5f,  0.5f,
                -0.5f, -0.5f,  0.5f,
                 0.5f, -0.5f,  0.5f,
                -0.5f,  0.5f, -0.5f,
                 0.5f,  0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f,
                 0.5f, -0.5f, -0.5f
        };

        meshData.indices = {
                0, 2, 1,
                1, 2, 3,
                4, 5, 6,
                5, 7, 6,
                0, 4, 2,
                4, 6, 2,
                1, 3, 5,
                5, 3, 7,
                0, 1, 4,
                4, 1, 5,
                2, 6, 3,
                6, 7, 3,
        };

        meshData.primitives.push_back(Vin::Primitive{0, 8, 0, 36, material});

        mesh = Vin::MakeRef<Vin::Mesh>(meshData);
    };

    void Update(Vin::TimeStep) final {
        for (const auto& primitive : *mesh) {
            bgfx::setVertexBuffer( 0, mesh->GetVertexBufferHandle(), primitive.startVertex, primitive.numVertices);
            bgfx::setIndexBuffer( mesh->GetIndexBufferHandle(), primitive.startIndex, primitive.numIndices);
            bgfx::submit(0, primitive.material->GetProgramHandle());
        }
    }
};

int main() {
    //setbuf(stdout, 0);

    Vin::Logger::Logger::AddLogOutput(&std::cout);

    std::filesystem::current_path("/mnt/SSD1TO/Project/vin-engine");

    Vin::VirtualFileSystem::AddFileSystem("/", Vin::MakeRef<Vin::IO::NativeFileSystem>());
    Vin::VirtualFileSystem::AddFileSystem("/tmp", Vin::MakeRef<Vin::IO::NativeFileSystem>("/tmp"));

    Vin::App app{};
    app.AddModule<Vin::Modules::WindowModule>();
    app.AddModule<Vin::Modules::RenderingModule>(Vin::Modules::RenderingApi::Count);
    app.AddModule<TestModule>();
    app.Run();

}