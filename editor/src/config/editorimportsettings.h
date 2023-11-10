#ifndef VIN_EDITOR_EDITORIMPORTSETTINGS_H
#define VIN_EDITOR_EDITORIMPORTSETTINGS_H

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

#endif //VIN_EDITOR_EDITORIMPORTSETTINGS_H
