#include <iostream>
#define GLFW_EXPOSE_NATIVE_WAYLAND
#define GLFW_EXPOSE_NATIVE_EGL

#include <vin/vin.h>
#include <vin/vfs/vfs.h>
#include <vin/vfs/native/nativefilesystem.h>
#include <vin/scene/resources/shader.h>
#include <vin/scene/resources/program.h>
#include <vin/scene/resources/mesh.h>
#include <vin/math/glm.h>

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
    Vin::Ref<Vin::Program> program{};
    Vin::Ref<Vin::Material> material{};
    bgfx::VertexLayout layout{};
    Vin::Ref<Vin::Mesh> mesh{};
    Vin::Ref<Vin::Mesh> importedMesh{};
    Vin::GLM::mat4 proj{};
    Vin::GLM::mat4 view{};
    Vin::GLM::mat4 model{};


    void Initialize() final {
        Vin::Ref<Vin::Shader> vertexShader = Vin::ResourceManager::Load<Vin::Shader>("/data/shaders/examplevertex.vasset");
        Vin::Ref<Vin::Shader> fragmentShader = Vin::ResourceManager::Load<Vin::Shader>("/data/shaders/examplefragment.vasset");
        program = Vin::MakeRef<Vin::Program>(vertexShader, fragmentShader);
        material = Vin::MakeRef<Vin::Material>(program);

        importedMesh = Vin::ResourceManager::Load<Vin::Mesh>("/data/mesh/sponza.vasset");

        Vin::MeshData<float, uint16_t, Vin::Core::AllocationStrategy::SingleFrame> meshData{};

        meshData.vertexLayout.begin();
        meshData.vertexLayout.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float);
        meshData.vertexLayout.end();

        meshData.vertices = {
                -1.000000, -1.000000, 1.000000,
                -1.000000, 1.000000, 1.000000,
                -1.000000, 1.000000, -1.000000,
                -1.000000, -1.000000, -1.000000,
                -1.000000, -1.000000, -1.000000,
                -1.000000, 1.000000, -1.000000,
                1.000000, 1.000000, -1.000000,
                1.000000, -1.000000, -1.000000,
                1.000000, -1.000000, -1.000000,
                1.000000, 1.000000, -1.000000,
                1.000000, 1.000000, 1.000000,
                1.000000, -1.000000, 1.000000,
                1.000000, -1.000000, 1.000000,
                1.000000, 1.000000, 1.000000,
                -1.000000, 1.000000, 1.000000,
                -1.000000, -1.000000, 1.000000,
                -1.000000, -1.000000, -1.000000,
                1.000000, -1.000000, -1.000000,
                1.000000, -1.000000, 1.000000,
                -1.000000, -1.000000, 1.000000,
                1.000000, 1.000000, -1.000000,
                -1.000000, 1.000000, -1.000000,
                -1.000000, 1.000000, 1.000000,
                1.000000, 1.000000, 1.000000
        };

        meshData.indices = {
                0,
                1,
                2,
                0,
                2,
                3,
                4,
                5,
                6,
                4,
                6,
                7,
                8,
                9,
                10,
                8,
                10,
                11,
                12,
                13,
                14,
                12,
                14,
                15,
                16,
                17,
                18,
                16,
                18,
                19,
                20,
                21,
                22,
                20,
                22,
                23
        };

        /*meshData.vertices = {
                -0.5f,  0.5f,  0.0f,
                0.5f,  0.5f,  0.0f,
                -0.5f, -0.5f,  0.0f,
                0.5f, -0.5f,  0.0f,
        };

        meshData.indices = {
                0, 1, 2,
                1, 2, 3,
        };*/

        meshData.primitives.push_back(Vin::Primitive{0, 8, 0, 36, material});

        mesh = Vin::MakeRef<Vin::Mesh>(meshData);

        model = Vin::GLM::identity<Vin::GLM::mat4>();
        model = Vin::GLM::rotate(model, glm::radians(90.0f), Vin::GLM::normalize(Vin::GLM::vec3{0.0f, 1.0f, 0.0f}));
        model = Vin::GLM::translate(model, Vin::GLM::vec3{0.0f, 0.0f, 0.0f});
        view = Vin::GLM::identity<Vin::GLM::mat4>();
        //view = Vin::GLM::rotate(view, glm::radians(15.0f), Vin::GLM::normalize(Vin::GLM::vec3{1.0f, 0.0f, 0.0f}));
        view = Vin::GLM::translate(view, Vin::GLM::vec3{ 0.0f, -100.0f, -200.0f });
        proj = Vin::GLM::identity<Vin::GLM::mat4>();
        //view = Vin::GLM::transpose(view);
        //proj = Vin::GLM::transpose(proj);

        proj = Vin::GLM::perspective(glm::radians(45.0f), 640.0f/480.0f, 0.1f, 10000.0f);
    };

    void Update(Vin::TimeStep dt) final {
        //Vin::Logger::Log((float)dt.GetSecond());

        /*for (const auto& primitive : *mesh) {
            bgfx::setTransform(&model[0][0]);
            bgfx::setViewTransform(0, &view[0][0], &proj[0][0]);

            bgfx::setVertexBuffer( 0, mesh->GetVertexBufferHandle(), primitive.startVertex, primitive.numVertices);
            bgfx::setIndexBuffer( mesh->GetIndexBufferHandle(), primitive.startIndex, primitive.numIndices);
            bgfx::submit(0, material->GetProgramHandle());
        }*/

        for (const auto& primitive : *importedMesh) {
            bgfx::setTransform(&model[0][0]);
            bgfx::setViewTransform(0, &view[0][0], &proj[0][0]);

            bgfx::setVertexBuffer( 0, importedMesh->GetVertexBufferHandle(), primitive.startVertex, primitive.numVertices);
            bgfx::setIndexBuffer( importedMesh->GetIndexBufferHandle(), primitive.startIndex, primitive.numIndices);
            bgfx::submit(0, material->GetProgramHandle());
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