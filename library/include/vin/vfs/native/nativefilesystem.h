#ifndef VIN_ENGINE_NATIVEFILESYSTEM_H
#define VIN_ENGINE_NATIVEFILESYSTEM_H

#include <vin/vfs/filesystem.h>

namespace Vin::VFS {

    class NativeFileSystem : public FileSystem {
    public:
        NativeFileSystem();
        NativeFileSystem(Core::StringView root);

        Core::Ref<File> Open(Core::StringView path, FileMode mode) final;
        bool Exists(Core::StringView path) final;

    private:
        Core::String root{};
    };

}

#endif //VIN_ENGINE_NATIVEFILESYSTEM_H
