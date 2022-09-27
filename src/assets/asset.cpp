#include "asset.hpp"
#include <limits.h>

#include <filesystem/gamefilesystem.hpp>

Vin::AssetTypeId Vin::AssetTypeTrait::lastId{ 0 };

/*static Vin::AssetId lastId = MAX_ASSET_ID_VALUE - ASSET_ID_CORE_COUNT;

Vin::AssetId Vin::GenerateAssetId()
{
    return --lastId;
}*/

bool Vin::AssetFileSerDes::GetHeader(AssetHeader& header, const char* path)
{
    if (!GameFilesystem::Exists(path))
        return false;

    eastl::unique_ptr<GameFile> file = GameFilesystem::Open(path, FileMode::Read);
    header = file->ReadType<AssetHeader>();

    return true;
}
