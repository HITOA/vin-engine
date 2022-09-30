#include "assetfile.hpp"
#include "filesystem/gamefilesystem.hpp"

bool Vin::AssetFileSerializer::Load(const char* path, AssetHeader& header, void* buff, usize buffsize)
{
    if (!GameFilesystem::Exists(path))
        return false;

    eastl::unique_ptr<GameFile> file = GameFilesystem::Open(path, FileMode::Read);

    if (!file->IsValid())
        return false;

    header = file->ReadType<AssetHeader>();

    if (strncmp(header.magic, ASSET_MAGIC, 4) != 0)
        return false;

    if (buffsize != file->ReadBytes((char*)buff, buffsize))
        return false;

    return true;
}

bool Vin::AssetFileSerializer::Save(const char* path, AssetHeader& header, void* buff, usize buffsize)
{
    static const char magic[4] = { ASSET_MAGIC[0], ASSET_MAGIC[1], ASSET_MAGIC[2], ASSET_MAGIC[3] };

    eastl::unique_ptr<GameFile> file = GameFilesystem::Open(path, FileMode::Write);

    memcpy(header.magic, magic, sizeof(magic));

    if (!file->IsValid())
        return false;

    file->WriteType(header);
    file->WriteBytes((char*)buff, buffsize);

    return true;
}
