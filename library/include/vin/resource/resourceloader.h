#ifndef VIN_ENGINE_RESOURCELOADER_H
#define VIN_ENGINE_RESOURCELOADER_H

#include <vin/core/templates/stdcontainers.h>
#include <vin/core/templates/ref.h>
#include <vin/vfs/file.h>

namespace Vin::Resource {

    template<typename T>
    struct ResourceLoader {
        static_assert(false, "No resource loader for specified type.");
    };

    //Example ResourceLoader

    template<>
    struct ResourceLoader<Core::String> {
        Core::Ref<Core::String> operator()(const Core::Ref<VFS::File>& file) {
            Core::Ref<Core::String> str{ Core::MakeRef<Core::String>() };

            str->resize(file->GetSize() + 1);
            file->ReadBytes(str->data(), str->size() - 1);
            (*str)[str->size() - 1] = 0;

            return str;
        }
    };

}

#endif //VIN_ENGINE_RESOURCELOADER_H
