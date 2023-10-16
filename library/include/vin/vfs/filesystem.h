#ifndef VIN_ENGINE_FILESYSTEM_H
#define VIN_ENGINE_FILESYSTEM_H

#include <vin/core/templates/ref.h>
#include <vin/core/templates/stdcontainers.h>
#include <vin/vfs/file.h>

namespace Vin::VFS {

    class FileSystem {
    public:
        virtual bool AllowWriting() { return false; }
        virtual Core::Ref<File> Open(Core::StringView path, FileMode mode) = 0;
        virtual bool Exists(Core::StringView path) = 0;
    };

}

#endif //VIN_ENGINE_FILESYSTEM_H
