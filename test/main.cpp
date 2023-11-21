#include <iostream>
#define GLFW_EXPOSE_NATIVE_WAYLAND
#define GLFW_EXPOSE_NATIVE_EGL

#include <vin/vin.h>
#include <vin/vfs/vfs.h>
#include <vin/vfs/native/nativefilesystem.h>
#include <vin/scene/resources/shader.h>

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
        0, 4, 5,
        0, 5, 6
};

class TestModule : public Vin::Module {
public:
    Vin::Ref<Vin::Shader> vertexShader{};
    Vin::Ref<Vin::Shader> fragmentShader{};
    bgfx::ProgramHandle program{};
    bgfx::VertexLayout layout{};

    void Initialize() final {
        vertexShader = Vin::ResourceManager::Load<Vin::Shader>("/data/shaders/examplevertex.vasset");
        fragmentShader = Vin::ResourceManager::Load<Vin::Shader>("/data/shaders/examplefragment.vasset");

        bgfx::ShaderHandle vertexHandle = vertexShader->GetShaderHandle();
        bgfx::ShaderHandle fragmentHandle = fragmentShader->GetShaderHandle();

        program = bgfx::createProgram(vertexHandle, fragmentHandle);

        layout.begin();
        layout.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float);
        layout.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true);
        layout.end();
    };

    void Update(Vin::TimeStep) final {
        bgfx::TransientVertexBuffer tvb{};
        bgfx::TransientIndexBuffer tib{};

        uint32_t numVertices = sizeof(vertices) / sizeof(float) / 4;
        uint32_t numIndices  = sizeof(indices) / sizeof(short);

        if ( !bgfx::getAvailTransientVertexBuffer( numVertices, layout ) || !bgfx::getAvailTransientIndexBuffer( numIndices ) )
        {
            return;
        }

        bgfx::allocTransientVertexBuffer( &tvb, numVertices, layout );
        bgfx::allocTransientIndexBuffer( &tib, numIndices );

        memcpy( tvb.data, vertices, numVertices * sizeof( float ) * 4 );
        memcpy( tib.data, indices, numIndices * sizeof( short ) );

        uint64_t state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA;
        state |= BGFX_STATE_BLEND_FUNC( BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA );

        bgfx::setState( state );

        bgfx::setVertexBuffer( 0, &tvb, 0, numVertices);
        bgfx::setIndexBuffer( &tib, 0, numIndices);
        bgfx::submit(0, program);
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