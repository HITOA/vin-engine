#include "assetregistry.hpp"
#include "filesystem/gamefilesystem.hpp"
#include "assert.hpp"
#include <physfs.h>
#include "logger/logger.hpp"

bool Vin::AssetRegistrySerDes::Load(AssetRegistry& registry, const char* path)
{
    if (!GameFilesystem::Exists(path))
        return false;

    eastl::unique_ptr<GameFile> file = GameFilesystem::Open(path, FileMode::Read);
    
    AssetRegistryHeader header = file->ReadType<AssetRegistryHeader>();
    if (strncmp(header.magic, ASSET_REGISTRY_MAGIC, 4) != 0)
        return false;

    registry.m_Offset = header.offset;
    registry.m_Pathes.resize(header.count);
    for (AssetRegistryPath& registrypath : registry.m_Pathes)
        file->ReadBytes(registrypath.path, ASSET_REGISTRY_PATH_LENGTH);

    return true;
}

bool Vin::AssetRegistrySerDes::Save(AssetRegistry& registry, const char* path)
{
    static const char magic[4] = { ASSET_REGISTRY_MAGIC[0], ASSET_REGISTRY_MAGIC[1], ASSET_REGISTRY_MAGIC[2], ASSET_REGISTRY_MAGIC[3] };
    
    eastl::unique_ptr<GameFile> file = GameFilesystem::Create(path);

    VIN_ASSERT(file->IsValid(), "AssetRegistrySerializer was not able to save, file not valid.");

    AssetRegistryHeader header{};
    memcpy(header.magic, magic, 4);
    header.count = registry.m_Pathes.size();
    header.offset = registry.m_Offset;

    file->WriteType(header);

    for (AssetRegistryPath& registrypath : registry.m_Pathes)
        file->WriteType(registrypath);

    return true;
}

Vin::AssetRegistryPath Vin::AssetRegistry::GetPath(AssetId id)
{
    if (id - m_Offset < 0 || id - m_Offset > m_Pathes.size())
        return AssetRegistryPath{};

    return m_Pathes[id - m_Offset];
}

Vin::AssetId Vin::AssetRegistry::AddPath(const char* path, size_t size)
{
    if (size > 256)
        return -1;

    uint32_t pos = m_Pathes.size();
    m_Pathes.push_back(AssetRegistryPath{});
    memset(m_Pathes[pos].path, 0, ASSET_REGISTRY_PATH_LENGTH);
    memcpy(m_Pathes[pos].path, path, size);

    return pos + m_Offset;
}

Vin::AssetId Vin::AssetRegistry::GetOffset() const
{
    return m_Offset;
}


