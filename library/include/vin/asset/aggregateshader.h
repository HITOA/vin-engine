#ifndef SHADERCROSSCOMPILER_AGGREGATESHADER_H
#define SHADERCROSSCOMPILER_AGGREGATESHADER_H

#include <stddef.h>
#include <vector>
#include <string.h>

#ifdef SHADERCC_IMPL_AGGREGATE_WRITE
#include <fstream>
#include <shadercrosscompiler.h>
#endif

namespace ShaderCC {

    struct AggregateShaderFileEntry {
        size_t position{};
        size_t size{};
    };

    struct AggregateShaderFileHeader {
        AggregateShaderFileEntry glsl{};
        AggregateShaderFileEntry spirv{};
        AggregateShaderFileEntry hlsl{};
        AggregateShaderFileEntry msl{};
    };

#ifdef SHADERCC_IMPL_AGGREGATE_WRITE
    inline bool WriteAggregateShader(std::ofstream& file, Shader& shader) {
        const std::vector<char>& glslSource{ shader.GetGlslSource() };
        const std::vector<char>& spirvSource{ shader.GetSpirVSource() };
        const std::vector<char>& hlslSource{ shader.GetHlslSource() };
        const std::vector<char>& mslSource{ shader.GetMslSource() };

        AggregateShaderFileHeader header{};
        header.glsl.position = sizeof(AggregateShaderFileHeader);
        header.glsl.size = glslSource.size();
        header.spirv.position = header.glsl.position + header.glsl.size;
        header.spirv.size = spirvSource.size();
        header.hlsl.position = header.spirv.position + header.spirv.size;
        header.hlsl.size = hlslSource.size();
        header.msl.position = header.hlsl.position + header.hlsl.size;
        header.msl.size = mslSource.size();

        file.write((char*)&header, sizeof(AggregateShaderFileHeader));
        file.write(glslSource.data(), glslSource.size());
        file.write(spirvSource.data(), spirvSource.size());
        file.write(hlslSource.data(), hlslSource.size());
        file.write(mslSource.data(), mslSource.size());

        return true;
    }
#endif

    inline bool ReadAggregateShaderFileHeader(char* data, size_t size, AggregateShaderFileHeader& header) {
        if (size < sizeof(AggregateShaderFileHeader))
            return false;
        memcpy(&header, data, sizeof(AggregateShaderFileHeader));
        return true;
    }

    inline bool GetOpenGLShader(char* data, size_t size, char* out, size_t outsize) {
        AggregateShaderFileHeader header{};
        if (!ReadAggregateShaderFileHeader(data, size, header))
            return false;
        if (size < header.msl.size + header.msl.position)
            return false;
        if (outsize < header.glsl.size)
            return false;
        memcpy(out, data + header.glsl.position, header.glsl.size);
        return true;
    }

    inline bool GetVulkanShader(char* data, size_t size, char* out, size_t outsize) {
        AggregateShaderFileHeader header{};
        if (!ReadAggregateShaderFileHeader(data, size, header))
            return false;
        if (size < header.msl.size + header.msl.position)
            return false;
        if (outsize < header.spirv.size)
            return false;
        memcpy(out, data + header.spirv.position, header.spirv.size);
        return true;
    }

    inline bool GetDirectXShader(char* data, size_t size, char* out, size_t outsize) {
        AggregateShaderFileHeader header{};
        if (!ReadAggregateShaderFileHeader(data, size, header))
            return false;
        if (size < header.msl.size + header.msl.position)
            return false;
        if (outsize < header.hlsl.size)
            return false;
        memcpy(out, data + header.hlsl.position, header.hlsl.size);
        return true;
    }

    inline bool GetMetalShader(char* data, size_t size, char* out, size_t outsize) {
        AggregateShaderFileHeader header{};
        if (!ReadAggregateShaderFileHeader(data, size, header))
            return false;
        if (size < header.msl.size + header.msl.position)
            return false;
        if (outsize < header.msl.size)
            return false;
        memcpy(out, data + header.msl.position, header.msl.size);
        return true;
    }
}

#endif //SHADERCROSSCOMPILER_AGGREGATESHADER_H
