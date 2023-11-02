#ifndef VIN_ENGINE_ASSETFILE_H
#define VIN_ENGINE_ASSETFILE_H

#include <vin/asset/assettype.h>
#include <stddef.h>

#define ASSET_FILE_MAGIC "VINASSET"

namespace Vin {

    struct AssetFileHeader {
        char magic[8];
        AssetType type;
        //Size of the file MINUS size of the header
        size_t size;
    };

    inline bool CheckMagic(AssetFileHeader& header) {
        return !memcmp(header.magic, ASSET_FILE_MAGIC, 8);
    }
}

#endif //VIN_ENGINE_ASSETFILE_H
