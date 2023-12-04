#ifndef VIN_EDITOR_ASSETREGISTRY_H
#define VIN_EDITOR_ASSETREGISTRY_H

#include <vin/vin.h>
#include <vin/json/serializable.h>

typedef Vin::String AssetRegistryID;

struct AssetRegistryEntry {
    Vin::AssetType type{ Vin::AssetType::None };
    Vin::Vector<AssetRegistryID> children{};
    bool isOrphan{ false };
    nlohmann::json metadata{};
};

class AssetRegistry : public Vin::Serializable {
public:
    bool AddAsset(AssetRegistryID id, AssetRegistryEntry& entry);
    AssetRegistryEntry& GetAsset(AssetRegistryID id);

    [[nodiscard]] nlohmann::json Serialize() const final;
    void Deserialize(const nlohmann::json& data) final;

private:
    Vin::UnorderedMap<AssetRegistryID, AssetRegistryEntry> assets{};
};

#endif //VIN_EDITOR_ASSETREGISTRY_H
