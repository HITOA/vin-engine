#ifndef VIN_ENGINE_ASSETFILE_H
#define VIN_ENGINE_ASSETFILE_H

#include <vin/asset/assettype.h>
#include <stddef.h>

#define ASSET_FILE_MAGIC "VINASSET"

namespace Vin {

    struct AssetFileHeader {
        char magic[8];
        AssetType type;
        //Number of dependencies, will always be zero for shader, texture, text file...
        uint8_t depCount;
        //Size of this asset
        uint64_t size;
    };

    struct AssetFileDependency {
        bool isOrphan;
        AssetType type;
        /*If dep is orphan, offset will be non-zero referencing the dep within this file.
         *If dep is not orphan, offset will be zero and id will be the path to the asset file.*/
        uint64_t offset;
        uint16_t idSize;
    };

    inline bool CheckMagic(AssetFileHeader& header) {
        return !memcmp(header.magic, ASSET_FILE_MAGIC, 8);
    }
}

#endif //VIN_ENGINE_ASSETFILE_H
