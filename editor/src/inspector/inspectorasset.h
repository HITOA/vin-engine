#ifndef VIN_EDITOR_INSPECTORASSET_H
#define VIN_EDITOR_INSPECTORASSET_H

#include "inspector.h"
#include "../importer/assetimporter.h"

class InspectorTextAsset : public Inspector {
public:
    void Draw() final;

public:
    AssetTextImportSettings importSettings{};
    Vin::String assetPath{};
    EditorModule* editor{};
};

class InspectorTextureAsset : public Inspector {
public:
    void Draw() final;

private:
    static Vin::String GetTextureEncodingFormatString(TextureEncodingFormat format);
    static Vin::String GetTextureEncodingQualityString(TextureEncodingQuality quality);

public:
    AssetTextureImportSettings importSettings{};
    Vin::String assetPath{};
    EditorModule* editor{};
};

#endif //VIN_EDITOR_INSPECTORASSET_H
