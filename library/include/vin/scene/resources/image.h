#ifndef VIN_ENGINE_IMAGE_H
#define VIN_ENGINE_IMAGE_H

#include <vin/core/templates/stdcontainers.h>
#include <vin/resource/resourceloader.h>
#include <bimg/bimg.h>

namespace Vin {

    class Image {
    public:
        Image(void* data, size_t size); //Create Image by passing directly image data.
        ~Image();
    private:
        bimg::ImageContainer* imageContainer{};
    };

    template<>
    struct ResourceLoader<Image> {
        Ref<Image> operator()(const Ref<IO::File>& file) {
            size_t size = file->GetSize();
            void* buff = Vin::Core::MemoryManager::Allocate<Vin::Core::AllocationStrategy::SingleFrame>(size);
            file->ReadBytes((char*)buff, size);

            return Vin::MakeRef<Image>(buff, size);
        }
    };

}


#endif //VIN_ENGINE_IMAGE_H
