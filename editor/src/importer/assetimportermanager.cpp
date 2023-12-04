#include "assetimportermanager.h"

uint8_t AssetImporterManager::GetMatchingTypes(Vin::StringView path, Vin::Vector<Vin::AssetType> &types) const {
    uint8_t c = 0;
    for (auto& entry : importers) {
        if (entry.second->Match(path)) {
            types.push_back(entry.first);
            ++c;
        }
    }
    return c;
}

Vin::Ref<AssetImporter> AssetImporterManager::GetImporter(Vin::AssetType type) {
    if (importers.count(type))
        return importers[type];
    return Vin::Ref<AssetImporter>{};
}