#ifndef VIN_EDITOR_TEXTUREINSPECTOR_H
#define VIN_EDITOR_TEXTUREINSPECTOR_H

#include "inspector.h"
#include "../importer/textureimporter.h"

class TextureInspector : public Inspector {
public:
    TextureInspector(AssetRegistryID id, EditorModule* editorModule);

    void Draw() final;

private:
    Vin::String GetTextureEncodingFormatString(TextureEncodingFormat format);
    Vin::String GetTextureEncodingQualityString(TextureEncodingQuality quality);

private:
    AssetRegistryID id{};
    EditorModule* editorModule{};
    TextureImporter::ImportSettings textureImportSettings{};
};

#endif
