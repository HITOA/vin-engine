#include "editorimportsettings.h"

void EditorImportSettings::Serialize(Config &config) {
    Section section{};

    if (!textureCompilerPath.empty())
        section.properties["TextureCPath"] = textureCompilerPath;

    if (!shaderCompilerPath.empty())
        section.properties["ShaderCPath"] = shaderCompilerPath;

    if (!geometryCompilerPath.empty())
        section.properties["GeometryCPath"] = geometryCompilerPath;

    if (!section.properties.empty())
        config.sections["ImportSettings"] = section;
}

void EditorImportSettings::Deserialize(const Config &config) {
    if (!config.sections.count("ImportSettings"))
        return;

    const Section& section = config.sections.at("ImportSettings");

    if (section.properties.count("TextureCPath"))
        textureCompilerPath = section.properties.at("TextureCPath");

    if (section.properties.count("ShaderCPath"))
        shaderCompilerPath = section.properties.at("ShaderCPath");

    if (section.properties.count("GeometryCPath"))
        geometryCompilerPath = section.properties.at("GeometryCPath");

}