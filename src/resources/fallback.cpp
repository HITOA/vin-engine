#include "fallback.hpp"
#include "logger/logger.hpp"

std::shared_ptr<Vin::Texture> Vin::Fallback::GetFallbackTexture()
{
    static char fallbackTextureData[16]{ (char)255, 0, (char)255, (char)255, 0, 0, 0, (char)255, 0, 0, 0, (char)255, (char)255, 0, (char)255, (char)255 };
    static std::shared_ptr<Vin::Texture> fallbackTexture;
    if (!fallbackTexture) {
        fallbackTexture = Texture::Create(2, 2, TextureFormat::BGRA32, true);
        fallbackTexture->SetData(fallbackTextureData);
        fallbackTexture->SetFiltering(TextureFiltering::Point);
    }

    return fallbackTexture;
}

std::shared_ptr<Vin::Program> Vin::Fallback::GetFallbackProgram()
{
    static const char* fallbackVertexShader = 
        "#version 330 core\n"
        "layout (location=0) in vec3 aPos;\n"
        "uniform mat4 vin_matrix_mvp;\n"
        "void main() {gl_Position = (vin_matrix_mvp * vec4(aPos.xyz, 1.0));}";

    static const char* fallbackFragmentShader =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() {FragColor=vec4(1, 0, 1, 1);}";

    static std::shared_ptr<Program> fallbackProgram;
    if (!fallbackProgram) {
        fallbackProgram = Program::Create();
        fallbackProgram->AddShader(ShaderType::VertexShader, fallbackVertexShader);
        fallbackProgram->AddShader(ShaderType::FragmentShader, fallbackFragmentShader);
        fallbackProgram->CompileProgram();
        if (!fallbackProgram->IsProgramComplete())
            Logger::Err("FALLBACK SHADER WAS NOT ABLE TO COMPILE!!");
    }

    return fallbackProgram;
}
