#include "material.hpp"
#include "resources/fallback.hpp"
#include "logger/logger.hpp"
#include <optick.h>

Vin::Material::Material(std::shared_ptr<Program> program) : 
    m_Program{ program }, m_Textures{}
{
    if (!m_Program || !m_Program->IsProgramComplete())
        m_Program = Fallback::GetFallbackProgram();

    for (usize i = 0; i < 16; i++)
        m_Textures[i].used = false;

    RecalculateId();
}

void Vin::Material::Bind()
{
    OPTICK_EVENT();

    m_Program->Bind();
    for (short i = 0; i < 16; ++i)
        if (m_Textures[i].used) {
            m_Program->SetInt(i, i);
            m_Textures[i].texture->Bind(i);
        }

    for (int i = 0; i < VIN_MATERIAL_MAX_PROPERTIES; ++i) {
        if (m_Properties[i].type == MaterialPropertyType::None)
            continue;

        switch (m_Properties[i].type) {
        case MaterialPropertyType::Float:
            m_Program->SetFloat(i, ((float*)m_Properties[i].data)[0]);
            break;
        case MaterialPropertyType::Float2:
            m_Program->SetFloat2(i, ((float*)m_Properties[i].data));
            break;
        case MaterialPropertyType::Float3:
            m_Program->SetFloat3(i, ((float*)m_Properties[i].data));
            break;
        case MaterialPropertyType::Float4:
            m_Program->SetFloat4(i, ((float*)m_Properties[i].data));
            break;
        case MaterialPropertyType::Int:
            m_Program->SetInt(i, ((int*)m_Properties[i].data)[0]);
            break;
        case MaterialPropertyType::Int2:
            m_Program->SetInt2(i, ((int*)m_Properties[i].data));
            break;
        case MaterialPropertyType::Int3:
            m_Program->SetInt3(i, ((int*)m_Properties[i].data));
            break;
        case MaterialPropertyType::Int4:
            m_Program->SetInt4(i, ((int*)m_Properties[i].data));
            break;
        }
    }

}

int Vin::Material::GetField(const char* name)
{
    int location = m_Program->GetField(name);
    //if (location == -1)
        //Logger::Warn("Property \"{}\" does not exists.", name);
    return location;
}

void Vin::Material::SetFloat(int location, float value)
{
    if (location < 0)
        return;

    m_Program->SetFloat(location, value);
    m_Properties[location].type = MaterialPropertyType::Float;
    memcpy(m_Properties[location].data, &value, sizeof(float));
}

void Vin::Material::SetFloat2(int location, Vector2<float> values)
{
    if (location < 0)
        return;

    m_Program->SetFloat2(location, values.data);
    m_Properties[location].type = MaterialPropertyType::Float2;
    memcpy(m_Properties[location].data, values.data, sizeof(Vector2<float>));

}

void Vin::Material::SetFloat3(int location, Vector3<float> values)
{
    if (location < 0)
        return;

    m_Program->SetFloat3(location, values.data);
    m_Properties[location].type = MaterialPropertyType::Float3;
    memcpy(m_Properties[location].data, values.data, sizeof(Vector3<float>));

}

void Vin::Material::SetFloat4(int location, Vector4<float> values)
{
    if (location < 0)
        return;

    m_Program->SetFloat4(location, values.data);
    m_Properties[location].type = MaterialPropertyType::Float4;
    memcpy(m_Properties[location].data, values.data, sizeof(Vector4<float>));

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
    m_Properties[location].type = MaterialPropertyType::Int;
    memcpy(m_Properties[location].data, &value, sizeof(int));
}

void Vin::Material::SetInt2(int location, Vector2<int> values)
{
    if (location < 0)
        return;

    m_Program->SetInt2(location, values.data);
    m_Properties[location].type = MaterialPropertyType::Int2;
    memcpy(m_Properties[location].data, values.data, sizeof(Vector2<int>));
}

void Vin::Material::SetInt3(int location, Vector3<int> values)
{
    if (location < 0)
        return;

    m_Program->SetInt3(location, values.data);
    m_Properties[location].type = MaterialPropertyType::Int3;
    memcpy(m_Properties[location].data, values.data, sizeof(Vector3<int>));
}

void Vin::Material::SetInt4(int location, Vector4<int> values)
{
    if (location < 0)
        return;

    m_Program->SetInt4(location, values.data);
    m_Properties[location].type = MaterialPropertyType::Int4;
    memcpy(m_Properties[location].data, values.data, sizeof(Vector4<int>));
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
    m_Properties[location].type = MaterialPropertyType::Mat3;
    memcpy(m_Properties[location].data, values.data, sizeof(Matrix3x3<float>));
}

void Vin::Material::SetMat4(int location, Matrix4x4<float> values)
{
    if (location < 0)
        return;

    m_Program->SetMat4(location, values.data);
    m_Properties[location].type = MaterialPropertyType::Mat4;
    memcpy(m_Properties[location].data, values.data, sizeof(Matrix4x4<float>));
}

void Vin::Material::SetTexture(int location, std::shared_ptr<Texture> texture)
{
    if (!texture) {
        Vin::Logger::Err("Bad texture provided. switching to fallback texture.");
        texture = Fallback::GetFallbackTexture();
    }
    
    if (location < 0 || location > 16)
        return;

    m_Textures[location].texture = texture;
    m_Textures[location].used = true;

    RecalculateId();
}

void Vin::Material::SetTexture(int location, std::shared_ptr<RenderTexture> renderTexture)
{
    if (!renderTexture) {
        Vin::Logger::Err("Bad render texture provided. switching to fallback texture.");
    }

    if (location < 0 || location > 16)
        return;

    m_Textures[location].texture = renderTexture;
    m_Textures[location].used = true;

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

    for (usize i = 0; i < 16; ++i)
        if (m_Textures[i].used)
            bottom += m_Textures[i].texture->GetId();

    unsigned int id{ 0 };
    id += top;
    id = id << (sizeof(Program::IdType) * 8);
    id += bottom;
    m_Id = id;
}