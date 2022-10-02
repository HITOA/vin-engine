#include "assetregistry.hpp"
#include "core/assert.hpp"
#include "logger/logger.hpp"

bool Vin::AssetRegistrySerializer::Load(AssetRegistry& registry, const char* path)
{
    /*if (!GameFilesystem::Exists(path))
        return false;

    std::unique_ptr<GameFile> file = GameFilesystem::Open(path, FileMode::Read);
    
    AssetRegistryHeader header = file->ReadType<AssetRegistryHeader>();
    if (strncmp(header.magic, ASSET_REGISTRY_MAGIC, 4) != 0)
        return false;

    memcpy(registry.m_Name, header.name, sizeof(registry.m_Name));
    memcpy(registry.m_Path, header.path, sizeof(registry.m_Name));
    registry.m_Pathes.resize(header.count);
    for (AssetRegistryPath& registrypath : registry.m_Pathes)
        file->ReadBytes(registrypath.path, ASSET_REGISTRY_PATH_LENGTH);*/

    return true;
}

bool Vin::AssetRegistrySerializer::Save(AssetRegistry& registry)
{
    /*static const char magic[4] = {ASSET_REGISTRY_MAGIC[0], ASSET_REGISTRY_MAGIC[1], ASSET_REGISTRY_MAGIC[2], ASSET_REGISTRY_MAGIC[3]};
    
    std::unique_ptr<GameFile> file = GameFilesystem::Create(registry.GetRegistryPath());

    if (!file->IsValid()) {
        Logger::Err("Couldn't save registry to : {}", registry.GetRegistryPath());
        return false;
    }

    AssetRegistryHeader header{};
    memcpy(header.magic, magic, 4);
    memcpy(header.name, registry.m_Name, sizeof(header.name));
    memcpy(header.path, registry.m_Path, sizeof(header.path));
    header.count = registry.m_Pathes.size();

    file->WriteType(header);

    for (AssetRegistryPath& registrypath : registry.m_Pathes)
        file->WriteType(registrypath);*/

    return true;
}

Vin::AssetRegistryPath Vin::AssetRegistry::GetAssetPath(AssetId id)
{
    if (id  > m_Pathes.size())
        return AssetRegistryPath{};

    return m_Pathes[id];
}

Vin::AssetId Vin::AssetRegistry::AddAssetPath(const char* path, size_t size)
{
    if (size > 256)
        return -1;

    AssetId assetId = m_Pathes.size();
    m_Pathes.push_back(AssetRegistryPath{});
    memset(m_Pathes[assetId].path, 0, ASSET_REGISTRY_PATH_LENGTH);
    memcpy(m_Pathes[assetId].path, path, size);

    return assetId;
}

int Vin::AssetRegistry::GetAssetCount() const
{
    return m_Pathes.size();
}

void Vin::AssetRegistry::SetRegistryName(char name[64])
{
    memcpy(m_Name, name, sizeof(m_Name));
}

const char* Vin::AssetRegistry::GetRegistryName()
{
    return m_Name;
}

void Vin::AssetRegistry::SetRegistryPath(char path[256])
{
    memcpy(m_Path, path, sizeof(m_Path));
}

const char* Vin::AssetRegistry::GetRegistryPath()
{
    return m_Path;
}


