#ifndef VIN_ENGINE_NATIVEFILESYSTEM_H
#define VIN_ENGINE_NATIVEFILESYSTEM_H

#include <vin/vfs/filesystem.h>

namespace Vin::IO {

    class NativeFileSystem : public FileSystem {
    public:
        NativeFileSystem();
        NativeFileSystem(StringView root);

        Ref<File> Open(StringView path, FileMode mode) final;
        bool Exists(StringView path) final;

    private:
        String root{};
    };

}

#endif //VIN_ENGINE_NATIVEFILESYSTEM_H
