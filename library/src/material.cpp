#include <vin/scene/resources/material.h>
#include <vin/core/templates/hash.h>
/*
Vin::Material::Material(Ref<Vin::Program> program) : program{ program } {

}

Vin::Material::~Material() {
    for (uint32_t i = 0; i < propertyCount; ++i) {
        bgfx::destroy(properties[i].handle);
        if (properties[i].num > 1)
            Vin::Core::MemoryManager::GetInstance()->Deallocate<Vin::Core::AllocationStrategy::Persistent>(properties[i].data);
    }
}

bgfx::ProgramHandle Vin::Material::GetProgramHandle() {
    return program->GetProgramHandle();
}

void Vin::Material::UpdateUniforms() {
    for (uint32_t i = 0; i < propertyCount; ++i) {
        MaterialProperty& materialProperty = properties[i];
        if (materialProperty.num > 1) {
            bgfx::setUniform(materialProperty.handle, materialProperty.data, materialProperty.num);
            continue;
        }

        switch (materialProperty.type) {
            case bgfx::UniformType::Vec4:
                bgfx::setUniform(materialProperty.handle, &materialProperty.v4[0], 1);
                break;
            case bgfx::UniformType::Mat3:
                bgfx::setUniform(materialProperty.handle, &materialProperty.m3[0][0], 1);
                break;
            case bgfx::UniformType::Mat4:
                bgfx::setUniform(materialProperty.handle, &materialProperty.m4[0][0], 1);
                break;
        }
    }

    for (uint32_t i = 0; i < textureCount; ++i) {
        MaterialTexture& materialTexture = textures[i];
        bgfx::setTexture(0, materialTexture.handle, materialTexture.tex->GetTextureHandle());
    }
}

void Vin::Material::SetVec4(const Vin::StringView& name, const GLM::vec4 &v4) {
    uint32_t hname = Hash(name);
    for (uint32_t i = 0; i < propertyCount; ++i) {
        if (properties[i].name == hname) {
            properties[i].v4 = v4;
            return;
        }
    }
    if (propertyCount < 15) {
        properties[propertyCount].handle = bgfx::createUniform(name.data(), bgfx::UniformType::Vec4, 1);
        properties[propertyCount].type = bgfx::UniformType::Vec4;
        properties[propertyCount].num = 1;
        properties[propertyCount].name = hname;
        properties[propertyCount].v4 = v4;
        ++propertyCount;
    }
}

void Vin::Material::SetMat3(const Vin::StringView& name, const GLM::mat3 &m3) {
    uint32_t hname = Hash(name);
    for (uint32_t i = 0; i < propertyCount; ++i) {
        if (properties[i].name == hname) {
            properties[i].m3 = m3;
            return;
        }
    }
    if (propertyCount < 15) {
        properties[propertyCount].handle = bgfx::createUniform(name.data(), bgfx::UniformType::Mat3, 1);
        properties[propertyCount].type = bgfx::UniformType::Mat3;
        properties[propertyCount].num = 1;
        properties[propertyCount].name = hname;
        properties[propertyCount].m3 = m3;
        ++propertyCount;
    }
}

void Vin::Material::SetMat4(const Vin::StringView& name, const GLM::mat4 &m4) {
    uint32_t hname = Hash(name);
    for (uint32_t i = 0; i < propertyCount; ++i) {
        if (properties[i].name == hname) {
            properties[i].m4 = m4;
            return;
        }
    }
    if (propertyCount < 15) {
        properties[propertyCount].handle = bgfx::createUniform(name.data(), bgfx::UniformType::Mat4, 1);
        properties[propertyCount].type = bgfx::UniformType::Mat4;
        properties[propertyCount].num = 1;
        properties[propertyCount].name = hname;
        properties[propertyCount].m3 = m4;
        ++propertyCount;
    }
}

void Vin::Material::SetTexture(const Vin::StringView& name, const Vin::Ref<Vin::Texture>& texture) {
    uint32_t hname = Hash(name);
    for (uint32_t i = 0; i < textureCount; ++i) {
        if (textures[i].name == hname) {
            textures[i].tex = texture;
            return;
        }
    }
    if (textureCount < 15) {
        textures[textureCount].handle = bgfx::createUniform(name.data(), bgfx::UniformType::Sampler, 1);
        textures[textureCount].name = hname;
        textures[textureCount].tex = texture;
        ++textureCount;
    }
}*/