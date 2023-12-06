#ifndef VIN_ENGINE_PROGRAM_H
#define VIN_ENGINE_PROGRAM_H

#include <vin/scene/resources/shader.h>
#include <bgfx/bgfx.h>

namespace Vin {

    class Program {
    public:
        Program(Ref<Shader> vertexShader, Ref<Shader> fragmentShader);

        bgfx::ProgramHandle GetProgramHandle();

    private:
        Ref<Shader> vsh{};
        Ref<Shader> fsh{};
        bgfx::ProgramHandle handle{};
    };

    static Ref<Shader> CreateShader(Vin::Vector<char>& shaderData, Vin::ShaderType stage) {
        ShaderCC::AggregateShaderFileHeader aggregateShaderFileHeader = *(ShaderCC::AggregateShaderFileHeader*)shaderData.data();

        const bgfx::Memory* buff{ nullptr };
        bool r{ false };

        uint32_t size{ 0 };

        switch (bgfx::getRendererType()) {
            case bgfx::RendererType::OpenGL:
                size = aggregateShaderFileHeader.glsl.size;
                buff = bgfx::alloc(size);
                if (buff == nullptr)
                    return Ref<Shader>{};
                r = ShaderCC::GetOpenGLShader(shaderData.data(), shaderData.size(), (char*)buff->data, buff->size);
                break;
            case bgfx::RendererType::Vulkan:
                size = aggregateShaderFileHeader.spirv.size;
                buff = bgfx::alloc(size);
                if (buff == nullptr)
                    return Ref<Shader>{};
                r = ShaderCC::GetVulkanShader(shaderData.data(), shaderData.size(), (char*)buff->data, buff->size);
                break;
            case bgfx::RendererType::Direct3D9:
            case bgfx::RendererType::Direct3D11:
            case bgfx::RendererType::Direct3D12:
                size = aggregateShaderFileHeader.hlsl.size;
                buff = bgfx::alloc(size);
                if (buff == nullptr)
                    return Ref<Shader>{};
                r = ShaderCC::GetDirectXShader(shaderData.data(), shaderData.size(), (char*)buff->data, buff->size);
                break;
            case bgfx::RendererType::Metal:
                size = aggregateShaderFileHeader.msl.size;
                buff = bgfx::alloc(size);
                if (buff == nullptr)
                    return Ref<Shader>{};
                r = ShaderCC::GetMetalShader(shaderData.data(), shaderData.size(), (char*)buff->data, buff->size);
                break;
            default:
                break;
        }

        return Vin::MakeRef<Shader>(buff);
    }

    template<>
    struct ResourceLoader<Program> {
        Ref<Program> operator()(const Ref<IO::File>& file) {
            AssetFileHeader header{};
            if (file->ReadBytes((char*)&header, sizeof(AssetFileHeader)) != sizeof(AssetFileHeader) || !CheckMagic(header)) {
                Vin::Logger::Err("Couldn't load Texture. Provided asset file isn't in the right format or is corrupted.");
                return Ref<Program>{};
            }

            if (header.type != Vin::AssetType::Program) {
                Vin::Logger::Err("Couldn't load Texture. Provided asset isn't a Texture.");
                return Ref<Program>{};
            }


            uint16_t propertyCount{};
            file->ReadBytes((char*)&propertyCount, sizeof(uint16_t));

            uint32_t vertexSize{};
            uint32_t fragmentSize{};
            file->ReadBytes((char*)&vertexSize, sizeof(uint32_t));
            file->ReadBytes((char*)&fragmentSize, sizeof(uint32_t));

            Vin::Vector<char> vertexData{};
            Vin::Vector<char> fragmentData{};
            vertexData.resize(vertexSize);
            fragmentData.resize(fragmentSize);
            file->ReadBytes(vertexData.data(), vertexData.size());
            file->ReadBytes(fragmentData.data(), fragmentData.size());

            Vin::Ref<Program> program = Vin::MakeRef<Program>(CreateShader(vertexData, Vin::ShaderType::Vertex),
                                                              CreateShader(fragmentData, Vin::ShaderType::Fragment));

            return program;
        }
    };

}

#endif //VIN_ENGINE_PROGRAM_H
