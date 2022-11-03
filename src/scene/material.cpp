#include "material.hpp"
#include "resources/fallback.hpp"
#include "logger/logger.hpp"
#include <optick.h>

Vin::Material::Material(std::shared_ptr<Program> program) : 
    m_Program{ program }, m_Textures{}
{
    if (!m_Program || !m_Program->IsProgramComplete())
        m_Program = Fallback::GetFallbackProgram();

    RecalculateId();
}

void Vin::Material::Bind()
{
    OPTICK_EVENT();

    m_Program->Bind();
    
    for (auto& textureData : m_Textures) {
        m_Program->SetInt(textureData.location, textureData.location);
        textureData.texture->Bind(textureData.location);
    }

    for (auto& propertyData : m_Properties) {
        switch (propertyData.type) {
        case MaterialPropertyType::Float:
            m_Program->SetFloat(propertyData.location, ((float*)propertyData.data)[0]);
            break;
        case MaterialPropertyType::Float2:
            m_Program->SetFloat2(propertyData.location, ((float*)propertyData.data));
            break;
        case MaterialPropertyType::Float3:
            m_Program->SetFloat3(propertyData.location, ((float*)propertyData.data));
            break;
        case MaterialPropertyType::Float4:
            m_Program->SetFloat4(propertyData.location, ((float*)propertyData.data));
            break;
        case MaterialPropertyType::Int:
            m_Program->SetInt(propertyData.location, ((int*)propertyData.data)[0]);
            break;
        case MaterialPropertyType::Int2:
            m_Program->SetInt2(propertyData.location, ((int*)propertyData.data));
            break;
        case MaterialPropertyType::Int3:
            m_Program->SetInt3(propertyData.location, ((int*)propertyData.data));
            break;
        case MaterialPropertyType::Int4:
            m_Program->SetInt4(propertyData.location, ((int*)propertyData.data));
            break;
        }
    }

}

std::shared_ptr<Vin::Program> Vin::Material::GetProgram()
{
    return m_Program;
}

int Vin::Material::GetField(const char* name)
{
    int location = m_Program->GetField(name);
    //if (location == -1)
    //    Logger::Warn("Property \"{}\" does not exists.", name);
    return location;
}

void Vin::Material::SetFloat(int location, float value)
{
    if (location < 0)
        return;

    m_Program->SetFloat(location, value);

    MaterialProperty propertyData{};
    propertyData.type = MaterialPropertyType::Float;
    propertyData.location = location;
    memcpy(propertyData.data, &value, sizeof(decltype(value)));

    AddPropertyData(std::move(propertyData));
}

void Vin::Material::SetFloat2(int location, Vector2<float> values)
{
    if (location < 0)
        return;

    m_Program->SetFloat2(location, values.data);

    MaterialProperty propertyData{};
    propertyData.type = MaterialPropertyType::Float2;
    propertyData.location = location;
    memcpy(propertyData.data, &values, sizeof(decltype(values)));

    AddPropertyData(std::move(propertyData));

}

void Vin::Material::SetFloat3(int location, Vector3<float> values)
{
    if (location < 0)
        return;

    m_Program->SetFloat3(location, values.data);

    MaterialProperty propertyData{};
    propertyData.type = MaterialPropertyType::Float3;
    propertyData.location = location;
    memcpy(propertyData.data, &values, sizeof(decltype(values)));

    AddPropertyData(std::move(propertyData));
}

void Vin::Material::SetFloat4(int location, Vector4<float> values)
{
    if (location < 0)
        return;

    m_Program->SetFloat4(location, values.data);

    MaterialProperty propertyData{};
    propertyData.type = MaterialPropertyType::Float4;
    propertyData.location = location;
    memcpy(propertyData.data, &values, sizeof(decltype(values)));

    AddPropertyData(std::move(propertyData));

}

void Vin::Material::SetFloats(int location, float* values, size_t count)
{
    if (location < 0)
        return;

    m_Program->SetFloats(location, values, count);
}

void Vin::Material::SetInt(int location, int value)
{
    if (location < 0)
        return;

    m_Program->SetInt(location, value);

    MaterialProperty propertyData{};
    propertyData.type = MaterialPropertyType::Int;
    propertyData.location = location;
    memcpy(propertyData.data, &value, sizeof(decltype(value)));

    AddPropertyData(std::move(propertyData));
}

void Vin::Material::SetInt2(int location, Vector2<int> values)
{
    if (location < 0)
        return;

    m_Program->SetInt2(location, values.data);

    MaterialProperty propertyData{};
    propertyData.type = MaterialPropertyType::Int2;
    propertyData.location = location;
    memcpy(propertyData.data, &values, sizeof(decltype(values)));

    AddPropertyData(std::move(propertyData));
}

void Vin::Material::SetInt3(int location, Vector3<int> values)
{
    if (location < 0)
        return;

    m_Program->SetInt3(location, values.data);

    MaterialProperty propertyData{};
    propertyData.type = MaterialPropertyType::Int3;
    propertyData.location = location;
    memcpy(propertyData.data, &values, sizeof(decltype(values)));
}

void Vin::Material::SetInt4(int location, Vector4<int> values)
{
    if (location < 0)
        return;

    m_Program->SetInt4(location, values.data);

    MaterialProperty propertyData{};
    propertyData.type = MaterialPropertyType::Int4;
    propertyData.location = location;
    memcpy(propertyData.data, &values, sizeof(decltype(values)));
}

void Vin::Material::SetInts(int location, int* values, size_t count)
{
    if (location < 0)
        return;

    m_Program->SetInts(location, values, count);
}

void Vin::Material::SetMat3(int location, Matrix3x3<float> values)
{
    if (location < 0)
        return;

    m_Program->SetMat3(location, values.data);

    MaterialProperty propertyData{};
    propertyData.type = MaterialPropertyType::Mat3;
    propertyData.location = location;
    memcpy(propertyData.data, &values, sizeof(decltype(values)));
}

void Vin::Material::SetMat4(int location, Matrix4x4<float> values)
{
    if (location < 0)
        return;

    m_Program->SetMat4(location, values.data);

    MaterialProperty propertyData{};
    propertyData.type = MaterialPropertyType::Mat4;
    propertyData.location = location;
    memcpy(propertyData.data, &values, sizeof(decltype(values)));
}

void Vin::Material::SetTexture(int location, std::shared_ptr<Texture> texture)
{
    if (!texture) {
        Vin::Logger::Err("Bad texture provided. switching to fallback texture.");
        texture = Fallback::GetFallbackTexture();
    }
    
    if (location < 0)
        return;

    //m_Textures[location].texture = texture;
    //m_Textures[location].used = true;

    MaterialTextureData textureData{};
    textureData.location = location;
    textureData.texture = texture;

    AddTextureData(std::move(textureData));

    RecalculateId();
}

void Vin::Material::SetTexture(int location, std::shared_ptr<RenderTexture> renderTexture)
{
    if (!renderTexture) {
        Vin::Logger::Err("Bad render texture provided. switching to fallback texture.");
    }

    if (location < 0)
        return;
    
    MaterialTextureData textureData{};
    textureData.location = location;
    textureData.texture = renderTexture;

    AddTextureData(std::move(textureData));

    RecalculateId();
}

void Vin::Material::SetTransparency(bool v) {
    m_Transparency = v;
}

bool Vin::Material::GetTransparency() {
    return m_Transparency;
}

void Vin::Material::SetDoubleSided(bool v)
{
    m_DoubleSided = v;
}

bool Vin::Material::GetDoubleSided()
{
    return m_DoubleSided;
}

unsigned int Vin::Material::GetId() {
    return m_Id;
}

void Vin::Material::RecalculateId()
{
    Program::IdType top = m_Program->GetId();
    Texture::IdType bottom{};

    //for (usize i = 0; i < VIN_MATERIAL_MAX_TEXTURES; ++i)
        //if (m_Textures[i].used)
            //bottom += m_Textures[i].texture->GetId();

    for (auto& textureData : m_Textures) {
        bottom += textureData.texture->GetId();
    }

    unsigned int id{ 0 };
    id += top;
    id = id << (sizeof(Program::IdType) * 8);
    id += bottom;
    m_Id = id;
}

void Vin::Material::AddTextureData(MaterialTextureData&& data)
{
    for (int i = 0; i < m_Textures.size(); ++i) {
        if (m_Textures[i].location == data.location) {
            m_Textures[i] = data;
            return;
        }
    }

    m_Textures.push_back(data);
}

void Vin::Material::AddPropertyData(MaterialProperty&& data)
{
    for (int i = 0; i < m_Properties.size(); ++i) {
        if (m_Properties[i].location == data.location) {
            m_Properties[i] = data;
            return;
        }
    }

    m_Properties.push_back(data);
}
