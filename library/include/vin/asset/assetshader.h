#ifndef VIN_ENGINE_ASSETSHADER_H
#define VIN_ENGINE_ASSETSHADER_H

#include <vin/asset/assetfile.h>

namespace Vin {

    enum class ShaderType {
        Fragment,
        Vertex,
        Compute
    };

    struct ShaderFileHeader {
        AssetFileHeader header{};
        ShaderType type{};
    };

}

#endif
