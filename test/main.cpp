#include <iostream>
#define GLFW_EXPOSE_NATIVE_WAYLAND
#define GLFW_EXPOSE_NATIVE_EGL

#include <vin/vin.h>
#include <vin/vfs/vfs.h>
#include <vin/vfs/native/nativefilesystem.h>
#include <vin/scene/resources/shader.h>
#include <vin/scene/resources/program.h>
#include <vin/scene/resources/mesh.h>
#include <vin/scene/resources/texture.h>
#include <vin/math/glm.h>
#include <vin/rendering/renderer.h>
#include <fstream>

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
    /*Vin::Ref<Vin::Material> material{};
    Vin::Ref<Vin::Mesh> importedMesh{};
    Vin::GLM::mat4 proj{};
    Vin::GLM::mat4 view{};
    Vin::GLM::mat4 model{};
    Vin::GLM::vec4 data{ 1.0, 0.0, 1.0, 1.0 };
    Vin::Ref<Vin::Texture> texture{};*/
    Vin::ShaderHandle vertexShader{};
    Vin::ShaderHandle fragmentShader{};
    Vin::ProgramHandle program{};
    Vin::FramebufferHandle framebuffer{};

    void Initialize() final {
        std::filesystem::path vertexShaderPath{ "/mnt/SSD1TO/Project/testShader/outputvertex.spirv" };
        std::filesystem::path fragmentShaderPath{ "/mnt/SSD1TO/Project/testShader/outputfragment.spirv" };

        std::fstream vertexShaderFile{ vertexShaderPath, std::ios_base::binary | std::ios_base::in };

        vertexShaderFile.ignore( std::numeric_limits<std::streamsize>::max() );
        size_t size = vertexShaderFile.gcount();
        vertexShaderFile.clear();
        vertexShaderFile.seekg(0, std::ios_base::beg);

        std::vector<char> vertexData{};
        vertexData.resize(size + 1);
        vertexShaderFile.read(vertexData.data(), vertexData.size());
        vertexShaderFile.close();
        vertexData[size] = 0;

        vertexShader = Vin::Renderer::LoadShader(vertexData.data(), vertexData.size());

        std::fstream fragmentShaderFile{ fragmentShaderPath, std::ios_base::binary | std::ios_base::in };

        fragmentShaderFile.ignore( std::numeric_limits<std::streamsize>::max() );
        size = fragmentShaderFile.gcount();
        fragmentShaderFile.clear();
        fragmentShaderFile.seekg(0, std::ios_base::beg);

        std::vector<char> fragmentData{};
        fragmentData.resize(size + 1);
        fragmentShaderFile.read(fragmentData.data(), fragmentData.size());
        fragmentShaderFile.close();
        fragmentData[size] = 0;

        fragmentShader = Vin::Renderer::LoadShader(fragmentData.data(), fragmentData.size());

        program = Vin::Renderer::CreateProgram(vertexShader, fragmentShader);

        Vin::FramebufferCreationInfo framebufferCreationInfo{};
        framebufferCreationInfo.attachmentCount = 2;
        framebufferCreationInfo.attachments[0].format = Vin::FramebufferAttachmentDescription::RGBA32;
        framebufferCreationInfo.attachments[1].isDepthBuffer = true;

        framebuffer = Vin::Renderer::CreateFramebuffer(framebufferCreationInfo);

        /*Vin::Ref<Vin::Program> program = Vin::ResourceManager::Load<Vin::Program>("/data/shaders/default.vasset");
        material = Vin::MakeRef<Vin::Material>(program);
        material->SetVec4("colorA", Vin::GLM::vec4{0.0, 1.0, 1.0, 1.0});
        material->SetVec4("colorB", Vin::GLM::vec4{1.0, 1.0, 0.0, 1.0});

        importedMesh = Vin::ResourceManager::Load<Vin::Mesh>("/data/mesh/sponza.vasset");
        texture = Vin::ResourceManager::Load<Vin::Texture>("/data/icons/text-x-generic.vasset");
        material->SetTexture("albedo", texture);

        model = Vin::GLM::identity<Vin::GLM::mat4>();
        model = Vin::GLM::rotate(model, glm::radians(90.0f), Vin::GLM::normalize(Vin::GLM::vec3{0.0f, 1.0f, 0.0f}));
        model = Vin::GLM::translate(model, Vin::GLM::vec3{0.0f, 0.0f, 0.0f});
        view = Vin::GLM::identity<Vin::GLM::mat4>();
        //view = Vin::GLM::rotate(view, glm::radians(15.0f), Vin::GLM::normalize(Vin::GLM::vec3{1.0f, 0.0f, 0.0f}));
        view = Vin::GLM::translate(view, Vin::GLM::vec3{ 0.0f, -100.0f, -200.0f });
        proj = Vin::GLM::identity<Vin::GLM::mat4>();
        //view = Vin::GLM::transpose(view);
        //proj = Vin::GLM::transpose(proj);

        proj = Vin::GLM::perspective(glm::radians(45.0f), 640.0f/480.0f, 0.1f, 10000.0f);*/
    };

    void Update(Vin::TimeStep dt) final {
        /*material->UpdateUniforms();

        for (const auto& primitive : *importedMesh) {
            bgfx::setTransform(&model[0][0]);
            bgfx::setViewTransform(0, &view[0][0], &proj[0][0]);

            bgfx::setVertexBuffer( 0, importedMesh->GetVertexBufferHandle(), primitive.startVertex, primitive.numVertices);
            bgfx::setIndexBuffer( importedMesh->GetIndexBufferHandle(), primitive.startIndex, primitive.numIndices);

            bgfx::submit(0, material->GetProgramHandle());
        }*/
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
    app.AddModule<Vin::Modules::RenderingModule>();
    app.AddModule<TestModule>();
    app.Run();

   /* Vin::RendererInitInfo initInfo{};

    Vin::RenderingApi::Init(initInfo);
    Vin::RenderingApi::Terminate();*/
}