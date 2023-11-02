#ifndef VIN_ENGINE_RESOURCELOADER_H
#define VIN_ENGINE_RESOURCELOADER_H

#include <vin/core/templates/stdcontainers.h>
#include <vin/core/templates/ref.h>
#include <vin/vfs/file.h>

namespace Vin {

    template<typename T>
    struct ResourceLoader {
        //static_assert(false, "No resource loader for specified type.");
    };

    //Example ResourceLoader

    template<>
    struct ResourceLoader<String> {
        Ref<String> operator()(const Ref<IO::File>& file) {
            Ref<String> str{ MakeRef<String>() };

            str->resize(file->GetSize() + 1);
            file->ReadBytes(str->data(), str->size() - 1);
            (*str)[str->size() - 1] = 0;

            return str;
        }
    };

}

#endif //VIN_ENGINE_RESOURCELOADER_H
