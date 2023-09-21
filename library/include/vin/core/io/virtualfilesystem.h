#ifndef VIN_ENGINE_VIRTUALFILESYSTEM_H
#define VIN_ENGINE_VIRTUALFILESYSTEM_H

#include <vin/core/templates/ref.h>

namespace Vin::Core::IO {

    class FileAccess {
    public:
        virtual bool IsValid() = 0;
    };

    class VirtualFileSystemInterface {
    public:

    };

    class VirtualFileSystem {
    public:
        Ref<FileAccess> Open();
    };

}

#endif //VIN_ENGINE_VIRTUALFILESYSTEM_H
