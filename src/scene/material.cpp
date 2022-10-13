#include "material.hpp"
#include "resources/fallback.hpp"
#include "logger/logger.hpp"

Vin::Material::Material(std::shared_ptr<Program> program) : 
    m_Program{ program }, m_Textures{}
{
    if (!m_Program || !m_Program->IsProgramComplete())
        m_Program = Fallback::GetFallbackProgram();

    for (usize i = 0; i < 16; i++)
        m_Textures[i].used = false;
}

void Vin::Material::Bind()
{
    m_Program->Bind();
    for (usize i = 0; i < 16; ++i)
        if (m_Textures[i].used)
            m_Textures[i].texture->Bind(i);
}

int Vin::Material::GetField(const char* name)
{
    int location = m_Program->GetField(name);
    if (location == -1)
        Logger::Warn("Property \"{}\" does not exists.", name);
    return location;
}

void Vin::Material::SetFloat(int location, float value)
{
    m_Program->SetFloat(location, value);
}

void Vin::Material::SetFloat2(int location, float* values)
{
    m_Program->SetFloat2(location, values);
}

void Vin::Material::SetFloat3(int location, float* values)
{
    m_Program->SetFloat3(location, values);
}

void Vin::Material::SetFloat4(int location, float* values)
{
    m_Program->SetFloat4(location, values);
}

void Vin::Material::SetFloats(int location, float* values, size_t count)
{
    m_Program->SetFloats(location, values, count);
}

void Vin::Material::SetInt(int location, int value)
{
    m_Program->SetInt(location, value);
}

void Vin::Material::SetInt2(int location, int* values)
{
    m_Program->SetInt2(location, values);
}

void Vin::Material::SetInt3(int location, int* values)
{
    m_Program->SetInt3(location, values);
}

void Vin::Material::SetInt4(int location, int* values)
{
    m_Program->SetInt4(location, values);
}

void Vin::Material::SetInts(int location, int* values, size_t count)
{
    m_Program->SetInts(location, values, count);
}

void Vin::Material::SetMat3(int location, float* values)
{
    m_Program->SetMat3(location, values);
}

void Vin::Material::SetMat4(int location, float* values)
{
    if (!m_Program->IsProgramComplete())
        return;
    m_Program->SetMat4(location, values);
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
}