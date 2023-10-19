#ifndef VIN_ENGINE_FILESYSTEM_H
#define VIN_ENGINE_FILESYSTEM_H

#include <vin/core/templates/ref.h>
#include <vin/core/templates/stdcontainers.h>
#include <vin/vfs/file.h>

namespace Vin::IO {

    class FileSystem {
    public:
        virtual ~FileSystem() = default;

        virtual bool AllowWriting() { return false; }
        virtual Ref<File> Open(StringView path, FileMode mode) = 0;
        virtual bool Exists(StringView path) = 0;
    };

}

#endif //VIN_ENGINE_FILESYSTEM_H
