#ifndef VIN_EDITOR_ASSETIMPORTERMANAGER_H
#define VIN_EDITOR_ASSETIMPORTERMANAGER_H

#include "assetimporter.h"
#include <vin/vin.h>

class AssetImporterManager {
public:
    template<typename T>
    bool AddImporter(Vin::Ref<Project> project) {
        Vin::Ref<T> importer = Vin::MakeRef<T>();
        importer->project = project;
        Vin::AssetType type = importer->GetType();
        if (importers.count(type))
            return false;
        importers[type] = importer;
        return true;
    }

    uint8_t GetMatchingTypes(Vin::StringView path, Vin::Vector<Vin::AssetType>& types) const;
    Vin::Ref<AssetImporter> GetImporter(Vin::AssetType type);


private:
    Vin::UnorderedMap<Vin::AssetType, Vin::Ref<AssetImporter>> importers{};
};


#endif
