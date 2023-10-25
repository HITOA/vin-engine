#ifndef VIN_ENGINE_EDITORIMPORTSETTINGS_H
#define VIN_ENGINE_EDITORIMPORTSETTINGS_H

#include <vin/vin.h>
#include "editorconfig.h"

class EditorImportSettings : public EditorConfig {
public:
    void Serialize(Config& config) final;
    void Deserialize(const Config& config) final;

    Vin::String textureCompilerPath{};
    Vin::String shaderCompilerPath{};
    Vin::String geometryCompilerPath{};
};

#endif //VIN_ENGINE_EDITORIMPORTSETTINGS_H
