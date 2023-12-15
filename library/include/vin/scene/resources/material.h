#ifndef VIN_ENGINE_MATERIAL_H
#define VIN_ENGINE_MATERIAL_H

#include <vin/scene/resources/program.h>
#include <vin/scene/resources/texture.h>
#include <vin/math/glm.h>

namespace Vin {
    /*struct MaterialProperty {
        bgfx::UniformHandle handle{};
        bgfx::UniformType::Enum type{};
        uint32_t name{};
        uint16_t num{}; //If it's > 1, then array is in data.
        union {
            GLM::vec4 v4;
            GLM::mat3 m3;
            GLM::mat4 m4;
            void* data;
        };
    };

    struct MaterialTexture {
        bgfx::UniformHandle handle{};
        uint32_t name{};
        Vin::Ref<Texture> tex{};
    };

    class Material {
    public:
        explicit Material(Ref<Program> program);
        ~Material();

        bgfx::ProgramHandle GetProgramHandle();
        void UpdateUniforms();

        void SetVec4(const Vin::StringView& name, const GLM::vec4& v4);
        void SetMat3(const Vin::StringView& name, const GLM::mat3& m3);
        void SetMat4(const Vin::StringView& name, const GLM::mat4& m4);

        void SetTexture(const Vin::StringView& name, const Vin::Ref<Vin::Texture>& texture);

    private:
        Ref<Program> program{};
        //Ref<Material> nextPass{}

        uint16_t propertyCount{ 0 };
        MaterialProperty properties[16];
        uint16_t textureCount{ 0 };
        MaterialTexture textures[16];
    };*/

}

#endif //VIN_ENGINE_MATERIAL_H
