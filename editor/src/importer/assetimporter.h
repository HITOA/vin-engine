#ifndef VIN_EDITOR_ASSETIMPORTER_H
#define VIN_EDITOR_ASSETIMPORTER_H

#include <vin/vin.h>
#include "../config/editorimportsettings.h"

inline Vin::AssetType GetType(Vin::StringView ext) {
    static const char* textExt = ".txt";
    static const char* textureExt = ".bmp.dds.exr.gif.jpg.jpeg.hdr.ktx.png.psd.pvr.tga";
    static const char* shaderExt = ".glsl.compute.frag.vert";

    if (std::string{ textExt }.find(ext) != std::string::npos)
        return Vin::AssetType::Text;
    if (std::string{ textureExt }.find(ext) != std::string::npos)
        return Vin::AssetType::Texture;
    if (std::string{ shaderExt }.find(ext) != std::string::npos)
        return Vin::AssetType::Shader;

    return Vin::AssetType::None;
}

template<Vin::AssetType type>
struct AssetImporter {};

#endif //VIN_EDITOR_ASSETIMPORTER_H
