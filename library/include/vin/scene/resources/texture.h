#ifndef VIN_ENGINE_TEXTURE_H
#define VIN_ENGINE_TEXTURE_H

#include <vin/core/templates/stdcontainers.h>
#include <vin/resource/resourceloader.h>
#include <vin/asset/assetfile.h>
#include <vin/core/logger/logger.h>

namespace Vin {

    /*class Texture {
    public:
        explicit Texture(const bgfx::Memory* data); //Create texture by passing directly texture data (DDS, KTX or PVR)
        ~Texture();

        bgfx::TextureHandle GetTextureHandle();
    private:
        bgfx::TextureHandle handle{};
    };

    template<>
    struct ResourceLoader<Texture> {
        Ref<Texture> operator()(const Ref<IO::File>& file) {
            AssetFileHeader header{};
            if (file->ReadBytes((char*)&header, sizeof(AssetFileHeader)) != sizeof(AssetFileHeader) || !CheckMagic(header)) {
                Vin::Logger::Err("Couldn't load Texture. Provided asset file isn't in the right format or is corrupted.");
                return Ref<Texture>{};
            }

            if (header.type != Vin::AssetType::Texture) {
                Vin::Logger::Err("Couldn't load Texture. Provided asset isn't a Texture.");
                return Ref<Texture>{};
            }

            const bgfx::Memory* buff = bgfx::alloc(header.size);

            if (buff == nullptr)
                return Ref<Texture>{};

            if (file->ReadBytes((char*)buff->data, header.size) != header.size) {
                Vin::Logger::Err("Couldn't load Texture. Asset file header doesn't match asset content. Is it corrupted ?");
            }

            return Vin::MakeRef<Texture>(buff);
        }
    };*/

}

#endif //VIN_ENGINE_TEXTURE_H
