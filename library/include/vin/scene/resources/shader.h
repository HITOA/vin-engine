#ifndef VIN_ENGINE_SHADER_H
#define VIN_ENGINE_SHADER_H

#include <vin/core/templates/stdcontainers.h>
#include <vin/resource/resourceloader.h>
#include <vin/asset/assetfile.h>
#include <vin/asset/assetshader.h>
#include <vin/asset/aggregateshader.h>
#include <vin/core/logger/logger.h>
#include <bgfx/bgfx.h>

#define BGFX_SHADER_HEADER_SIZE 18

namespace Vin {

    class Shader {
    public:
        explicit Shader(const bgfx::Memory* data);
        ~Shader();

        bgfx::ShaderHandle GetShaderHandle();
    private:
        bgfx::ShaderHandle handle{};
    };

    template<>
    struct ResourceLoader<Shader> {
        Ref<Shader> operator()(const Ref<IO::File>& file) {
            ShaderFileHeader header{};
            if (file->ReadBytes((char*)&header, sizeof(ShaderFileHeader)) != sizeof(ShaderFileHeader) || !CheckMagic(header.header)) {
                Vin::Logger::Err("Couldn't load Shader. Provided asset file isn't in the right format or is corrupted.");
                return Ref<Shader>{};
            }

            if (header.header.type != Vin::AssetType::Shader) {
                Vin::Logger::Err("Couldn't load Shader. Provided asset isn't a Shader.");
                return Ref<Shader>{};
            }

            if (header.type == Vin::ShaderType::Compute) {
                //Warning
                Vin::Logger::Warn("Using Shader class for compute shader instead of the ComputeShader class.");
            }

            Vin::Vector<char, Vin::Core::AllocationStrategy::SingleFrame> data{};
            ShaderCC::AggregateShaderFileHeader aggregateShaderFileHeader{};
            data.resize(sizeof(ShaderCC::AggregateShaderFileHeader));

            if (file->ReadBytes(data.data(), data.size()) !=
                    data.size()) {
                Vin::Logger::Err("Couldn't load Shader. The file seems corrupted.");
                return Ref<Shader>{};
            }

            aggregateShaderFileHeader = *(ShaderCC::AggregateShaderFileHeader*)data.data();

            data.resize(aggregateShaderFileHeader.msl.size + aggregateShaderFileHeader.msl.position);

            if (file->ReadBytes(data.data() + sizeof(ShaderCC::AggregateShaderFileHeader), data.size() -
                    sizeof(ShaderCC::AggregateShaderFileHeader)) != data.size() - sizeof(ShaderCC::AggregateShaderFileHeader)) {
                Vin::Logger::Err("Couldn't load Shader. The file seems corrupted.");
                return Ref<Shader>{};
            }

            const bgfx::Memory* buff{ nullptr };
            bool r{ false };

            uint32_t size{ 0 };

            switch (bgfx::getRendererType()) {
                case bgfx::RendererType::OpenGL:
                    size = aggregateShaderFileHeader.glsl.size;
                    buff = bgfx::alloc(size);
                    if (buff == nullptr)
                        return Ref<Shader>{};
                    r = ShaderCC::GetOpenGLShader(data.data(), data.size(), (char*)buff->data, buff->size);
                    break;
                case bgfx::RendererType::Vulkan:
                    size = aggregateShaderFileHeader.spirv.size;
                    buff = bgfx::alloc(size);
                    if (buff == nullptr)
                        return Ref<Shader>{};
                    r = ShaderCC::GetVulkanShader(data.data(), data.size(), (char*)buff->data, buff->size);
                    break;
                case bgfx::RendererType::Direct3D9:
                case bgfx::RendererType::Direct3D11:
                case bgfx::RendererType::Direct3D12:
                    size = aggregateShaderFileHeader.hlsl.size;
                    buff = bgfx::alloc(size);
                    if (buff == nullptr)
                        return Ref<Shader>{};
                    r = ShaderCC::GetDirectXShader(data.data(), data.size(), (char*)buff->data, buff->size);
                    break;
                case bgfx::RendererType::Metal:
                    size = aggregateShaderFileHeader.msl.size;
                    buff = bgfx::alloc(size);
                    if (buff == nullptr)
                        return Ref<Shader>{};
                    r = ShaderCC::GetMetalShader(data.data(), data.size(), (char*)buff->data, buff->size);
                    break;
                default:
                    break;
            }

            return Vin::MakeRef<Shader>(buff);
        }
    };

}

#endif //VIN_ENGINE_SHADER_H
