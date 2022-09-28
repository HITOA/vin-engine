#include "material.hpp"

Vin::Material::Material(eastl::shared_ptr<Program> program) : 
    m_Program{ program }, m_Textures{}
{
    for (usize i = 0; i < 16; i++)
        m_Textures[i].used = false;
}

void Vin::Material::Bind()
{
    if (!m_Program->IsShaderComplete())
        return;
    m_Program->Bind();
    for (usize i = 0; i < 16; ++i)
        if (m_Textures[i].used)
            m_Textures[i].texture->Bind(i);
}

int Vin::Material::GetField(const char* name)
{
    if (!m_Program->IsShaderComplete())
        return -1;
    return m_Program->GetField(name);
}

void Vin::Material::SetFloat(int location, float value)
{
    if (!m_Program->IsShaderComplete())
        return;
    m_Program->SetFloat(location, value);
}

void Vin::Material::SetFloat2(int location, float* values)
{
    if (!m_Program->IsShaderComplete())
        return;
    m_Program->SetFloat2(location, values);
}

void Vin::Material::SetFloat3(int location, float* values)
{
    if (!m_Program->IsShaderComplete())
        return;
    m_Program->SetFloat3(location, values);
}

void Vin::Material::SetFloat4(int location, float* values)
{
    if (!m_Program->IsShaderComplete())
        return;
    m_Program->SetFloat4(location, values);
}

void Vin::Material::SetFloats(int location, float* values, size_t count)
{
    if (!m_Program->IsShaderComplete())
        return;
    m_Program->SetFloats(location, values, count);
}

void Vin::Material::SetInt(int location, int value)
{
    if (!m_Program->IsShaderComplete())
        return;
    m_Program->SetInt(location, value);
}

void Vin::Material::SetInt2(int location, int* values)
{
    if (!m_Program->IsShaderComplete())
        return;
    m_Program->SetInt2(location, values);
}

void Vin::Material::SetInt3(int location, int* values)
{
    if (!m_Program->IsShaderComplete())
        return;
    m_Program->SetInt3(location, values);
}

void Vin::Material::SetInt4(int location, int* values)
{
    if (!m_Program->IsShaderComplete())
        return;
    m_Program->SetInt4(location, values);
}

void Vin::Material::SetInts(int location, int* values, size_t count)
{
    if (!m_Program->IsShaderComplete())
        return;
    m_Program->SetInts(location, values, count);
}

void Vin::Material::SetMat3(int location, float* values)
{
    if (!m_Program->IsShaderComplete())
        return;
    m_Program->SetMat3(location, values);
}

void Vin::Material::SetMat4(int location, float* values)
{
    if (!m_Program->IsShaderComplete())
        return;
    m_Program->SetMat4(location, values);
}

void Vin::Material::SetTexture(int location, eastl::shared_ptr<Texture> texture)
{
    if (!m_Program->IsShaderComplete())
        return;
    m_Textures[location].texture = texture;
    m_Textures[location].used = true;
}