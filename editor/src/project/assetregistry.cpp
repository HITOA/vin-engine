#include "assetregistry.h"

bool AssetRegistry::AddAsset(AssetRegistryID id, AssetRegistryEntry &entry) {
    assets[id] = entry;
    return true;
}

AssetRegistryEntry& AssetRegistry::GetAsset(AssetRegistryID id) {
    if (assets.count(id))
        return assets[id];
    static AssetRegistryEntry dummy{};
    dummy.type = Vin::AssetType::None;
    return dummy;
}

nlohmann::json AssetRegistry::Serialize() const {
    nlohmann::json serializedData{};

    for (auto& entry : assets) {
        std::string key{ entry.first };
        nlohmann::json serializedEntry{};
        serializedEntry["Id"] = entry.first;
        serializedEntry["Type"] = (int)entry.second.type;
        for (auto& children : entry.second.children)
            serializedEntry["Children"] += children;
        serializedEntry["Orphan"] = entry.second.isOrphan;
        serializedEntry["Metadata"] = entry.second.metadata;
        serializedData += serializedEntry;
    }

    return serializedData;
}

void AssetRegistry::Deserialize(const nlohmann::json &data) {
    for (auto& serializedEntry : data) {
        if (serializedEntry.count("Id")) {
            AssetRegistryID id = serializedEntry["Id"].get<Vin::String>();
            AssetRegistryEntry entry{};
            if (serializedEntry.count("Type"))
                entry.type = (Vin::AssetType)serializedEntry["Type"].get<int>();
            if (serializedEntry.count("Children"))
                for (auto& child : serializedEntry["Children"])
                    entry.children.push_back(child.get<Vin::String>());
            if (serializedEntry.count("Orphan"))
                entry.isOrphan = serializedEntry["Orphan"].get<bool>();
            if (serializedEntry.count("Metadata"))
                entry.metadata = serializedEntry["Metadata"];
            AddAsset(id, entry);
        }
    }
}